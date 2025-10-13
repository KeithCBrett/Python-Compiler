#include <gtest/gtest.h>
#include <string>
#include "../Lexer/lexer.h"
#include "../Lexer/lexer.c"


TEST(LexerTest, GetSourceFromFileValidInp) {
	FILE *inp_test_file = fopen ("inputfile.py", "w");
	const char *comp_string = "for i in range(5):\n    print(i)\0";
	fputs (comp_string, inp_test_file);
	fclose (inp_test_file);
	// get_source_from_file is for working with files. Since files have a
	// special EOF symbol at the end and raw strings do not, we chop off 
	// final char of our string.
	const char *comp_string2 = "for i in range(5):\n    print(i";
	FILE *test_file = fopen ("inputfile.py", "rb");
	char *out_string = get_source_from_file (test_file);
	fclose (test_file);
	EXPECT_STREQ (out_string, comp_string2);
}


TEST(LexerTest, IsAtEndValidInp) {
	Lexer lex;
	initialize_lexer ("\0");
	EXPECT_EQ (is_at_end (), true);
	initialize_lexer ("etc\0");
	EXPECT_EQ (is_at_end (), false);
}


TEST(LexerTest, SpawnTokenValidInp) {
	Lexer lex;
	const char *test_string = "for i in range(5):\n    print(i)";
	initialize_lexer(test_string);
	Token test_token = spawn_token (TOKEN_FOR);
	EXPECT_EQ(test_token.type, TOKEN_FOR);
	EXPECT_EQ(test_token.line_number, 1);
	EXPECT_EQ(test_token.length, 0);
	EXPECT_EQ(test_token.first_char, test_string);
}


TEST(LexerTest, SpawnErrorValidInp) {
	Lexer lex;
	const char *test_string = "for i in range(5):\n    print(i)";
	const char *test_err = "Test error";
	size_t test_err_len = strlen (test_err);
	initialize_lexer(test_string);
	static Token test_err_token = spawn_error (test_err);
	EXPECT_EQ(test_err_token.line_number, 1);
	EXPECT_EQ(test_err_token.type, TOKEN_ERROR);
	EXPECT_EQ(test_err_token.length, test_err_len);
	EXPECT_EQ(test_err_token.first_char, test_err);
}


TEST(LexerTest, ConsumeCharValidInp) {
	Lexer lex;
	const char *test_string = "for i in range(5):\n    print(i)";
	initialize_lexer(test_string);
	static char target_char = 'f';
	EXPECT_EQ(consume_char (), target_char);
}


TEST(LexerTest, PeakOnceValidInp) {
	Lexer lex;
	const char *test_string = "for i in range(5):\n    print(i)";
	initialize_lexer(test_string);
	EXPECT_EQ(peak_once(), 'f');
}


TEST(LexerTest, LookAheadValidInp) {
	Lexer lex;
	const char *test_string = "for i in range(5):\n    print(i)";
	initialize_lexer(test_string);
	EXPECT_EQ(look_ahead('f'), true);
	EXPECT_EQ(peak_once(), 'o');
}


TEST(LexerTest, SkipWhitespaceValidInp) {
	Lexer lex;
	const char *test_string = "       range(5):\n    print(i)";
	initialize_lexer(test_string);
	skip_whitespace(false);
	EXPECT_EQ(peak_once(), 'r');
}


TEST(LexerTest, IsDigitValidInp) {
	EXPECT_EQ(is_digit ('1'), true);
	EXPECT_EQ(is_digit ('9'), true);
	EXPECT_EQ(is_digit ('5'), true);
	EXPECT_EQ(is_digit ('a'), false);
	EXPECT_EQ(is_digit ('b'), false);
	EXPECT_EQ(is_digit ('c'), false);
}
