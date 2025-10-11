/*
 * A Tree Tiling Code Generator.
 */


#include "code_generator.h"


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
	TreeNode *nearest_newline;
	size_t indent_level, temp_reg;
	TreeNode *temp;
	if (count_array->force_break_recursion == true)
	{
		return;
	}
	bool is_loop_body = count_array->loop_body;
	if (count_array->loop_body)
	{
		// Annotate newline so that we don't code gen twice.
		if (calc_type (n) == TOKEN_NEWLINE)
		{
			// Check if we have to break loop
			if (calc_type (n->left) == TOKEN_TAB)
			{
				indent_level = n->left->contents.length;
			}
			else
			{
				indent_level = 0;
			}
			if (indent_level < count_array->indent_level)
			{
				count_array->loop_body = false;
				return;
			}
			// Special 'boolean' that says we don't code gen for
			// this branch again.
			n->register_number = 1;
			if ((calc_if_last_line (n))
					&& (count_array->first_entry))
			{
				// Handling for when last line is in body.
				n->register_number = 3;
				count_array->first_entry = false;
			}
		}
		if (is_tree_node_empty (n) == false)
		{
			tile
				(n->left, root, symbol_table, vasm, line_num,
				 error, count_array);
			if (count_array->force_break_recursion == true)
			{
				return;
			}
			tile
				(n->right, root, symbol_table, vasm, line_num,
				 error, count_array);
			if (count_array->force_break_recursion == true)
			{
				return;
			}
			nearest_newline = calc_nearest_newline (n, root);
			switch (nearest_newline->register_number)
			{
				case 3:
					nearest_newline->register_number = 0;
					tile
						(nearest_newline->left, root,
						 symbol_table, vasm, line_num,
						 error, count_array);
					nearest_newline->register_number = 2;
					count_array->force_break_recursion
						= true;
					return;
				case 2:
					nearest_newline->register_number = 0;
					tile
						(nearest_newline->right, root,
						 symbol_table, vasm, line_num,
						 error, count_array);
					nearest_newline->register_number = 1;
					count_array->force_break_recursion
						= true;
					return;
				default:
					label
						(n, root, symbol_table, vasm,
						 line_num, error, count_array);
					return;
			}
		}
		else
		{
			if (calc_indent_level (root, n)
					< count_array->indent_level)
			{
				return;
			}
			label
				(n, root, symbol_table, vasm, line_num,
				 error, count_array);
		}
	}
	// Handling for all other cases.
	else
	{
		if (calc_if_last_line (n)
				&& (count_array->first_entry))
		{
			n->register_number = 3;
			count_array->first_entry = false;
		}
		if (is_tree_node_empty (n) == false)
		{
			tile
				(n->left, root, symbol_table, vasm, line_num,
				 error, count_array);
			if (count_array->force_break_recursion == true)
			{
				return;
			}
			tile
				(n->right, root, symbol_table, vasm, line_num,
				 error, count_array);
			if (count_array->force_break_recursion == true)
			{
				return;
			}
			// Prevents code gen twice for loop body.
			nearest_newline = calc_nearest_newline (n, root);
			// Check if we have to break due to indented block
			// ending.
			if ((check_indent_level (nearest_newline)
					< count_array->indent_level)
					&& (nearest_newline->register_number
						!= 1)
					// crude check to see if loop is
					// nested. ~NOTE~
					&& (calc_type (nearest_newline->left)
						== TOKEN_TAB))
			{
				label
					(n, root, symbol_table, vasm,
					 line_num, error, count_array);
				count_array->force_break_recursion = true;
				nearest_newline->register_number = 1;
				return;
			}
			switch (nearest_newline->register_number)
			{
				case 0:
					label
						(n, root, symbol_table, vasm,
						 line_num, error, count_array);
					break;
				case 2:
					nearest_newline->register_number = 0;
					tile
						(nearest_newline->right, root,
						 symbol_table, vasm, line_num,
						 error, count_array);
					nearest_newline->register_number = 1;
					count_array->force_break_recursion
						= true;
					break;
				case 3:
					nearest_newline->register_number = 0;
					tile
						(nearest_newline->left, root,
						 symbol_table, vasm, line_num,
						 error, count_array);
					nearest_newline->register_number = 2;
					count_array->force_break_recursion
						= true;
					break;
				default:
					return;
			}
		}
		else
		{
			// Prevents code gen twice for loop body.
			nearest_newline = calc_nearest_newline (n, root);
			switch (nearest_newline->register_number)
			{
				case 0:
					label
						(n, root, symbol_table, vasm,
						 line_num, error, count_array);
					break;
				case 2:
					nearest_newline->register_number = 0;
					tile
						(nearest_newline->right, root,
						 symbol_table, vasm, line_num,
						 error, count_array);
					nearest_newline->register_number = 1;
					count_array->force_break_recursion
						= true;
					break;
				case 3:
					nearest_newline->register_number = 0;
					tile
						(nearest_newline->left, root,
						 symbol_table, vasm, line_num,
						 error, count_array);
					nearest_newline->register_number = 2;
					count_array->force_break_recursion
						= true;
					break;
				default:
					break;
			}
		}
	}
}


/*
 * label ()
 * Input:
 * 	TreeNode *		<- Node in our A.S.T. to label. Labeling in
 * 				this context means examining the node's type
 * 				and choosing the corresponding instruction
 * 				selection rule (we match tree patterns).
 *
 * 	TreeNode *		<- Root of our A.S.T. so that we can find a
 * 				node's parent if we need to.
 *
 * 	StNode **		<- Symbol table for locating the register
 * 				number assigned to variables, and other symbol
 * 				table things if we need.
 *
 *	VasmInstruction **	<- A linked list of VASM instructions. In this
 *				function we eventually get to a call of
 *				generate_vasm () which will insert
 *				instructions into our list.
 * 
 * 	bool *			<-  A pointer to some bool that describes
 * 				whether or not an error occured. If an error
 * 				occurs, we do all of our compiler work but
 * 				delete our file afterwards. Essentially we
 * 				emit nothing if this bool is true.
 *
 * 	CountArray *		<- A wrapper for various counters who's state
 * 				must be perserved between function calls.
 * Output:
 * 	void			<- We dont emit anything, but the state of
 * 				our VasmInstruction ** will be altered.
*/
void
label (TreeNode *n, TreeNode *root, StNode **symbol_table,
		VasmInstruction **vasm, size_t *line_num, bool *error,
		CountArray *count_array)
{
	TreeNode *temp_parent;
	size_t prev_indent_level;
	size_t curr_indent_level;
	TreeNode *temp_node;
	TreeNode *nearest_newline;
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
						 vasm, line_num, count_array);
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
						 line_num, count_array);
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
					 symbol_table, vasm, line_num,
					 count_array);
				break;
			}
		case TOKEN_INTEGER:
			n->reg = true;
			temp_parent = get_parent(n, root);
			if (calc_type (temp_parent) == TOKEN_IN)
			{
				n->rule_number = ASM_DEFER;
			}
			else
			{

				n->rule_number = ASM_CONSTANT;
			}
			generate_vasm
				(n->rule_number, n, root,
				 symbol_table,
				 vasm, line_num, count_array);
			break;
		case TOKEN_ADD:
			if ((n->left->reg == true) && (n->right->reg == true))
			{
				n->reg = true;
				n->rule_number = ASM_ADDITION;
				generate_vasm
					(n->rule_number, n, root,
					 symbol_table,
					 vasm, line_num, count_array);
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
					 vasm, line_num, count_array);
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
					 vasm, line_num, count_array);
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
					 vasm, line_num, count_array);
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
					 vasm, line_num, count_array);
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
					 vasm, line_num, count_array);
				break;
			}
			else
			{
				// ~NOTE~ For nested loops we sometimes get
				// to this error.
				n->reg = true;
				//printf ("TOKEN_PRINT tile error");
				break;
			}
		case TOKEN_COMMA:
			if ((n->left->reg == true) && (n->right->reg == true))
			{
				n->reg = true;
				n->rule_number = ASM_COMMA;
				generate_vasm
					(n->rule_number, n, root,
					 symbol_table,
					 vasm, line_num, count_array);
				break;
			}
			break;
		case TOKEN_RANGE:
			// We will go down different I.S. paths depending
			// on whether or not the loop has other loops nested
			// within it.
			temp_parent = get_parent (n, root);
			temp_parent = get_parent (temp_parent, root);
			// Now we are on TOKEN_FOR.
			prev_indent_level = get_nesting_level
				(root, temp_parent);
			// Non-nested loop.
			if ((prev_indent_level == 0)
					&& (get_reverse_nesting_level
						(root, temp_parent, 0) == 0))
			{
				n->reg = true;
				n->rule_number = ASM_FOR_INIT;
				generate_vasm (n->rule_number, n, root,
						symbol_table,
						vasm, line_num, count_array);
				break;
			}
			// Nested loop.
			else
			{
				n->reg = true;
				n->rule_number = ASM_FOR_INIT_NESTED;
				generate_vasm (n->rule_number, n, root,
						symbol_table,
						vasm, line_num, count_array);
				break;
			}
			break;
		case TOKEN_IN:
			if ((n->left->reg == true) && (n->right->reg == true))
			{
				n->reg = true;
				n->rule_number = ASM_IN;
				generate_vasm
					(n->rule_number, n, root,
					 symbol_table,
					 vasm, line_num, count_array);
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
				// Captures body, then closes loop around body.
				n->rule_number = ASM_FOR_TERM;
				generate_vasm
					(n->rule_number, n, root,
					 symbol_table, vasm, line_num,
					 count_array);
				break;
			}
			else
			{
				n->reg = false;
				fprintf (stderr, "Error, tile(), TOKEN_FOR\n");
				break;
			}
			break;
		case TOKEN_RIGHT_PAREN:
			break;
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
				curr_indent_level = n->left->contents.length;
			}
			else
			{
				curr_indent_level = 0;
			}
			if (temp_parent != NULL)
			{
				if (check_left_branch (temp_parent)
						== TOKEN_TAB)
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
			break;
		case TOKEN_TAB:
			n->reg = true;
			break;
		default:
			fprintf (stderr, "error in label\n");
			break;
	}
}


size_t
convert_constant (int inp_length, const char *inp_string)
{
	char *string = malloc (sizeof (*string) * (inp_length + 2));
	for (size_t i = 0; i < inp_length ; i++)
	{
		if (string != NULL)
		{
			string[i] = inp_string[i];
		}
	}
	if (string != NULL)
	{
		string[inp_length+1] = 0;
	}
	size_t return_num = 0;
	if (string != NULL)
	{
		return_num = atoi (string);
	}
	return return_num;
}


void
generate_vasm (size_t r, TreeNode *n, TreeNode *root, StNode **symbol_table,
		VasmInstruction **vasm, size_t *line_num,
		CountArray *count_array)
{
	VasmInstruction *instruction;
	TreeNode *temp_node;
	TreeNode *other_temp_node;
	size_t label_offset;
	size_t label_num;
	// For register numbers associated with variables.
	size_t register_num_l;
	size_t register_num_r;
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
			register_num_l = st_search (symbol_table, n);
			instruction = spawn_vasm_op (VASM_MOV,
					count_array->regcount, register_num_l,
					true, true, false);
			*vasm = insert_vasm_instruction (*vasm, instruction);
			n->register_number = count_array->regcount;
			count_array->regcount += 1;
			break;
		// Constants
		case ASM_CONSTANT:
			// MOV reg(i), CONST
			register_num_l = convert_constant (n->contents.length,
					n->contents.first_char);
			instruction = spawn_vasm_op (VASM_MOV,
					count_array->regcount, register_num_l,
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
				 (VASM_DIV, count_array->regcount, -1, true,
				  true, false));
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
				
				size_t num_nodes = ((count_tree_nodes
							(n->right) + 1) / 2);

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
				register_num_l = st_search (symbol_table,
						n->right);
				instruction = spawn_vasm_op (VASM_MOV,
						REG_RCX, register_num_l,
						true, true, false);
				*vasm = insert_vasm_instruction
					(*vasm, instruction);

				instruction = spawn_vasm_op (VASM_CALL,
						VASM_FUNC_PRINTSTR, -1,
						false, false, false);
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
			n->reg = true;
			break;
		case ASM_FOR_INIT:
			// Get register num from symbol table.
			temp_node = get_parent (n, root);
			// We should be on TOKEN_IN
			temp_node = get_parent (temp_node, root);
			/*
			symbol_table = st_insert (symbol_table,
					temp_node->left,
					count_array->regcount);
			*/
			count_array->regcount += 1;

			// Jump to loop bottom.
			instruction = spawn_vasm_op
				(VASM_JMP, count_array->header_count, -1,
				 false, false, false);
			*vasm = insert_vasm_instruction (*vasm, instruction);

			label_offset = 0;

			label_num = label_offset
				+ count_array->header_count + 1;
			temp_node = get_parent (temp_node, root);
			temp_node = get_parent (temp_node, root);
			if (loop_nested (root, temp_node))
			{
				count_array->header_count += 1;
			}
			else
			{
				count_array->header_count += 2;
			}

			instruction = spawn_vasm_op
				(VASM_LOOP_CONST, label_num, -1, false, false,
				 true);
			*vasm = insert_vasm_instruction (*vasm, instruction);
			n->reg = true;
			break;
		case ASM_FOR_INIT_NESTED:
			// Below is the same as non-nested.
			// Associate register id with loop var.
			temp_node = get_parent (n, root);
			// We should be on TOKEN_IN
			temp_node = get_parent (n, root);
			symbol_table = st_insert (symbol_table,
					temp_node->left,
					count_array->regcount);
			count_array->regcount += 1;

			// Jump to loop bottom.
			instruction = spawn_vasm_op
				(VASM_JMP, count_array->header_count, -1,
				 false, false, false);
			*vasm = insert_vasm_instruction (*vasm, instruction);

			// Below differs from non-nested.
			temp_node = get_parent (temp_node, root);
			label_offset = get_nesting_level (root, temp_node);
			label_num = count_array->header_count
				* (label_offset + 1) + 1;
			if (get_reverse_nesting_level (root, temp_node, 0) == 0)
			{
				label_num = (get_nesting_level
						(root, temp_node) + 1) * 2 + 1;
			}
			else
			{
				other_temp_node = get_outer_for
					(temp_node, root);
				label_num = (get_nesting_level
						(root, other_temp_node) + 1)
					* 2 + 1;
				label_num -= get_reverse_nesting_level
					(root, temp_node, 0);
			}
			instruction = spawn_vasm_op
				(VASM_LOOP_CONST, label_num, -1, false, false,
				 true);
			*vasm = insert_vasm_instruction (*vasm, instruction);
			count_array->header_count += 1;
			n->reg = true;
			break;
		case ASM_FOR_TERM:
			// We must capture the loop body first, then generate
			// the closure.

			// First, we position our pointer on the next line in
			// our A.S.T. (pointer should be TreeNode * of type
			// TOKEN_NEWLINE.
			// In order to this we first get the node's parent,
			// excluding tabs. Then we go to the right branch
			// (next line).
			temp_node = calc_nearest_newline (n, root);
			temp_node = temp_node->right;
			// We should now be on the next line (node should be
			// of type TOKEN_NEWLINE).

			// Special handling for loop body also being eof.
			if (calc_type (temp_node) == TOKEN_TAB)
			{
				other_temp_node = get_parent
					(temp_node, root);
				other_temp_node->register_number = 1;
			}

			// We have to handle degenerate case ii in notebook.
			// Essentially, when temp_node is a newline with two
			// tab children, sometimes the length of the left tab
			// is greater than the right. We handle this case
			// manually.
			// We will basically tile n.right and try to break
			// afterwards.
			/*
			if (tree_pattern_match (temp_node, PATTERN_V_LOOP))
			{
				temp_node->register_number = 0;
				tile (temp_node->right, root, symbol_table,
						vasm, line_num, NULL,
						count_array);
				break;
			}
			*/
			count_array->indent_level
				= temp_node->left->contents.length;

			// Now we can call tile () with our special boolean we
			// placed in our CountArray. This boolean tells tile
			// to annotate lines that we code gen for the body.
			// This way, when we resume normal code gen, we won't
			// perform instruction selection for those sub trees
			// twice.
			
			// This special boolean also tells tile to return as
			// soon as we leave the loop body.
			count_array->loop_body = true;
			temp_node->register_number = 1;
			tile (temp_node, root, symbol_table, vasm, line_num,
					NULL, count_array);
			// If the for loop's body is also EOF, we have to
			// manually annotate that node so we don't I.S.
			// again.
			if (calc_type (temp_node->right) == TOKEN_TAB)
			{
				temp_node = get_parent (temp_node, root);
				temp_node->register_number = 1;
				count_array->loop_body = false;
			}

			// Now that we have captured the body, we can close
			// our loop now.

			// We will start by incrementing our loop var. First,
			// get loop var's associated register.
			register_num_l = st_search (symbol_table, n->left);
			// We will just hardcode 1 for now ~NOTE~.
			instruction = spawn_vasm_op (VASM_ADD, register_num_l,
					1, true, false, false);
			*vasm = insert_vasm_instruction (*vasm, instruction);

			// Now we will spawn our loop label.
			// First, we need to check whether this for loop is
			// nested within another for loop (or has a for loop
			// nested whitin itself). If this is the case, we need
			// to generate labels differently.
			if ((loop_nested (root, n))
					|| (get_nesting_level (root, n) > 0))
			{
				label_num = get_reverse_nesting_level
					(root, n, 0);
				label_num += 2;
				instruction = spawn_vasm_op
					(VASM_LOOP_CONST, label_num, -1, false,
					 false, true);
				*vasm = insert_vasm_instruction
					(*vasm, instruction);
			}
			else
			{
				instruction = spawn_vasm_op
					(VASM_LOOP_CONST, count_array->footer_count,
					 -1, false, false, true);
				*vasm = insert_vasm_instruction (*vasm, instruction);
			}
			count_array->footer_count += 1;

			// Now we do our cmp and jmp to restart the loop if
			// necessary.
			register_num_l = st_search (symbol_table, n->left);
			temp_node = n->right->right;
			register_num_r = convert_constant
				(temp_node->contents.length,
				 temp_node->contents.first_char);
			register_num_r--;
			instruction = spawn_vasm_op (VASM_CMP, register_num_l,
					register_num_r, true, false, false);
			*vasm = insert_vasm_instruction (*vasm, instruction);
			n->right->register_number = register_num_r;

			if ((get_reverse_nesting_level (root, n, 0) > 0)
					&& (get_nesting_level (root, n) == 0))
			{
				count_array->footer_count
					= count_array->header_count;
			//	count_array->footer_count -= 1;
				instruction = spawn_vasm_op
					(VASM_JLE, count_array->footer_count,
					 -1, false, false, false);
				*vasm = insert_vasm_instruction
					(*vasm, instruction);
				count_array->footer_count += 1;
			}
			else if ((get_reverse_nesting_level (root, n, 0) > 0)
					|| (get_nesting_level (root, n)))
			{
				count_array->footer_count -= 1;
				instruction = spawn_vasm_op
					(VASM_JLE, count_array->footer_count,
					 -1, false, false, false);
				*vasm = insert_vasm_instruction
					(*vasm, instruction);
				count_array->footer_count += 1;
			}
			else
			{
				instruction = spawn_vasm_op
					(VASM_JLE, count_array->footer_count,
					 -1, false, false, false);
				*vasm = insert_vasm_instruction
					(*vasm, instruction);
				count_array->footer_count += 1;
			}

			// Done consuming loop body
			count_array->loop_body = false;
			count_array->force_break_recursion = false;
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
	if (return_node != NULL)
	{
		return_node->node = node;
		return_node->contents = value;
		return_node->next = NULL;
	}
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
	if (temp[index] != NULL)
	{
		if (temp[index]->lexeme == string_hash)
		{
			return temp[index]->contents;
		}
	}
	// If not, traverse linked list.
	if (temp[index] != NULL)
	{
		if (temp[index]->contents != -1)
		{
			while ((temp[index]->next != NULL) && (temp[index]->lexeme
						!= string_hash))
			{
				temp[index] = temp[index]->next;
			}
		}
	}
	// Should be at end of linked list or at target now.
	if (temp[index] != NULL)
	{
		if (temp[index]->lexeme == string_hash)
		{
			// On target, return associated int.
			return temp[index]->contents;
		}
	}
	else
	{
		// Item not in table, return special numeric.
		return -1;
	}
	return -1;
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
	if (return_table != NULL)
	{
		for (int i = 0 ; i < SYMBOL_TABLE_SIZE ; i++)
		{
			return_table[i] = malloc (sizeof (StNode));
			return_table[i]->contents = -1;
		}
	}
	return return_table;
}


// For symbol table. Fibonacci hash function. TAOCP vol 3. 2nd ed, pp 516
// listing 5.
size_t
fib_hash (size_t input)
{
	// word size (2^64) * nearest int to phi^-1 * wordsize relativeley
	// prime to wordsize (0.618...)
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
	if (out_inst != NULL)
	{
		out_inst->op = inp_op;
		out_inst->regl = inp_regl;
		out_inst->regr = inp_regr;
		out_inst->regr_reg = inp_regr_reg;
		out_inst->regl_reg = inp_regl_reg;
	}
	if (inp_label)
	{
		if (inp_op == VASM_LOOP_CONST)
		{
			if (out_inst != NULL)
			{
				out_inst->label_data.id = inp_regl;
				out_inst->label_data.type = LOOP_CONST;
			}
		}
		else
		{
			fprintf (stderr, "Error in spawn_vasm_op (labels)\n");
		}
	}
	else
	{
		if (out_inst != NULL)
		{
			out_inst->label_data.id = 0;
			out_inst->label_data.type = 0;
		}
	}
	if (out_inst != NULL)
	{
		out_inst->next = NULL;
	}
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
			case VASM_JLE:
				if (no_registers (inp_vasm))
				{
					fprintf (inp_file,
						"jle\t\tLC%zu\n",
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
					"Error in output_vasm_file VASM_JLE\n");
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
					"Error in output_vasm_file");
					fprintf (stderr,
					" VASM_LOOP_CONST");
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
					&& temp_root->contents.type
					!= TOKEN_TAB)
			{
				return 0;
			}
			// If temp_root is of type TOKEN_TAB, this implys that
			// we are at eof.
			else if ((check_left_branch (temp_root) != TOKEN_TAB)
					&& temp_root->contents.type
					== TOKEN_TAB)
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
				&& (check_indent_level (temp_root)
					>= body_indent_level)
				&& (check_left_branch_tabless (temp_root)
					== TOKEN_FOR))
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
	if (out_arr != NULL)
	{
		out_arr->header_count = inp_header_count;
		out_arr->footer_count = inp_footer_count;
		out_arr->regcount = inp_regcount;
		out_arr->indent_level = 0;
		out_arr->loop_body = false;
		out_arr->force_break_recursion = false;
		out_arr->first_entry = true;
	}
	return out_arr;
}


TreeNode *
calc_nearest_newline (TreeNode *inp_node, TreeNode *inp_root)
{
	while (inp_node->contents.type != TOKEN_NEWLINE)
	{
		inp_node = get_parent(inp_node, inp_root);
	}
	return inp_node;
}


TokenType
calc_type (TreeNode *inp_node)
{
	return inp_node->contents.type;
}


size_t
calc_loop_num (TreeNode *inp_node)
{
	// Assuming node of type TOKEN_FOR.
	TreeNode *temp_node = inp_node->right->right;
	size_t out_num = convert_constant (temp_node->contents.length,
			temp_node->contents.first_char);
	return out_num;
}


bool
loop_nested (TreeNode *inp_root, TreeNode *inp_node)
{
	TreeNode *temp = get_parent (inp_node, inp_root);
	if (temp == NULL)
	{
		return false;
	}
	if (calc_type (temp) == TOKEN_TAB)
	{
		// If for is indented, its probably a nested loop.
		return true;
	}
	else
	{
		return false;
	}
}


size_t
calc_indent_level (TreeNode *inp_root, TreeNode *inp_node)
{
	TreeNode *curr_node = get_parent (inp_node, inp_root);
	while (true)
	{
		if (calc_type (curr_node) == TOKEN_NEWLINE)
		{
			return 0;
		}
		if (calc_type (curr_node) == TOKEN_TAB)
		{
			return curr_node->contents.length;
		}
		curr_node = get_parent (curr_node, inp_root);
	}
}


bool
tree_pattern_match (TreeNode *inp_node, PatternRule inp_rule)
{
	size_t left_indent;
	size_t right_indent;
	switch (inp_rule)
	{
		case PATTERN_V_LOOP:
			if ((calc_type (inp_node) == TOKEN_NEWLINE)
					&& (calc_type (inp_node->left)
						== TOKEN_TAB)
					&& (calc_type (inp_node->right)
						== TOKEN_TAB))
			{
				left_indent
					= inp_node->left->contents.length;
				right_indent
					= inp_node->right->contents.length;
				if (left_indent > right_indent)
				{
					return true;
				}
				else
				{
					return false;
				}
			}
			else if ((calc_type (inp_node) == TOKEN_NEWLINE)
					&& (calc_type (inp_node->left)
						== TOKEN_TAB)
					&& (calc_type (inp_node->right)
						== TOKEN_NEWLINE)
					&& (calc_type (inp_node->right->left)
						== TOKEN_TAB))
			{
				left_indent
					= inp_node->left->contents.length;
				right_indent
					= inp_node->right->left
					->contents.length;
				if (left_indent > right_indent)
				{
					return true;
				}
				else
				{
					return false;
				}
			}
			break;
		default:
			return false;
			break;
	}
	return false;
}


TreeNode *
get_outer_for (TreeNode *inp_node, TreeNode *inp_root)
{
	// Parent should be newline after this.
	TreeNode *parent = calc_nearest_newline (inp_node, inp_root);
	size_t indent_level;
	TreeNode *temp;
	// If the left branch is of type TOKEN_TAB, we use it's length as the
	// reference indent_level. Else we
	if (calc_type (parent->left) == TOKEN_TAB)
	{
		indent_level = parent->left->contents.length;
	}
	else
	{
		indent_level = 0;
	}
	while (indent_level != 0)
	{
		temp = parent;
		parent = get_parent (parent, inp_root);
		if (parent == NULL )
		{
			if (calc_type (temp->left) == TOKEN_TAB)
			{
				return temp->left->right;
			}
			else
			{
				return temp->left;
			}
		}
		if ((calc_type (parent->left) == TOKEN_TAB)
				&& calc_type (parent->left->right)
				== TOKEN_FOR)
		{
			indent_level = parent->left->contents.length;
		}
		else
		{
			indent_level = 0;
		}
	}
	if (calc_type (parent->left) == TOKEN_TAB)
	{
		return parent->left->right;
	}
	else
	{
		return parent->left;
	}
}


bool
calc_if_num_even (size_t inp_num)
{
	if ((inp_num % 2) == 0)
	{
		return true;
	}
	else
	{
		return false;
	}
}


bool
calc_if_last_line (TreeNode *inp_node)
{
	if (calc_type (inp_node) != TOKEN_NEWLINE)
	{
		return false;
	}
	size_t left_indent_level;
	if (calc_type (inp_node->left) == TOKEN_TAB)
	{
		left_indent_level = inp_node->left->contents.length;
	}
	else
	{
		left_indent_level = 0;
	}

	size_t right_indent_level;
	if (calc_type (inp_node->right) == TOKEN_TAB)
	{
		right_indent_level = inp_node->right->contents.length;
	}
	else
	{
		right_indent_level = 0;
	}

	if ((calc_type (inp_node) == TOKEN_NEWLINE)
			&& (calc_type (inp_node->right) != TOKEN_NEWLINE)
			&& (left_indent_level > right_indent_level))
	{
		return true;
	}
	else
	{
		return false;
	}
}


size_t
get_reverse_nesting_level (TreeNode *inp_root, TreeNode *inp_node,
		size_t target_indent)
{
	size_t reverse_nesting_level, indent_level;
	TreeNode *temp, *peak;

	reverse_nesting_level = 0;
	temp = calc_nearest_newline (inp_node, inp_root);
	if (calc_type (temp->left) == TOKEN_TAB)
	{
		indent_level = temp->left->contents.type;
		// We look past the tab.
		peak = temp->left->right;
	}
	else
	{
		indent_level = 0;
		peak = temp->left;
	}
	
	if (indent_level == target_indent)
	{
		return reverse_nesting_level;
	}
	if (calc_type (peak) == TOKEN_FOR)
	{
		reverse_nesting_level++;
	}
	while (indent_level != target_indent)
	{
		temp = get_parent (temp, inp_root);
		if (calc_type (temp->left) == TOKEN_TAB)
		{
			indent_level = temp->left->contents.length;
			peak = temp->left->right;
		}
		else
		{
			indent_level = 0;
			peak = temp->left;
		}
		if (calc_type (peak) == TOKEN_FOR)
		{
			reverse_nesting_level++;
		}
	}
	return reverse_nesting_level - 1;
}
