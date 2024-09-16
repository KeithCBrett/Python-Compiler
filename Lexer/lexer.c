#include "lexer.h"
#include "common.h"
#include <stdio.h>


Lexer lex;


int main (int argc, char **argv) {
    FILE *fp = fopen("inputfile.py", "r");
    const char *source = get_source_from_file(fp);
    initialize_lexer(source);
    for (;;) {
        Token token = get_next_token();
        printf("Line Number: %d \t", token.line_number);
        if (token.type != TOKEN_NEWLINE) {
            printf("Lexeme: '%.*s' \t", token.length, token.first_char);
        }
        printf("Type: %d \n", token.type);
        if (token.type == TOKEN_EOF) {
            break;
        }
    }
    return 0;
}


static void initialize_lexer(const char *source){
    lex.start_char = source;
    lex.current_char = source;
    lex.line_number = 1;
}


static int is_at_end() {
    if (*lex.current_char == '\0') {
        return 1;
    } else {
        return 0;
    }
}


static Token spawn_token(TokenType type) {
    Token return_token;
    return_token.type = type;
    return_token.line_number = lex.line_number;
    return_token.length = (size_t) (lex.current_char - lex.start_char);
    return_token.first_char = lex.start_char;
    return return_token;
}


static Token spawn_error(const char *error_message) {
    Token return_token;
    return_token.type = TOKEN_ERROR;
    return_token.line_number = lex.line_number;
    return_token.length = (size_t) strlen(error_message);
    return_token.first_char = error_message;
    return return_token;
}


static char consume_char () {
    lex.current_char++;
    return lex.current_char[-1];
}


static Token get_next_token() {
    lex.start_char = lex.current_char;

    if (is_at_end() == 1) {
        return spawn_token(TOKEN_EOF);
    }

    char c = consume_char();

    switch (c) {
        case '+':       return spawn_token(TOKEN_ADD);
        case '-':       return spawn_token(TOKEN_MINUS);
        case '*':       return spawn_token(TOKEN_MULTIPLICATION);
        case '/':       return spawn_token(TOKEN_DIVISION);
        case '(':       return spawn_token(TOKEN_LEFT_PAREN);
        case ')':       return spawn_token(TOKEN_RIGHT_PAREN);
        case '=':       return spawn_token(TOKEN_EQUALS);
        case '\n':      return spawn_token(TOKEN_NEWLINE);
        default:        return spawn_token(TOKEN_ERROR);
    }
    return spawn_error("Unknown token encountered");
}


// Stores program as a string to be lexed
static char *get_source_from_file (FILE *file) {
    // Handle file error
    if (file == NULL) {
        fprintf(stderr, "Unable to open input file\n");
        exit(127);
    }


    // Get buffer size
    fseek(file, 0, SEEK_END);
    size_t source_length = ftell(file) - 1;
    rewind(file);


    // Store file in string
    char *source = malloc(source_length + 1);
    if (source == NULL) {
        fprintf(stderr, "Could not allocate enough memory\n");
        exit(127);
    }
    size_t buffer_size = fread(source, sizeof(char), source_length, file);
    if (buffer_size < source_length) {
        fprintf(stderr, "Could not read file to completion, fread failed\n");
        exit(127);
    }
    source[buffer_size] = '\0'; // Null terminate string
    return source;
}
