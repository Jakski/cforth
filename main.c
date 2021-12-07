#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#include <sds.h>

#include "util.h"

#define DATA_STACK_SIZE 255
int data_stack[DATA_STACK_SIZE] = { 0 };
int data_stack_top = -1;

#define RETURN_STACK_SIZE 255
void *return_stack[RETURN_STACK_SIZE] = { NULL };
int return_stack_top = -1;

#define USER_MEMORY_SIZE 6144
char user_memory[USER_MEMORY_SIZE] = { 0 };
char *here = user_memory;
char *latest = user_memory;

struct dictionary {
	struct dictionary *prev;
	char name_size;
	int flags;
};

void data_stack_put(int value) {
	if (data_stack_top == DATA_STACK_SIZE) {
		THROW_WITH_MESSAGE("Data stack overflow");
	}
	data_stack[++data_stack_top] = value;
}

void data_stack_show(void) {
	for (int i = data_stack_top; i >= 0; i--) {
		printf("%d: %d\n", i, data_stack[i]);
	}
}

int data_stack_peek(int offset) {
	if (data_stack_top - offset < 0) {
		THROW_WITH_MESSAGE("Data stack underflow");
	}
	return data_stack[data_stack_top - offset];
}

void word_next(void) {
}

void word_drop(void) {
	(void) data_stack_peek(0);
	data_stack_top--;
}

void word_swap(void) {
	int tmp = data_stack_peek(1);
	data_stack[data_stack_top - 1] = data_stack[data_stack_top];
	data_stack[data_stack_top] = tmp;
}

void word_dup(void) {
	data_stack_put(data_stack[data_stack_top]);
}

void word_over(void) {
	data_stack_put(data_stack_peek(1));
}

void word_rot(void) {
	int tmp = data_stack_peek(2);
	word_swap();
	data_stack[data_stack_top - 2] = data_stack[data_stack_top];
	data_stack[data_stack_top] = tmp;
}

void word_plus(void) {
	int r = data_stack_peek(1) + data_stack_peek(0);
	word_drop();
	data_stack[data_stack_top] = r;
}

void word_minus(void) {
	int r = data_stack_peek(1) - data_stack_peek(0);
	word_drop();
	data_stack[data_stack_top] = r;
}

void word_star(void) {
	int r = data_stack_peek(1) * data_stack_peek(0);
	word_drop();
	data_stack[data_stack_top] = r;
}

void word_slash(void) {
	int r = data_stack_peek(1) / data_stack_peek(0);
	word_drop();
	data_stack[data_stack_top] = r;
}

void word_slash_mod(void) {
	int n1 = data_stack_peek(1);
	int n2 = data_stack_peek(0);
	data_stack[data_stack_top - 1] = n1 % n2;
	data_stack[data_stack_top] = n1 / n2;
}

void word_mod(void) {
	int r = data_stack_peek(1) % data_stack_peek(0);
	word_drop();
	data_stack[data_stack_top] = r;
}

void word_2swap(void) {
	int n3 = data_stack_peek(3);
	int n2 = data_stack_peek(2);
	int n1 = data_stack_peek(1);
	int n0 = data_stack_peek(0);
	data_stack[data_stack_top - 3] = n1;
	data_stack[data_stack_top - 2] = n0;
	data_stack[data_stack_top - 1] = n3;
	data_stack[data_stack_top] = n2;
}

void word_2dup(void) {
	data_stack_put(data_stack_peek(1));
	data_stack_put(data_stack_peek(1));
}

void word_2over(void) {
	data_stack_put(data_stack_peek(3));
	data_stack_put(data_stack_peek(3));
}

void word_2drop(void) {
	word_drop();
	word_drop();
}

void word_exit(void) {
	exit(EXIT_SUCCESS);
}

int main(int argc, char **argv) {
	CEXCEPTION_T error;
	Try {
		int in_char = 0;
		sds word = sdsempty();
		while ((in_char = fgetc(stdin)) != EOF) {
			if (!isspace(in_char)) {
				word = sdscatlen(word, (char *) &in_char, 1);
				continue;
			}
			if (strcmp(word, "SWAP") == 0) {
				word_swap();
			} else if (strcmp(word, "*") == 0) {
				word_star();
			} else if (strcmp(word, "/MOD") == 0) {
				word_slash_mod();
			} else if (strcmp(word, "/") == 0) {
				word_slash();
			} else if (strcmp(word, "ROT") == 0) {
				word_rot();
			} else if (strcmp(word, "+") == 0) {
				word_plus();
			} else if (strcmp(word, "OVER") == 0) {
				word_over();
			} else if (strcmp(word, "MOD") == 0) {
				word_mod();
			} else if (strcmp(word, "-") == 0) {
				word_minus();
			} else if (strcmp(word, "EXIT") == 0) {
				sdsfree(word);
				word_exit();
			} else if (strcmp(word, "DUP") == 0) {
				word_dup();
			} else if (strcmp(word, "DROP") == 0) {
				word_drop();
			} else if (strcmp(word, "2SWAP") == 0) {
				word_2swap();
			} else if (strcmp(word, "2OVER") == 0) {
				word_2over();
			} else if (strcmp(word, "2DUP") == 0) {
				word_2dup();
			} else if (strcmp(word, "2DROP") == 0) {
				word_2drop();
			} else if (strcmp(word, ".S") == 0) {
				data_stack_show();
			} else if (strcmp(word, ".") == 0) {
				printf("%d\n", data_stack_peek(0));
				word_drop();
			} else if (sdslen(word) == 0) {
				continue;
			} else {
				char *endptr;
				long n = strtol(word, &endptr, 10);
				if (endptr == word) {
					THROW_WITH_BUFFER("Unknown word: %s", word, sdslen(word));
				}
				data_stack_put((int) n);
			}
			sdsfree(word);
			word = sdsempty();
		}
	} Catch(error) {
		print_error(error);
		exit(EXIT_FAILURE);
	}
	return 0;
}
