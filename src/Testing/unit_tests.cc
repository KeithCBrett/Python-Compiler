#include <gtest/gtest.h>
#include <string>
#include "../Lexer/lexer.h"
#include "../Lexer/lexer.c"


TEST(LexerTest, GetSourceFromFileValidInp) {
	FILE *inp_test_file = fopen ("inputfile.py", "w");
	const char *comp_string = "for i in range(5):\n    print(i)";
	fputs (comp_string, inp_test_file);
	fclose (inp_test_file);
	// get_source_from_file is for working with files. Since files have a
	// special EOF symbol at the end and raw strings do not, we chop off 
	// final char of our string.
	const char *comp_string2 = "for i in range(5):\n    print(i";
	FILE *test_file = fopen ("inputfile.py", "rb");
	char *out_string = get_source_from_file (test_file);
	fclose (test_file);
	EXPECT_STREQ(out_string, comp_string2);
}
