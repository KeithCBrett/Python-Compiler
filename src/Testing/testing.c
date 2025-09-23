#include "testing.h"


void
display_result (bool inp_bool, size_t inp_test_num)
{
	if (inp_bool)
	{
		fprintf (stdout, "-----------------------------------------------\n");
	}
	fprintf (stdout, "\n");
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
	fprintf (stdout, "-----------------------------------------------\n");
}


void
run_tests ()
{
	bool test_passed;
	for (size_t i = 1 ; i <= 6 ; i++)
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
			comparison_output = VASM_UNIT_zero_register
				(comparison_output, 5);
			// jmp LC2
			comparison_output = VASM_UNIT_jmp
				(comparison_output, 2);
			// LC3:
			comparison_output = VASM_UNIT_loop_const
				(comparison_output, 3);
			// mov rcx, r(5)
			comparison_output = VASM_UNIT_mov_reg_to_reg
				(comparison_output, REG_RCX, 5);
			// call printstr
			comparison_output = VASM_UNIT_call_vasm_func
				(comparison_output, VASM_FUNC_PRINTSTR);
			// add r(5), 1
			comparison_output = VASM_UNIT_add_const_to_reg
				(comparison_output, 5, 1);
			// LC2:
			comparison_output = VASM_UNIT_loop_const
				(comparison_output, 2);
			// cmp r(5), 4
			comparison_output = VASM_UNIT_cmp_const_to_reg
				(comparison_output, 5, 4);
			// jle LC3
			comparison_output = VASM_UNIT_jle_label
				(comparison_output, 3);
			// xor r(7), r(7)
			comparison_output = VASM_UNIT_zero_register
				(comparison_output, 7);
			// jmp LC4
			comparison_output = VASM_UNIT_jmp
				(comparison_output, 4);
			// LC5:
			comparison_output = VASM_UNIT_loop_const
				(comparison_output, 5);
			// mov rcx r(7)
			comparison_output = VASM_UNIT_mov_reg_to_reg
				(comparison_output, REG_RCX, 7);
			// call printstr
			comparison_output = VASM_UNIT_call_vasm_func
				(comparison_output, VASM_FUNC_PRINTSTR);
			// add r(7), 1
			comparison_output = VASM_UNIT_add_const_to_reg
				(comparison_output, 7, 1);
			// LC4:
			comparison_output = VASM_UNIT_loop_const
				(comparison_output, 4);
			// cmp r(7), 9
			comparison_output = VASM_UNIT_cmp_const_to_reg
				(comparison_output, 7, 9);
			// jle LC5
			comparison_output = VASM_UNIT_jle_label
				(comparison_output, 5);

			out_bool = compare_vasm_list 
				(code_generator_output, comparison_output);
			return out_bool;
		case 2:
			// xor r(5), r(5)
			comparison_output = VASM_UNIT_zero_register
				(comparison_output, 5);
			// jmp LC2
			comparison_output = VASM_UNIT_jmp
				(comparison_output, 2);
			// LC5:
			comparison_output = VASM_UNIT_loop_const
				(comparison_output, 5);
			// xor r(7), r(7)
			comparison_output = VASM_UNIT_zero_register
				(comparison_output, 7);
			// jmp LC3
			comparison_output = VASM_UNIT_jmp
				(comparison_output, 3);
			// LC4:
			comparison_output = VASM_UNIT_loop_const
				(comparison_output, 4);
			// mov rcx, r(7)
			comparison_output = VASM_UNIT_mov_reg_to_reg
				(comparison_output, REG_RCX, 7);
			// call printstr
			comparison_output = VASM_UNIT_call_vasm_func
				(comparison_output, VASM_FUNC_PRINTSTR);
			// add r(7), 1
			comparison_output = VASM_UNIT_add_const_to_reg
				(comparison_output, 7, 1);
			// LC3:
			comparison_output = VASM_UNIT_loop_const
				(comparison_output, 3);
			// cmp r(7), 9
			comparison_output = VASM_UNIT_cmp_const_to_reg
				(comparison_output, 7, 9);
			// jle LC4
			comparison_output = VASM_UNIT_jle_label
				(comparison_output, 4);
			// mov rcx, r(5)
			comparison_output = VASM_UNIT_mov_reg_to_reg
				(comparison_output, REG_RCX, 5);
			// call printstr
			comparison_output = VASM_UNIT_call_vasm_func
				(comparison_output, VASM_FUNC_PRINTSTR);
			// add r(5), 1
			comparison_output = VASM_UNIT_add_const_to_reg
				(comparison_output, 5, 1);
			// LC2:
			comparison_output = VASM_UNIT_loop_const
				(comparison_output, 2);
			// cmp r(5), 4
			comparison_output = VASM_UNIT_cmp_const_to_reg
				(comparison_output, 5, 4);
			// jle LC5
			comparison_output = VASM_UNIT_jle_label
				(comparison_output, 5);

			out_bool = compare_vasm_list 
				(code_generator_output, comparison_output);
			return out_bool;
		case 3:
			// xor r(5), r(5)
			comparison_output = VASM_UNIT_zero_register
				(comparison_output, 5);
			// jmp LC2
			comparison_output = VASM_UNIT_jmp
				(comparison_output, 2);
			// LC5:
			comparison_output = VASM_UNIT_loop_const
				(comparison_output, 5);
			// mov rcx, r(5)
			comparison_output = VASM_UNIT_mov_reg_to_reg
				(comparison_output, REG_RCX, 5);
			// call printstr
			comparison_output = VASM_UNIT_call_vasm_func
				(comparison_output, VASM_FUNC_PRINTSTR);
			// xor r(7), r(7)
			comparison_output = VASM_UNIT_zero_register
				(comparison_output, 7);
			// jmp LC3
			comparison_output = VASM_UNIT_jmp
				(comparison_output, 3);
			// LC4:
			comparison_output = VASM_UNIT_loop_const
				(comparison_output, 4);
			// mov rcx, r(7)
			comparison_output = VASM_UNIT_mov_reg_to_reg
				(comparison_output, REG_RCX, 7);
			// call printstr
			comparison_output = VASM_UNIT_call_vasm_func
				(comparison_output, VASM_FUNC_PRINTSTR);
			// add r(7), 1
			comparison_output = VASM_UNIT_add_const_to_reg
				(comparison_output, 7, 1);
			// LC3:
			comparison_output = VASM_UNIT_loop_const
				(comparison_output, 3);
			// cmp r(7), 9
			comparison_output = VASM_UNIT_cmp_const_to_reg
				(comparison_output, 7, 9);
			// jle LC4
			comparison_output = VASM_UNIT_jle_label
				(comparison_output, 4);
			// add r(5), 1
			comparison_output = VASM_UNIT_add_const_to_reg
				(comparison_output, 5, 1);
			// LC2:
			comparison_output = VASM_UNIT_loop_const
				(comparison_output, 2);
			// cmp r(5), 4
			comparison_output = VASM_UNIT_cmp_const_to_reg
				(comparison_output, 5, 4);
			// jle LC5
			comparison_output = VASM_UNIT_jle_label
				(comparison_output, 5);

			out_bool = compare_vasm_list 
				(code_generator_output, comparison_output);
			return out_bool;
		case 4:
			// xor r(5), r(5)
			comparison_output = VASM_UNIT_zero_register
				(comparison_output, 5);
			// jmp LC2
			comparison_output = VASM_UNIT_jmp
				(comparison_output, 2);
			// LC7:
			comparison_output = VASM_UNIT_loop_const
				(comparison_output, 7);
			// xor r(7), r(7)
			comparison_output = VASM_UNIT_zero_register
				(comparison_output, 7);
			// jmp LC3
			comparison_output = VASM_UNIT_jmp
				(comparison_output, 3);
			// LC6:
			comparison_output = VASM_UNIT_loop_const
				(comparison_output, 6);
			// xor r(9), r(9)
			comparison_output = VASM_UNIT_zero_register
				(comparison_output, 9);
			// jmp LC4
			comparison_output = VASM_UNIT_jmp
				(comparison_output, 4);
			// LC5:
			comparison_output = VASM_UNIT_loop_const
				(comparison_output, 5);
			// mov rcx, r(9)
			comparison_output = VASM_UNIT_mov_reg_to_reg
				(comparison_output, REG_RCX, 9);
			// call printstr
			comparison_output = VASM_UNIT_call_vasm_func
				(comparison_output, VASM_FUNC_PRINTSTR);
			// add r(9), 1
			comparison_output = VASM_UNIT_add_const_to_reg
				(comparison_output, 9, 1);
			// LC4:
			comparison_output = VASM_UNIT_loop_const
				(comparison_output, 4);
			// cmp r(9), 14
			comparison_output = VASM_UNIT_cmp_const_to_reg
				(comparison_output, 9, 14);
			// jle LC5
			comparison_output = VASM_UNIT_jle_label
				(comparison_output, 5);
			// mov rcx, r(7)
			comparison_output = VASM_UNIT_mov_reg_to_reg
				(comparison_output, REG_RCX, 7);
			// call printstr
			comparison_output = VASM_UNIT_call_vasm_func
				(comparison_output, VASM_FUNC_PRINTSTR);
			// add r(7), 1
			comparison_output = VASM_UNIT_add_const_to_reg
				(comparison_output, 7, 1);
			// LC3:
			comparison_output = VASM_UNIT_loop_const
				(comparison_output, 3);
			// cmp r(7), 9
			comparison_output = VASM_UNIT_cmp_const_to_reg
				(comparison_output, 7, 9);
			// jle LC6
			comparison_output = VASM_UNIT_jle_label
				(comparison_output, 6);
			// mov rcx, r(5)
			comparison_output = VASM_UNIT_mov_reg_to_reg
				(comparison_output, REG_RCX, 5);
			// call printstr
			comparison_output = VASM_UNIT_call_vasm_func
				(comparison_output, VASM_FUNC_PRINTSTR);
			// add r(5), 1
			comparison_output = VASM_UNIT_add_const_to_reg
				(comparison_output, 5, 1);
			// LC2:
			comparison_output = VASM_UNIT_loop_const
				(comparison_output, 2);
			// cmp r(5), 4
			comparison_output = VASM_UNIT_cmp_const_to_reg
				(comparison_output, 5, 4);
			// jle LC7
			comparison_output = VASM_UNIT_jle_label
				(comparison_output, 7);

			out_bool = compare_vasm_list 
				(code_generator_output, comparison_output);
			return out_bool;
		case 5:
			// xor r(5), r(5)
			comparison_output = VASM_UNIT_zero_register
				(comparison_output, 5);
			// jmp LC2
			comparison_output = VASM_UNIT_jmp
				(comparison_output, 2);
			// LC17:
			comparison_output = VASM_UNIT_loop_const
				(comparison_output, 17);
			// xor r(7), r(7)
			comparison_output = VASM_UNIT_zero_register
				(comparison_output, 7);
			// jmp LC3
			comparison_output = VASM_UNIT_jmp
				(comparison_output, 3);
			// LC16:
			comparison_output = VASM_UNIT_loop_const
				(comparison_output, 16);
			// xor r(9), r(9)
			comparison_output = VASM_UNIT_zero_register
				(comparison_output, 9);
			// jmp LC4
			comparison_output = VASM_UNIT_jmp
				(comparison_output, 4);
			// LC15:
			comparison_output = VASM_UNIT_loop_const
				(comparison_output, 15);
			// xor r(11), r(11)
			comparison_output = VASM_UNIT_zero_register
				(comparison_output, 11);
			// jmp LC5
			comparison_output = VASM_UNIT_jmp
				(comparison_output, 5);
			// LC14:
			comparison_output = VASM_UNIT_loop_const
				(comparison_output, 14);
			// xor r(13), r(13)
			comparison_output = VASM_UNIT_zero_register
				(comparison_output, 13);
			// jmp LC6
			comparison_output = VASM_UNIT_jmp
				(comparison_output, 6);
			// LC13:
			comparison_output = VASM_UNIT_loop_const
				(comparison_output, 13);
			// xor r(15), r(15)
			comparison_output = VASM_UNIT_zero_register
				(comparison_output, 15);
			// jmp LC7
			comparison_output = VASM_UNIT_jmp
				(comparison_output, 7);
			// LC12:
			comparison_output = VASM_UNIT_loop_const
				(comparison_output, 12);
			// xor r(17), r(17)
			comparison_output = VASM_UNIT_zero_register
				(comparison_output, 17);
			// jmp LC8
			comparison_output = VASM_UNIT_jmp
				(comparison_output, 8);
			// LC11:
			comparison_output = VASM_UNIT_loop_const
				(comparison_output, 11);
			// xor r(19), r(19)
			comparison_output = VASM_UNIT_zero_register
				(comparison_output, 19);
			// jmp LC9
			comparison_output = VASM_UNIT_jmp
				(comparison_output, 9);
			// LC10:
			comparison_output = VASM_UNIT_loop_const
				(comparison_output, 10);
			// mov rcx, r(19)
			comparison_output = VASM_UNIT_mov_reg_to_reg
				(comparison_output, REG_RCX, 19);
			// call printstr
			comparison_output = VASM_UNIT_call_vasm_func
				(comparison_output, VASM_FUNC_PRINTSTR);
			// add r(19), 1
			comparison_output = VASM_UNIT_add_const_to_reg
				(comparison_output, 19, 1);
			// LC9:
			comparison_output = VASM_UNIT_loop_const
				(comparison_output, 9);
			// cmp r(19), 7
			comparison_output = VASM_UNIT_cmp_const_to_reg
				(comparison_output, 19, 7);
			// jle LC10
			comparison_output = VASM_UNIT_jle_label
				(comparison_output, 10);
			// mov rcx, r(17)
			comparison_output = VASM_UNIT_mov_reg_to_reg
				(comparison_output, REG_RCX, 17);
			// call printstr
			comparison_output = VASM_UNIT_call_vasm_func
				(comparison_output, VASM_FUNC_PRINTSTR);
			// add r(17), 1
			comparison_output = VASM_UNIT_add_const_to_reg
				(comparison_output, 17, 1);
			// LC8:
			comparison_output = VASM_UNIT_loop_const
				(comparison_output, 8);
			// cmp r(17), 6
			comparison_output = VASM_UNIT_cmp_const_to_reg
				(comparison_output, 17, 6);
			// jle LC11
			comparison_output = VASM_UNIT_jle_label
				(comparison_output, 11);
			// mov rcx, r(15)
			comparison_output = VASM_UNIT_mov_reg_to_reg
				(comparison_output, REG_RCX, 15);
			// call printstr
			comparison_output = VASM_UNIT_call_vasm_func
				(comparison_output, VASM_FUNC_PRINTSTR);
			// add r(15), 1
			comparison_output = VASM_UNIT_add_const_to_reg
				(comparison_output, 15, 1);
			// LC7:
			comparison_output = VASM_UNIT_loop_const
				(comparison_output, 7);
			// cmp r(15), 5
			comparison_output = VASM_UNIT_cmp_const_to_reg
				(comparison_output, 15, 5);
			// jle LC12
			comparison_output = VASM_UNIT_jle_label
				(comparison_output, 12);
			// mov rcx, r(13)
			comparison_output = VASM_UNIT_mov_reg_to_reg
				(comparison_output, REG_RCX, 13);
			// call printstr
			comparison_output = VASM_UNIT_call_vasm_func
				(comparison_output, VASM_FUNC_PRINTSTR);
			// add r(13), 1
			comparison_output = VASM_UNIT_add_const_to_reg
				(comparison_output, 13, 1);
			// LC6:
			comparison_output = VASM_UNIT_loop_const
				(comparison_output, 6);
			// cmp r(13), 4
			comparison_output = VASM_UNIT_cmp_const_to_reg
				(comparison_output, 13, 4);
			// jle LC13
			comparison_output = VASM_UNIT_jle_label
				(comparison_output, 13);
			// mov rcx, r(11)
			comparison_output = VASM_UNIT_mov_reg_to_reg
				(comparison_output, REG_RCX, 11);
			// call printstr
			comparison_output = VASM_UNIT_call_vasm_func
				(comparison_output, VASM_FUNC_PRINTSTR);
			// add r(11), 1
			comparison_output = VASM_UNIT_add_const_to_reg
				(comparison_output, 11, 1);
			// LC5:
			comparison_output = VASM_UNIT_loop_const
				(comparison_output, 5);
			// cmp r(11), 3
			comparison_output = VASM_UNIT_cmp_const_to_reg
				(comparison_output, 11, 3);
			// jle LC14
			comparison_output = VASM_UNIT_jle_label
				(comparison_output, 14);
			// mov rcx, r(9)
			comparison_output = VASM_UNIT_mov_reg_to_reg
				(comparison_output, REG_RCX, 9);
			// call printstr
			comparison_output = VASM_UNIT_call_vasm_func
				(comparison_output, VASM_FUNC_PRINTSTR);
			// add r(9), 1
			comparison_output = VASM_UNIT_add_const_to_reg
				(comparison_output, 9, 1);
			// LC4:
			comparison_output = VASM_UNIT_loop_const
				(comparison_output, 4);
			// cmp r(9), 2
			comparison_output = VASM_UNIT_cmp_const_to_reg
				(comparison_output, 9, 2);
			// jle LC15
			comparison_output = VASM_UNIT_jle_label
				(comparison_output, 15);
			// mov rcx, r(7)
			comparison_output = VASM_UNIT_mov_reg_to_reg
				(comparison_output, REG_RCX, 7);
			// call printstr
			comparison_output = VASM_UNIT_call_vasm_func
				(comparison_output, VASM_FUNC_PRINTSTR);
			// add r(7), 1
			comparison_output = VASM_UNIT_add_const_to_reg
				(comparison_output, 7, 1);
			// LC3:
			comparison_output = VASM_UNIT_loop_const
				(comparison_output, 3);
			// cmp r(7), 1
			comparison_output = VASM_UNIT_cmp_const_to_reg
				(comparison_output, 7, 1);
			// jle LC16
			comparison_output = VASM_UNIT_jle_label
				(comparison_output, 16);
			// mov rcx, r(5)
			comparison_output = VASM_UNIT_mov_reg_to_reg
				(comparison_output, REG_RCX, 5);
			// call printstr
			comparison_output = VASM_UNIT_call_vasm_func
				(comparison_output, VASM_FUNC_PRINTSTR);
			// add r(5), 1
			comparison_output = VASM_UNIT_add_const_to_reg
				(comparison_output, 5, 1);
			// LC2:
			comparison_output = VASM_UNIT_loop_const
				(comparison_output, 2);
			// cmp r(5), 0
			comparison_output = VASM_UNIT_cmp_const_to_reg
				(comparison_output, 5, 0);
			// jle LC17
			comparison_output = VASM_UNIT_jle_label
				(comparison_output, 17);
			
			out_bool = compare_vasm_list 
				(code_generator_output, comparison_output);
			return out_bool;
		case 6:
			// xor r(5), r(5)
			comparison_output = VASM_UNIT_zero_register
				(comparison_output, 5);
			// mov r(6), 0
			comparison_output = VASM_UNIT_mov_const_to_reg
				(comparison_output, 6, 0);
			// mov r(5), r(6)
			comparison_output = VASM_UNIT_mov_reg_to_reg
				(comparison_output, 5, 6);
			// xor r(7), r(7)
			comparison_output = VASM_UNIT_zero_register
				(comparison_output, 7);
			// jmp LC2
			comparison_output = VASM_UNIT_jmp
				(comparison_output, 2);
			// LC5:
			comparison_output = VASM_UNIT_loop_const
				(comparison_output, 5);
			// xor r(9), r(9)
			comparison_output = VASM_UNIT_zero_register
				(comparison_output, 9);
			// jmp LC3
			comparison_output = VASM_UNIT_jmp
				(comparison_output, 3);
			// LC4:
			comparison_output = VASM_UNIT_loop_const
				(comparison_output, 4);
			// mov r(2), r(9)
			comparison_output = VASM_UNIT_mov_reg_to_reg
				(comparison_output, 2, 9);
			// call printstr
			comparison_output = VASM_UNIT_call_vasm_func
				(comparison_output, VASM_FUNC_PRINTSTR);
			// xor r(11), r(11)
			comparison_output = VASM_UNIT_zero_register
				(comparison_output, 11);
			// mov r(12), 16
			comparison_output = VASM_UNIT_mov_const_to_reg
				(comparison_output, 12, 16);
			// mov r(13), 9
			comparison_output = VASM_UNIT_mov_const_to_reg
				(comparison_output, 13, 9);
			// add r(12), r(13)
			comparison_output = VASM_UNIT_add_reg_to_reg
				(comparison_output, 12, 13);
			// mov r(11), r(12)
			comparison_output = VASM_UNIT_mov_reg_to_reg
				(comparison_output, 11, 12);
			// mov r(2), r(12)
			comparison_output = VASM_UNIT_mov_reg_to_reg
				(comparison_output, REG_RCX, 12);
			// call printstr
			comparison_output = VASM_UNIT_call_vasm_func
				(comparison_output, VASM_FUNC_PRINTSTR);
			// add r(9), 1
			comparison_output = VASM_UNIT_add_const_to_reg
				(comparison_output, 9, 1);
			// LC3:
			comparison_output = VASM_UNIT_loop_const
				(comparison_output, 3);
			// cmp r(9), 9
			comparison_output = VASM_UNIT_cmp_const_to_reg
				(comparison_output, 9, 9);
			// jle LC4
			comparison_output = VASM_UNIT_jle_label
				(comparison_output, 4);
			// mov r(2), r(7)
			comparison_output = VASM_UNIT_mov_reg_to_reg
				(comparison_output, REG_RCX, 7);
			// call printstr
			comparison_output = VASM_UNIT_call_vasm_func
				(comparison_output, VASM_FUNC_PRINTSTR);
			// xor r(14), r(14)
			comparison_output = VASM_UNIT_zero_register
				(comparison_output, 14);
			// mov r(15), r(6)
			comparison_output = VASM_UNIT_mov_reg_to_reg
				(comparison_output, 15, 6);
			// mov r(16), r(7)
			comparison_output = VASM_UNIT_mov_reg_to_reg
				(comparison_output, 16, 7);
			// add r(15), r(16)
			comparison_output = VASM_UNIT_add_reg_to_reg
				(comparison_output, 15, 16);
			// mov r(14), r(15)
			comparison_output = VASM_UNIT_mov_reg_to_reg
				(comparison_output, 14, 15);
			// add r(7), 1
			comparison_output = VASM_UNIT_add_const_to_reg
				(comparison_output, 7, 1);
			// LC2:
			comparison_output = VASM_UNIT_loop_const
				(comparison_output, 2);
			// cmp r(7), 4
			comparison_output = VASM_UNIT_cmp_const_to_reg
				(comparison_output, 7, 4);
			// jle LC5
			comparison_output = VASM_UNIT_jle_label
				(comparison_output, 5);
			// mov r(2), r(14)
			comparison_output = VASM_UNIT_mov_reg_to_reg
				(comparison_output, REG_RCX, 14);
			// call printstr
			comparison_output = VASM_UNIT_call_vasm_func
				(comparison_output, VASM_FUNC_PRINTSTR);

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
			fprintf (stderr, "\nExpected: %d  %zu, %zu, (%d, %d)\n",
					inp_vasm2->op, inp_vasm2->regl,
					inp_vasm2->regr, inp_vasm2->regl_reg,
					inp_vasm2->regr_reg);
			fprintf (stderr, "\nObtained: %d  %zu, %zu, (%d, %d)\n",
					inp_vasm1->op, inp_vasm1->regl,
					inp_vasm1->regr, inp_vasm1->regl_reg,
					inp_vasm1->regr_reg);
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
		fprintf (stderr, "\nExpected: %d  %zu, %zu, (%d, %d)\n",
				inp_vasm2->op, inp_vasm2->regl,
				inp_vasm2->regr, inp_vasm2->regl_reg,
				inp_vasm2->regr_reg);
		fprintf (stderr, "\nObtained: %d  %zu, %zu, (%d, %d)\n",
				inp_vasm1->op, inp_vasm1->regl,
				inp_vasm1->regr, inp_vasm1->regl_reg,
				inp_vasm1->regr_reg);
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
		case 6:
			return "y = 0\nfor i in range(5):\n    for j in range"
			       "(10):\n        print(j)\n        x = 16 + "
			       "9\n        print(x)\n    print(i)\n    y = y"
			       " + i\nprint(y)";
		default:
			fprintf (stdout, "get_test_string error\n");
			break;
	}
	return NULL;
}


VasmInstruction *
VASM_UNIT_loop_const (VasmInstruction *inp_vasm, size_t inp_const)
{
	VasmInstruction *instruction = spawn_vasm_op
		(VASM_LOOP_CONST, inp_const, -1, false, false, true);
	inp_vasm = insert_vasm_instruction
		(inp_vasm, instruction);
	return inp_vasm;
}


VasmInstruction *
VASM_UNIT_zero_register (VasmInstruction *inp_vasm, size_t inp_reg_num)
{
	VasmInstruction *instruction = spawn_vasm_op
		(VASM_XOR, inp_reg_num, inp_reg_num, true, true, false);
	inp_vasm = insert_vasm_instruction
		(inp_vasm, instruction);
	return inp_vasm;
}


VasmInstruction *
VASM_UNIT_jmp (VasmInstruction *inp_vasm, size_t inp_const)
{
	VasmInstruction *instruction = spawn_vasm_op
		(VASM_JMP, inp_const, -1, false, false, false);
	inp_vasm = insert_vasm_instruction
		(inp_vasm, instruction);
	return inp_vasm;
}


VasmInstruction *
VASM_UNIT_mov_reg_to_reg (VasmInstruction *inp_vasm, size_t l, size_t r)
{
	VasmInstruction *instruction = spawn_vasm_op
		(VASM_MOV, l, r, true, true, false);
	inp_vasm = insert_vasm_instruction
		(inp_vasm, instruction);
	return inp_vasm;
}


VasmInstruction *
VASM_UNIT_call_vasm_func (VasmInstruction *inp_vasm, size_t func)
{
	VasmInstruction *instruction = spawn_vasm_op
		(VASM_CALL, func, -1, false, false, false);
	inp_vasm = insert_vasm_instruction
		(inp_vasm, instruction);
	return inp_vasm;
}


VasmInstruction *
VASM_UNIT_add_const_to_reg (VasmInstruction *inp_vasm, size_t l, size_t r)
{
	VasmInstruction *instruction = spawn_vasm_op
		(VASM_ADD, l, r, true, false, false);
	inp_vasm = insert_vasm_instruction
		(inp_vasm, instruction);
	return inp_vasm;
}


VasmInstruction *
VASM_UNIT_cmp_const_to_reg (VasmInstruction *inp_vasm, size_t l, size_t r)
{
	VasmInstruction *instruction = spawn_vasm_op
		(VASM_CMP, l, r, true, false, false);
	inp_vasm = insert_vasm_instruction
		(inp_vasm, instruction);
	return inp_vasm;
}


VasmInstruction *
VASM_UNIT_jle_label (VasmInstruction *inp_vasm, size_t inp_label_num)
{
	VasmInstruction *instruction = spawn_vasm_op
		(VASM_JLE, inp_label_num, -1, false, false, false);
	inp_vasm = insert_vasm_instruction
		(inp_vasm, instruction);
	return inp_vasm;
}


VasmInstruction *
VASM_UNIT_mov_const_to_reg (VasmInstruction *inp_vasm, size_t l, size_t r)
{
	VasmInstruction *instruction = spawn_vasm_op
		(VASM_MOV, l, r, true, false, false);
	inp_vasm = insert_vasm_instruction
		(inp_vasm, instruction);
	return inp_vasm;
}


VasmInstruction *
VASM_UNIT_add_reg_to_reg (VasmInstruction *inp_vasm, size_t l, size_t r)
{
	VasmInstruction *instruction = spawn_vasm_op
		(VASM_ADD, l, r, true, true, false);
	inp_vasm = insert_vasm_instruction
		(inp_vasm, instruction);
	return inp_vasm;
}
