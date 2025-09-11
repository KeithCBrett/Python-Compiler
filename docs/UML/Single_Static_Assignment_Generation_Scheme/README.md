# Synopsis
- The following UML activity diagrams describe the process the compiler uses
to generate semipruned single static-assignment form. Essentially we use
dominance information within our control flow graph to make decisions on where
to place phi functions. This allows later segments of the allocator to do less
work, which should lead to faster compilation times (compared to using maximal
SSA form).

https://www.cs.tufts.edu/~nr/cs257/archive/keith-cooper/dom14.pdf

## This scheme assumes the following data types:
typedef struct
CFGNode
{
    VasmInstruction *contents;
    size_t color;
    struct CFGNode *left;
    struct CFGNode *right;
}
CFGNode;
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


typedef struct
PredLL
{
    size_t contents;
    struct PredLL *next;
}
PredLL;
// Linked list of a nodes predecessors (Useful in finding dominance
// information about our graph, which is used to generate not so naive single
// static assignment form (for more information, refer to the portions about
// 'semi-pruned' SSA in 'Engineering A Compiler - Keith Cooper, Linda Torczon'
// (Chapter 9 Section 3)).
// In practice, we will have an array of these linked lists. The array will be
// indexed in such a way, that when a CFG nodes post-order number is entered
// as the index to the array, that same nodes predecessor list will be
// located at that position.


// Linked list to store the dominance frontier of each node in our CFG. In
// practice we will have a relatively positioned array of these linked list,
// where the relative position explicity represents which CFG node said list
// belongs to (based on post-order number).
typdef struct
DominanceFrontier
{
    size_t contents;
    struct DominanceFrontier *next;
}
DominanceFrontier;
