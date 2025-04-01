#include "testing.h"


bool
test_ast (TreeNode *gen, TreeNode *expected)
{
	size_t num_gen_nodes = count_tree_nodes (gen);
	size_t num_expected_nodes = count_tree_nodes (expected);
	if (num_gen_nodes != num_expected_nodes)
	{
		fprintf (stderr, "Error, tree size mismatch.\n");
		fprintf (stderr, "Expected: %zu\n", num_expected_nodes);
		fprintf (stderr, "Actual size: %zu\n", num_gen_nodes);
		return false;
	}
	else
	{
		TreeNode **gen_arr = preorder (gen, num_gen_nodes);
		TreeNode **exp_arr = preorder (expected, num_expected_nodes);
		for (size_t i = 0 ; i < num_gen_nodes ; i++)
		{
			TreeNode *p_gen_arr = gen_arr[i];
			TreeNode *p_exp_arr = exp_arr[i];
			if (p_gen_arr->contents.type
					!= p_exp_arr->contents.type)
			{
				fprintf (stderr, "Error, token type mismatch.\n");
				return false;
			}
		}
		return true;
	}
}


void
test_parser ()
{
	fprintf (stderr, "\n");
	bool tests_passed = true;
	size_t num_tests_failed = 0;
	fprintf (stderr, "test_parser ():\n\n");


	// Test 1: Simple assignment 1.
	// Input: x = 3
	TreeNode *exp1 = spawn_node (spawn_token (TOKEN_EQUALS));
	exp1->left = spawn_node (spawn_token (TOKEN_IDENTIFIER));
	exp1->right = spawn_node (spawn_token (TOKEN_INTEGER));
	TreeNode *gen1 = NULL;
	bool gen1bool = false;
	initialize_lexer ("x = 3");
	gen1 = parse (Prec_Start, gen1, gen1bool);
	if (test_ast (gen1, exp1))
	{
		fprintf (stderr, "Test 1:\n");
		fprintf (stderr, "\tx = 3\n");
		fprintf (stderr, "\tPASS\n");
	}
	else
	{
		fprintf (stderr, "Test 1:\n\tFAIL\n");
		tests_passed = false;
		num_tests_failed++;
	}


	// Test 2: Simple assignment 2
	// Input: y = 2 + 3
	TreeNode *exp2 = spawn_node (spawn_token (TOKEN_EQUALS));
	exp2->left = spawn_node (spawn_token (TOKEN_IDENTIFIER));
	exp2->right = spawn_node (spawn_token (TOKEN_ADD));
	exp2->right->left = spawn_node (spawn_token (TOKEN_INTEGER));
	exp2->right->right = spawn_node (spawn_token (TOKEN_INTEGER));
	TreeNode *gen2 = NULL;
	bool gen2bool = false;
	initialize_lexer ("y = 2 + 3");
	gen2 = parse (Prec_Start, gen2, gen2bool);
	if (test_ast (gen2, exp2))
	{
		fprintf (stderr, "Test 2:\n");
		fprintf (stderr, "\ty = 2 + 3\n");
		fprintf (stderr, "\tPASS\n");
	}
	else
	{
		fprintf (stderr, "Test 2:\n\tFAIL\n");
		tests_passed = false;
		num_tests_failed++;
	}


	fprintf (stderr, "\n");
	fprintf (stderr, "SUMMARY:\n");
	if (tests_passed)
	{
		fprintf(stderr, "ALL TESTS PASSED\n");
	}
	else
	{
		fprintf(stderr, "ERROR, %zu tests failed\n", num_tests_failed);
	}
	fprintf (stderr, "\n");
}
