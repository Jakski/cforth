#ifndef CFORTH_UTIL_INCLUDED
#define CFORTH_UTIL_INCLUDED

#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <CException.h>

#define _unused_ __attribute__((unused))
#define _cleanup_(x) __attribute__((__cleanup__(x)))
#define xmalloc(size) verbose_xmalloc(size, __FILE__, __LINE__)
#define xrealloc(size) verbose_xrealloc(ptr, size, __FILE__, __LINE__)
#define LOG(n, ...) \
	do { \
		fprintf(n, "%s:%d: ", __FILE__, __LINE__); \
		fprintf(n, __VA_ARGS__); \
		fprintf(n, "\n"); \
	} while(0)
#define THROW_WITH_ERRNO() \
	do { \
		cforth_error_line = __LINE__; \
		cforth_error_file = __FILE__; \
		cforth_error = errno; \
		Throw(CFORTH_ERROR_ERRNO); \
	} while(0)
#define THROW(e) \
	do { \
		cforth_error_line = __LINE__; \
		cforth_error_file = __FILE__; \
		Throw(e); \
	} while(0)
#define THROW_WITH_MESSAGE(m) \
	do { \
		cforth_error_line = __LINE__; \
		cforth_error_file = __FILE__; \
		cforth_error_message = m; \
		Throw(CFORTH_ERROR_MESSAGE); \
	} while(0)
#define CFORTH_ERROR_BUFFER_SIZE 255
#define THROW_WITH_BUFFER(f, m, l) \
	do { \
		cforth_error_line = __LINE__; \
		cforth_error_file = __FILE__; \
		cforth_error_message = f; \
		memset(cforth_error_buffer, 0, CFORTH_ERROR_BUFFER_SIZE); \
		if (l > CFORTH_ERROR_BUFFER_SIZE - 1) { \
			Throw(CFORTH_ERROR_BUFFER_OVERFLOW); \
		} \
		memcpy(cforth_error_buffer, m, l); \
		Throw(CFORTH_ERROR_BUFFER); \
	} while(0)

enum cforth_error {
	CFORTH_ERROR,
	CFORTH_ERROR_ERRNO,
	CFORTH_ERROR_MESSAGE,
	CFORTH_ERROR_BUFFER,
	CFORTH_ERROR_BUFFER_OVERFLOW,
};

int cforth_error = 0;
int cforth_error_line = 0;
const char *cforth_error_file = NULL;
const char *cforth_error_message = NULL;
char cforth_error_buffer[CFORTH_ERROR_BUFFER_SIZE] = { 0 };

void print_error(enum cforth_error error) {
	const char *error_message = NULL;
	switch (error) {
		case CFORTH_ERROR_ERRNO:
			error_message = strerror(cforth_error);
			break;
		case CFORTH_ERROR:
			error_message = "Fatal error";
			break;
		case CFORTH_ERROR_MESSAGE:
			error_message = cforth_error_message;
			break;
		case CFORTH_ERROR_BUFFER_OVERFLOW:
			error_message = "Buffer for handling error messages reached overflow";
			break;
		case CFORTH_ERROR_BUFFER:
			fprintf(stderr, "%s:%d: ", cforth_error_file, cforth_error_line);
			fprintf(stderr, cforth_error_message, cforth_error_buffer);
			fprintf(stderr, "\n");
			return;
		default:
			error_message = "Unknown error";
			break;
	}
	fprintf(
		stderr,
		"%s:%d: %s\n",
		cforth_error_file,
		cforth_error_line,
		error_message
	);
}

_unused_ static inline void *verbose_xmalloc(size_t size, const char *src, int line) {
	void *r = malloc(size);
	if (r == NULL) {
		THROW_WITH_MESSAGE("Failed to allocate memory");
	}
	return r;
}

_unused_ static inline void *verbose_xrealloc(void *ptr, size_t size, const char *src, int line) {
	void *r = realloc(ptr, size);
	if (r == NULL && size != 0) {
		THROW_WITH_MESSAGE("Failed to reallocate memory");
	}
	return r;
}

#endif
