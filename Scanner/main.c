/* Contains our token declarations */
#include "tokens.h"

#include <stdio.h>

/* https://ftp.gnu.org/old-gnu/Manuals/flex-2.5.4/html_node/flex_15.html */
extern FILE *yyin;      /* flex uses yyin as the default file it reads from */

/* https://www.ibm.com/docs/en/zos/2.4.0?topic=lex-using-yylex */
extern int yylex();     /* yylex() is our lexical analyzer (flex). As output returns an integer
                           corresponding to the token encountered in the file */

/* INVALUABLE RESOURCE FOR FLEX */
/* http://westes.github.io/flex/manual/Matching.html#Matching */
extern char *yytext;    /* points to character of the current token */ 

const char* verboseToken(int i) {
    const char* output;

    switch(i) {
        case 0:
            output = "EOF";
            break;
        case 1:
            output = "ADD";
            break;
        case 2:
            output = "MINUS";
            break;
        case 3:
            output = "MULTIPLY";
            break;
        case 4:
            output = "DIVIDE";
            break;
        case 5:
            output = "IDENTIFIER";
            break;
        case 6:
            output = "ASSIGN";
            break;
        case 7:
            output = "INTEGER";
            break;
        case 8:
            output = "FLOAT";
            break;
        case 9:
            output = "ERROR";
            break;
        default:
            output = "NOT DEFINED IN VERBOSE TOKEN";
            break;
    }
    return output;
}
int main() {
    yyin = fopen("inputfile.py", "r");                  /* opens as read only inputfile.py for scanning */

    if(!yyin) {                                         /* Error handling for lack of input file */
        printf("Could not open inputfile.py\n");
        return 1;
    }

    while(1) {
        token_type t = yylex();                         /* while inputfile found and EOF not reached,
                                                           t will hold the token identifier. */
        if(t==TOKEN_EOF) {
            break;
        }
        printf("Text: %s \t Token: %s\n", yytext, verboseToken(t));   /* Print outputs for debugging purposes */
    }
}
