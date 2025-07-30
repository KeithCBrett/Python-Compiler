#ifndef testing_h
#define testing_h
#include "../Parser/parser.h"
#include "../Code_Generator/code_generator.h"

/*
 * A simple testing apperatus. Helps ensure that changes made to parser don't
 * affect already semantically correct ASTs. We basically have a collection of
 * inputs with desired outputs already known. We run the parser on these inputs
 * and check to make sure the AST is as expected. (Equivilant to a unit test).
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
#endif
