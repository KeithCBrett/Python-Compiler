#ifndef testing_h
#define testing_h
#include "../Code_Generator/code_generator.h"


/*
 * Testing apparatus for modules and submodules. So far we have test for:
 * - Code generation module.
*/


/*
 * display_result ()
 * Input:
 * 	bool	<- Bool describing whether the test was passed or not.
 * 	size_t	<- Test number for display purposes.
 * Output:
 * 	void	<- Displays the number of the test, as well as whether the
 * 		test was passed or not to stdout.
*/
void
display_result (bool, size_t);


/*
 * run_tests ()
 * Input:
 * 	None	<- We run all of our instruction selection tests.
*/
void
run_tests ();


/*
 * test_vasm ()
 * Input:
 * 	size_t	<- Test number, so we know which test we are running.
 * Output:
 * 	bool	<- Whether the test passed or not.
*/
bool
test_vasm (size_t);


/*
 * compare_vasm ()
 * Input:
 * 	VasmInstruction	<- A vasm instruction. We will compare it with another
 * 			one by checking equality between each struct member.
 * 	VasmInstruction <- The other instruction in the comparison.
 * Output:
 * 	bool		<- Describes whether the two instructions are
 * 			identical or not.
*/
bool
compare_vasm_instruction (VasmInstruction, VasmInstruction);

bool
compare_vasm_list (VasmInstruction *, VasmInstruction *);

/*
 * get_test_string ()
 * Input:
 * 	size_t	<- Rule number decides which python source we use as a test.
 * Output:
 * 	char *	<- A string representing the python source we are compiling.
*/
char *
get_test_string (size_t);


/*
 * VASM_spawn_loop_const ()
 * Input:
 * 	VasmInstruction *	<- Linked list of VASM instructions so far.
 *
 * 	size_t			<- Number to associate with loop const.
 * Output:
 * 	VasmInstruction *	<- Updated linked list
*/
VasmInstruction *
VASM_UNIT_loop_const (VasmInstruction *, size_t);


/*
 * VASM_UNIT_zero_register ()
 * Input:
 * 	VasmInstruction *	<- Linked list of VASM instructions so far.
 *
 * 	size_t			<- Register number of register we want to
 * 				zero.
 * Output:
 * 	VasmInstruction *	<- Updated linked list
*/
VasmInstruction *
VASM_UNIT_zero_register (VasmInstruction *, size_t);


/*
 * VASM_UNIT_jmp ()
 * Input:
 * 	VasmInstruction *	<- Linked list of VASM instructions so far.
 *
 * 	size_t			<- Const associated with label we want to jmp
 * 				to.
 * Output:
 * 	VasmInstruction *	<- Updated linked list
*/
VasmInstruction *
VASM_UNIT_jmp (VasmInstruction *, size_t);


/*
 * VASM_UNIT_mov_reg_into_reg ()
 * Input:
 * 	VasmInstruction *	<- Linked list of VASM instructions so far.
 *
 * 	size_t			<- Left register number.
 *
 * 	size_t			<- Right register number.
 * Output:
 * 	VasmInstruction *	<- Updated linked list
*/
VasmInstruction *
VASM_UNIT_mov_reg_to_reg (VasmInstruction *, size_t, size_t);


/*
 * VASM_UNIT_call_vasm_func ()
 * Input:
 * 	VasmInstruction *	<- Linked list of VASM instructions so far.
 *
 * 	size_t			<- Number associated with function.
 * Output:
 * 	VasmInstruction *	<- Updated linked list
*/
VasmInstruction *
VASM_UNIT_call_vasm_func (VasmInstruction *, size_t);


/*
 * VASM_UNIT_add_const_into_reg ()
 * Input:
 * 	VasmInstruction *	<- Linked list of VASM instructions so far.
 *
 * 	size_t			<- Left register.
 *
 * 	size_t			<- Right const.
 * Output:
 * 	VasmInstruction *	<- Updated linked list
*/
VasmInstruction *
VASM_UNIT_add_const_to_reg (VasmInstruction *, size_t, size_t);


/*
 * VASM_UNIT_add_const_into_reg ()
 * Input:
 * 	VasmInstruction *	<- Linked list of VASM instructions so far.
 *
 * 	size_t			<- Left register.
 *
 * 	size_t			<- Right const.
 * Output:
 * 	VasmInstruction *	<- Updated linked list
*/
VasmInstruction *
VASM_UNIT_cmp_const_to_reg (VasmInstruction *, size_t, size_t);


/*
 * VASM_UNIT_jle_label ()
 * Input:
 * 	VasmInstruction *	<- Linked list of VASM instructions so far.
 *
 * 	size_t			<- Label number to jump to.
 * Output:
 * 	VasmInstruction *	<- Updated linked list
*/
VasmInstruction *
VASM_UNIT_cmp_const_to_reg (VasmInstruction *, size_t, size_t);


/*
 * VASM_UNIT_jle_label ()
 * Input:
 * 	VasmInstruction *	<- Linked list of VASM instructions so far.
 *
 * 	size_t			<- Label number to jump to.
 * Output:
 * 	VasmInstruction *	<- Updated linked list
*/
VasmInstruction *
VASM_UNIT_jle_label (VasmInstruction *, size_t);


/*
 * VASM_UNIT_mov_const_to_reg ()
 * Input:
 * 	VasmInstruction *	<- Linked list of VASM instructions so far.
 *
 * 	size_t			<- Left register.
 *
 * 	size_t			<- Right const.
 * Output:
 * 	VasmInstruction *	<- Updated linked list
*/
VasmInstruction *
VASM_UNIT_mov_const_to_reg (VasmInstruction *, size_t, size_t);



/*
 * VASM_UNIT_add_reg_to_reg ()
 * Input:
 * 	VasmInstruction *	<- Linked list of VASM instructions so far.
 *
 * 	size_t			<- Left register.
 *
 * 	size_t			<- Right register.
 * Output:
 * 	VasmInstruction *	<- Updated linked list
*/
VasmInstruction *
VASM_UNIT_add_reg_to_reg (VasmInstruction *, size_t, size_t);
#endif
