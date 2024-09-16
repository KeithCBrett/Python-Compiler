#ifndef lexer_h
#define lexer_h


#include <stdio.h>


typedef struct {
    const char *start_char;
    const char *current_char;
    size_t line_number;
} Lexer;


typedef enum {
    TOKEN_ADD, TOKEN_MINUS, TOKEN_MULTIPLICATION,
    TOKEN_DIVISION, TOKEN_EOF, TOKEN_ERROR,
    TOKEN_NEWLINE, TOKEN_EQUALS, TOKEN_LEFT_PAREN,
    TOKEN_RIGHT_PAREN
} TokenType;


typedef struct {
    TokenType type;
    const char *first_char;
    int length;
    int line_number;
} Token;


static char *get_source_from_file (FILE *);
static char consume_char ();
static Token get_next_token ();
static Token spawn_token (TokenType);
static Token spawn_error (const char *);
static void initialize_lexer (const char *);
static int is_at_end ();


#endif
