#include <gtest/gtest.h>
#include <string>
#include "../Lexer/lexer.h"
#include "../Lexer/lexer.c"


TEST (LexerTest, GetSourceFromFileValidInp)
{
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


TEST (LexerTest, IsAtEndValidInp)
{
	Lexer lex;
	initialize_lexer ("\0");
	EXPECT_EQ (is_at_end (), true);
	initialize_lexer ("etc\0");
	EXPECT_EQ (is_at_end (), false);
}


TEST (LexerTest, SpawnTokenValidInp)
{
	Lexer lex;
	const char *test_string = "for i in range(5):\n    print(i)";
	initialize_lexer (test_string);
	Token test_token = spawn_token (TOKEN_FOR);
	EXPECT_EQ (test_token.type, TOKEN_FOR);
	EXPECT_EQ (test_token.line_number, 1);
	EXPECT_EQ (test_token.length, 0);
	EXPECT_EQ (test_token.first_char, test_string);
}


TEST (LexerTest, SpawnErrorValidInp)
{
	Lexer lex;
	const char *test_string = "for i in range(5):\n    print(i)";
	const char *test_err = "Test error";
	size_t test_err_len = strlen (test_err);
	initialize_lexer (test_string);
	static Token test_err_token = spawn_error (test_err);
	EXPECT_EQ (test_err_token.line_number, 1);
	EXPECT_EQ (test_err_token.type, TOKEN_ERROR);
	EXPECT_EQ (test_err_token.length, test_err_len);
	EXPECT_EQ (test_err_token.first_char, test_err);
}


TEST (LexerTest, ConsumeCharValidInp)
{
	Lexer lex;
	const char *test_string = "for i in range(5):\n    print(i)";
	initialize_lexer (test_string);
	static char target_char = 'f';
	EXPECT_EQ (consume_char (), target_char);
}


TEST (LexerTest, PeakOnceValidInp)
{
	Lexer lex;
	const char *test_string = "for i in range(5):\n    print(i)";
	initialize_lexer (test_string);
	EXPECT_EQ (peak_once (), 'f');
}


TEST (LexerTest, LookAheadValidInp)
{
	Lexer lex;
	const char *test_string = "for i in range(5):\n    print(i)";
	initialize_lexer (test_string);
	EXPECT_EQ (look_ahead ('f'), true);
	EXPECT_EQ (peak_once (), 'o');
}


TEST (LexerTest, SkipWhitespaceValidInp)
{
	Lexer lex;
	const char *test_string = "       range(5):\n    print(i)";
	initialize_lexer (test_string);
	skip_whitespace (false);
	EXPECT_EQ (peak_once (), 'r');
}


TEST (LexerTest, IsDigitValidInp)
{
	EXPECT_EQ (is_digit ('1'), true);
	EXPECT_EQ (is_digit ('9'), true);
	EXPECT_EQ (is_digit ('5'), true);
	EXPECT_EQ (is_digit ('a'), false);
	EXPECT_EQ (is_digit ('b'), false);
	EXPECT_EQ (is_digit ('c'), false);
}


TEST (LexerTest, NumberValidInp)
{
	Lexer lex;
	const char *test_string1 = "100";
	initialize_lexer (test_string1);
	static Token test_token1 = number ();
	EXPECT_EQ (test_token1.type, TOKEN_INTEGER);
	const char *test_string2 = "1.00";
	initialize_lexer (test_string2);
	static Token test_token2 = number ();
	EXPECT_EQ (test_token2.type, TOKEN_FLOAT);
}


TEST (LexerTest, IsLetterValidInp)
{
	EXPECT_EQ (is_letter ('a'), true);
	EXPECT_EQ (is_letter ('z'), true);
	EXPECT_EQ (is_letter ('l'), true);
	EXPECT_EQ (is_letter ('0'), false);
	EXPECT_EQ (is_letter ('9'), false);
	EXPECT_EQ (is_letter ('5'), false);
}


TEST (LexerTest, SpawnIdentifierValidInp)
{
	Lexer lex;
	const char *test_string1 = "var1 = 10";
	initialize_lexer (test_string1);
	char c = consume_char ();
	static Token test_token = spawn_identifier ();
	EXPECT_EQ (test_token.line_number, 1);
	EXPECT_EQ (test_token.first_char, test_string1);
	EXPECT_EQ (test_token.length, 4);
	EXPECT_EQ (test_token.type, TOKEN_IDENTIFIER);

	const char *test_string2 = "n = 10";
	initialize_lexer (test_string2);
	c = consume_char ();
	static Token test_token2 = spawn_identifier ();
	EXPECT_EQ (test_token2.line_number, 1);
	EXPECT_EQ (test_token2.first_char, test_string2);
	EXPECT_EQ (test_token2.length, 1);
	EXPECT_EQ (test_token2.type, TOKEN_IDENTIFIER);
}


TEST (LexerTest, BoolCheckKeywordValidInp)
{
	const char *test_string1 = "for";
	initialize_lexer (test_string1);
	char c = consume_char ();
	EXPECT_EQ (bool_check_keyword ("or"), true);
	const char *test_string2 = "var1";
	initialize_lexer (test_string2);
	c = consume_char ();
	// Needs to return false when its not a match.
	EXPECT_EQ (bool_check_keyword ("or"), false);
}


TEST (LexerTest, CheckABranchValidInp)
{
	static Token tok;
	char c;

	const char *str1 = "abs";
	initialize_lexer (str1);
	c = consume_char ();
	tok = check_a_branch ();
	EXPECT_EQ (tok.type, TOKEN_ABS);

	const char *str2 = "aiter";
	initialize_lexer (str2);
	c = consume_char ();
	tok = check_a_branch ();
	EXPECT_EQ (tok.type, TOKEN_AITER);

	const char *str3 = "all";
	initialize_lexer (str3);
	c = consume_char ();
	tok = check_a_branch ();
	EXPECT_EQ (tok.type, TOKEN_ALL);

	const char *str4 = "and";
	initialize_lexer (str4);
	c = consume_char ();
	tok = check_a_branch ();
	EXPECT_EQ (tok.type, TOKEN_AND);

	const char *str5 = "any";
	initialize_lexer (str5);
	c = consume_char ();
	tok = check_a_branch ();
	EXPECT_EQ (tok.type, TOKEN_ANY);

	const char *str6 = "anext";
	initialize_lexer (str6);
	c = consume_char ();
	tok = check_a_branch ();
	EXPECT_EQ (tok.type, TOKEN_ANEXT);

	const char *str7 = "as";
	initialize_lexer (str7);
	c = consume_char ();
	tok = check_a_branch ();
	EXPECT_EQ (tok.type, TOKEN_AS);

	const char *str8 = "assert";
	initialize_lexer (str8);
	c = consume_char ();
	tok = check_a_branch ();
	EXPECT_EQ (tok.type, TOKEN_ASSERT);

	const char *str9 = "async";
	initialize_lexer (str9);
	c = consume_char ();
	tok = check_a_branch ();
	EXPECT_EQ (tok.type, TOKEN_ASYNC);

	const char *str10 = "ascii";
	initialize_lexer (str10);
	c = consume_char ();
	tok = check_a_branch ();
	EXPECT_EQ (tok.type, TOKEN_ASCII);

	const char *str11 = "await";
	initialize_lexer (str11);
	c = consume_char ();
	tok = check_a_branch ();
	EXPECT_EQ (tok.type, TOKEN_AWAIT);
}


TEST (LexerTest, CheckBBranchValidInp)
{
	char c;
	static Token tok;

	const char *str0 = "bin";
	initialize_lexer (str0);
	c = consume_char ();
	tok = check_b_branch ();
	EXPECT_EQ (tok.type, TOKEN_BIN);

	const char *str1 = "bool";
	initialize_lexer (str1);
	c = consume_char ();
	tok = check_b_branch ();
	EXPECT_EQ (tok.type, TOKEN_BOOL);

	const char *str2 = "breakpoint";
	initialize_lexer (str2);
	c = consume_char ();
	tok = check_b_branch ();
	EXPECT_EQ (tok.type, TOKEN_BREAKPOINT);

	const char *str3 = "break";
	initialize_lexer (str3);
	c = consume_char ();
	tok = check_b_branch ();
	EXPECT_EQ (tok.type, TOKEN_BREAK);

	const char *str4 = "bytearray";
	initialize_lexer (str4);
	c = consume_char ();
	tok = check_b_branch ();
	EXPECT_EQ (tok.type, TOKEN_BYTEARRAY);

	const char *str5 = "bytes";
	initialize_lexer (str5);
	c = consume_char ();
	tok = check_b_branch ();
	EXPECT_EQ (tok.type, TOKEN_BYTES);
}
