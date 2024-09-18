#include "lexer.h"
#include "common.h"


Lexer lex;


int main (int argc, char **argv) {
    FILE *fp = fopen("inputfile.py", "r");
    const char *source = get_source_from_file(fp);
    initialize_lexer(source);
    for (;;) {
        Token token = get_next_token();
        if (token.type == TOKEN_NEWLINE) {
            lex.line_number++;
        }
        if (token.type != TOKEN_NEWLINE) {
            printf("Line Number: %d \t", token.line_number);
            printf("Lexeme: '%.*s' \t", token.length, token.first_char);
            printf("Type: %s \n", print_type(token.type));
        }
        if (token.type == TOKEN_EOF) {
            break;
        }
    }
    return 0;
}


static char *print_type (int type) {
    switch (type) {
        case 0:         return "TOKEN_ADD";
        case 1:         return "TOKEN_MINUS";
        case 2:         return "TOKEN_MULTIPLICATION";
        case 3:         return "TOKEN_DIVISION";
        case 4:         return "TOKEN_EOF";
        case 5:         return "TOKEN_ERROR";
        case 6:         return "TOKEN_NEWLINE";
        case 7:         return "TOKEN_LEFT_PAREN";
        case 8:         return "TOKEN_RIGHT_PAREN";
        case 9:         return "TOKEN_EQUALS";
        case 10:        return "TOKEN_EQUAL_EQUAL";
        default:        return "Not defined in function print token";
    }
}


static void initialize_lexer(const char *source){
    lex.start_char = source;
    lex.current_char = source;
    lex.line_number = 1;
}


static bool is_at_end() {
    if (*lex.current_char == '\0') {
        return true;
    } else {
        return false;
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


// Checks if input is the next char in stream, used for checking
// if lexeme is two character (==) or one (=)
static bool look_ahead (char check) {
    if (is_at_end() == true) {
        return false;
    }
    if (lex.current_char[0] != check) {
        return false;
    }
    lex.current_char++;
    return true;
}


static Token get_next_token() {
    lex.start_char = lex.current_char;

    if (is_at_end() == true) {
        return spawn_token(TOKEN_EOF);
    }

    char c = consume_char();

    switch (c) {
        // Single character tokens
        case '-':       return spawn_token(TOKEN_MINUS);
        case '(':       return spawn_token(TOKEN_LEFT_PAREN);
        case ')':       return spawn_token(TOKEN_RIGHT_PAREN);
        case '{':       return spawn_token(TOKEN_LEFT_CURLY);
        case '}':       return spawn_token(TOKEN_RIGHT_CURLY);
        case '[':       return spawn_token(TOKEN_LEFT_SQUARE);
        case ']':       return spawn_token(TOKEN_RIGHT_SQUARE);
        case ',':       return spawn_token(TOKEN_COMMA);
        case '.':       return spawn_token(TOKEN_PERIOD);
        case ';':       return spawn_token(TOKEN_SEMI_COLON);
        case '\n':      return spawn_token(TOKEN_NEWLINE);
        // Single lookahead tokens (one or two characters)
        case '=':       
            return spawn_token(
                look_ahead('=') == true ? TOKEN_EQUAL_EQUAL : TOKEN_EQUALS);
        case '+':       
            return spawn_token(
                look_ahead('=') == true ? TOKEN_ADD_ASSIGN : TOKEN_ADD);
        case '%':       
            return spawn_token(
                look_ahead('=') == true ? TOKEN_MODULUS_ASSIGN : TOKEN_MODULUS);
        case '&':       
            return spawn_token(
                look_ahead('=') == true ? TOKEN_AMPERSAND_ASSIGN : TOKEN_AMPERSAND);
        case '@':       
            return spawn_token(
                look_ahead('=') == true ? TOKEN_MATRIX_MULT_ASSIGN : TOKEN_MATRIX_MULT);
        case '^':       
            return spawn_token(
                look_ahead('=') == true ? TOKEN_XOR_ASSIGN : TOKEN_XOR);
        // Double lookahead tokens (one - three characters)
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
