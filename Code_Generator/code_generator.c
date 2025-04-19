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


// tile() performs a postorder traversal via recursion. Passes off execution
// to ofp, label which annotates nodes with a boolean. The boolean represents
// whether or not we can transform the node into a register, which represents
// a valid instruction in our register to register model.
void
tile (TreeNode *n, TreeNode *root, size_t *regcount,
      StNode **symbol_table, size_t *loopcount, VasmInstruction **vasm,
      IndentLL *indent_level, size_t *line_num)
{
	if (is_tree_node_empty (n) == false)
	{
		tile
			(n->left, root, regcount, symbol_table, loopcount,
			 vasm, indent_level, line_num);
		tile
			(n->right, root, regcount, symbol_table, loopcount,
			 vasm, indent_level, line_num);
		label
			(n, root, regcount, symbol_table, loopcount,
			 vasm, indent_level, line_num);
	}
	else
	{
		label
			(n, root, regcount, symbol_table, loopcount,
			 vasm, indent_level, line_num);
	}
}


// label() annotates the node passed and then calls generate_asm to generate
// virtual assembly, i.e. assembly without real registers allocated (only
// instruction selection is performed).
void
label (TreeNode *n, TreeNode *root, size_t *regcount, StNode **symbol_table,
		size_t *loopcount, VasmInstruction **vasm,
		IndentLL *indent_level, size_t *line_num)
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
					generate_vasm
						(n->rule_number, n, regcount,
						 symbol_table, loopcount,
						 vasm, indent_level,
						 line_num);
					break;
				}
				else if (get_parent_type (n, root)
						== TOKEN_PRINT)
				{
					n->reg = true;
					break;
				}
				else
				{
					n->reg = true;
					n->rule_number = ASM_IDENTIFIER_RS;
					generate_vasm
						(n->rule_number, n, regcount,
						 symbol_table, loopcount,
						 vasm, indent_level,
						 line_num);
					break;
				}
			}
			else
			{
				// Can be reduced to register.
				n->reg = true;
				// Use rule 1 to rewrite to register.
				n->rule_number = ASM_IDENTIFIER;
				generate_vasm
					(n->rule_number, n, regcount,
					 symbol_table, loopcount,
					 vasm, indent_level,
					 line_num);
				break;
			}
		case TOKEN_INTEGER:
			n->reg = true;
			n->rule_number = ASM_CONSTANT;
			generate_vasm
				(n->rule_number, n, regcount, symbol_table,
				 loopcount, vasm, indent_level,
				 line_num);
			break;
		case TOKEN_ADD:
			if ((n->left->reg == true) && (n->right->reg == true))
			{
				n->reg = true;
				n->rule_number = ASM_ADDITION;
				generate_vasm
					(n->rule_number, n, regcount,
					 symbol_table, loopcount,
					 vasm, indent_level, line_num);
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
				generate_vasm
					(n->rule_number, n, regcount,
					 symbol_table, loopcount,
					 vasm, indent_level, line_num);
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
				generate_vasm
					(n->rule_number, n, regcount,
					 symbol_table, loopcount,
					 vasm, indent_level, line_num);
				break;
			}
			else
			{
				n->reg = false;
				printf
				("error, could not find tile for TOKEN_MULT");
				break;
			}
		case TOKEN_DIVISION:
			if ((n->left->reg == true) && (n->right->reg == true))
			{
				n->reg = true;
				n->rule_number = ASM_DIVISION;
				generate_vasm
					(n->rule_number, n, regcount,
					 symbol_table, loopcount,
					 vasm, indent_level, line_num);
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
				generate_vasm
					(n->rule_number, n, regcount,
					 symbol_table, loopcount,
					 vasm, indent_level, line_num);
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
			// place holder nodes are right paren tokens without
			// lexeme.
			if (n->right->reg == true)
			{
				n->reg = true;
				n->rule_number = ASM_PRINT;
				generate_vasm
					(n->rule_number, n, regcount,
					 symbol_table, loopcount,
					 vasm, indent_level, line_num);
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
				generate_vasm
					(n->rule_number, n, regcount,
					 symbol_table, loopcount,
					 vasm, indent_level, line_num);
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
				generate_vasm
					(n->rule_number, n, regcount,
					 symbol_table, loopcount,
					 vasm, indent_level, line_num);
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
				generate_vasm
					(n->rule_number, n, regcount,
					 symbol_table, loopcount,
					 vasm, indent_level, line_num);
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
			if (((n->left->contents.type == TOKEN_FOR)
					&& (n->right->contents.type
						== TOKEN_TAB)
					|| n->is_root))
			{
				n->rule_number = ASM_FORCLOSE;
				n->reg = true;
				n->left->reg = true;
				n->right->reg = true;
				generate_vasm
					(n->rule_number, n, regcount,
					 symbol_table, loopcount,
					 vasm, indent_level, line_num);
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


size_t
convert_constant (int inp_length, const char *inp_string)
{
	char string[inp_length+1];
	for (size_t i = 0; i < inp_length ; i++)
	{
		string[i] = inp_string[i];
	}
	string[inp_length+1] = 0;
	size_t return_num = atoi(string);
	return return_num;
}


void
generate_vasm (size_t r, TreeNode *n, size_t *regcount, StNode **symbol_table,
		size_t *loopcount, VasmInstruction **vasm,
		IndentLL *indent_list, size_t *prev_line_number)
{
	size_t prev_indent_level = get_indent_level
		(indent_list, *prev_line_number);
	size_t curr_indent_level = get_indent_level
		(indent_list, n->contents.line_number);
	if (is_indent_level_dropped (prev_indent_level, curr_indent_level))
	{
		*vasm = insert_vasm_instruction
			(*vasm, spawn_vasm_op
			 (VASM_FOR_EXIT_LABEL, *loopcount, -1, false, false,
			  true, n->contents.line_number));
		*loopcount += 1;
	}
	*prev_line_number = n->contents.line_number;
	switch (r)
	{
		// Identifiers
		case ASM_IDENTIFIER:
			// XOR reg(i), reg(i)
			*vasm = insert_vasm_instruction
				(*vasm, spawn_vasm_op
				 (VASM_XOR, *regcount, *regcount, true, true,
				  false, n->contents.line_number));
			n->register_number = *regcount;
			*regcount += 1;
			break;
		// Non-lhs variable assignment
		case ASM_IDENTIFIER_RS:
			// MOV reg(i), reg(j)
			*vasm = insert_vasm_instruction
				(*vasm, spawn_vasm_op
				 (VASM_MOV, *regcount, st_search
				  (symbol_table, n), true, true, false,
				  n->contents.line_number));
			n->register_number = *regcount;
			*regcount += 1;
			break;
		// Constants
		case ASM_CONSTANT:
			// MOV reg(i), CONST
			*vasm = insert_vasm_instruction
				(*vasm, spawn_vasm_op
				 (VASM_MOV, *regcount, convert_constant
				  (n->contents.length, n->contents.first_char),
				  true, false, false,
				  n->contents.line_number));
			n->register_number = *regcount;
			// Associate register number in symbol table.
			symbol_table = st_insert
				(symbol_table, n, n->register_number);
			*regcount += 1;
			break;
		// Addition
		case ASM_ADDITION:
			*vasm = insert_vasm_instruction
				(*vasm, spawn_vasm_op
				 (VASM_ADD, n->left->register_number,
				  n->right->register_number, true, true, false,
				  n->contents.line_number));
			n->register_number = n->left->register_number;
			symbol_table = st_insert
				(symbol_table, n, n->left->register_number);
			break;
		// Subtraction
		case ASM_SUBTRACTION:
			*vasm = insert_vasm_instruction
				(*vasm, spawn_vasm_op
				 (VASM_SUB, n->left->register_number,
				  n->right->register_number, true, true, false,
				  n->contents.line_number));
			n->register_number = n->left->register_number;
			symbol_table = st_insert
				(symbol_table, n, n->left->register_number);
			break;
		// Multiplication
		case ASM_MULTIPLICATION:
			*vasm = insert_vasm_instruction
				(*vasm, spawn_vasm_op
				 (VASM_MUL, n->left->register_number,
				  n->right->register_number, true, true, false,
				  n->contents.line_number));
			n->register_number = n->left->register_number;
			symbol_table = st_insert
				(symbol_table, n, n->left->register_number);
			break;
		// Division
		case ASM_DIVISION:
			*vasm = insert_vasm_instruction
				(*vasm, spawn_vasm_op
				 (VASM_MOV, *regcount, 4, true, true, false,
				  n->contents.line_number));
			*regcount += 1;
			*vasm = insert_vasm_instruction
				(*vasm, spawn_vasm_op
				 (VASM_XOR, 4, 4, true, true, false,
				  n->contents.line_number));
			*vasm = insert_vasm_instruction
				(*vasm, spawn_vasm_op
				 (VASM_MOV, 1, n->left->register_number, true,
				  true, false, n->contents.line_number));
			*vasm = insert_vasm_instruction
				(*vasm, spawn_vasm_op
				 (VASM_MOV, *regcount,
				  n->right->register_number, true, true, false,
				  n->contents.line_number));
			// NULL because div takes one operand
			*vasm = insert_vasm_instruction
				(*vasm, spawn_vasm_op
				 (VASM_DIV, *regcount, -1, true, true, false,
				  n->contents.line_number));
			*regcount += 1;
			break;
		// Equals
		case ASM_EQUALS:
			*vasm = insert_vasm_instruction
				(*vasm, spawn_vasm_op
				 (VASM_MOV, n->left->register_number,
				  n->right->register_number, true, true, false,
				  n->contents.line_number));
			n->register_number = n->left->register_number;
			symbol_table = st_insert
				(symbol_table, n->left,
				 n->right->register_number);
			break;
		case ASM_PRINT:
			// We are going to treat print like a vasm primitive
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

				// We can call our concatenation function now
				*vasm = insert_vasm_instruction
					(*vasm, spawn_vasm_op
					 (VASM_CALL, VASM_FUNC_CAT_ARGS, -1,
					  false, false, false,
					  n->contents.line_number));

				// String buffer in rax.
				//fprintf (ofp, "mov\t\trcx\trax\n");
				//fprintf (ofp, "call\t\tprintstr\n");
				break;
			}
			else if (n->right->reg == true)
			{
				*vasm = insert_vasm_instruction
					(*vasm, spawn_vasm_op
					 (VASM_MOV, REG_RCX, st_search
					  (symbol_table, n->right), true, true,
					  false, n->contents.line_number));
				*vasm = insert_vasm_instruction
					(*vasm, spawn_vasm_op
					 (VASM_CALL, VASM_FUNC_PRINTSTR, -1,
					  true, false, false,
					  n->contents.line_number));
				break;
			}
			break;
		case ASM_COMMA:
			if (n->left->rule_number != ASM_COMMA)
			{
				*vasm = insert_vasm_instruction
					(*vasm, spawn_vasm_op
					 (VASM_PUSH, n->left->register_number,
					  -1, true, false, false,
					  n->contents.line_number));
			}
			*vasm = insert_vasm_instruction
				(*vasm, spawn_vasm_op
				 (VASM_PUSH, n->right->register_number, -1,
				  true, true, false, n->contents.line_number));
			break;
		// Handling for variable declarations within for loops.
		case ASM_FORLOOPID:
			*vasm = insert_vasm_instruction
				(*vasm, spawn_vasm_op
				 (VASM_XOR, *regcount, *regcount, true, true,
				  false, n->contents.line_number));
			n->register_number = *regcount;
			n->reg = true;
			symbol_table = st_insert
				(symbol_table, n, n->register_number);
			*regcount += 1;
			break;
		case ASM_IN:
			// Standard 'for x in range(y):' esque statement.
			if ((n->left->contents.type == TOKEN_RANGE)
					&& (n->right->contents.type
						= TOKEN_INTEGER))
			{
				// Get register number from integer already in
				// register.
				n->register_number = n->right->register_number;
				n->reg = true;
				break;
			}
			break;
		case ASM_FOR:
			*vasm = insert_vasm_instruction
				(*vasm, spawn_vasm_op
				 (VASM_DEC, n->right->register_number, -1,
				  true, false, false,
				  n->contents.line_number));
			*vasm = insert_vasm_instruction
				(*vasm, spawn_vasm_op
				 (VASM_FOR_ENTRY_LABEL, *loopcount, -1, false,
				  false, true, n->contents.line_number));
			*vasm = insert_vasm_instruction
				(*vasm, spawn_vasm_op
				 (VASM_CMP, n->left->register_number,
				  n->right->register_number, true, true, false,
				  n->contents.line_number));
			*vasm = insert_vasm_instruction
				(*vasm, spawn_vasm_op
				 (VASM_JE, *loopcount, -1, false, false, false,
				  n->contents.line_number));
			*vasm = insert_vasm_instruction
				(*vasm, spawn_vasm_op
				 (VASM_FOR_BODY_LABEL, *loopcount, -1, false,
				  false, true, n->contents.line_number));
			n->reg = true;
			n->register_number = n->left->register_number;
			break;
		case ASM_FORCLOSE:
			if ((n->left->reg == true) && (n->right->reg == true))
			{
				*vasm = insert_vasm_instruction
					(*vasm, spawn_vasm_op
					 (VASM_INC, n->left->register_number,
					  -1, true, false, false,
					  n->contents.line_number));
				*vasm = insert_vasm_instruction
					(*vasm, spawn_vasm_op
					 (VASM_JMP, *loopcount, -1, false,
					  false, false,
					  n->contents.line_number));
				*vasm = insert_vasm_instruction
					(*vasm, spawn_vasm_op
					 (VASM_FOR_EXIT_LABEL, *loopcount, -1,
					  false, false, true,
					  n->contents.line_number));
				// loop exit reached, loop done.
				*loopcount += 1;
			}
			else
			{
				fprintf
				(stderr, "Error in gen_asm ASM_FORCLOSE\n");
				break;
			}
			break;
		default:
			fprintf (stderr, "error in generate_asm\n");
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
		return_table[index]->lexeme = st_convert_string
			(entry->contents.first_char, entry->contents.length);
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
	size_t index = fib_hash
		(st_convert_string
		 (node->contents.first_char, node->contents.length));
	StNode **temp = table;
	size_t string_hash = st_convert_string
		(node->contents.first_char, node->contents.length);
	// Check if we are already on target.
	if (temp[index]->lexeme == string_hash)
	{
		return temp[index]->contents;
	}
	// If not, traverse linked list.
	while ((temp[index]->next != NULL) && (temp[index]->lexeme
				!= string_hash))
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
	StNode **return_table = malloc (SYMBOL_TABLE_SIZE * sizeof
			(*return_table));
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
spawn_vasm_op (VasmOperation inp_op, size_t inp_regl,
		size_t inp_regr, bool inp_regl_reg, bool inp_regr_reg,
		bool inp_label, size_t inp_line_number)
{
	VasmInstruction *out_inst = malloc (sizeof (VasmInstruction));
	out_inst->op = inp_op;
	out_inst->regl = inp_regl;
	out_inst->regr = inp_regr;
	out_inst->regr_reg = inp_regr_reg;
	out_inst->regl_reg = inp_regl_reg;
	out_inst->line_number = inp_line_number;
	if (inp_label)
	{
		if (inp_op == VASM_FOR_BODY_LABEL)
		{
			out_inst->label_data.id = inp_regl;
			out_inst->label_data.type = LT_FOR;
			out_inst->label_data.subtype = LST_FOR_BODY;
		}
		else if (inp_op == VASM_FOR_EXIT_LABEL)
		{
			out_inst->label_data.id = inp_regl;
			out_inst->label_data.type = LT_FOR;
			out_inst->label_data.subtype = LST_FOR_EXIT;
		}
		else if (inp_op == VASM_FOR_ENTRY_LABEL)
		{
			out_inst->label_data.id = inp_regl;
			out_inst->label_data.type = LT_FOR;
			out_inst->label_data.subtype = LST_FOR_ENTRY;
		}
		else
		{
			fprintf (stderr, "Error in spawn_vasm_op (labels)\n");
		}
	}
	else
	{
		out_inst->label_data.id = 0;
		out_inst->label_data.type = 0;
		out_inst->label_data.subtype = 0;
	}
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


bool
left_side_register (VasmInstruction *inp_vasm)
{
	if (inp_vasm->regl_reg && (inp_vasm->regr_reg == false))
	{
		return true;
	}
	else
	{
		return false;
	}
}


bool
right_side_register (VasmInstruction *inp_vasm)
{
	if ((inp_vasm->regl_reg == false ) && inp_vasm->regr_reg)
	{
		return true;
	}
	else
	{
		return false;
	}
}


bool
both_side_register (VasmInstruction *inp_vasm)
{
	if (inp_vasm->regl_reg && inp_vasm->regr_reg)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool
no_registers (VasmInstruction *inp_vasm)
{
	if ((inp_vasm->regl_reg == false) && (inp_vasm->regr_reg == false))
	{
		return true;
	}
	else
	{
		return false;
	}
}

void
output_vasm_file (FILE *inp_file, VasmInstruction *inp_vasm)
{
	// Print whole vasm list.
	for (;;)
	{
		switch (inp_vasm->op)
		{
			case VASM_ADD:
				// Handling for adding two registers.
				if (both_side_register (inp_vasm))
				{
					fprintf
					(inp_file,
					 "add\t\tr(%zu), r(%zu)\n",
					 inp_vasm->regl, inp_vasm->regr);
					break;
				}
				// Handling for adding const into reg.
				else if (left_side_register (inp_vasm))
				{
					fprintf
					(inp_file,
					 "add\t\tr(%zu), %zu\n",
					 inp_vasm->regl, inp_vasm->regr);
					break;
				}
				else
				{
					fprintf
					(stderr,
					 "ERROR in output_vasm_file ");
					fprintf
					(stderr,
					 "VASM_ADD\n");
					break;
				}
				break;
			case VASM_MOV:
				// Case I: moving register into another
				// register.
				if (both_side_register (inp_vasm))
				{
					fprintf
					(inp_file,
					 "mov\t\tr(%zu), r(%zu)\n",
					 inp_vasm->regl,inp_vasm->regr);
					break;
				}
				// Case II: moving const into register.
				else if (left_side_register (inp_vasm))
				{
					fprintf (inp_file,
						"mov\t\tr(%zu), %zu\n",
						inp_vasm->regl,
						inp_vasm->regr);
					break;
				}
				else
				{
					fprintf (stderr,
					"Error in output_vasm_file VASM_MOV\n");
					break;
				}
			case VASM_XOR:
				// Case I: xor register by itself.
				if (both_side_register (inp_vasm))
				{
					fprintf (inp_file,
						"xor\t\tr(%zu), r(%zu)\n",
						inp_vasm->regl,
						inp_vasm->regr);
					break;
				}
				else
				{
					fprintf (stderr,
					"Error in output_vasm_file VASM_XOR\n");
					break;
				}
			case VASM_CALL:
				if (inp_vasm->regl == VASM_FUNC_PRINTSTR)
				{
					fprintf (inp_file,
					"call\t\tprintstr\n");
					break;
				}
				else
				{
					fprintf (stderr,
					"Error in output_vasm_file VASM_CALL\n");
				}
			case VASM_MUL:
				if (both_side_register (inp_vasm))
				{
					fprintf (inp_file,
						"mul\t\tr(%zu), r(%zu)\n",
						inp_vasm->regl,
						inp_vasm->regr);
					break;
				}
				else if (left_side_register (inp_vasm))
				{
					fprintf (inp_file,
						"mul\t\tr(%zu), %zu\n",
						inp_vasm->regl,
						inp_vasm->regr);
					break;
				}
				else
				{
					fprintf (stderr,
					"ERROR in output_vasm_file VASM_MULT\n");
					break;
				}
			case VASM_DEC:
				if (left_side_register (inp_vasm))
				{
					fprintf (inp_file,
					"dec\t\tr(%zu)\n", inp_vasm->regl);
					break;
				}
				else
				{
					fprintf (stderr,
					"Error in output_vasm_file VASM_DEC\n");
					break;
				}
			case VASM_FOR_ENTRY_LABEL:
				if (no_registers (inp_vasm))
				{
					// FLB for loop beginning.
					fprintf
						(inp_file, "FLB%zu:\n",
						 inp_vasm->label_data.id);
					break;
				}
				else
				{
					fprintf (stderr,
					"Error in output_vasm_file ");
					fprintf (stderr,
					"VASM_LABEL_FOR_START\n");
					break;
				}
			case VASM_CMP:
				// Case I: Comparing two registers
				if (both_side_register (inp_vasm))
				{
					fprintf (inp_file,
						"cmp\t\tr(%zu), r(%zu)\n",
						inp_vasm->regl,
						inp_vasm->regr);
					break;
				}
				// Case II: Comparing register to const
				else if (left_side_register (inp_vasm))
				{
					fprintf (inp_file,
						"cmp\t\tr(%zu), %zu\n",
						inp_vasm->regl,
						inp_vasm->regr);
					break;
				}
				// Case III: Error
				else
				{
					fprintf (stderr,
					"Error in output_vasm_file VASM_CMP\n");
					break;
				}
			case VASM_JE:
				if (no_registers (inp_vasm))
				{
					fprintf (inp_file,
						"je\t\tFLE%zu\n",
						inp_vasm->label_data.id);
					break;
				}
				else
				{
					fprintf (stderr,
					"Error in output_vasm_file VASM_JE\n");
					break;
				}
			case VASM_INC:
				if (left_side_register (inp_vasm))
				{
					fprintf (inp_file,
						"inc\t\tr(%zu)\n",
						inp_vasm->regl);
					break;
				}
				else
				{
					fprintf (stderr,
					"Error in output_vasm_file VASM_INC\n");
					break;
				}
			case VASM_JMP:
				if (no_registers (inp_vasm))
				{
					fprintf (inp_file,
						"jmp\t\tFLB%zu\n",
						inp_vasm->regl);
					// for jumps, maybe have label data
					// contain destination info.
					inp_vasm->label_data.id
						= inp_vasm->regl;
					break;
				}
				else
				{
					fprintf (stderr,
					"Error in output_vasm_file VASM_JMP\n");
					break;
				}
			case VASM_FOR_EXIT_LABEL:
				if (no_registers (inp_vasm))
				{
					fprintf (inp_file, "FLE%zu:\n",
							inp_vasm->regl);
					// end of loop, increment for next set
					// of loops
					break;
				}
				else
				{
					fprintf (stderr,
					"Error in output_vasm_file ");
					fprintf (stderr,
					"VASM_LABEL_FOR_END\n");
					break;
				}
			case VASM_FOR_BODY_LABEL:
				if (no_registers (inp_vasm))
				{
					fprintf (inp_file, "FLM%zu:\n",
							inp_vasm->regl);
					break;
				}
				else
				{
					fprintf (stderr,
					"Error in output_vasm_file");
					fprintf (stderr,
					" VASM_FOR_BODY_LABEL\n");
					break;
				}
			default:
				fprintf (stderr,
					"Error in output_vasm_file %u\n",
					inp_vasm->op);
				break;
		}
		if (inp_vasm->next != NULL)
		{
			inp_vasm = inp_vasm->next;
		}
		else
		{
			break;
		}
	}
}


VasmInstruction *
close_loops (IndentLL *inp_indent_list, VasmInstruction *inp_vasm,
		size_t loop_count)
{
	VasmInstruction *prev_vasm = inp_vasm;
	VasmInstruction *head = prev_vasm;
	inp_vasm = inp_vasm->next;
	VasmInstruction *curr_vasm = inp_vasm;
	size_t prev_indent_level = get_indent_level
		(inp_indent_list, prev_vasm->line_number);
	size_t curr_indent_level = get_indent_level
		(inp_indent_list, curr_vasm->line_number);
	while (curr_vasm->next != NULL)
	{
		if (prev_indent_level > curr_indent_level)
		{
			VasmInstruction *temp_next = curr_vasm->next;
			curr_vasm->next = spawn_vasm_op
				(VASM_FOR_EXIT_LABEL, loop_count--, -1, false,
				 false, true, curr_vasm->line_number);
			curr_vasm->next->next = temp_next;
		}
		prev_vasm = curr_vasm;
		curr_vasm = curr_vasm->next;
	}
	if (prev_indent_level > curr_indent_level)
	{
		VasmInstruction *temp_next = curr_vasm->next;
		curr_vasm->next = spawn_vasm_op
			(VASM_FOR_EXIT_LABEL, loop_count--, -1, false,
			 false, true, curr_vasm->line_number);
		curr_vasm->next->next = temp_next;
	}
	// out_list->next is null, top of list reached.
	return head;
}


bool
is_indent_level_dropped (size_t inp_prev, size_t inp_curr)
{
	if (inp_prev > inp_curr)
	{
		return true;
	}
	else
	{
		return false;
	}
}
