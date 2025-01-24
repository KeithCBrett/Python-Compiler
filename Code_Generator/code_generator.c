/*
 * A Tree Tiling Code Generator.
 */


#include "code_generator.h"
#include <stdio.h>


#define ASM_IDENTIFIER		1
#define ASM_CONSTANT		2	
#define ASM_ADDITION		3
#define ASM_MULTIPLICATION	4
#define ASM_EQUALS		5
#define ASM_SUBTRACTION		6
#define ASM_DIVISION		7


void tile(TreeNode *n, int *regcount){
	if (is_tree_node_empty(n) == false) {
		tile(n->left, regcount);
		tile(n->right, regcount);
		label(n, regcount);
	} else {
		label(n, regcount);
	}
}

void label(TreeNode *n, int *regcount){
	switch (n->contents.type) {
		case TOKEN_IDENTIFIER:
			// Can be reduced to register.
			n->reg = true;
			// Use rule 1 to rewrite to register.
			n->rule_number = ASM_IDENTIFIER;
			generate_asm(n->rule_number, n, regcount);
			break;
		case TOKEN_INTEGER:
			n->reg = true;
			n->rule_number = ASM_CONSTANT;
			generate_asm(n->rule_number, n, regcount);
			break;
		case TOKEN_ADD:
			if ((n->left->reg == true) && (n->right->reg == true)) {
				n->reg = true;
				n->rule_number = ASM_ADDITION;
				generate_asm(n->rule_number, n, regcount);
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
				generate_asm(n->rule_number, n, regcount);
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
				generate_asm(n->rule_number, n, regcount);
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
				generate_asm(n->rule_number, n, regcount);
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
				generate_asm(n->rule_number, n, regcount);
				break;
			} else {
				n->reg = false;
				printf("error tiling \n");
				break;
			}
		default:
			printf("error in label\n");
			break;
	}
}


void generate_asm(size_t r, TreeNode *n, int *regcount){
	switch (r) {
		// Identifiers
		case ASM_IDENTIFIER:
			printf("xor\t\treg(%d), reg(%d)\n", *regcount, *regcount);
			n->register_number = *regcount;
			*regcount += 1;
			break;
		// Constants
		case ASM_CONSTANT:
			printf("mov\t\treg(%d), %.*s\n", *regcount,
					n->contents.length, n->contents.first_char);
			n->register_number = *regcount;
			*regcount += 1;
			break;
		// Addition
		case ASM_ADDITION:
			printf("add\t\treg(%d), reg(%d)\n", n->left->register_number,
					n->right->register_number);
			n->register_number = n->left->register_number;
			break;
		// Subtraction
		case ASM_SUBTRACTION:
			printf("sub\t\treg(%d), reg(%d)\n", n->left->register_number,
					n->right->register_number);
			n->register_number = n->left->register_number;
			break;
		// Multiplication
		case ASM_MULTIPLICATION:
			printf("mul\t\treg(%d), reg(%d)\n", n->left->register_number,
					n->right->register_number);
			n->register_number = n->left->register_number;
			break;
		// Division
		case ASM_DIVISION:
			printf("mov\t\treg(%d), rdx\n", *regcount);
			*regcount += 1;
			printf("xor\t\trdx, rdx\n");
			printf("mov\t\trax, reg(%d)\n", n->left->register_number);
			printf("mov\t\treg(%d), reg(%d)\n", *regcount, n->right->register_number);
			printf("div\t\treg(%d)\n", *regcount);
			*regcount += 1;
			break;
		// Equals
		case ASM_EQUALS:
			printf("mov\t\treg(%d), reg(%d)\n", n->left->register_number,
					n->right->register_number);
			n->register_number = n->left->register_number;
			break;
		default:
			printf("error in generate_asm\n");
			break;
	}
}
