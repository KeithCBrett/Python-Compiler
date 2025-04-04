# Pyco - A PYthon COmpiler
## GOAL
This program is meant to take as input a *high level* python program and produce as output
*low level* x64 assembly which can then be assembled, linked, and loaded into memory to emit
a functional program.

### SPECIFIC GOALS
1. Write a Python program that initializes two variables. Assign these variables integer values and add them together,
and assign the result to another variable. Call the print function on this last variable to print the result.
    - Variable assignment.
    - Integer addition.
    - Function calling.


## DESIGN
- Written in C.

- Hand coded scanner/lexer written in C.
    - Keywords encoded in trie.

- Hand coded parser written in C.
    - Uses top-down operator precedence scheme from Pratt's paper:
    - https://web.archive.org/web/20151223215421/http://hall.org.ua/halls/wizzard/pdf/Vaughan.Pratt.TDOP.pdf 
    - Weaves a binary, abstract syntax tree.

- Hand coded code generator.
    - Uses register-to-register model for data (generated assembly maximizes register use,
    leading to somewhat optimized code).
    - Uses tree pattern matching to generate assembly. This method is *bad*, I haven't
    implemented a value system to weigh the costs of different instructions.
        - The positive tradeoff however is fast compilation times.

## TO DO
### Lexer:
- Better handling of indentation levels.
- Handle floating point.
- Handle strings.

### Parser:
- Extend parser to handle function definitions and function calls.
- Handle floating point.
- Handle strings.

### Code Generator:
- Write global register allocator so that we can actually assemble are programs.
- Handle floating point.
- Handle strings.
- Handle instruction selection for function definitions and function calls.
