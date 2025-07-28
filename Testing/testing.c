#include "testing.h"


void
display_result (bool inp_bool, size_t inp_test_num)
{
	printf ("---------------------------\n");
	printf ("\tTest %zu\n\n", inp_test_num);
	if (inp_bool)
	{
		printf ("STATUS: PASSED\n");
	}
	else
	{
		printf ("STATUS: FAILED\n");
	}
	printf ("---------------------------\n");
}
