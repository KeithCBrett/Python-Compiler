#ifndef lexer_h
#define lexer_h


#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>


typedef struct {
	const char *start_char;
	const char *current_char;
	size_t line_number;
} Lexer;


typedef enum {
	TOKEN_ADD = 0,
	TOKEN_MINUS,
	TOKEN_MULTIPLICATION,
	TOKEN_DIVISION,
	TOKEN_EOF,
	TOKEN_ERROR,
	TOKEN_NEWLINE,
	TOKEN_LEFT_PAREN,
	TOKEN_RIGHT_PAREN,
	TOKEN_EQUALS,
	TOKEN_EQUAL_EQUAL = 10,
	TOKEN_LESS_THAN,
	TOKEN_GREATER_THAN,
	TOKEN_MODULUS,
	TOKEN_EXPONENTIATION,
	TOKEN_FLOOR_DIVISION,
	TOKEN_ADD_ASSIGN,
	TOKEN_MODULUS_ASSIGN,
	TOKEN_LEFT_CURLY,
	TOKEN_RIGHT_CURLY,
	TOKEN_LEFT_SQUARE = 20,
	TOKEN_RIGHT_SQUARE,
	TOKEN_PERIOD,
	TOKEN_SEMI_COLON,
	TOKEN_AMPERSAND,
	TOKEN_AMPERSAND_ASSIGN,
	TOKEN_COMMA,
	TOKEN_MATRIX_MULT_ASSIGN,
	TOKEN_MATRIX_MULT,
	TOKEN_XOR_ASSIGN,
	TOKEN_XOR = 30,
	TOKEN_COLON,
	TOKEN_BNF,
	TOKEN_NOT,
	TOKEN_NOT_EQUAL,
	TOKEN_OR,
	TOKEN_OR_ASSIGN,
	TOKEN_TILDA,
	TOKEN_ARROW,
	TOKEN_MINUS_ASSIGN,
	TOKEN_EXPONENTIATION_ASSIGN = 40,
	TOKEN_MULTIPLICATION_ASSIGN,
	TOKEN_FLOOR_DIVISION_ASSIGN,
	TOKEN_DIVISION_ASSIGN,
	TOKEN_BITWISE_SHIFT_LEFT_ASSIGN,
	TOKEN_BITWISE_SHIFT_RIGHT_ASSIGN,
	TOKEN_BITWISE_SHIFT_LEFT,
	TOKEN_LESS_THAN_ASSIGN,
	TOKEN_BITWISE_SHIFT_RIGHT,
	TOKEN_GREATER_THAN_ASSIGN,
	TOKEN_FLOAT = 50,
	TOKEN_INTEGER,
	TOKEN_IDENTIFIER,
	TOKEN_FALSE,
	TOKEN_PRINT,
	TOKEN_DEL,
	TOKEN_DEF,
	TOKEN_DELATTR,
	TOKEN_NONE,
	TOKEN_TRUE,
	TOKEN_AND = 60,
	TOKEN_AS,
	TOKEN_ASSERT,
	TOKEN_AWAIT,
	TOKEN_ASYNC,
	TOKEN_ABS,
	TOKEN_AITER,
	TOKEN_ALL,
	TOKEN_ANY,
	TOKEN_ANEXT,
	TOKEN_ASCII = 70,
	TOKEN_BIN,
	TOKEN_BOOL, 
	TOKEN_BREAKPOINT,
	TOKEN_BREAK,
	TOKEN_BYTEARRAY,
	TOKEN_BYTES, 
	TOKEN_CALLABLE,
	TOKEN_CHR,
	TOKEN_CLASS,
	TOKEN_CLASSMETHOD = 80,
	TOKEN_COMPILE,
	TOKEN_COMPLEX,
	TOKEN_CONTINUE,
	TOKEN_DICT,
	TOKEN_DIR,
	TOKEN_DIVMOD,
	TOKEN_ELIF,
	TOKEN_ELSE,
	TOKEN_ENUMERATE,
	TOKEN_EVAL = 90,
	TOKEN_EXEC,
	TOKEN_EXCEPT,
	TOKEN_FILTER,
	TOKEN_FINALLY,
	TOKEN_FLOAT_FUNC,
	TOKEN_FORMAT,
	TOKEN_FOR,
	TOKEN_FROZENSET,
	TOKEN_FROM,
	TOKEN_GETATTR = 100,
	TOKEN_GLOBALS,
	TOKEN_GLOBAL,
	TOKEN_HASATTR,
	TOKEN_HASH,
	TOKEN_HELP,
	TOKEN_HEX,
	TOKEN_ID,
	TOKEN_IMPORT,
	TOKEN_IN,
	TOKEN_INPUT = 110,
	TOKEN_INT,
	TOKEN_ISINSTANCE,
	TOKEN_ISSUBCLASS,
	TOKEN_IS,
	TOKEN_IF,
	TOKEN_ITER,
	TOKEN_LAMBDA,
	TOKEN_LEN,
	TOKEN_LIST,
	TOKEN_LOCALS = 120,
	TOKEN_MAP,
	TOKEN_MAX,
	TOKEN_MEMORYVIEW,
	TOKEN_MIN,
	TOKEN_NEXT,
	TOKEN_NONLOCAL,
	TOKEN_NOT_KEYWORD,
	TOKEN_OBJECT,
	TOKEN_OR_KEYWORD,
	TOKEN_ORD = 130,
	TOKEN_OCT,
	TOKEN_OPEN,
	TOKEN_PASS,
	TOKEN_POW,
	TOKEN_PROPERTY,
	TOKEN_RAISE,
	TOKEN_RANGE,
	TOKEN_REPR,
	TOKEN_RETURN,
	TOKEN_REVERSED = 140,
	TOKEN_ROUND,
	TOKEN_SETATTR,
	TOKEN_SET,
	TOKEN_SLICE,
	TOKEN_SORTED,
	TOKEN_STATICMETHOD,
	TOKEN_STR,
	TOKEN_SUM,
	TOKEN_SUPER,
	TOKEN_TUPLE = 150,
	TOKEN_TRY,
	TOKEN_TYPE,
	TOKEN_VARS,
	TOKEN_WHILE,
	TOKEN_WITH,
	TOKEN_YIELD,
	TOKEN_ZIP,
	TOKEN_IMPORT_FUNC,
	TOKEN_TREE_ROOT,
	TOKEN_TAB
} TokenType;


typedef struct {
	TokenType type;
	const char *first_char;
	int length;
	int line_number;
} Token;


char *
get_source_from_file (FILE *);

char *
print_type (int);

static char
consume_char ();

Token
get_next_token (bool);

Token
spawn_token (TokenType);

Token
spawn_tab ();

static Token
spawn_error (const char *);

static Token
number ();

static Token
identifier_or_keyword ();

static Token
spawn_identifier ();

static Token
check_a_branch ();

static Token
check_b_branch ();

static Token
check_c_branch ();

static Token
check_d_branch ();

static Token
check_e_branch ();

static Token
check_f_branch ();

static Token
check_g_branch ();

static Token
check_h_branch ();

static Token
check_i_branch ();

static Token
check_l_branch ();

static Token
check_m_branch ();

static Token
check_n_branch ();

static Token
check_o_branch ();

static Token
check_p_branch ();

static Token
check_r_branch ();

static Token
check_s_branch ();

static Token
check_t_branch ();

static Token
check_v_branch ();

static Token
check_w_branch ();

static Token
check_y_branch ();

static Token
check_z_branch ();

static Token
check_underscore_branch ();

static bool
is_at_end ();

static bool
look_ahead (char);

static bool
is_digit (char);

static bool
is_letter (char);

static bool
bool_check_keyword (char *);

static char
peak_once ();

static char
peak_twice ();

void
initialize_lexer (const char *);

static void
skip_whitespace (bool);


#endif
