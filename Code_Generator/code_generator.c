/*
 * A Tree Tiling Code Generator.
 */


#include "code_generator.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>


#define SYMBOL_TABLE_SIZE	31


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
#define ASM_FORLOOPID		11
#define ASM_IN			12
#define ASM_FOR			13
#define ASM_FORCLOSE		14


// tile() performs a postorder traversal via recursion. Passes off execution to ofp, label
// which annotates nodes with a boolean. The boolean represents whether or not we can
// transform the node into a register, which represents a valid instruction in our 
// register to register model.
void
tile (TreeNode *n, TreeNode *root, size_t *regcount, FILE *ofp,
      StNode **symbol_table, int *loopcount)
{
	if (is_tree_node_empty (n) == false)
	{
		tile (n->left, root, regcount, ofp, symbol_table, loopcount);
		tile (n->right, root, regcount, ofp, symbol_table, loopcount);
		label (n, root, regcount, ofp, symbol_table, loopcount);
	}
	else
	{
		label (n, root, regcount, ofp, symbol_table, loopcount);
	}
}


// label() annotates the node passed and then calls generate_asm to generate virtual
// assembly, i.e. assembly without real registers allocated (only instruction selection
// is performed).
void
label (TreeNode *n, TreeNode *root, size_t *regcount, FILE *ofp,
		StNode **symbol_table, int *loopcount)
{
	switch (n->contents.type)
	{
		case TOKEN_IDENTIFIER:
			// We have to handle lhs and rhs identifiers, i.e.
			// assignment and by reference.
			if (is_leftside (n, root) == false)
			{
				// Special handling for id's found in for
				// statements.
				if (get_parent_type (n, root) == TOKEN_FOR)
				{
					n->reg = true;
					n->rule_number = ASM_FORLOOPID;
					generate_vasm (n->rule_number, n,
							regcount, ofp,
							symbol_table,
							loopcount);
					break;
				}
				else
				{
					n->reg = true;
					n->rule_number = ASM_IDENTIFIER_RS;
					generate_vasm (n->rule_number, n,
							regcount, ofp,
							symbol_table,
							loopcount);
					break;
				}
			}
			else
			{
				// Can be reduced to register.
				n->reg = true;
				// Use rule 1 to rewrite to register.
				n->rule_number = ASM_IDENTIFIER;
				generate_vasm (n->rule_number, n, regcount, ofp,
						symbol_table, loopcount);
				break;
			}
		case TOKEN_INTEGER:
			n->reg = true;
			n->rule_number = ASM_CONSTANT;
			generate_vasm (n->rule_number, n, regcount, ofp,
					symbol_table, loopcount);
			break;
		case TOKEN_ADD:
			if ((n->left->reg == true) && (n->right->reg == true))
			{
				n->reg = true;
				n->rule_number = ASM_ADDITION;
				generate_vasm (n->rule_number, n, regcount, ofp,
						symbol_table, loopcount);
				break;
			}
			else
			{
				n->reg = false;
				printf ("error, could not find tile");
				break;
			}
		case TOKEN_MINUS:
			if ((n->left->reg == true) && (n->right->reg == true))
			{
				n->reg = true;
				n->rule_number = ASM_SUBTRACTION;
				generate_vasm (n->rule_number, n, regcount, ofp,
						symbol_table, loopcount);
				break;
			}
			else
			{
				n->reg = false;
				printf ("error, could not find tile");
				break;
			}
		case TOKEN_MULTIPLICATION:
			if ((n->left->reg == true) && (n->right->reg == true))
			{
				n->reg = true;
				n->rule_number = ASM_MULTIPLICATION;
				generate_vasm (n->rule_number, n, regcount, ofp,
						symbol_table, loopcount);
				break;
			}
			else
			{
				n->reg = false;
				printf ("error, could not find tile for TOKEN_MULT");
				break;
			}
		case TOKEN_DIVISION:
			if ((n->left->reg == true) && (n->right->reg == true))
			{
				n->reg = true;
				n->rule_number = ASM_DIVISION;
				generate_vasm (n->rule_number, n, regcount, ofp,
						symbol_table, loopcount);
				break;
			}
			else
			{
				n->reg = false;
				printf ("Error, could not tile");
				break;
			}
		case TOKEN_EQUALS:
			if ((n->left->reg == true) && (n->right->reg == true))
			{
				n->reg = true;
				n->rule_number = ASM_EQUALS;
				generate_vasm (n->rule_number, n, regcount, ofp,
						symbol_table, loopcount);
				break;
			}
			else
			{
				n->reg = false;
				printf ("error tiling \n");
				break;
			}
		case TOKEN_PRINT:
			// left doesn't matter, tree builds print with args on 
			// right branch and some place holder node on the left.
			// place holder nodes are right paren tokens without lexeme.
			if (n->right->reg == true)
			{
				n->reg = true;
				n->rule_number = ASM_PRINT;
				generate_vasm (n->rule_number, n, regcount, ofp,
						symbol_table, loopcount);
				break;
			}
			else
			{
				n->reg = false;
				printf ("TOKEN_PRINT tile error");
				break;
			}
		case TOKEN_COMMA:
			if ((n->left->reg == true) && (n->left->reg == true))
			{
				n->reg = true;
				n->rule_number = ASM_COMMA;
				generate_vasm (n->rule_number, n, regcount, ofp,
						symbol_table, loopcount);
				break;
			}
			break;
		case TOKEN_RANGE:
			// We will let other tokens like FOR and IN handle
			// this. They are higher up in the tree and thus
			// have more information to work with.
			n->reg = true;
			break;
		case TOKEN_IN:
			if ((n->left->reg == true) && (n->right->reg == true))
			{
				n->reg = true;
				n->rule_number = ASM_IN;
				generate_vasm (n->rule_number, n, regcount, ofp,
						symbol_table, loopcount);
				break;
			}
			else
			{
				n->reg = false;
				fprintf (stderr, "Error, tile() TOKEN_IN\n");
				break;
			}
		case TOKEN_FOR:
			if ((n->left->reg == true) && (n->right->reg == true))
			{
				n->reg = true;
				n->rule_number = ASM_FOR;
				generate_vasm(n->rule_number, n, regcount, ofp,
						symbol_table, loopcount);
				break;
			}
			else
			{
				n->reg = false;
				fprintf (stderr, "Error, tile(), TOKEN_FOR\n");
				break;
			}
		case TOKEN_RIGHT_PAREN:
			break;
		// Newline is somewhat important. It will probably handle
		// multiple things, but for now it definitely sure handles
		// closing for loops.
		case TOKEN_NEWLINE:
			if ((n->left->contents.type == TOKEN_FOR)
					&& (n->right->contents.type
						== TOKEN_TAB))
			{
				n->rule_number = ASM_FORCLOSE;
				n->reg = true;
				generate_vasm(n->rule_number, n, regcount, ofp,
						symbol_table, loopcount);
				break;
			}
			else
			{
				break;
			}
			break;
		case TOKEN_TAB:
			n->reg = true;
			break;
		default:
			fprintf(stderr, "error in label\n");
			break;
	}
}


VasmInstruction *
generate_vasm(size_t r, TreeNode *n, size_t *regcount, StNode **symbol_table,
		int *loopcount, VasmInstruction *vasm)
{
	VasmInstruction *return_list = vasm;
	switch (r)
	{
		// Identifiers
		case ASM_IDENTIFIER:
			return_list = insert_vasm_instruction (
					vasm, spawn_vasm_op (VASM_XOR,
						*regcount, *regcount, true, true));
			n->register_number = *regcount;
			*regcount += 1;
			return return_list;
			break;
		// Non-lhs variable assignment
		case ASM_IDENTIFIER_RS:
			return_list = insert_vasm_instruction (
					vasm, spawn_vasm_op (VASM_MOV,
						*regcount, st_search
						(symbol_table, n), true, true));
			n->register_number = *regcount;
			*regcount += 1;
			break;
		// Constants
		case ASM_CONSTANT:
			fprintf (ofp, "mov\t\treg(%zu),\t%.*s\n", *regcount,
					n->contents.length,
					n->contents.first_char);
			n->register_number = *regcount;
			// Associate register number in symbol table.
			symbol_table = st_insert (symbol_table, n,
					n->register_number);
			*regcount += 1;
			break;
		// Addition
		case ASM_ADDITION:
			fprintf (ofp, "add\t\treg(%zu),\treg(%zu)\n",
					n->left->register_number,
					n->right->register_number);
			n->register_number = n->left->register_number;
			symbol_table = st_insert (symbol_table, n,
					n->left->register_number);
			break;
		// Subtraction
		case ASM_SUBTRACTION:
			fprintf (ofp, "sub\t\treg(%zu),\treg(%zu)\n",
					n->left->register_number,
					n->right->register_number);
			n->register_number = n->left->register_number;
			symbol_table = st_insert (symbol_table, n,
					n->left->register_number);
			break;
		// Multiplication
		case ASM_MULTIPLICATION:
		fprintf (ofp, "mul\t\treg(%zu),\treg(%zu)\n",
				n->left->register_number,
				n->right->register_number);
			n->register_number = n->left->register_number;
			symbol_table = st_insert (symbol_table, n,
					n->left->register_number);
			break;
		// Division
		case ASM_DIVISION:
			fprintf (ofp, "mov\t\treg(%zu),\trdx\n", *regcount);
			*regcount += 1;
			fprintf (ofp, "xor\t\trdx,\trdx\n");
			fprintf (ofp, "mov\t\trax,\treg(%zu)\n",
					n->left->register_number);
			fprintf (ofp, "mov\t\treg(%zu),\treg(%zu)\n",
					*regcount, n->right->register_number);
			fprintf (ofp, "div\t\treg(%zu)\n", *regcount);
			*regcount += 1;
			break;
		// Equals
		case ASM_EQUALS:
			fprintf (ofp, "mov\t\treg(%zu),\treg(%zu)\n",
					n->left->register_number,
					n->right->register_number);
			n->register_number = n->left->register_number;
			symbol_table = st_insert (symbol_table, n->left,
					n->right->register_number);
			break;
		case ASM_PRINT:
			// Handling for comma delimited list of print args.
			if (n->right->rule_number == ASM_COMMA)
			{
				// If right is a comma, get the argument array
				// rooted at comma.
				TreeNode *arg_array = get_arg_array (n->right);
				
				size_t num_nodes = ((count_tree_nodes (n->right)
							+ 1) / 2);

				// Once we have the array of args, we call some
				// assembly function that combines the args
				// into a single string buffer. This assembly
				// function, cat_args(), takes all of its
				// arguments on the stack, so lets do that
				// before calling.

				// We can call out concatenation function now
				fprintf (ofp, "call\t\tcat_args\n");

				// String buffer in rax.
				fprintf (ofp, "mov\t\trcx\trax\n");
				fprintf (ofp, "call\t\tprintstr\n");
				break;
			}
			else if (n->right->reg == true)
			{
				fprintf (ofp, "mov\t\trcx,\treg(%zu)\n",
						st_search (symbol_table, n->right));
				fprintf (ofp, "call\t\tprintstr\n");
				break;
			}
			break;
		case ASM_COMMA:
			if (n->left->rule_number != ASM_COMMA)
			{
				fprintf (ofp, "push\t\treg(%zu)\n",
						n->left->register_number);
			}
			fprintf (ofp, "push\t\treg(%zu)\n",
					n->right->register_number);
			break;
		// Handling for variable declarations within for loops.
		case ASM_FORLOOPID:
			fprintf (ofp, "mov\t\treg(%zu), 0\n", *regcount);
			n->register_number = *regcount;
			n->reg = true;
			symbol_table = st_insert (symbol_table, n,
					n->register_number);
			*regcount += 1;
			break;
		case ASM_IN:
			// Standard 'for x in range(y):' esque statement.
			if ((n->left->contents.type == TOKEN_RANGE)
					&& (n->right->contents.type = TOKEN_INTEGER))
			{
				// Get register number from integer already in
				// register.
				n->register_number = n->right->register_number;
				n->reg = true;
				break;
			}
			break;
		case ASM_FOR:
			fprintf (ofp, "dec\t\treg(%zu)\n",
					n->right->register_number);
			fprintf (ofp, "loop%dstart:\n", *loopcount);
			fprintf (ofp, "cmp\t\treg(%zu), reg(%zu)\n",
					n->left->register_number,
					n->right->register_number);
			fprintf (ofp, "je\t\tloop%dend\n", *loopcount);
			n->reg = true;
			n->register_number = n->left->register_number;
			break;
		case ASM_FORCLOSE:
			if ((n->left->reg == true) && (n->right->reg == true))
			{
				fprintf (ofp, "inc\t\treg(%zu)\n", n->left->register_number);
				fprintf (ofp, "jmp\t\tloop%dstart\n", *loopcount);
				fprintf (ofp, "loop%dend:\n", *loopcount);
				*loopcount += 1;
			}
			else
			{
				fprintf (stderr, "Error in gen_asm ASM_FORCLOSE\n");
				break;
			}
			break;
		default:
			fprintf (ofp, "error in generate_asm\n");
			break;
	}
}


bool
is_leftside (TreeNode *node, TreeNode *root)
{
	StackNode *stack = NULL;
	TreeNode *p = root;
	size_t return_num = 0;
	for (;;)
	{
		if (is_tree_node_empty (p) == true)
		{
			return_num++;
			if (is_stack_empty (&stack) == true)
			{
				return false;
				break;
			}
			else
			{
				p = pop (&stack);
				return_num++;
				p = p->right;
			}
		}
		else
		{
			if ((p->contents.type == TOKEN_EQUALS)
					&& (p->left == node))
			{
				return true;
				break;
			}
			push (&stack, p);
			p = p->left;
		}
	}
}


TreeNode *
get_arg_array (TreeNode *tree)
{
	// Number of nodes (including delimeters).
	size_t num_nodes = count_tree_nodes (tree);
	TreeNode *root = tree;
	
	// (num_nodes + 1) / 2 because we dont want to store commas in
	// our arg array.
	TreeNode *return_array = malloc (((num_nodes + 1) / 2)
			* sizeof (TreeNode));

	StackNode *stack = NULL;

	for (;;)
	{
		if (is_tree_node_empty (root) == true)
		{
			// Add leaf to arg list if its an arg.
			if (root->contents.type != TOKEN_COMMA)
			{
				return_array = root;
				return_array++;
			}
			// Done.
			if (is_stack_empty (&stack) == true)
			{
				break;
			}
			else
			{
				root = pop (&stack);
				root = root->right;
			}
		}
		else
		{
			push (&stack, root);
			root = root->left;
		}
	}
	return return_array;
}


#define ASCII_OFFSET 96
size_t
st_convert_string (const char *string, int string_len)
{
	size_t return_num = 0;
	size_t curr = (size_t) (string[0] - ASCII_OFFSET);
	return_num = curr;
	for (int i = 1; i < string_len; i++)
	{
		if (curr > 9)
		{
			curr *= 100;
			curr += ((size_t) string[i] - ASCII_OFFSET);
		}
		else
		{
			curr *= 10;
			curr += ((size_t) string[i] - ASCII_OFFSET);
		}
	}
	return_num = curr;
	return return_num;
}


StNode *
st_spawn_node (TreeNode *node, int value)
{
	StNode *return_node = malloc (sizeof (*return_node));
	return_node->node = node;
	return_node->contents = value;
	return_node->next = NULL;
	return return_node;
}


StNode **
st_insert (StNode **table, TreeNode *entry, int value)
{
	StNode **return_table = table;
	// Get index via fibonacci hash.
	size_t index = fib_hash (st_convert_string (entry->contents.first_char,
				entry->contents.length));
	// if entry doesn't exist.
	if (st_search (return_table, entry) == -1)
	{
		// Add string to symbol table.
		return_table[index]->lexeme = st_convert_string (
				entry->contents.first_char,
				entry->contents.length);
		// Associate new int.
		return_table[index]->contents = value;
	}
	// If entry does exist.
	else
	{
		// Replace old associated integer with new one.
		return_table[index]->contents = value;
	}
	// Return edited table.
	return return_table;
}


size_t
st_search (StNode **table, TreeNode *node)
{
	size_t index = fib_hash (st_convert_string(
				node->contents.first_char, node->contents.length));
	StNode **temp = table;
	size_t string_hash = st_convert_string (node->contents.first_char,
			node->contents.length);
	// Check if we are already on target.
	if (temp[index]->lexeme == string_hash)
	{
		return temp[index]->contents;
	}
	// If not, traverse linked list.
	while ((temp[index]->next != NULL) && (temp[index]->lexeme != string_hash))
	{
		temp[index] = temp[index]->next;
	}
	// Should be at end of linked list or at target now.
	if (temp[index]->lexeme == string_hash)
	{
		// On target, return associated int.
		return temp[index]->contents;
	}
	else
	{
		// Item not in table, return special numeric.
		return -1;
	}
}


void
st_delete (StNode **table, TreeNode *node)
{
	// Get slot.
	size_t index = fib_hash (st_convert_string (node->contents.first_char,
				node->contents.length));
	// Get lexeme.
	size_t target_lexeme = st_convert_string (node->contents.first_char,
			node->contents.length);
	// Get return table.
	// If where are already on target.
	if (table[index]->lexeme == target_lexeme)
	{
		// Delete the entry.
		table[index] = table[index]->next;
	// Otherwise, traverse the linked list and find element.
	}
	else
	{
		StNode *prev = table[index];
		while ((table[index]->next != NULL)
				&& (table[index]->lexeme != target_lexeme))
		{
			prev = table[index];
			table[index] = table[index]->next;
		}
		// Should be on target now, delete element.
		prev->next = table[index]->next;
	}
}


void
st_kill_table (StNode **input_table)
{
	for (size_t i = 0; i < SYMBOL_TABLE_SIZE; i++)
	{
		free (input_table[i]);
	}
	free (input_table);
}


StNode **
st_spawn_table ()
{
	StNode **return_table = malloc (SYMBOL_TABLE_SIZE * sizeof (*return_table));
	for (int i = 0 ; i < SYMBOL_TABLE_SIZE ; i++)
	{
		return_table[i] = malloc (sizeof (StNode));
	}
	return return_table;
}


// For symbol table. Fibonacci hash function. TAOCP vol 3. 2nd ed, pp 516
// listing 5.
size_t
fib_hash (size_t input)
{
	// word size (2^64) * nearest int to phi^-1 * wordsize relativeley prime
	// to wordsize (0.618...)
	size_t a = 11400714819323198485llu;
	size_t output = a * input;
	// 64 - 59 = 5 bytes = 31 max hashtable slots.
	// Adjust bit shift as needed. In TAOCP we shift left because MIX is
	// big-endian, here we have to shift right.
	output = output >> 59;
	return output;
}


VasmInstruction *
spawn_vasm_op (VasmOperation inp_op, VasmRegister inp_regl,
		VasmRegister inp_regr, bool inp_regl_reg, bool inp_regr_reg)
{
	VasmInstruction *out_inst = malloc (sizeof (VasmInstruction));
	out_inst->op = inp_op;
	out_inst->regl = inp_regl;
	out_inst->regr = inp_regr;
	out_inst->regr_reg = inp_regr_reg;
	out_inst->regl_reg = inp_regl_reg;
	out_inst->label_num = -1;
	out_inst->next = NULL;
	return out_inst;
}


VasmInstruction *
insert_vasm_instruction (VasmInstruction *inp_list, VasmInstruction *inp_inst)
{
	// Case 0: Empty list
	if (inp_list == NULL)
	{
		// Return instruction
		return inp_inst;
	}
	else
	{
		// Store head
		VasmInstruction *head = inp_list;
		// Traverse
		while (inp_list->next != NULL)
		{
			inp_list = inp_list->next;
		}
		// Add
		inp_list->next = inp_inst;
		return head;
	}
}
