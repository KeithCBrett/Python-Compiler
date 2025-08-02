#include "testing.h"


void
display_result (bool inp_bool, size_t inp_test_num)
{
	fprintf (stdout, "---------------------------\n");
	fprintf (stdout, "\tTest %zu\n\n", (inp_test_num));
	fprintf (stdout, "%s\n\n", get_test_string (inp_test_num));
	if (inp_bool)
	{
		fprintf (stdout, "STATUS: PASSED\n");
	}
	else
	{
		fprintf (stdout, "STATUS: FAILED\n");
	}
	fprintf (stdout, "---------------------------\n");
}


void
run_tests ()
{
	bool test_passed;
	for (size_t i = 1 ; i <= 5 ; i++)
	{
		test_passed = test_vasm (i);
		display_result (test_passed, i);
	}
}


bool
test_vasm (size_t inp_test_num)
{
	VasmInstruction *code_generator_output = NULL;
	VasmInstruction **p_code_generator_output = &code_generator_output;
	size_t line_num = 0;
	size_t *p_line_num = &line_num;
	bool error = false;
	bool *p_error = &error;
	CountArray *count_array = spawn_count_array (2, 5, 2);
	VasmInstruction *comparison_output = NULL;
	VasmInstruction *instruction;
	TreeNode *parser_output = NULL;
	StNode **symbol_table = st_spawn_table ();
	bool was_newline = true;
	char *source;
	bool out_bool;

	source = get_test_string (inp_test_num);
	initialize_lexer (source);
	parser_output = parse (Prec_Start, parser_output,
			was_newline);
	tile
		(parser_output, parser_output, symbol_table,
		 p_code_generator_output, p_line_num, p_error,
		 count_array);

	switch (inp_test_num)
	{
		// Input I
		case 1:
			// xor r(5), r(5)
			instruction = spawn_vasm_op
				(VASM_XOR, 5, 5, true, true, false);
			comparison_output = insert_vasm_instruction
				(comparison_output, instruction);
			// jmp LC2
			instruction = spawn_vasm_op
				(VASM_JMP, 2, -1, false, false, false);
			comparison_output = insert_vasm_instruction
				(comparison_output, instruction);
			// LC3:
			instruction = spawn_vasm_op
				(VASM_LOOP_CONST, 3, -1, false, false, true);
			comparison_output = insert_vasm_instruction
				(comparison_output, instruction);
			// mov rcx, r(5)
			instruction = spawn_vasm_op
				(VASM_MOV, REG_RCX, 5, true, true, false);
			comparison_output = insert_vasm_instruction
				(comparison_output, instruction);
			// call printstr
			instruction = spawn_vasm_op
				(VASM_CALL, VASM_FUNC_PRINTSTR, -1, false, false,
				 false);
			comparison_output = insert_vasm_instruction
				(comparison_output, instruction);
			// add r(5), 1
			instruction = spawn_vasm_op
				(VASM_ADD, 5, 1, true, false, false);
			comparison_output = insert_vasm_instruction
				(comparison_output, instruction);
			// LC2:
			instruction = spawn_vasm_op
				(VASM_LOOP_CONST, 2, -1, false, false, true);
			comparison_output = insert_vasm_instruction
				(comparison_output, instruction);
			// cmp r(5), 4
			instruction = spawn_vasm_op
				(VASM_CMP, 5, 4, true, false, false);
			comparison_output = insert_vasm_instruction
				(comparison_output, instruction);
			// jle LC3
			instruction = spawn_vasm_op
				(VASM_JLE, 3, -1, false, false, false);
			comparison_output = insert_vasm_instruction
				(comparison_output, instruction);
			// xor r(7), r(7)
			instruction = spawn_vasm_op
				(VASM_XOR, 7, 7, true, true, false);
			comparison_output = insert_vasm_instruction
				(comparison_output, instruction);
			// jmp LC4
			instruction = spawn_vasm_op
				(VASM_JMP, 4, -1, false, false, false);
			comparison_output = insert_vasm_instruction
				(comparison_output, instruction);
			// LC5:
			instruction = spawn_vasm_op
				(VASM_LOOP_CONST, 5, -1, false, false, true);
			comparison_output = insert_vasm_instruction
				(comparison_output, instruction);
			// mov rcx r(7)
			instruction = spawn_vasm_op
				(VASM_MOV, REG_RCX, 7, true, true, false);
			comparison_output = insert_vasm_instruction
				(comparison_output, instruction);
			// call printstr
			instruction = spawn_vasm_op
				(VASM_CALL, VASM_FUNC_PRINTSTR, -1, false,
				 false, false);
			comparison_output = insert_vasm_instruction
				(comparison_output, instruction);
			// add r(7), 1
			instruction = spawn_vasm_op
				(VASM_ADD, 7, 1, true, false, false);
			comparison_output = insert_vasm_instruction
				(comparison_output, instruction);
			// LC4:
			instruction = spawn_vasm_op
				(VASM_LOOP_CONST, 4, -1, false, false, true);
			comparison_output = insert_vasm_instruction
				(comparison_output, instruction);
			// cmp r(7), 9
			instruction = spawn_vasm_op
				(VASM_CMP, 7, 9, true, false, false);
			comparison_output = insert_vasm_instruction
				(comparison_output, instruction);
			// jle LC5
			instruction = spawn_vasm_op
				(VASM_JLE, 5, -1, false, false, false);
			comparison_output = insert_vasm_instruction
				(comparison_output, instruction);

			out_bool = compare_vasm_list 
				(code_generator_output, comparison_output);
			return out_bool;
		case 2:
			// xor r(5), r(5)
			instruction = spawn_vasm_op
				(VASM_XOR, 5, 5, true, true, false);
			comparison_output = insert_vasm_instruction
				(comparison_output, instruction);
			// jmp LC2
			instruction = spawn_vasm_op
				(VASM_JMP, 2, -1, false, false, false);
			comparison_output = insert_vasm_instruction
				(comparison_output, instruction);
			// LC5:
			instruction = spawn_vasm_op
				(VASM_LOOP_CONST, 5, -1, false, false, true);
			comparison_output = insert_vasm_instruction
				(comparison_output, instruction);
			// xor r(7), r(7)
			instruction = spawn_vasm_op
				(VASM_XOR, 7, 7, true, true, false);
			comparison_output = insert_vasm_instruction
				(comparison_output, instruction);
			// jmp LC3
			instruction = spawn_vasm_op
				(VASM_JMP, 3, -1, false, false, false);
			comparison_output = insert_vasm_instruction
				(comparison_output, instruction);
			// LC4:
			instruction = spawn_vasm_op
				(VASM_LOOP_CONST, 4, -1, false, false, true);
			comparison_output = insert_vasm_instruction
				(comparison_output, instruction);
			// mov rcx, r(7)
			instruction = spawn_vasm_op
				(VASM_MOV, REG_RCX, 7, true, true, false);
			comparison_output = insert_vasm_instruction
				(comparison_output, instruction);
			// call printstr
			instruction = spawn_vasm_op
				(VASM_CALL, VASM_FUNC_PRINTSTR, -1, false,
				 false, false);
			comparison_output = insert_vasm_instruction
				(comparison_output, instruction);
			// add r(7), 1
			instruction = spawn_vasm_op
				(VASM_ADD, 7, 1, true, false, false);
			comparison_output = insert_vasm_instruction
				(comparison_output, instruction);
			// LC3:
			instruction = spawn_vasm_op
				(VASM_LOOP_CONST, 3, -1, false, false, true);
			comparison_output = insert_vasm_instruction
				(comparison_output, instruction);
			// cmp r(7), 9
			instruction = spawn_vasm_op
				(VASM_CMP, 7, 9, true, false, false);
			comparison_output = insert_vasm_instruction
				(comparison_output, instruction);
			// jle LC4
			instruction = spawn_vasm_op
				(VASM_JLE, 4, -1, false, false, false);
			comparison_output = insert_vasm_instruction
				(comparison_output, instruction);
			// mov rcx, r(5)
			instruction = spawn_vasm_op
				(VASM_MOV, REG_RCX, 5, true, true, false);
			comparison_output = insert_vasm_instruction
				(comparison_output, instruction);
			// call printstr
			instruction = spawn_vasm_op
				(VASM_CALL, VASM_FUNC_PRINTSTR, -1, false,
				 false, false);
			comparison_output = insert_vasm_instruction
				(comparison_output, instruction);

			// add r(5), 1
			instruction = spawn_vasm_op
				(VASM_ADD, 5, 1, true, false, false);
			comparison_output = insert_vasm_instruction
				(comparison_output, instruction);
			// LC2:
			instruction = spawn_vasm_op
				(VASM_LOOP_CONST, 2, -1, false, false, true);
			comparison_output = insert_vasm_instruction
				(comparison_output, instruction);
			// cmp r(5), 4
			instruction = spawn_vasm_op
				(VASM_CMP, 5, 4, true, false, false);
			comparison_output = insert_vasm_instruction
				(comparison_output, instruction);
			// jle LC5
			instruction = spawn_vasm_op
				(VASM_JLE, 5, -1, false, false, false);
			comparison_output = insert_vasm_instruction
				(comparison_output, instruction);

			out_bool = compare_vasm_list 
				(code_generator_output, comparison_output);
			return out_bool;
		case 3:
			// xor r(5), r(5)
			instruction = spawn_vasm_op
				(VASM_XOR, 5, 5, true, true, false);
			comparison_output = insert_vasm_instruction
				(comparison_output, instruction);
			// jmp LC2
			instruction = spawn_vasm_op
				(VASM_JMP, 2, -1, false, false, false);
			comparison_output = insert_vasm_instruction
				(comparison_output, instruction);
			// LC5:
			instruction = spawn_vasm_op
				(VASM_LOOP_CONST, 5, -1, false, false, true);
			comparison_output = insert_vasm_instruction
				(comparison_output, instruction);
			// mov rcx, r(5)
			instruction = spawn_vasm_op
				(VASM_MOV, REG_RCX, 5, true, true, false);
			comparison_output = insert_vasm_instruction
				(comparison_output, instruction);
			// call printstr
			instruction = spawn_vasm_op
				(VASM_CALL, VASM_FUNC_PRINTSTR, -1, false,
				 false, false);
			comparison_output = insert_vasm_instruction
				(comparison_output, instruction);
			// xor r(7), r(7)
			instruction = spawn_vasm_op
				(VASM_XOR, 7, 7, true, true, false);
			comparison_output = insert_vasm_instruction
				(comparison_output, instruction);
			// jmp LC3
			instruction = spawn_vasm_op
				(VASM_JMP, 3, -1, false, false, false);
			comparison_output = insert_vasm_instruction
				(comparison_output, instruction);
			// LC4:
			instruction = spawn_vasm_op
				(VASM_LOOP_CONST, 4, -1, false, false, true);
			comparison_output = insert_vasm_instruction
				(comparison_output, instruction);
			// mov rcx, r(7)
			instruction = spawn_vasm_op
				(VASM_MOV, REG_RCX, 7, true, true, false);
			comparison_output = insert_vasm_instruction
				(comparison_output, instruction);
			// call printstr
			instruction = spawn_vasm_op
				(VASM_CALL, VASM_FUNC_PRINTSTR, -1, false,
				 false, false);
			// add r(7), 1
			instruction = spawn_vasm_op
				(VASM_ADD, 7, 1, true, false, false);
			comparison_output = insert_vasm_instruction
				(comparison_output, instruction);
			// LC3:
			instruction = spawn_vasm_op
				(VASM_LOOP_CONST, 3, -1, false, false, true);
			comparison_output = insert_vasm_instruction
				(comparison_output, instruction);
			// cmp r(7), 9
			instruction = spawn_vasm_op
				(VASM_CMP, 7, 9, true, false, false);
			comparison_output = insert_vasm_instruction
				(comparison_output, instruction);
			// jle LC4
			instruction = spawn_vasm_op
				(VASM_JLE, 4, -1, false, false, false);
			comparison_output = insert_vasm_instruction
				(comparison_output, instruction);
			// add r(5), 1
			instruction = spawn_vasm_op
				(VASM_ADD, 5, 1, true, false, false);
			comparison_output = insert_vasm_instruction
				(comparison_output, instruction);
			// LC2:
			instruction = spawn_vasm_op
				(VASM_LOOP_CONST, 2, -1, false, false, true);
			comparison_output = insert_vasm_instruction
				(comparison_output, instruction);
			// cmp r(5), 4
			instruction = spawn_vasm_op
				(VASM_CMP, 5, 4, true, false, false);
			comparison_output = insert_vasm_instruction
				(comparison_output, instruction);
			// jle LC5
			instruction = spawn_vasm_op
				(VASM_JLE, 5, -1, false, false, false);
			comparison_output = insert_vasm_instruction
				(comparison_output, instruction);

			out_bool = compare_vasm_list 
				(code_generator_output, comparison_output);
			return out_bool;
		case 4:
			// xor r(5), r(5)
			instruction = spawn_vasm_op
				(VASM_XOR, 5, 5, true, true, false);
			comparison_output = insert_vasm_instruction
				(comparison_output, instruction);
			// jmp LC2
			instruction = spawn_vasm_op
				(VASM_JMP, 2, -1, false, false, false);
			comparison_output = insert_vasm_instruction
				(comparison_output, instruction);
			// LC7:
			instruction = spawn_vasm_op
				(VASM_LOOP_CONST, 7, -1, false, false, true);
			comparison_output = insert_vasm_instruction
				(comparison_output, instruction);
			// xor r(7), r(7)
			instruction = spawn_vasm_op
				(VASM_XOR, 7, 7, true, true, false);
			comparison_output = insert_vasm_instruction
				(comparison_output, instruction);
			// jmp LC3
			instruction = spawn_vasm_op
				(VASM_JMP, 3, -1, false, false, false);
			comparison_output = insert_vasm_instruction
				(comparison_output, instruction);
			// LC6:
			instruction = spawn_vasm_op
				(VASM_LOOP_CONST, 6, -1, false, false, true);
			comparison_output = insert_vasm_instruction
				(comparison_output, instruction);
			// xor r(9), r(9)
			instruction = spawn_vasm_op
				(VASM_XOR, 9, 9, true, true, false);
			comparison_output = insert_vasm_instruction
				(comparison_output, instruction);
			// jmp LC4
			instruction = spawn_vasm_op
				(VASM_JMP, 4, -1, false, false, false);
			comparison_output = insert_vasm_instruction
				(comparison_output, instruction);
			// LC5:
			instruction = spawn_vasm_op
				(VASM_LOOP_CONST, 5, -1, false, false, true);
			comparison_output = insert_vasm_instruction
				(comparison_output, instruction);
			// mov rcx, r(9)
			instruction = spawn_vasm_op
				(VASM_MOV, REG_RCX, 9, true, true, false);
			comparison_output = insert_vasm_instruction
				(comparison_output, instruction);
			// call printstr
			instruction = spawn_vasm_op
				(VASM_CALL, VASM_FUNC_PRINTSTR, -1, false,
				 false, false);
			// add r(9), 1
			instruction = spawn_vasm_op
				(VASM_ADD, 9, 1, true, false, false);
			comparison_output = insert_vasm_instruction
				(comparison_output, instruction);
			// LC4:
			instruction = spawn_vasm_op
				(VASM_LOOP_CONST, 4, -1, false, false, true);
			comparison_output = insert_vasm_instruction
				(comparison_output, instruction);
			// cmp r(9), 14
			instruction = spawn_vasm_op
				(VASM_CMP, 9, 14, true, false, false);
			comparison_output = insert_vasm_instruction
				(comparison_output, instruction);
			// jle LC5
			instruction = spawn_vasm_op
				(VASM_JLE, 5, -1, false, false, false);
			comparison_output = insert_vasm_instruction
				(comparison_output, instruction);
			// mov rcx, r(7)
			instruction = spawn_vasm_op
				(VASM_MOV, REG_RCX, 7, true, true, false);
			comparison_output = insert_vasm_instruction
				(comparison_output, instruction);
			// call printstr
			instruction = spawn_vasm_op
				(VASM_CALL, VASM_FUNC_PRINTSTR, -1, false,
				 false, false);
			// add r(7), 1
			instruction = spawn_vasm_op
				(VASM_ADD, 7, 1, true, false, false);
			comparison_output = insert_vasm_instruction
				(comparison_output, instruction);
			// LC3:
			instruction = spawn_vasm_op
				(VASM_LOOP_CONST, 3, -1, false, false, true);
			comparison_output = insert_vasm_instruction
				(comparison_output, instruction);
			// cmp r(7), 9
			instruction = spawn_vasm_op
				(VASM_CMP, 7, 9, true, false, false);
			comparison_output = insert_vasm_instruction
				(comparison_output, instruction);
			// jle LC6
			instruction = spawn_vasm_op
				(VASM_JLE, 6, -1, false, false, false);
			comparison_output = insert_vasm_instruction
				(comparison_output, instruction);
			// mov rcx, r(5)
			instruction = spawn_vasm_op
				(VASM_MOV, REG_RCX, 5, true, true, false);
			comparison_output = insert_vasm_instruction
				(comparison_output, instruction);
			// call printstr
			instruction = spawn_vasm_op
				(VASM_CALL, VASM_FUNC_PRINTSTR, -1, false,
				 false, false);
			// add r(5), 1
			instruction = spawn_vasm_op
				(VASM_ADD, 5, 1, true, false, false);
			comparison_output = insert_vasm_instruction
				(comparison_output, instruction);
			// LC2:
			instruction = spawn_vasm_op
				(VASM_LOOP_CONST, 2, -1, false, false, true);
			comparison_output = insert_vasm_instruction
				(comparison_output, instruction);
			// cmp r(5), 4
			instruction = spawn_vasm_op
				(VASM_CMP, 5, 4, true, false, false);
			comparison_output = insert_vasm_instruction
				(comparison_output, instruction);
			// jle LC7
			instruction = spawn_vasm_op
				(VASM_JLE, 7, -1, false, false, false);
			comparison_output = insert_vasm_instruction
				(comparison_output, instruction);

			out_bool = compare_vasm_list 
				(code_generator_output, comparison_output);
			return out_bool;
		case 5:
			// xor r(5), r(5)
			instruction = spawn_vasm_op
				(VASM_XOR, 5, 5, true, true, false);
			comparison_output = insert_vasm_instruction
				(comparison_output, instruction);
			// jmp LC2
			instruction = spawn_vasm_op
				(VASM_JMP, 2, -1, false, false, false);
			comparison_output = insert_vasm_instruction
				(comparison_output, instruction);
			// LC17:
			instruction = spawn_vasm_op
				(VASM_LOOP_CONST, 17, -1, false, false, true);
			comparison_output = insert_vasm_instruction
				(comparison_output, instruction);
			// xor r(7), r(7)
			instruction = spawn_vasm_op
				(VASM_XOR, 7, 7, true, true, false);
			comparison_output = insert_vasm_instruction
				(comparison_output, instruction);
			// jmp LC3
			instruction = spawn_vasm_op
				(VASM_JMP, 3, -1, false, false, false);
			comparison_output = insert_vasm_instruction
				(comparison_output, instruction);
			// LC16:
			instruction = spawn_vasm_op
				(VASM_LOOP_CONST, 16, -1, false, false, true);
			comparison_output = insert_vasm_instruction
				(comparison_output, instruction);
			// xor r(9), r(9)
			instruction = spawn_vasm_op
				(VASM_XOR, 9, 9, true, true, false);
			comparison_output = insert_vasm_instruction
				(comparison_output, instruction);
			// jmp LC4
			instruction = spawn_vasm_op
				(VASM_JMP, 4, -1, false, false, false);
			comparison_output = insert_vasm_instruction
				(comparison_output, instruction);
			// LC15:
			instruction = spawn_vasm_op
				(VASM_LOOP_CONST, 15, -1, false, false, true);
			comparison_output = insert_vasm_instruction
				(comparison_output, instruction);
			// xor r(11), r(11)
			instruction = spawn_vasm_op
				(VASM_XOR, 11, 11, true, true, false);
			comparison_output = insert_vasm_instruction
				(comparison_output, instruction);
			// jmp LC5
			instruction = spawn_vasm_op
				(VASM_JMP, 5, -1, false, false, false);
			comparison_output = insert_vasm_instruction
				(comparison_output, instruction);
			// LC14:
			instruction = spawn_vasm_op
				(VASM_LOOP_CONST, 14, -1, false, false, true);
			comparison_output = insert_vasm_instruction
				(comparison_output, instruction);
			// xor r(13), r(13)
			instruction = spawn_vasm_op
				(VASM_XOR, 13, 13, true, true, false);
			comparison_output = insert_vasm_instruction
				(comparison_output, instruction);
			// jmp LC6
			instruction = spawn_vasm_op
				(VASM_JMP, 6, -1, false, false, false);
			comparison_output = insert_vasm_instruction
				(comparison_output, instruction);
			// LC13:
			instruction = spawn_vasm_op
				(VASM_LOOP_CONST, 13, -1, false, false, true);
			comparison_output = insert_vasm_instruction
				(comparison_output, instruction);
			// xor r(15), r(15)
			instruction = spawn_vasm_op
				(VASM_XOR, 15, 15, true, true, false);
			comparison_output = insert_vasm_instruction
				(comparison_output, instruction);
			// jmp LC7
			instruction = spawn_vasm_op
				(VASM_JMP, 7, -1, false, false, false);
			comparison_output = insert_vasm_instruction
				(comparison_output, instruction);
			// LC12:
			instruction = spawn_vasm_op
				(VASM_LOOP_CONST, 12, -1, false, false, true);
			comparison_output = insert_vasm_instruction
				(comparison_output, instruction);
			// xor r(17), r(17)
			instruction = spawn_vasm_op
				(VASM_XOR, 17, 17, true, true, false);
			comparison_output = insert_vasm_instruction
				(comparison_output, instruction);
			// jmp LC8
			instruction = spawn_vasm_op
				(VASM_JMP, 8, -1, false, false, false);
			comparison_output = insert_vasm_instruction
				(comparison_output, instruction);
			// LC11:
			instruction = spawn_vasm_op
				(VASM_LOOP_CONST, 11, -1, false, false, true);
			comparison_output = insert_vasm_instruction
				(comparison_output, instruction);
			// xor r(19), r(19)
			instruction = spawn_vasm_op
				(VASM_XOR, 19, 19, true, true, false);
			comparison_output = insert_vasm_instruction
				(comparison_output, instruction);
			// jmp LC9
			instruction = spawn_vasm_op
				(VASM_JMP, 9, -1, false, false, false);
			comparison_output = insert_vasm_instruction
				(comparison_output, instruction);
			// LC10:
			instruction = spawn_vasm_op
				(VASM_LOOP_CONST, 10, -1, false, false, true);
			comparison_output = insert_vasm_instruction
				(comparison_output, instruction);
			// mov rcx, r(19)
			instruction = spawn_vasm_op
				(VASM_MOV, REG_RCX, 19, true, true, false);
			comparison_output = insert_vasm_instruction
				(comparison_output, instruction);
			// call printstr
			instruction = spawn_vasm_op
				(VASM_CALL, VASM_FUNC_PRINTSTR, -1, false,
				 false, false);
			// add r(19), 1
			instruction = spawn_vasm_op
				(VASM_ADD, 19, 1, true, false, false);
			comparison_output = insert_vasm_instruction
				(comparison_output, instruction);
			// LC9:
			instruction = spawn_vasm_op
				(VASM_LOOP_CONST, 9, -1, false, false, true);
			comparison_output = insert_vasm_instruction
				(comparison_output, instruction);
			// cmp r(19), 7
			instruction = spawn_vasm_op
				(VASM_CMP, 19, 7, true, false, false);
			comparison_output = insert_vasm_instruction
				(comparison_output, instruction);
			// jle LC10
			instruction = spawn_vasm_op
				(VASM_JLE, 10, -1, false, false, false);
			comparison_output = insert_vasm_instruction
				(comparison_output, instruction);
			// mov rcx, r(17)
			instruction = spawn_vasm_op
				(VASM_MOV, REG_RCX, 17, true, true, false);
			comparison_output = insert_vasm_instruction
				(comparison_output, instruction);
			// call printstr
			instruction = spawn_vasm_op
				(VASM_CALL, VASM_FUNC_PRINTSTR, -1, false,
				 false, false);
			// add r(17), 1
			instruction = spawn_vasm_op
				(VASM_ADD, 17, 1, true, false, false);
			comparison_output = insert_vasm_instruction
				(comparison_output, instruction);
			// LC8:
			instruction = spawn_vasm_op
				(VASM_LOOP_CONST, 8, -1, false, false, true);
			comparison_output = insert_vasm_instruction
				(comparison_output, instruction);
			// cmp r(17), 6
			instruction = spawn_vasm_op
				(VASM_CMP, 17, 6, true, false, false);
			comparison_output = insert_vasm_instruction
				(comparison_output, instruction);
			// jle LC11
			instruction = spawn_vasm_op
				(VASM_JLE, 11, -1, false, false, false);
			comparison_output = insert_vasm_instruction
				(comparison_output, instruction);
			// mov rcx, r(15)
			instruction = spawn_vasm_op
				(VASM_MOV, REG_RCX, 15, true, true, false);
			comparison_output = insert_vasm_instruction
				(comparison_output, instruction);
			// call printstr
			instruction = spawn_vasm_op
				(VASM_CALL, VASM_FUNC_PRINTSTR, -1, false,
				 false, false);
			// add r(15), 1
			instruction = spawn_vasm_op
				(VASM_ADD, 15, 1, true, false, false);
			comparison_output = insert_vasm_instruction
				(comparison_output, instruction);
			// LC7:
			instruction = spawn_vasm_op
				(VASM_LOOP_CONST, 7, -1, false, false, true);
			comparison_output = insert_vasm_instruction
				(comparison_output, instruction);
			// cmp r(15), 5
			instruction = spawn_vasm_op
				(VASM_CMP, 15, 5, true, false, false);
			comparison_output = insert_vasm_instruction
				(comparison_output, instruction);
			// jle LC12
			instruction = spawn_vasm_op
				(VASM_JLE, 12, -1, false, false, false);
			comparison_output = insert_vasm_instruction
				(comparison_output, instruction);
			// mov rcx, r(13)
			instruction = spawn_vasm_op
				(VASM_MOV, REG_RCX, 13, true, true, false);
			comparison_output = insert_vasm_instruction
				(comparison_output, instruction);
			// call printstr
			instruction = spawn_vasm_op
				(VASM_CALL, VASM_FUNC_PRINTSTR, -1, false,
				 false, false);
			// add r(13), 1
			instruction = spawn_vasm_op
				(VASM_ADD, 13, 1, true, false, false);
			comparison_output = insert_vasm_instruction
				(comparison_output, instruction);
			// LC6:
			instruction = spawn_vasm_op
				(VASM_LOOP_CONST, 6, -1, false, false, true);
			comparison_output = insert_vasm_instruction
				(comparison_output, instruction);
			// cmp r(13), 4
			instruction = spawn_vasm_op
				(VASM_CMP, 13, 4, true, false, false);
			comparison_output = insert_vasm_instruction
				(comparison_output, instruction);
			// jle LC13
			instruction = spawn_vasm_op
				(VASM_JLE, 13, -1, false, false, false);
			comparison_output = insert_vasm_instruction
				(comparison_output, instruction);
			// mov rcx, r(11)
			instruction = spawn_vasm_op
				(VASM_MOV, REG_RCX, 11, true, true, false);
			comparison_output = insert_vasm_instruction
				(comparison_output, instruction);
			// call printstr
			instruction = spawn_vasm_op
				(VASM_CALL, VASM_FUNC_PRINTSTR, -1, false,
				 false, false);
			// add r(11), 1
			instruction = spawn_vasm_op
				(VASM_ADD, 11, 1, true, false, false);
			comparison_output = insert_vasm_instruction
				(comparison_output, instruction);
			// LC5:
			instruction = spawn_vasm_op
				(VASM_LOOP_CONST, 5, -1, false, false, true);
			comparison_output = insert_vasm_instruction
				(comparison_output, instruction);
			// cmp r(11), 3
			instruction = spawn_vasm_op
				(VASM_CMP, 11, 3, true, false, false);
			comparison_output = insert_vasm_instruction
				(comparison_output, instruction);
			// jle LC14
			instruction = spawn_vasm_op
				(VASM_JLE, 14, -1, false, false, false);
			comparison_output = insert_vasm_instruction
				(comparison_output, instruction);
			// mov rcx, r(9)
			instruction = spawn_vasm_op
				(VASM_MOV, REG_RCX, 9, true, true, false);
			comparison_output = insert_vasm_instruction
				(comparison_output, instruction);
			// call printstr
			instruction = spawn_vasm_op
				(VASM_CALL, VASM_FUNC_PRINTSTR, -1, false,
				 false, false);
			// add r(9), 1
			instruction = spawn_vasm_op
				(VASM_ADD, 9, 1, true, false, false);
			comparison_output = insert_vasm_instruction
				(comparison_output, instruction);
			// LC4:
			instruction = spawn_vasm_op
				(VASM_LOOP_CONST, 4, -1, false, false, true);
			comparison_output = insert_vasm_instruction
				(comparison_output, instruction);
			// cmp r(9), 2
			instruction = spawn_vasm_op
				(VASM_CMP, 9, 2, true, false, false);
			comparison_output = insert_vasm_instruction
				(comparison_output, instruction);
			// jle LC15
			instruction = spawn_vasm_op
				(VASM_JLE, 15, -1, false, false, false);
			comparison_output = insert_vasm_instruction
				(comparison_output, instruction);
			// mov rcx, r(7)
			instruction = spawn_vasm_op
				(VASM_MOV, REG_RCX, 7, true, true, false);
			comparison_output = insert_vasm_instruction
				(comparison_output, instruction);
			// call printstr
			instruction = spawn_vasm_op
				(VASM_CALL, VASM_FUNC_PRINTSTR, -1, false,
				 false, false);
			// add r(7), 1
			instruction = spawn_vasm_op
				(VASM_ADD, 7, 1, true, false, false);
			comparison_output = insert_vasm_instruction
				(comparison_output, instruction);
			// LC3:
			instruction = spawn_vasm_op
				(VASM_LOOP_CONST, 3, -1, false, false, true);
			comparison_output = insert_vasm_instruction
				(comparison_output, instruction);
			// cmp r(7), 1
			instruction = spawn_vasm_op
				(VASM_CMP, 7, 1, true, false, false);
			comparison_output = insert_vasm_instruction
				(comparison_output, instruction);
			// jle LC16
			instruction = spawn_vasm_op
				(VASM_JLE, 16, -1, false, false, false);
			comparison_output = insert_vasm_instruction
				(comparison_output, instruction);
			// mov rcx, r(5)
			instruction = spawn_vasm_op
				(VASM_MOV, REG_RCX, 5, true, true, false);
			comparison_output = insert_vasm_instruction
				(comparison_output, instruction);
			// call printstr
			instruction = spawn_vasm_op
				(VASM_CALL, VASM_FUNC_PRINTSTR, -1, false,
				 false, false);
			// add r(5), 1
			instruction = spawn_vasm_op
				(VASM_ADD, 5, 1, true, false, false);
			comparison_output = insert_vasm_instruction
				(comparison_output, instruction);
			// LC2:
			instruction = spawn_vasm_op
				(VASM_LOOP_CONST, 2, -1, false, false, true);
			comparison_output = insert_vasm_instruction
				(comparison_output, instruction);
			// cmp r(5), 0
			instruction = spawn_vasm_op
				(VASM_CMP, 5, 0, true, false, false);
			comparison_output = insert_vasm_instruction
				(comparison_output, instruction);
			// jle LC17
			instruction = spawn_vasm_op
				(VASM_JLE, 17, -1, false, false, false);
			comparison_output = insert_vasm_instruction
				(comparison_output, instruction);
			
			out_bool = compare_vasm_list 
				(code_generator_output, comparison_output);
			return out_bool;
		default:
			break;
	}
	return true;
}


bool
compare_vasm_instruction (VasmInstruction inp_vasm1, VasmInstruction inp_vasm2)
{
	bool label_data_id = false;
	bool label_data_type = false;
	bool line_number = false;
	bool op = false;
	bool regl = false;
	bool regr = false;
	bool regl_reg = false;
	bool regr_reg = false;

	if (inp_vasm1.label_data.id == inp_vasm2.label_data.id)
	{
		label_data_id = true;
	}
	if (inp_vasm1.label_data.type == inp_vasm2.label_data.type)
	{
		label_data_type = true;
	}
	if (inp_vasm1.line_number == inp_vasm2.line_number)
	{
		line_number = true;
	}
	if (inp_vasm1.op == inp_vasm2.op)
	{
		op = true;
	}
	if (inp_vasm1.regl == inp_vasm2.regl)
	{
		regl = true;
	}
	if (inp_vasm1.regr == inp_vasm2.regr)
	{
		regr = true;
	}
	if (inp_vasm1.regl_reg == inp_vasm2.regl_reg)
	{
		regl_reg = true;
	}
	if (inp_vasm1.regr_reg == inp_vasm2.regr_reg)
	{
		regr_reg = true;
	}


	if ((!label_data_id) || (!label_data_type) || (!line_number) || (!op)
			|| (!regl) || (!regr) || (!regl_reg) || (!regr_reg))
	{
		return false;
	}
	else
	{
		return true;
	}
}


bool
compare_vasm_list (VasmInstruction *inp_vasm1, VasmInstruction *inp_vasm2)
{
	bool list_equal = true;
	bool last_equal;
	while (inp_vasm1->next != NULL)
	{
		list_equal = compare_vasm_instruction (*inp_vasm1, *inp_vasm2);
		if (list_equal == false)
		{
			return false;
		}
		inp_vasm1 = inp_vasm1->next;
		inp_vasm2 = inp_vasm2->next;
	}
	last_equal = compare_vasm_instruction (*inp_vasm1, *inp_vasm2);
	if ((last_equal == true) && (list_equal == true))
	{
		return true;
	}
	else
	{
		return false;
	}
}


char *
get_test_string (size_t inp_rule_num)
{
	switch (inp_rule_num)
	{
		case 1:
			return "for i in range(5):\n    print(i)\nfor j in ra"
			       "nge(10):\n    print(j)";
		case 2:
			return "for i in range(5):\n    for j in range(10)"
			       ":\n        print(j)\n    print(i)";
		case 3:
			return "for i in range(5):\n    print(i)\n    for j"
			       " in range(10):\n        print(j)";
		case 4:
			return "for i in range(5):\n    for j in range(10)"
			       ":\n        for x in range(15):\n"
			       "            print(x)\n        print(j)\n"
			       "    print(i)";
		case 5:
			return "for a in range(1):\n    for b in range"
			       "(2):\n        for c in range(3)"
			       ":\n            for d in range(4)"
			       ":\n                for e in range(5)"
			       ":\n                    for f in range(6)"
			       ":\n                        for g in range(7)"
			       ":\n                            for h in range"
			       "(8):\n                                print(h"
			       ")\n                            print(g"
			       ")\n                        print(f"
			       ")\n                    print(e"
			       ")\n                print(d)\n            pr"
			       "int(c)\n        print(b)\n    print(a)";
		default:
			fprintf (stdout, "get_test_string error\n");
			break;
	}
	return NULL;
}
