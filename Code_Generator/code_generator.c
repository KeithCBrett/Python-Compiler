/*
 * A Tree Tiling Code Generator.
 */


#include "code_generator.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>


#define ASM_IDENTIFIER		1
#define ASM_CONSTANT		2
#define ASM_ADDITION		3
#define ASM_MULTIPLICATION	4
#define ASM_EQUALS		5
#define ASM_SUBTRACTION		6
#define ASM_DIVISION		7
#define ASM_IDENTIFIER_RS	8
#define ASM_PRINT		9
#define ASM_COMMA		10


// tile() performs a postorder traversal via recursion. Passes off execution to ofp, label
// which annotates nodes with a boolean. The boolean represents whether or not we can
// transform the node into a register, which represents a valid instruction in our 
// register to register model.
void tile(TreeNode *n, TreeNode *root, int *regcount, FILE *ofp){
	if (is_tree_node_empty(n) == false) {
		tile(n->left, root, regcount, ofp);
		tile(n->right, root, regcount, ofp);
		label(n, root, regcount, ofp);
	} else {
		label(n, root, regcount, ofp);
	}
}


// label() annotates the node passed and then calls generate_asm to generate virtual
// assembly, i.e. assembly without real registers allocated (only instruction selection
// is performed).
void label(TreeNode *n, TreeNode *root, int *regcount, FILE *ofp){
	switch (n->contents.type) {
		case TOKEN_IDENTIFIER:
			// We have to handle lhs and rhs identifiers, i.e. assignment
			// and by reference.
			if (is_leftside(n, root) == false) {
				n->reg = true;
				n->rule_number = ASM_IDENTIFIER_RS;
				generate_asm(n->rule_number, n, regcount, ofp);
				break;
			} else {
				// Can be reduced to register.
				n->reg = true;
				// Use rule 1 to rewrite to register.
				n->rule_number = ASM_IDENTIFIER;
				generate_asm(n->rule_number, n, regcount, ofp);
				break;
			}
		case TOKEN_INTEGER:
			n->reg = true;
			n->rule_number = ASM_CONSTANT;
			generate_asm(n->rule_number, n, regcount, ofp);
			break;
		case TOKEN_ADD:
			if ((n->left->reg == true) && (n->right->reg == true)) {
				n->reg = true;
				n->rule_number = ASM_ADDITION;
				generate_asm(n->rule_number, n, regcount, ofp);
				break;
			} else {
				n->reg = false;
				printf("error, could not find tile");
				break;
			}
		case TOKEN_MINUS:
			if ((n->left->reg == true) && (n->right->reg == true)) {
				n->reg = true;
				n->rule_number = ASM_SUBTRACTION;
				generate_asm(n->rule_number, n, regcount, ofp);
				break;
			} else {
				n->reg = false;
				printf("error, could not find tile");
				break;
			}
		case TOKEN_MULTIPLICATION:
			if ((n->left->reg == true) && (n->right->reg == true)) {
				n->reg = true;
				n->rule_number = ASM_MULTIPLICATION;
				generate_asm(n->rule_number, n, regcount, ofp);
				break;
			} else {
				n->reg = false;
				printf("error, could not find tile for TOKEN_MULT");
				break;
			}
		case TOKEN_DIVISION:
			if ((n->left->reg == true) && (n->right->reg == true)) {
				n->reg = true;
				n->rule_number = ASM_DIVISION;
				generate_asm(n->rule_number, n, regcount, ofp);
				break;
			} else {
				n->reg = false;
				printf("Error, could not tile");
				break;
			}
		case TOKEN_EQUALS:
			if ((n->left->reg == true) && (n->right->reg == true)) {
				n->reg = true;
				n->rule_number = ASM_EQUALS;
				generate_asm(n->rule_number, n, regcount, ofp);
				break;
			} else {
				n->reg = false;
				printf("error tiling \n");
				break;
			}
		case TOKEN_PRINT:
			// left doesn't matter, tree builds print with args on 
			// right branch and some place holder node on the left.
			// place holder nodes are right paren tokens without lexeme.
			if (n->right->reg == true) {
				n->reg = true;
				n->rule_number = ASM_PRINT;
				generate_asm(n->rule_number, n, regcount, ofp);
				break;
			} else {
				n->reg = false;
				printf("TOKEN_PRINT tile error");
				break;
			}
		case TOKEN_COMMA:
			if ((n->left->reg == true) && (n->left->reg == true)) {
				n->reg = true;
				n->rule_number = ASM_COMMA;
				generate_asm(n->rule_number, n, regcount, ofp);
				break;
			}
			break;
		default:
			fprintf(ofp, "error in label\n");
			break;
	}
}


void generate_asm(size_t r, TreeNode *n, int *regcount, FILE *ofp){
	switch (r) {
		// Identifiers
		case ASM_IDENTIFIER:
			fprintf(ofp, "xor\t\treg(%d), reg(%d)\n", *regcount, *regcount);
			n->register_number = *regcount;
			*regcount += 1;
			break;
		// Non-lhs variable assignment
		case ASM_IDENTIFIER_RS:
			fprintf(ofp, "mov\t\treg(%d), reg(%d)\n", *regcount, n->register_number);
			n->register_number = *regcount;
			*regcount += 1;
			break;
		// Constants
		case ASM_CONSTANT:
			fprintf(ofp, "mov\t\treg(%d), %.*s\n", *regcount,
					n->contents.length, n->contents.first_char);
			n->register_number = *regcount;
			*regcount += 1;
			break;
		// Addition
		case ASM_ADDITION:
			fprintf(ofp, "add\t\treg(%d), reg(%d)\n", n->left->register_number,
					n->right->register_number);
			n->register_number = n->left->register_number;
			break;
		// Subtraction
		case ASM_SUBTRACTION:
			fprintf(ofp, "sub\t\treg(%d), reg(%d)\n", n->left->register_number,
					n->right->register_number);
			n->register_number = n->left->register_number;
			break;
		// Multiplication
		case ASM_MULTIPLICATION:
		fprintf(ofp, "mul\t\treg(%d), reg(%d)\n", n->left->register_number,
					n->right->register_number);
			n->register_number = n->left->register_number;
			break;
		// Division
		case ASM_DIVISION:
			fprintf(ofp, "mov\t\treg(%d), rdx\n", *regcount);
			*regcount += 1;
			fprintf(ofp, "xor\t\trdx, rdx\n");
			fprintf(ofp, "mov\t\trax, reg(%d)\n", n->left->register_number);
			fprintf(ofp, "mov\t\treg(%d), reg(%d)\n", *regcount, n->right->register_number);
			fprintf(ofp, "div\t\treg(%d)\n", *regcount);
			*regcount += 1;
			break;
		// Equals
		case ASM_EQUALS:
			fprintf(ofp, "mov\t\treg(%d), reg(%d)\n", n->left->register_number,
					n->right->register_number);
			n->register_number = n->left->register_number;
			break;
		case ASM_PRINT:
			// Handling for comma delimited list of print args.
			if (n->right->rule_number == ASM_COMMA) {
				// If right is a comma, get the argument array
				// rooted at comma.
				TreeNode *arg_array = get_arg_array(n->right);
				
				size_t num_nodes = ((count_tree_nodes(n->right) + 1) / 2);

				// Once we have the array of args, we call some assembly
				// function that combines the args into a single string
				// buffer.
				// This assembly function, cat_args(), takes all of its
				// arguments on the stack, so lets do that before calling.

				// We can call out concatenation function now
				fprintf(ofp, "call\t\tcat_args\n");
				break;
			}
			break;
		case ASM_COMMA:
			if (n->left->rule_number != ASM_COMMA) {
				fprintf(ofp, "push\t\treg(%d)\n", n->left->register_number);
			}
			fprintf(ofp, "push\t\treg(%d)\n", n->right->register_number);
			break;
		default:
			fprintf(ofp, "error in generate_asm\n");
			break;
	}
}


bool is_leftside(TreeNode *node, TreeNode *root){
	StackNode *stack = NULL;
	TreeNode *p = root;
	size_t return_num = 0;
	for (;;) {
		if (is_tree_node_empty(p) == true) {
			return_num++;
			if (is_stack_empty(&stack) == true) {
				return false;
				break;
			} else {
				p = pop(&stack);
				return_num++;
				p = p->right;
			}
		} else {
			if ((p->contents.type == TOKEN_EQUALS) && (p->left == node)) {
				return true;
				break;
			}
			push(&stack, p);
			p = p->left;
		}
	}
}


TreeNode *get_arg_array(TreeNode *tree){
	// Number of nodes (including delimeters).
	size_t num_nodes = count_tree_nodes(tree);
	TreeNode *root = tree;
	
	// (num_nodes + 1) / 2 because we dont want to store commas in our arg array.
	TreeNode *return_array = malloc(((num_nodes + 1) / 2) * sizeof(TreeNode));

	StackNode *stack = NULL;

	for (;;) {
		if (is_tree_node_empty(root) == true) {
			// Add leaf to arg list if its an arg.
			if (root->contents.type != TOKEN_COMMA) {
				return_array = root;
				return_array++;
			}
			// Done.
			if (is_stack_empty(&stack) == true) {
				break;
			} else {
				root = pop(&stack);
				root = root->right;
			}
		} else {
			push(&stack, root);
			root = root->left;
		}
	}
	return return_array;
}
