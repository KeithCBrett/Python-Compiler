#ifndef parser_h
#define parser_h
#include "../Lexer/lexer.h"


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

typedef struct StackNode{
    TreeNode *contents;
    struct StackNode *next;
} StackNode;

typedef TreeNode* (*SemanticCode)(TreeNode *, StackNode **);


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
 * spawn_parse_token()
 * Input: Takes in a token produced from our lexer module (get_next_token())
 * Output: New struct with more information needed for TDOP parser. Namely
 * this function accociates a precedence level, code to translate if token
 * is used as a prefix operator (nud()), or infix operator (led()).
 */
Rule spawn_parse_token(Token);

/*
 * print_rule()
 * Input: rule create by spawn_rule().
 * Output: Prints debugging information about parse token.
 */
void print_rule(Rule);

/*
 * associate_precedence()
 * Input: TokenType
 * Output: Precedence of token type, use this precendence in a assignment
 * operation.
 */
Precedence associate_precedence(TokenType);

/*
 * parse()
 * Input: a right binding power in the form of a precendence, tree object to build on
 * in form of TreeNode**
 * Output: This function will be recursively called to generate ast. equivalent to 
 * parse(rbp) as described in Pratt's paper
 */
TreeNode *parse(Precedence, TreeNode *, StackNode **);

TreeNode *spawn_node(Token);

TreeNode *led_binary(TreeNode *, StackNode **);
TreeNode *nud_integer(TreeNode *, StackNode **);
void print_tree(TreeNode *);
void traverse_binary_tree(TreeNode *);
TreeNode *led_eof(TreeNode *, StackNode **);
void push(StackNode **, TreeNode *);
StackNode *spawn_stack_node(TreeNode *);
TreeNode *pop(StackNode **);


#endif
