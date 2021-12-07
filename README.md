# cforth

cforth is a minimal implementation of Forth programming language for
educational purposes.

Author learned about Forth from countless sources, but to name
a few prominent ones:

- https://github.com/nornagon/jonesforth/blob/master/jonesforth.S
- https://www.complang.tuwien.ac.at/forth/gforth/Docs-html/
- https://skilldrick.github.io/easyforth/
- https://github.com/oh2aun/flashforth/blob/master/tutorials/ff5-elements.pdf
- http://thinking-forth.sourceforge.net/
- https://www.forth.com/starting-forth/

# Features

This section is to do list including desired system features.

## Data structures

- Data stack - Used implicitly by builtin functions and stack operators.
  Constant size decided upon startup.
- Return stack - Used internally by words. Available to user for call stack
  manipulations and as a temporary storage. Constant size decided upon startup.
- Dictionary - Singly linked list containing word definitions. Limited by heap
  size, but possibly dynamic.
- Heap - Memory for user definitions. It includes dictionary, but also
  variables, constants and compiled code.

## Variables

- STATE - Is the interpreter executing code (0) or compiling a word (non-zero).
- LATEST - Points to the latest word in dictionary.
- HERE - Points to the next free byte of memory. Compiled code goes there.
- S0 - Stores the address of top of the parameter stack.
- BASE - The current base for printing and reading numbers.

## Constants

- VERSION - Current Forth version.
- R0 - The address of top of the return stack.
- DOCOL - Pointer to Forth compiled code interpreter.

## Words implemented in machine code

- DROP - ( n1 n2 -- n1 )
- 2DROP - ( d1 d2 -- d1 )
- DUP - ( n1 -- n1 n1 )
- 2DUP - ( d1 -- d1 d1 )
- OVER - ( n1 n2 -- n1 n2 n1 )
- 2OVER - ( d1 d2 -- d1 d2 d1 )
- ROT - ( n1 n2 n3 -- n2 n3 n1 )
- 2ROT - ( d1 d2 d3 -- d2 d3 d1 )
- -ROT - ( n1 n2 n3 -- n3 n1 n2 )
- SWAP - ( n1 n2 -- n2 n1 )
- 2SWAP - ( d1 d2 -- d2 d1 )
- + - ( n1 n2 -- n1 + n2 )
- - - ( n1 n2 -- n1 - n2 )
- * - ( n1 n2 -- n1 * n2 )
- /MOD - ( n1 n2 -- n1 % n2 n1 / n2 )
- ?DUP - Duplicate top of stack if it's not zero
- 1+ - ( n1 -- n1 + 1 )
- 1- - ( n1 -- n1 - 1 )
- 4+ - ( n1 -- n1 + 4 )
- 4- - ( n1 -- n1 - 4 )
- Comparision operators:
  * =
  * <>
  * <
  * >
  * <=
  * >=
  * 0=
  * 0<>
  * 0<
  * 0>
  * 0<=
  * 0>=
- Logical operators:
  * AND
  * OR
  * XOR
  * INVERT
- EXIT - Return from word
- LIT - Treat next word literally
- ! - ( address data -- ) Store data under address
- @ - ( address -- data ) Fetch data from memory to stack
- +! - ( address amount -- ) Add amount to value under address
- -! - ( address amount -- ) Substract amount from value under address
- C! - ( address data -- ) Store byte under address
- C@ - ( address -- data ) Read byte from address
- C@C! - ( source dest -- ) Copy bytes from source address to destination
  address
- CMOVE - ( length dest source -- ) Copy block of bytes from source address to
  destination address
- >R - ( n -- ) Push parameter into return stack
- R> - ( -- n ) Pop element from return stack into data stack
- RSP@ - Get return stack pointer
- RSP! - Set return stack pointer
- RDROP - Drop element from return stack
- DSP@ - Get data stack top
- DSP! - Set data stack top
- KEY - ( -- n ) Read ASCII character from input
- EMIT - Write byte to output
- WORD ( -- address length ) Read characters with until whole word is ready in
  buffer
- NUMBER ( -- n1 n2 ) Read number and report unparsed characters
- FIND ( address length -- a ) Get address of the dictionary entry or null
- >CFA ( word -- address ) Get address of the codeword
- >DFA ( word -- address ) Get address of the first data field
- CREATE - ( address length -- ) Create new definition header in memory pointed
  by HERE
- COMA ( pointer -- ) Add number to memory pointed by HERE and progress it
- [ - Switch to immediate mode
- ] - Switch to compile mode
- : - Start defining a new word
- ; - Stop word definition
- IMMEDIATE - Toggle immediate flag on latest word
- HIDDEN - ( address -- ) Toggle hidden flag on word
- HIDE - Toggle hidden flag on word by name
- ' - Get codeword pointer of the next word
- BRANCH - Unconditional branch
- 0BRANCH - Branches code, if the top of the stack is zero
- LITSTRING - Helper for implementing strings
- TELL - Print string
- QUIT - Clean return stack
- INTERPRET - Interprets or compiles word depending on a state
- CHAR - Put ASCII code of following character on stack
- EXECUTE - Run execution token

## Words implemented in Forth

- / - ( n1 n2 -- n1 / n2 )
- MOD - ( n1 n2 -- n1 % n2 )
- '\n' - Literal newline
- BL - Space
- CR - See '\n'
- SPACE - See BL
- NEGATE - Leave negated number on stack.
- TRUE - Expression evaluating always to true
- FALSE - Expression evaluating always to false
- NOT - Logical negation
- LITERAL - Compiles last item from stack as literal
- Literal characters:
  * ':'
  * ';'
  * '('
  * ')'
  * '"'
  * 'A'
  * '0'
  * '-'
  * '.'
- [COMPILE] - Force compile next word even, if it's immediate
- RECURSE - Call currently compiled word ignoring hidden flag
- Flow control structures:
  * IF, THEN, ELSE - Compile-only version of basic conditional statement
  * UNLESS - Inversed basic conditional statement
  * CASE, OF, ENDOF, ENDCASE - Switch statement
  * BEGIN, UNTIL - While loop repeated when expression evaluates to false
  * AGAIN - Infinite loop
  * WHILE, REPEAT - While loop repeated when expression evaluates to true
- ( - Start dropping characters until ')' is found
- NIP - ( x y -- y )
- TUCK - ( x y -- y x y )
- PICK - ( x_u ... x_1 x_0 u -- x_u ... x_1 x_0 x_u )
- SPACES - Print n spaces
- DECIMAL - Change base to decimal
- HEX - Change base to hex
- U. - Print unsigned number
- .S - Print current stack
- UWIDTH - Get length of unsigned number
- U.R - Print unsigned number padded to width
- .R - Print signed number padded to width
- . - Drop and print top of the stack
- ? - Fetch and print integer from address
- WITHIN - Check, if number is within range
- DEPTH - Get data stack depth
- ALIGNED - Round address up to next 4 byte boundary
- ALIGN - Align HERE pointer
- C, - Append byte to currently compiled word
- S" - Define string
- ." - Print string
- CONSTANT - Define word returning constant value
- ALLOT - Allocate memory of arbitrary size and return its address
- CELLS - Multiply top of stack by standard integer size
- VARIABLE - Allocate memory and add dictionary definition pointing to it
- VALUE - Define variable in easy to access manner
- TO - Save data to variable defined with VALUE
- +TO - Add number to variable defined with VALUE
- ID. - Print word name having its address
- ?HIDDEN - Check, if word is hidden
- ?IMMEDIATE - Check, if word is immediate
- WORDS - Print all words defined in dictionary except hidden words
- FORGET - Forget everything after word definition making it possible to
  overwrite old user memory
- DUMP - Hexdump contents of memory
- CFA> - Find dictionary definition matching codeword address
- SEE - Print decompiled word
- :NONAME - Create definition without name
- ['] - Compile literal value
- EXCEPTION-MARKER - Mark exception handling frame
- CATCH - Handle exception
- THROW - Throw exception
- ABORT - Throw exception with standard -1 code from ANS Forth
- PRINT-STACK-TRACE - Traverse and print stack trace
- Z" - Create C-compatible string terminated with null
- STRLEN - Get length of null-terminated string
- CSTRING - Convert string to null-terminated version
- ARGC - Access Linux program argument count
- ARGV - Access Linux program argument
- ENVIRON - Return address of the first environment string
- BYE - Exit Forth
- NEXT - Jump to next word
- ;CODE - Treat word definition as assembly
- INLINE - Inline assembly primitive in assembly word
