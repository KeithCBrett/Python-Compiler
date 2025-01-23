#ifndef parser_h
#define parser_h
#include "../Lexer/lexer.h"
#include <stdio.h>


// This parser is based on Vaughan R. Pratt's algorithm. For more information,
// I would recommend the links below:
// Original paper:
    // https://dl.acm.org/doi/pdf/10.1145/512927.512931
// Paper original was based on:
    // https://dl.acm.org/doi/pdf/10.1145/321172.321179 
// Implementation in C can be seen in Robert Nystrom's book, Crafting Interpreters:
    // https://craftinginterpreters.com/compiling-expressions.html
// Nystrom also made one in Java:
    // https://journal.stuffwithstuff.com/2011/03/19/pratt-parsers-expression-parsing-made-easy
// Douglas Crockford has implemented a couple:
    // https://www.jslint.com/
    // https://crockford.com/javascript/tdop/tdop.html


// Semantic code our functions that build our AST. There our two types of
// semantic code (since symbols like (-) can be negation (as in -x) or
// subtraction (as in x - y)).
// These two types are denoted:
//      nud(): This is for prefix semantics, i.e. -x
//      led(): This is for infix semantics, i.e. x - y
// TreeNode represents a node on our AST.


typedef struct TreeNode{
	Token contents;
	// If its a prefix operation, left might be null
	struct TreeNode *left;
	struct TreeNode *right;
	bool reg;
	size_t rule_number;
	int register_number;
} TreeNode;


// Wrapper for a TreeNode, so that we can us it in our linked list based stack.
typedef struct StackNode{
	TreeNode *contents;
	struct StackNode *next;
} StackNode;


typedef TreeNode* (*SemanticCode)(TreeNode *);


// Requirement for TDOP algorithm, now we know that (+ < *) since multiplication
// has higher precendence.
typedef enum Precedence{
	Prec_EOF = 0,
	Prec_Right_Paren,
	Prec_Outcomes,
	Prec_Integers,
	Prec_Equals,
	Prec_Identifiers,
	Prec_AddSub,
	Prec_MultDiv,
	Prec_Unary,
	Prec_Paren
} Precedence;


typedef struct Parser{
	Token previous;
	Token current;
	Token next;
} Parser;


typedef struct Rule{
	Precedence precedence;
	SemanticCode nud; // prefix
	SemanticCode led; // infix
} Rule;


/*
 * parse()
 * Input: a right binding power in the form of a precendence, tree object to build on
 * in form of TreeNode*
 * Output: This function will be recursively called to generate ast. equivalent to 
 * parse(rbp) as described in Pratt's paper. We initialize this function with rbp of 0
 * and it will rapidly build up with recursive calls until a weaker operator is encountered.
 * In which case the parser will fold the tree and continue until eof encoutered.
 */
TreeNode *parse(Precedence, TreeNode *);


/*
 * spawn_node()
 * Input: A token spawned from lexer. Specifically, get_next_token() from lexer.c, lexer.h
 * Output: This token will be the root of some sub tree. One of the led functions will supply
 * it with its left and right branches, parse and operator precedence decides if it has a parent
 * (i.e. is it at the top of the tree (lowest precedence) or bottom (highest precedence).
 */
TreeNode *spawn_node(Token);


/*
 * led_binary()
 * Input: The AST so far
 * Output: Weaves new tree, the parent node is the current operator, its left branch is
 * the AST so far, its right branch is a call to parse with the current operators 
 * precedence. This precendence will ramp with the recursive calls, until weak operator is
 * found. This is when we fold the tree to get the correct semantic meaning.
 */
TreeNode *led_binary(TreeNode *);


/*
 * nud_atom()
 * Input: The AST so far
 * Output: Returns itself as an empty tree node. One of the leds will use this return
 * value as a leaf.
 */
TreeNode *nud_atom(TreeNode *);


TreeNode *nud_paren(TreeNode *);
TreeNode *led_paren(TreeNode *);


/*
 * spawn_stack_node()
 * Input: A TreeNode from our AST.
 * Output: A TreeNode that we can put on a stack. We will use this stack to traverse our tree
 * without recursion. (Donald Knuth TAOCP, VOL I, 3ed. Page 320, Algorithm T).
 */
StackNode *spawn_stack_node(TreeNode *);


/*
 * push()
 * Input: A stack and a TreeNode to be pushed onto the stack.
 * Output: None, stack will have pushed node on top. Access with pop().
 */
void push(StackNode **, TreeNode *);


/*
 * pop()
 * Input: A stack.
 * Output: Node at the top of the stack. Also shifts the stack and frees popped node.
 */
TreeNode *pop(StackNode **);


/*
 * count_tree_nodes()
 * Input: Finished AST
 * Output: Counts number of tree elements and returns that number as size_t. Used for checking bounds
 * for arrays and loops.
 */
size_t count_tree_nodes(TreeNode *);


/*
 * is_tree_node_empty()
 * Input: A node on our tree.
 * Output: This is for checking if we have hit a leaf in our AST (int or var, etc). Used in preorder()
 * to traverse our AST properly.
 */
bool is_tree_node_empty(TreeNode *);


/*
 * preorder()
 * Input: Our AST.
 * Output: This function traverses our AST and visits the nodes in preorder. Allows us to print our
 * AST as something similiar to S-Expressions. (prefix notation).
 */
TreeNode **preorder(TreeNode *, size_t);
TreeNode **postorder(TreeNode *, size_t);


/*
 * is_stack_empty()
 * Input: Stack
 * Output: Boolean telling if stack is empty or not. Useful for checking stack underflows and is
 * also used in our preorder() function to terminate.
 */
bool is_stack_empty(StackNode **);


/*
 * enqueue()
 * Input: Queue, TreeNode to be queued.
 * Output: Nothing, queue passed will have the TreeNode appended to it.
 */
void enqueue(StackNode **, TreeNode *);


/*
 * dequeue()
 * Input: Queue
 * Output: The TreeNode at the front of the queue. This looks o(n) but im too lazy for new method.
 */
TreeNode *dequeue(StackNode **);


#endif
