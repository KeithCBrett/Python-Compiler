# Pyco - A PYthon COmpiler
## GOAL
This program is meant to take as input a *high level* python program and produce as output
*low level* x64 assembly which can then be assembled, linked, and loaded into memory to emit
a functional program.

### SPECIFIC GOALS
- A compiler is useless if it cannot emit assembly (or binary) for simple
programs. Construct a set of test programs from the schemes below, and ensure
that semantics of assembly program are *indistinguishable* from CPython
interpreter:

STATUS: [INCOMPLETE]
1. Write a Python program that initializes two variables. Assign these
variables integer values and add them together, and assign the result to
another variable. Call the print function on this last variable to print the
result.
- Demonstrates:
    - Variable assignment.
    - Integer addition.
    - Primitive function calling.

STATUS: [INCOMPLETE]
2. Write a function 'fibonacci(n):'. Have the semantics for this function
produce the nth fibonacci number, and have that be its return value. Have a
main function call the fibonacci function a couple of times and print the 
result.
- Demonstrates:
    - Variable assignment.
    - Integer addition.
    - Primitive function calling.
    - Non-primitive function calling.
    - Main functions.



## DESIGN
- Written in C.

- Hand coded scanner/lexer written in C.
    - Keywords encoded in trie.

- Hand coded parser written in C.
    - Uses top-down operator precedence scheme from Pratt's paper:
    - https://web.archive.org/web/20151223215421/http://hall.org.ua/halls/wizzard/pdf/Vaughan.Pratt.TDOP.pdf 
    - Weaves a binary, abstract syntax tree.

- Hand coded code generator.
    - Uses register-to-register model for data (generated assembly maximizes
    register use, leading to somewhat optimized code).
    - Uses tree pattern matching to generate assembly. This method is *bad*, I
    haven't implemented a value system to weigh the costs of different
    instructions.
        - The positive tradeoff however is fast compilation times. With this
        method, we only calculate one set of semantically correct instructions,
        leading to linear scaling on time based on the number of tokens within our
        given program.

## TO DO
### Lexer:
- Better handling of indentation levels.
    - Scheme: link list of indent levels, 1 for each line.
        - This allows us to remove tabs from ast.
        - This furthers increases memory costs. We can probably confine this to the ast
        generation phase (parsing), since we only need it for checking syntactical validity
        of some program (whether blocks are using a single indentation level).
        - This may also help in the handling of lexical scope.
- Handle floating point.
- Handle strings.
- Handle lexical scope reliably.

### Parser:
- Extend parser to handle function definitions and function calls.
- Handle floating point.
- Handle strings.
- Handle lexical scope reliably.

### Code Generator:
- Write global register allocator so that we can actually assemble are programs.
    - Convert vasm to control flow graph.
    - Convert control flow graph into SSA (single static assignment) form.
        - Once vasm is converted to SSA, we can easily compute the set of live
        ranges.
        - The computed live ranges will act as nodes in our interference graph.
        Interferences between those nodes will be edges in our graph.
            - Once this is done, the problem turns into a coloring problem, i.e.
            (k-coloring graph).
- Handle floating point.
- Handle strings.
- Handle instruction selection for function definitions and function calls.
- Handle lexical scope reliably.

#### IGNORE
Static Single-Assignment: 5.4.2, 8.5.1, 9.3
Control-Flow Graphs: 5.2.2, 8.6.1 5.3.4
