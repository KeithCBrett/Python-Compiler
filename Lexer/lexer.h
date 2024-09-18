#ifndef lexer_h
#define lexer_h


#include "common.h"


typedef struct {
    const char *start_char;
    const char *current_char;
    size_t line_number;
} Lexer;


typedef enum {
    TOKEN_ADD, TOKEN_MINUS, TOKEN_MULTIPLICATION,
    TOKEN_DIVISION, TOKEN_EOF, TOKEN_ERROR,
    TOKEN_NEWLINE, TOKEN_LEFT_PAREN, TOKEN_RIGHT_PAREN,
    TOKEN_EQUALS, TOKEN_EQUAL_EQUAL, TOKEN_LESS_THAN,
    TOKEN_GREATER_THAN, TOKEN_MODULUS, TOKEN_EXPONENTIATION,
    TOKEN_FLOOR_DIVISION, TOKEN_ADD_ASSIGN, TOKEN_MODULUS_ASSIGN,
    TOKEN_LEFT_CURLY, TOKEN_RIGHT_CURLY, TOKEN_LEFT_SQUARE,
    TOKEN_RIGHT_SQUARE, TOKEN_PERIOD, TOKEN_SEMI_COLON,
    TOKEN_AMPERSAND, TOKEN_AMPERSAND_ASSIGN, TOKEN_COMMA,
    TOKEN_MATRIX_MULT_ASSIGN, TOKEN_MATRIX_MULT, TOKEN_XOR_ASSIGN,
    TOKEN_XOR
} TokenType;


typedef struct {
    TokenType type;
    const char *first_char;
    int length;
    int line_number;
} Token;


static char *get_source_from_file (FILE *);
static char *print_type (int);
static char consume_char ();
static Token get_next_token ();
static Token spawn_token (TokenType);
static Token spawn_error (const char *);
static void initialize_lexer (const char *);
static bool is_at_end ();
static bool look_ahead (char);


#endif
