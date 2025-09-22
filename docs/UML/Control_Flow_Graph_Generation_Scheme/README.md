# Synopsis
- The following UML activity diagrams describe the process the compiler uses
to generate control flow graphs from a linear I.R. (VASM).

## This scheme assumes the following data types:
```
typedef struct
CFGNode
{
    VasmInstruction *contents;
    size_t color;
    struct CFGNode *left;
    struct CFGNode *right;
}
CFGNode;
```
// Node of our control-flow graph. Based off what I have seen of x64,
// a conditional jump can have at most two targets. Take for example this
// small VASM snippet:
//
// LC0:
// mov      r(5), 5
// cmp      r(5), 2
// jle      LC0
// call     printstr
//
// To further illustrate this concept, let us also split the program into
// basic blocks:
//
// Block I
// ------------------
// LC0:
// mov      r(5), 5
// cmp      r(5), 4
// jle      LC0
// ------------------
//
// Block II
// ------------------
// call     printstr
// ------------------
//
// If we look above, when the 'jle' operation is executed, control-flow is
// split in two directions. If r(5) is less than or equal to 4, control flow
// re-enters into block I from block I. To capture this we draw the right edge
// to block I. Otherwise, control-flow continues to block II, which we draw as
// the left edge in our scheme.
