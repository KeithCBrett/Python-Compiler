/*
 * A Tree Tiling Code Generator.
 */


#include "code_generator.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>


#define SYMBOL_TABLE_SIZE	31


// tile() performs a postorder traversal via recursion. Passes off execution
// to ofp, label which annotates nodes with a boolean. The boolean represents
// whether or not we can transform the node into a register, which represents
// a valid instruction in our register to register model.
void
tile (TreeNode *n, TreeNode *root, StNode **symbol_table,
		VasmInstruction **vasm, size_t *line_num, bool *error,
		CountArray *count_array)
{
	if (is_tree_node_empty (n) == false)
	{
		tile
			(n->left, root, symbol_table, vasm, line_num, error,
			 count_array);
		tile
			(n->right, root, symbol_table, vasm, line_num, error,
			 count_array);
		label
			(n, root, symbol_table, vasm, error, count_array);
	}
	else
	{
		label
			(n, root, symbol_table, vasm, error, count_array);
	}
}


// label() annotates the node passed and then calls generate_asm to generate
// virtual assembly, i.e. assembly without real registers allocated (only
// instruction selection is performed).
// label() should be where all of our tree pattern matching should be done.
void
label (TreeNode *n, TreeNode *root, StNode **symbol_table,
		VasmInstruction **vasm, bool *error, CountArray *count_array)
{
	TreeNode *temp_parent;
	size_t prev_indent_level;
	size_t curr_indent_level;
	TreeNode *temp_node;
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
						(n->rule_number, n, root,
						 symbol_table,
						 vasm, count_array);
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
						(n->rule_number, n, root,
						 symbol_table, vasm,
						 count_array);
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
					(n->rule_number, n, root,
					 symbol_table, vasm, count_array);
				break;
			}
		case TOKEN_INTEGER:
			n->reg = true;
			n->rule_number = ASM_CONSTANT;
			generate_vasm
				(n->rule_number, n, root,
				 symbol_table,
				 vasm, count_array);
			break;
		case TOKEN_ADD:
			if ((n->left->reg == true) && (n->right->reg == true))
			{
				n->reg = true;
				n->rule_number = ASM_ADDITION;
				generate_vasm
					(n->rule_number, n, root,
					 symbol_table,
					 vasm, count_array);
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
					(n->rule_number, n, root,
					 symbol_table,
					 vasm, count_array);
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
					(n->rule_number, n, root,
					 symbol_table,
					 vasm, count_array);
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
					(n->rule_number, n, root,
					 symbol_table,
					 vasm, count_array);
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
					(n->rule_number, n, root,
					 symbol_table,
					 vasm, count_array);
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
					(n->rule_number, n, root,
					 symbol_table,
					 vasm, count_array);
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
					(n->rule_number, n, root,
					 symbol_table,
					 vasm, count_array);
				break;
			}
			break;
		case TOKEN_RANGE:
			// We will let other tokens like FOR and IN handle
			// this. They are higher up in the tree and thus
			// have more information to work with.
			n->rule_number = ASM_DEFER;
			generate_vasm (n->rule_number, n, root,
					symbol_table,
					vasm, count_array);
			break;
		case TOKEN_IN:
			if ((n->left->reg == true) && (n->right->reg == true))
			{
				n->reg = true;
				n->rule_number = ASM_IN;
				generate_vasm
					(n->rule_number, n, root,
					 symbol_table,
					 vasm, count_array);
				break;
			}
			else
			{
				n->reg = false;
				fprintf (stderr, "Error, tile() TOKEN_IN\n");
				break;
			}
		case TOKEN_FOR:
			// We will need to check if this loop contains another
			// nested loop. If this contains a nested loop, we
			// will need to handle the spawning of labels
			// differently than non-nested loops.

			// Handling for nested loop
			if ((n->left->reg == true) && (n->right->reg == true))
			{
				n->reg = true;
				// Inits a counter, jumps to bottom of loop,
				// spawns a label for loop body.
				n->rule_number = ASM_FOR;
				generate_vasm
					(n->rule_number, n, root,
					 symbol_table,
					 vasm, count_array);
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
			// Handling for lack of indented body after for loop.
			// ~NOTE~ Extract this into a mneumonic.
			if (((check_left_branch (n) == TOKEN_FOR)
					&& (check_right_branch (n)
						!= TOKEN_TAB))
					&&
					((check_left_branch (n) == TOKEN_FOR)
					 && (check_next_line (n)
						 != TOKEN_TAB)))
			{
				spawn_python_error (ERROR_INDENTATION,
						n->contents.line_number);
				*error = 1;
				break;
			}

			temp_parent = get_parent (n, root);
			if (check_left_branch (n) == TOKEN_TAB)
			{
				curr_indent_level = n->left->contents.type;
			}
			else
			{
				curr_indent_level = 0;
			}
			if (temp_parent != NULL)
			{
				if (check_left_branch (temp_parent) == TOKEN_TAB)
				{
					prev_indent_level
						= temp_parent->left->
						contents.type;
				}
				else
				{
					prev_indent_level = 0;
				}
			}
			else
			{
				prev_indent_level = 0;
			}

			// Handling for the closing of loops.
			if ((temp_parent == NULL) && (is_for_loop (n)))
			{
				n->reg = true;
				n->rule_number = ASM_CLOSE_LOOP;
				generate_vasm (n->rule_number, n, root,
						symbol_table,
						vasm, count_array);
				break;
			}
			else if ((is_for_loop (n))
					&& (indent_level_dropped
						(prev_indent_level,
						 curr_indent_level)))
			{
				n->reg = true;
				n->rule_number = ASM_CLOSE_LOOP;
				generate_vasm (n->rule_number, n, root,
						symbol_table,
						vasm, count_array);
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
generate_vasm (size_t r, TreeNode *n, TreeNode *root, StNode **symbol_table,
		VasmInstruction **vasm, CountArray *count_array)
{
	VasmInstruction *instruction;

	// For register numbers associated with variables.
	size_t register_num;
	switch (r)
	{
		// Identifiers
		case ASM_IDENTIFIER:
			// XOR reg(i), reg(i)
			instruction = spawn_vasm_op
				(VASM_XOR, count_array->regcount,
				 count_array->regcount, true, true, false);
			*vasm = insert_vasm_instruction (*vasm, instruction);
			n->register_number = count_array->regcount;
			count_array->regcount += 1;
			break;
		// Non-lhs variable assignment
		case ASM_IDENTIFIER_RS:
			// MOV reg(i), reg(j)
			register_num = st_search (symbol_table, n);
			instruction = spawn_vasm_op (VASM_MOV,
					count_array->regcount, register_num,
					true, true, false);
			*vasm = insert_vasm_instruction (*vasm, instruction);
			n->register_number = count_array->regcount;
			count_array->regcount += 1;
			break;
		// Constants
		case ASM_CONSTANT:
			// MOV reg(i), CONST
			register_num = convert_constant (n->contents.length,
					n->contents.first_char);
			instruction = spawn_vasm_op (VASM_MOV,
					count_array->regcount, register_num,
					true, false, false);
			*vasm = insert_vasm_instruction (*vasm, instruction);
			n->register_number = count_array->regcount;
			// Associate register number in symbol table.
			symbol_table = st_insert
				(symbol_table, n, n->register_number);
			count_array->regcount += 1;
			break;
		// Addition
		case ASM_ADDITION:
			instruction = spawn_vasm_op (VASM_ADD,
					n->left->register_number,
					n->right->register_number,
					true, true, false);
			*vasm = insert_vasm_instruction (*vasm, instruction);
			n->register_number = n->left->register_number;
			symbol_table = st_insert
				(symbol_table, n, n->left->register_number);
			break;
		// Subtraction
		case ASM_SUBTRACTION:
			instruction = spawn_vasm_op (VASM_SUB,
					n->left->register_number,
					n->right->register_number,
					true, true, false);
			*vasm = insert_vasm_instruction (*vasm, instruction);
			n->register_number = n->left->register_number;
			symbol_table = st_insert
				(symbol_table, n, n->left->register_number);
			break;
		// Multiplication
		case ASM_MULTIPLICATION:
			instruction = spawn_vasm_op (VASM_MUL,
					n->left->register_number,
					n->right->register_number,
					true, true, false);
			*vasm = insert_vasm_instruction (*vasm, instruction);
			n->register_number = n->left->register_number;
			symbol_table = st_insert
				(symbol_table, n, n->left->register_number);
			break;
		// Division
		case ASM_DIVISION:
			*vasm = insert_vasm_instruction
				(*vasm, spawn_vasm_op
				 (VASM_MOV, count_array->regcount, 4, true,
				  true, false));
			count_array->regcount += 1;
			*vasm = insert_vasm_instruction
				(*vasm, spawn_vasm_op
				 (VASM_XOR, 4, 4, true, true, false));
			*vasm = insert_vasm_instruction
				(*vasm, spawn_vasm_op
				 (VASM_MOV, 1, n->left->register_number, true,
				  true, false));
			*vasm = insert_vasm_instruction
				(*vasm, spawn_vasm_op
				 (VASM_MOV, count_array->regcount,
				  n->right->register_number, true, true,
				  false));
			// NULL because div takes one operand
			*vasm = insert_vasm_instruction
				(*vasm, spawn_vasm_op
				 (VASM_DIV, count_array->regcount, -1, true, true, false));
			count_array->regcount += 1;
			break;
		// Equals
		case ASM_EQUALS:
			instruction = spawn_vasm_op (VASM_MOV,
					n->left->register_number,
					n->right->register_number,
					true, true, false);
			*vasm = insert_vasm_instruction (*vasm, instruction);
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
				instruction = spawn_vasm_op (VASM_CALL,
						VASM_FUNC_CAT_ARGS, -1,
						false, false, false);
				*vasm = insert_vasm_instruction (*vasm,
						instruction);

				// String buffer in rax.
				//fprintf (ofp, "mov\t\trcx\trax\n");
				//fprintf (ofp, "call\t\tprintstr\n");
				break;
			}
			else if (n->right->reg == true)
			{
				register_num = st_search (symbol_table,
						n->right);
				instruction = spawn_vasm_op (VASM_MOV,
						REG_RCX, register_num,
						true, true, false);
				*vasm = insert_vasm_instruction
					(*vasm, instruction);

				instruction = spawn_vasm_op (VASM_CALL,
						VASM_FUNC_PRINTSTR, -1,
						true, false, false);
				*vasm = insert_vasm_instruction
					(*vasm, instruction);
				break;
			}
			break;
		case ASM_COMMA:
			if (n->left->rule_number != ASM_COMMA)
			{
				instruction = spawn_vasm_op (VASM_PUSH,
						n->left->register_number,
						-1, true, false, false);
				*vasm = insert_vasm_instruction
					(*vasm, instruction);
			}
			instruction = spawn_vasm_op (VASM_PUSH,
					n->right->register_number,
					-1, true, true, false);
			*vasm = insert_vasm_instruction (*vasm, instruction);
			break;
		// Handling for variable declarations within for loops.
		case ASM_FORLOOPID:
			instruction = spawn_vasm_op (VASM_XOR,
					count_array->regcount,
					count_array->regcount, true, true,
					false);
			*vasm = insert_vasm_instruction (*vasm, instruction);
			n->register_number = count_array->regcount;
			n->reg = true;
			symbol_table = st_insert
				(symbol_table, n, n->register_number);
			count_array->regcount += 1;
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
			// Initialize loop counter.
			instruction = spawn_vasm_op
				(VASM_XOR, count_array->regcount,
				 count_array->regcount, true, true, false);
			*vasm = insert_vasm_instruction (*vasm, instruction);

			// Associate register id with loop var.
			symbol_table = st_insert (symbol_table, n->left,
					count_array->regcount);
			count_array->regcount += 1;

			// Jump to loop bottom.
			instruction = spawn_vasm_op
				(VASM_JMP, count_array->header_count, -1,
				 false, false, false);
			*vasm = insert_vasm_instruction (*vasm, instruction);

			// Spawn label for proceeding code block. If the loop
			// has other loops nested within it, we will need to 
			// reserve multiple labels. We will us function
			// check_indent_level () in order to ascertain the
			// number of labels we need to reserve.

			// n should be of type TOKEN_FOR. check_indent_level
			// expects type TOKEN_NEWLINE so we pass n's parent
			// instead.
			size_t label_num;
			size_t label_offset = get_nesting_level (root, n);

			// Now that we have a nesting level, we can generate
			// labels according to whether we have a nested loop
			// or not.
			if (label_offset == 0)
			{
				label_num = label_offset
					+ count_array->header_count + 1;
				count_array->header_count += 2;
			}
			else
			{
				label_num = (label_offset
						+ count_array->header_count)
					+ label_offset + 1;
				count_array->header_count += 1;
			}

			instruction = spawn_vasm_op
				(VASM_LOOP_CONST, label_num, -1, false, false,
				 true);
			*vasm = insert_vasm_instruction (*vasm, instruction);

			n->reg = true;
			break;
		case ASM_CLOSE_LOOP:
			register_num = st_search (symbol_table,
					n->left->right->left);
			instruction = spawn_vasm_op (VASM_ADD, register_num, 1,
					true, false, false);
			*vasm = insert_vasm_instruction (*vasm, instruction);

			register_num = get_nesting_level (root, n);
			register_num += count_array->header_count;
			instruction = spawn_vasm_op (VASM_LOOP_CONST,
					register_num, -1, false, false, true);
			*vasm = insert_vasm_instruction (*vasm, instruction);
			break;
		case ASM_DEFER:
			// ASM_DEFER does nothing, we use it to skip curtain
			// nodes. We do this with the understanding that we
			// will utilize the skipped nodes when we are higher
			// up in the tree and have more information.
			n->reg = true;
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


// Our symbol table holds:
// - The virtual register number of a variable (i.e., the I.D. associated with
// the variable in our linear I.R..
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
		bool inp_label)
{
	VasmInstruction *out_inst = malloc (sizeof (VasmInstruction));
	out_inst->op = inp_op;
	out_inst->regl = inp_regl;
	out_inst->regr = inp_regr;
	out_inst->regr_reg = inp_regr_reg;
	out_inst->regl_reg = inp_regl_reg;
	if (inp_label)
	{
		if (inp_op == VASM_LOOP_CONST)
		{
			out_inst->label_data.id = inp_regl;
			out_inst->label_data.type = LOOP_CONST;
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
						"jmp\t\tLC%zu\n",
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
			case VASM_LOOP_CONST:
				if (no_registers (inp_vasm))
				{
					fprintf (inp_file,
							"LC%zu:\n",
							inp_vasm->regl);
					break;
				}
				else
				{
					fprintf (stderr,
					"Error in output_vasm_file VASM_LOOP_CONST\n");
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


TokenType
check_left_branch (TreeNode *inp_node)
{
	return inp_node->left->contents.type;
}


TokenType
check_right_branch (TreeNode *inp_node)
{
	return inp_node->right->contents.type;
}


TokenType
check_next_line (TreeNode *inp_node)
{
	TokenType out_type;
	out_type = inp_node->right->left->contents.type;
	return out_type;
}


void
spawn_python_error (ErrorCode inp_error_code, size_t inp_line_number)
{
	switch (inp_error_code)
	{
		case ERROR_INDENTATION:
			fprintf (stderr, "Error Type:\tIndentationError\n");
			fprintf (stderr, "(Lack of indented block after for ");
			fprintf (stderr, "statment)\n");
			fprintf (stderr, "Line Number:\t%zu\n",
					inp_line_number);
			break;
		default:
			fprintf (stderr, "UNKNOWN ERROR CODE ENTERED\n");
			break;
	}
}


bool
is_for_loop (TreeNode *inp_root)
{
	if ((check_left_branch (inp_root) == TOKEN_FOR)
			|| ((check_left_branch (inp_root) == TOKEN_TAB)
				&& (inp_root->left->right->contents.type
					== TOKEN_FOR)))
	{
		return true;
	}
	else
	{
		return false;
	}
}


TokenType
check_left_branch_tabless (TreeNode *inp_node)
{
	if (inp_node->left->contents.type == TOKEN_TAB)
	{
		if (inp_node->left->left->contents.type == TOKEN_RIGHT_PAREN)
		{
			return inp_node->left->right->contents.type;
		}
		else
		{
			return inp_node->left->left->contents.type;
		}
	}
	else
	{
		return inp_node->left->contents.type;

	}
}


size_t
get_nesting_level (TreeNode *inp_root, TreeNode *inp_node)
{
	// This is the level of indentation within the loop. In this context
	// this means that within this loop, there are no other loops (if the
	// value is 0). If its a triple nested loop, or two regular loop within
	// another, we return 2, etc.
	size_t out_level = 0;

	size_t body_indent_level = 0;

	// temp_root should now be of type TOKEN_NEWLINE. Essentially, we will
	// count the number of occurences of TOKEN_FOR within loop body.
	TreeNode *temp_root = get_parent (inp_node, inp_root);

	// However, if the for loop is indented, the result of the above line
	// will leave temp_root equal to type TOKEN_TAB. If this is the case,
	// we have to call get_parent again to get to the TOKEN_NEWLINE.
	if (temp_root->contents.type == TOKEN_TAB)
	{
		// This should put us on a newline.
		temp_root = get_parent (temp_root, inp_root);
	}


	bool done = false;
	bool just_started = true;
	while (done == false)
	{
		// Go to next line.
		temp_root = temp_root->right;

		// We need to initialize body_indent_level with the indent
		// level of our loops body.
		if (just_started)
		{
			// We should check if the next element is indented, if
			// it's not we will have to return an error (A for loop
			// needs a indented body).
			if ((check_left_branch (temp_root) != TOKEN_TAB)
					&& temp_root->contents.type != TOKEN_TAB)
			{
				return 0;
			}
			// If temp_root is of type TOKEN_TAB, this implys that
			// we are at eof.
			else if ((check_left_branch (temp_root) != TOKEN_TAB)
					&& temp_root->contents.type == TOKEN_TAB)
			{
				return out_level;
			}
			// Otherwise we set the initial indentation level.
			// Tokens with this indentation level or greater are
			// within our loop's body.
			else
			{
				just_started = false;
				body_indent_level =
					temp_root->left->contents.length;
			}
		}

		// Now that we have the indent level, we need to check how
		// many times TOKEN_FOR occurs within our loop body.
		
		// Case i: Either lack of tab or indentation level is lower
		// than body_indent_level. This implys that the search is done
		// and we are outside of the loops body.
		if ((check_left_branch (temp_root) != TOKEN_TAB)
				|| ((check_left_branch (temp_root)
						== TOKEN_TAB)
					&& (check_indent_level (temp_root)
						< body_indent_level)))
		{
			done = true;
			return out_level;
		}

		// Case ii: We have found another for loop within current
		// loops body. In this case we increment the return value.
		// After this is done we start the process again with the next
		// line.
		if ((check_left_branch (temp_root) == TOKEN_TAB)
				&& (check_indent_level (temp_root) >= body_indent_level)
				&& (check_left_branch_tabless (temp_root) == TOKEN_FOR))
		{
			out_level++;
		}
	}
	return out_level;
}


size_t
check_indent_level (TreeNode *inp_node)
{
	if (check_left_branch (inp_node) == TOKEN_TAB)
	{
		return inp_node->left->contents.length;
	}
	else
	{
		// If no tab, we are at base indentation level.
		return 0;
	}
}


bool
indent_level_dropped (size_t inp_node_prev, size_t inp_node_curr)
{
	if (inp_node_curr < inp_node_prev)
	{
		return true;
	}
	else
	{
		return false;
	}
}


TreeNode *
get_parent (TreeNode *inp_node, TreeNode *inp_root)
{
	StackNode *stack = NULL;
	for (;;)
	{
		if (is_tree_node_empty (inp_root) == true)
		{
			if (is_stack_empty (&stack) == true)
			{
				break;
			}
			else
			{
				inp_root = pop (&stack); // Backtrack
				inp_root = inp_root->right;
			}
		}
		else
		{
			if ((inp_root->left == inp_node)
					|| (inp_root->right == inp_node))
			{
				return inp_root;
			}
			push (&stack, inp_root); // Store spot for backtrack
			inp_root = inp_root->left;
		}
	}
	return NULL;
}


CountArray *
spawn_count_array (size_t inp_header_count, size_t inp_regcount,
		size_t inp_footer_count)
{
	CountArray *out_arr = malloc (sizeof (CountArray));
	out_arr->header_count = inp_header_count;
	out_arr->footer_count = inp_footer_count;
	out_arr->regcount = inp_regcount;
	return out_arr;
}
