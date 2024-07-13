/* Instructions in this area are executed at the beginning of scanner generation */
%{
#include "tokens.h"     /* This header file contains our token declarations */
%}
/* Declaration of character set. */
NUMBER  [0-9]
LETTER  [a-zA-Z_]
%%
" "     /* We will skip spaces but track newlines and tabs since Python is whitespace sensitive */
({LETTER}+|{LETTER}+{NUMBER})               { return TOKEN_IDENTIFIER; }
{NUMBER}+               { return TOKEN_INTEGER; }
({NUMBER}+"."{NUMBER}+) { return TOKEN_FLOAT; }
    /* Operators */
\+                      { return TOKEN_ADD; }
\-                      { return TOKEN_MINUS; }
\*                      { return TOKEN_MULTIPLY; }
\/                      { return TOKEN_DIVIDE; }
\=                      { return TOKEN_ASSIGN; }
    /* Operators */
.                       { return TOKEN_ERROR; }
%%
int yywrap()            { return 1; } /* Requirement of flex, function that indicates end of file so scanner stops scanning */
