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
} TreeNode;

typedef struct print_node{
    int depth;
    TreeNode *contents;
    struct print_node *next;
} print_node;

typedef struct StackNode{
    TreeNode *contents;
    struct StackNode *next;
} StackNode;

typedef TreeNode* (*SemanticCode)(TreeNode *);


// Requirement for TDOP algorithm, now we know that (+ < *) since multiplication
// has higher precendence.
typedef enum Precedence{
    Prec_EOF = 0,
    Prec_Outcomes,
    Prec_Integers,
    Prec_Equals,
    Prec_Identifiers,
    Prec_AddSub,
    Prec_MultDiv,
    Prec_Unary
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
TreeNode *nud_integer(TreeNode *);
void print_tree(TreeNode *);
void traverse_binary_tree(TreeNode *, print_node **);
TreeNode *led_eof(TreeNode *, StackNode **);
void push(StackNode **, TreeNode *);
StackNode *spawn_stack_node(TreeNode *);
TreeNode *pop(StackNode **);
void initialize_parser();
print_node *spawn_print_node(int, TreeNode *);
void push_print_node(print_node **, print_node *);
print_node pop_print_node(print_node **);
size_t count_tree_nodes(TreeNode *);
TreeNode **fill_array(TreeNode *, TreeNode *, size_t);
TreeNode **preorder(TreeNode *, size_t);


#endif
