# Python-Compiler


## GOAL

This program is meant to take as input a *high level* python program and produce as output
*low level* x86 assembly which can then be assembled, linked, and loaded into memory to emit
a functional program.


## DESIGN

- Written in C for portability and speed (also because I wanted to learn C).
- Scanner generated by flex. In future may write from scratch or use something more relevant/modern.
- Parser generated by Bison. Will definitely write from scratch in future after MVP reached.


## TO DO

### Add scanning for:

- Function and variable identifiers.
- Whitespace.
- Floats, integers, other bases such as binary.
- Strings.

### Generate:
- scanner.c
- parser.c
