#include "parser.h"
#include "../Lexer/lexer.h"
#include <stdio.h>


/*
 * A top down order precedence parser (Pratt parser).
 * Input: Token stream of the Python language generated by lexer.c/main.c.
 * Output: AST to be translate into lower-level IR by tree-walker
 */


Parser parser;


// This table is based on Nystrom's table, the only difference being the functions
// themselves do not produce bytecode, they instead weave a AST (like Crockford's
// nuds and leds).

// For more information on the table, contact me or refer to Robert Nystrom's 
// Crafting Interpreters, Chapter 17

// For more inforation on the nuds and leds, contact me or refer to Crockford's
// blog below:
// https://www.crockford.com/javascript/tdop/tdop.html


Rule table[] = {
    [TOKEN_INTEGER]        = {Prec_Integers,    nud_integer, NULL},
    [TOKEN_IDENTIFIER]     = {Prec_Identifiers, nud_integer, NULL},
    [TOKEN_ADD]            = {Prec_AddSub,      NULL,        led_binary},
    [TOKEN_EQUALS]         = {Prec_Equals,      NULL,        led_binary},
    [TOKEN_MULTIPLICATION] = {Prec_MultDiv,     NULL,        led_binary},
    [TOKEN_EOF]            = {Prec_EOF,         NULL,        NULL},
};


TreeNode *led_binary(TreeNode *tree){
    TreeNode *new_node = spawn_node(parser.current);
    new_node->right = parse(table[parser.current.type].precedence, tree);
    new_node->left = tree;
    return new_node;
}


TreeNode *nud_integer(TreeNode *tree){
    TreeNode *tree_node = spawn_node(parser.previous);
    parser.current = get_next_token();
    return tree_node;
}


TreeNode *spawn_node(Token t){
    TreeNode *return_node = malloc(sizeof(*return_node));
    return_node->left = NULL;
    return_node->right = NULL;
    return_node->contents = t;
    return return_node;
}


StackNode *spawn_stack_node(TreeNode *t){
    StackNode *return_node = malloc(sizeof(StackNode));
    return_node->contents = t;
    return_node->next = NULL;
    return return_node;
}


void push(StackNode **stack, TreeNode *t){
    StackNode *push_node = spawn_stack_node(t);
    push_node->next = *stack;
    *stack = push_node;
}


TreeNode *pop(StackNode **stack){
    StackNode *temp = *stack;
    TreeNode *return_t = temp->contents;
    *stack = (*stack)->next;
    free(temp);
    return return_t;
}


bool is_stack_empty(StackNode **stack){
    if (*stack == NULL) {
        return true;
    } else {
        return false;
    }
}


bool is_tree_node_empty(TreeNode *node){
    if ((node->left == NULL) && (node->right == NULL)) {
        return true;
    } else {
        return false;
    }
}


print_node *spawn_print_node(int depth, TreeNode *node){
    print_node *return_node = malloc(sizeof(print_node));
    return_node->next = NULL;
    return_node->depth = depth;
    return_node->contents = node;
    return return_node;
}


print_node pop_print_node(print_node **stack){
    print_node *temp = *stack;
    print_node *return_node = temp;
    *stack = (*stack)->next;
    free(temp);
    return *return_node;
}


void push_print_node(print_node **stack, print_node *push_node){
    push_node->next = *stack;
    *stack = push_node;
}


size_t count_tree_nodes(TreeNode *tree){
    StackNode *stack = NULL;
    TreeNode *p = tree;
    size_t return_num = 0;
    for (;;) {
        if (is_tree_node_empty(p) == true) {
            return_num++;
            if (is_stack_empty(&stack) == true) {
                break;
            } else {
                p = pop(&stack);
                return_num++;
                p = p->right;
            }
        } else {
            push(&stack, p);
            p = p->left;
        }
    }
    return return_num;
}


TreeNode **fill_array(TreeNode *array, TreeNode *root, size_t size){
    TreeNode **return_array = malloc(size * sizeof(TreeNode));
    StackNode *stack = NULL;
    StackNode *fillstack = NULL;
    TreeNode *p = root;
    size_t return_num = 0;
    for (;;) {
        if (is_tree_node_empty(p) == true) {
            push(&fillstack, p);
            if (is_stack_empty(&stack) == true) {
                for (size_t i = 0; i < size; i++) {
                    return_array[i] = pop(&fillstack);
                }
                break;
            } else {
                p = pop(&stack);
                push(&fillstack, p);
                p = p->right;
            }
        } else {
            push(&stack, p);
            p = p->left;
        }
    }
    return return_array;
}


TreeNode *parse(Precedence rbp, TreeNode *tree){
    Token t = get_next_token();
    parser.previous = t;
    SemanticCode c = table[t.type].nud;
    if (c == NULL) {
        //printf("error in parse()\n");
    }
    TreeNode *left = c(tree);
    while (rbp <= table[parser.current.type].precedence) {
        if (t.type == TOKEN_EOF) {
            break;
        }
        c = table[parser.current.type].led;
        left = c(left);
    }
    return left;
}
