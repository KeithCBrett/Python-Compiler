#include "Lexer/lexer.h"
#include "Parser/parser.h"
#include "Lexer/common.h"


int main(int argc, char **argv){
    FILE *fp = fopen("inputfile.py", "r");
    char *source = get_source_from_file(fp);
    fclose(fp);

    initialize_lexer(source);
    int line_number = 1;

    for (;;) {
        Token token = get_next_token();
        /***************************
         * Lexer printing aperatus *
         ***************************/
        /*
        if (token.type == TOKEN_NEWLINE) {
            line_number++;
        }

        if (token.type != TOKEN_NEWLINE) {
            printf("Line Number: %4d \t", token.line_number);
            printf("Lexeme: %12.*s \t", token.length, token.first_char);
            printf("Type: %s \n", print_type(token.type));
        }
        */

        /****************************
         * Parser printing aperatus *
         ****************************/
        ParserToken parse_token = spawn_parse_token(token);
        print_parse_token(parse_token);
        free(parse_token.lexeme);
        if (token.type == TOKEN_EOF) {
            break;
        }
    }
    free(source);
}
