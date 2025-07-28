/*
 * A simple testing apperatus. Helps ensure that changes made to parser don't
 * affect already semantically correct ASTs. We basically have a collection of
 * inputs with desired outputs already known. We run the parser on these inputs
 * and check to make sure the AST is as expected. (Equivilant to a unit test).
 */
#include "../Parser/parser.h"


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
