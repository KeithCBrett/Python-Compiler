#include "lexer.h"
#include "common.h"


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
            printf("Lexeme: %12.*s \t", token.length, token.first_char);
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
        case 52:        return "TOKEN_IDENTIFIER";
        case 53:        return "TOKEN_FALSE";
        case 54:        return "TOKEN_PRINT";
        case 55:        return "TOKEN_DEL";
        case 56:        return "TOKEN_DEF";
        case 57:        return "TOKEN_DELATTR";
        case 58:        return "TOKEN_NONE";
        case 59:        return "TOKEN_TRUE";
        case 60:        return "TOKEN_AND";
        case 61:        return "TOKEN_AS";
        case 62:        return "TOKEN_ASSERT";
        case 63:        return "TOKEN_AWAIT";
        case 64:        return "TOKEN_ASYNC";
        case 65:        return "TOKEN_ABS";
        case 66:        return "TOKEN_AITER";
        case 67:        return "TOKEN_ALL";
        case 68:        return "TOKEN_ANY";
        case 69:        return "TOKEN_ANEXT";
        case 70:        return "TOKEN_ASCII";
        case 71:        return "TOKEN_BIN";
        case 72:        return "TOKEN_BOOL";
        case 73:        return "TOKEN_BREAKPOINT";
        case 74:        return "TOKEN_BREAK";
        case 75:        return "TOKEN_BYTEARRAY";
        case 76:        return "TOKEN_BYTES";
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


// Returns the current character and increments the character stream.
// We use this function to traverse our string (source code).
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


static bool is_letter(char c) {
    if ((c >= 'a' && c <= 'z') ||
        (c >= 'A' && c <= 'Z') ||
        (c == '_')) {
        return true;
    } else {
        return false;
    }
}


static Token make_identifier() {
    // This switch is for single character identifiers, i.e. n = 6
    switch (lex.current_char[-1]) {
        case '\n': case '\0': case ' ':
            lex.current_char--;
            return spawn_token(TOKEN_IDENTIFIER);
        default:
            break;
    }
    while ((is_letter(lex.current_char[0])) == true ||
    (is_digit(lex.current_char[0]) == true)) {
        lex.current_char++;
    }
    return spawn_token(TOKEN_IDENTIFIER);
}


static bool bool_check_keyword(char *string) {
    size_t string_length = strlen(string);
    size_t count = 0;
    while (count < string_length) {
        if (lex.current_char[0] != string[count]) {
            lex.current_char -= count;
            return false;
        }
        consume_char();
        count++;
    }
    if ((is_digit(peak_once()) == true) || 
    (is_letter(peak_once()) == true)) {
        return false;
    }
    return true;
}


// This is the 'a' branch for our keyword/default function trie
// -- default function as in print(), abs(), etc.
static Token check_a_branch() {
    char c = consume_char();
    switch (c) {
        case 'b':
            if (bool_check_keyword("s") == true) {
                // Check for TOKEN_ABS
                return spawn_token(TOKEN_ABS);
            } else {
                return make_identifier();
            }
        case 'i':
            if (bool_check_keyword("ter") == true) {
                // Check for TOKEN_AITER
                return spawn_token(TOKEN_AITER);
            } else {
                return make_identifier();
            }
        case 'l':
            if (bool_check_keyword("l") == true) {
                // Check for TOKEN_ALL
                return spawn_token(TOKEN_ALL);
            } else {
                return make_identifier();
            }
        case 'n':
            if (bool_check_keyword("d") == true) {
                // Check for TOKEN_AND
                return spawn_token(TOKEN_AND);
            } else if (bool_check_keyword("y") == true) {
                // Check for TOKEN_ANY
                return spawn_token(TOKEN_ANY);
            } else if (bool_check_keyword("ext") == true) {
                // Check for TOKEN_ANEXT
                return spawn_token(TOKEN_ANEXT);
            } else {
                return make_identifier();
            }
        case 's':
            if (bool_check_keyword("") == true) {
                // Check for TOKEN_AS
                return spawn_token(TOKEN_AS);
            } else if (bool_check_keyword("sert") == true) {
                // Check for TOKEN_ASSERT
                return spawn_token(TOKEN_ASSERT);
            } else if (bool_check_keyword("ync") == true) {
                // Check for TOKEN_ASYNC
                return spawn_token(TOKEN_ASYNC);
            } else if (bool_check_keyword("cii") == true) {
                // Check for TOKEN_ASCII
                return spawn_token(TOKEN_ASCII);
            } else {
                return make_identifier();
            }
        case 'w':
            if (bool_check_keyword("ait") == true) {
                // Check for TOKEN_AWAIT
                return spawn_token(TOKEN_AWAIT);
            } else {
                return make_identifier();
            }
        default:
            return make_identifier();
    }
}


// This is the 'b' branch for our keyword/default function trie
static Token check_b_branch() {
    char c = consume_char();
    switch (c) {
        case 'i':
            if (bool_check_keyword("n") == true) {
                // Check for TOKEN_BIN
                return spawn_token(TOKEN_BIN);
            }
        case 'o':
            if (bool_check_keyword("ol") == true) {
                // Check for TOKEN_BOOL
                return spawn_token(TOKEN_BOOL);
            }
        case 'r':
            if (bool_check_keyword("eakpoint") == true) {
                // Check for TOKEN_BREAKPOINT
                return spawn_token(TOKEN_BREAKPOINT);
            } 
            if (bool_check_keyword("eak") == true) {
                // Check for TOKEN_BREAK
                return spawn_token(TOKEN_BREAK);
            }
        case 'y':
            if (bool_check_keyword("tearray") == true) {
                // Check for TOKEN_BYTEARRAY
                return spawn_token(TOKEN_BYTEARRAY);
            } 
            if (bool_check_keyword("tes") == true) {
                // Check for TOKEN_BYTES
                return spawn_token(TOKEN_BYTES);
            }
        default:
            return make_identifier();
    }
}


// Checks if string of characters is a keyword and returns the corresponding token.
// If not keyword, capture lexeme and return identifier token.
static Token identifier_or_keyword() {
    switch (lex.current_char[-1]) {
        case 'F':
            if (bool_check_keyword("alse") == true) {
                // Check for TOKEN_FALSE
                return spawn_token(TOKEN_FALSE);
            }
        case 'N':
            if (bool_check_keyword("one") == true) {
                // Check for TOKEN_NONE
                return spawn_token(TOKEN_NONE);
            }
        case 'T':
            if (bool_check_keyword("rue") == true) {
                // Check for TOKEN_TRUE
                return spawn_token(TOKEN_TRUE);
            }
        case 'a':
            return check_a_branch();
        case 'b':
            return check_b_branch();
        default:
            return make_identifier();
    }
}


// Main logic, call this repeatedly in a loop until EOF token reached
// in order to lex a program. (Be sure to initialize Lexer with source
// code first with initialize_lexer() and get_source_from_file()).
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

    if (is_letter(c) == true) {
        return identifier_or_keyword();
    }

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
