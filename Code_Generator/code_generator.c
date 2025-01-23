/*
 * A Tree Tiling Code Generator.
 */


#include "code_generator.h"


#define ASM_IDENTIFIER		1
#define ASM_CONSTANT		2	
#define ASM_ADDITION		3
#define ASM_MULTIPLICATION	4
#define ASM_EQUALS		5
#define ASM_SUBTRACTION		6
#define ASM_DIVISION		7


IntllNode *spawn_IntllNode(int input){
	IntllNode *return_node = malloc(sizeof(IntllNode));
	return_node->contents = input;
	return_node->next = NULL;
	return return_node;
}


void IntllEnqueue(int input, IntllNode **queue){
	IntllNode *enqueue_node = spawn_IntllNode(input);
	enqueue_node->next = *queue;
	*queue = enqueue_node;
}


IntllNode *IntllDequeue(IntllNode **queue){
	IntllNode *curr = *queue;
	IntllNode *prev = NULL;
	while (curr->next != NULL) {
		prev = curr;
		curr = curr->next;
	}
	IntllNode *return_node = curr;
	if (prev != NULL) {
		prev->next = NULL;
	}
	free(curr);
	return return_node;
}


void init_regnames(IntllNode **register_name_array){
	for (int i = 0; i < 8; i++) {
		IntllEnqueue(i, register_name_array);
	}
}


void init_reginuse(IntllNode **register_inuse_array){
	for (int i = 0; i < 8; i++) {
		IntllEnqueue(0, register_inuse_array);
	}
}


int get_reg(IntllNode **names, IntllNode **inuse){
	IntllNode *temp_inuse = *inuse;
	IntllNode *temp_names = *names;
	for (int i = 0; i < 7; i++) {
		if (temp_inuse->contents == 1) {
			temp_inuse = temp_inuse->next;
			temp_names = temp_names->next;
		} else {
			return temp_names->contents;
		}
		if (temp_inuse->contents == 1) {
			printf("out of registers\n");
			return 0;
		}
	}
	return temp_names->contents;
}




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
