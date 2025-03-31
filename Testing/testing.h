/*
 * A simple testing apperatus. Helps ensure that changes made to parser don't
 * affect already semantically correct ASTs. We basically have a collection of
 * inputs with desired outputs already known. We run the parser on these inputs
 * and check to make sure the AST is as expected. (Equivilant to a unit test).
 */
#include "../Parser/parser.h"


// Input: Two AST's, one is the AST parse generated, the other one is the AST
// we expected parse to generate.
// Output: A boolean representing whether the tree was as expected or not.
bool
test_ast (TreeNode *, TreeNode *);


// Input: Nothing.
// Output: Runs the parser testing aperatus proper. Runs test_ast a whole bunch
// and displays the results of the tests.
void
test_parser ();
