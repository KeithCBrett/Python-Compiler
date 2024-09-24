#include "lexer.h"
#include "common.h"
#include <stdio.h>


Lexer lex;


int main (int argc, char **argv) {
    FILE *fp = fopen("inputfile.py", "r");
    char *source = get_source_from_file(fp);
    initialize_lexer(source);
    for (;;) {
        Token token = get_next_token();
        if (token.type == TOKEN_NEWLINE) {
            lex.line_number++;
        }
        if (token.type != TOKEN_NEWLINE) {
            printf("Line Number: %4d \t", token.line_number);
            printf("Lexeme: '%.*s' \t", token.length, token.first_char);
            printf("Type: %s \n", print_type(token.type));
        }
        if (token.type == TOKEN_EOF) {
            break;
        }
    }
    free(source);
    fclose(fp);
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
        case 11:        return "TOKEN_LESS_THAN";
        case 12:        return "TOKEN_GREATER_THAN";
        case 13:        return "TOKEN_MODULUS";
        case 14:        return "TOKEN_EXPONENTIATION";
        case 15:        return "TOKEN_FLOOR_DIVISION";
        case 16:        return "TOKEN_ADD_ASSIGN";
        case 17:        return "TOKEN_MODULUS_ASSIGN";
        case 18:        return "TOKEN_LEFT_CURLY";
        case 19:        return "TOKEN_RIGHT_CURLY";
        case 20:        return "TOKEN_LEFT_SQUARE";
        case 21:        return "TOKEN_RIGHT_SQUARE";
        case 22:        return "TOKEN_PERIOD";
        case 23:        return "TOKEN_SEMI_COLON";
        case 24:        return "TOKEN_AMPERSAND";
        case 25:        return "TOKEN_AMPERSAND_ASSIGN";
        case 26:        return "TOKEN_COMMA";
        case 27:        return "TOKEN_MATRIX_MULT_ASSIGN";
        case 28:        return "TOKEN_MATRIX_MULT";
        case 29:        return "TOKEN_XOR_ASSIGN";
        case 30:        return "TOKEN_XOR";
        case 31:        return "TOKEN_COLON";
        case 32:        return "TOKEN_BNF";
        case 33:        return "TOKEN_NOT";
        case 34:        return "TOKEN_NOT_EQUAL";
        case 35:        return "TOKEN_OR";
        case 36:        return "TOKEN_OR_ASSIGN";
        case 37:        return "TOKEN_TILDA";
        case 38:        return "TOKEN_ARROW";
        case 39:        return "TOKEN_MINUS_ASSIGN";
        case 40:        return "TOKEN_EXPONENTIATION_ASSIGN";
        case 41:        return "TOKEN_MULTIPLICATION_ASSIGN";
        case 42:        return "TOKEN_FLOOR_DIVISION_ASSIGN";
        case 43:        return "TOKEN_DIVISION_ASSIGN";
        case 44:        return "TOKEN_BITWISE_SHIFT_LEFT_ASSIGN";
        case 45:        return "TOKEN_BITWISE_SHIFT_RIGHT_ASSIGN";
        case 46:        return "TOKEN_BITWISE_SHIFT_LEFT";
        case 47:        return "TOKEN_LESS_THAN_ASSIGN";
        case 48:        return "TOKEN_BITWISE_SHIFT_RIGHT";
        case 49:        return "TOKEN_GREATER_THAN_ASSIGN";
        case 50:        return "TOKEN_FLOAT";
        case 51:        return "TOKEN_INTEGER";
        default:        return "Not defined in function print_type()";
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


static char consume_char() {
    lex.current_char++;
    return lex.current_char[-1];
}


// Checks if input is the next char in stream, used for checking
// if lexeme is two character (==) or one (=)
static bool look_ahead(const char check) {
    if (is_at_end() == true) {
        return false;
    }
    if (lex.current_char[0] != check) {
        return false;
    }
    lex.current_char++;
    return true;
}


// Like look ahead but we dont advance our character stream
static bool look_ahead_dont_advance(const char check) {
    if (is_at_end() == true) {
        return false;
    }
    if (lex.current_char[0] != check) {
        return false;
    }
    return true;
}


static void skip_whitespace() {
    for (;;) {
        char c = peak_once();
        switch (c) {
            case ' ':
            case '\t':
            case '\r':
                lex.current_char++;
                break;
            case '\n':
                lex.current_char++;
                lex.line_number++;
                break;
            default:
                return;
        }
    }
}


static bool is_digit(char c) {
    if ((c >= '0') && (c <= '9')) {
        return true;
    } else {
        return false;
    }
}


static Token number() {
    while (is_digit(peak_once()) == true) {
        consume_char();
    }
    if (peak_once() == '.') {
        consume_char();
        while (is_digit(peak_once()) == true) {
            consume_char();
        }
        return spawn_token(TOKEN_FLOAT);
    }
    return spawn_token(TOKEN_INTEGER);
}


static Token get_next_token() {
    skip_whitespace();
    lex.start_char = lex.current_char;

    if (is_at_end() == true) {
        return spawn_token(TOKEN_EOF);
    }

    char c = consume_char();


    if (is_digit(c) == true) {
        return number();
    }

    /*
    if (is_letter(c) == true) {
        return identifier_or_keyword();
    }
    */

    switch (c) {
        // Single character tokens
        case '~':       return spawn_token(TOKEN_TILDA);
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
        case ':':       
            return spawn_token(
                look_ahead('=') == true ? TOKEN_BNF : TOKEN_COLON);
        case '!':       
            return spawn_token(
                look_ahead('=') == true ? TOKEN_NOT_EQUAL : TOKEN_NOT);
        case '|':       
            return spawn_token(
                look_ahead('=') == true ? TOKEN_OR_ASSIGN : TOKEN_OR);
        case '-':       
            if (look_ahead('>') == true) {
                return spawn_token(TOKEN_ARROW);
            } else if (look_ahead('=') == true) {
                return spawn_token(TOKEN_MINUS_ASSIGN);
            } else {
                return spawn_token(TOKEN_MINUS);
            }
        // Double lookahead tokens (one - three characters)
        case '*':
            if ((peak_once() == '*') && (peak_twice() == '=')) {
                // peak() doesnt increment char stream, so, if we have lexed our token,
                // we need to do it manually. This way we can start over and check for other
                // tokens that contain said character.
                lex.current_char += 2;
                return spawn_token(TOKEN_EXPONENTIATION_ASSIGN);
            } else if (peak_once() == '*') {
                lex.current_char += 1;
                return spawn_token(TOKEN_EXPONENTIATION);
            } else if (peak_once() == '=') {
                lex.current_char += 1;
                return spawn_token(TOKEN_MULTIPLICATION_ASSIGN);
            } else {
                return spawn_token(TOKEN_MULTIPLICATION);
            }
        case '/':
            if ((peak_once() == '/') && (peak_twice() == '=')) {
                lex.current_char += 2;
                return spawn_token(TOKEN_FLOOR_DIVISION_ASSIGN);
            } else if (peak_once() == '/') {
                lex.current_char += 1;
                return spawn_token(TOKEN_FLOOR_DIVISION);
            } else if (peak_once() == '=') {
                lex.current_char += 1;
                return spawn_token(TOKEN_DIVISION_ASSIGN);
            } else {
                return spawn_token(TOKEN_DIVISION);
            }
        case '<':
            if ((peak_once() == '<') && (peak_twice() == '=')) {
                lex.current_char += 2;
                return spawn_token(TOKEN_BITWISE_SHIFT_LEFT_ASSIGN);
            } else if (peak_once() == '<') {
                lex.current_char += 1;
                return spawn_token(TOKEN_BITWISE_SHIFT_LEFT);
            } else if (peak_once() == '=') {
                lex.current_char += 1;
                return spawn_token(TOKEN_LESS_THAN_ASSIGN);
            } else {
                return spawn_token(TOKEN_LESS_THAN);
            }
        case '>':
            if ((peak_once() == '>') && (peak_twice() == '=')) {
                lex.current_char += 2;
                return spawn_token(TOKEN_BITWISE_SHIFT_RIGHT_ASSIGN);
            } else if (peak_once() == '<') {
                lex.current_char += 1;
                return spawn_token(TOKEN_BITWISE_SHIFT_RIGHT);
            } else if (peak_once() == '=') {
                lex.current_char += 1;
                return spawn_token(TOKEN_GREATER_THAN_ASSIGN);
            } else {
                return spawn_token(TOKEN_GREATER_THAN);
            }
        default: return spawn_token(TOKEN_ERROR);
    }
    return spawn_error("Unknown token encountered");
}


static char peak_once() {
    return lex.current_char[0];
}


static char peak_twice() {
    return lex.current_char[1];
}

// Stores program as a string to be lexed
static char *get_source_from_file(FILE *file) {
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
