/* Instructions in this area are executed at the beginning of scanner generation */
%{
#include "tokens.h"     /* This header file contains our token declarations */
%}
/* Declaration of character set. */
NUMBER  [0-9]
LETTER  [a-zA-Z]
%%
" "     /* We will skip spaces but track newlines and tabs since Python is whitespace sensitive */
    /* Operators */
\+              { return TOKEN_ADD; }
\-              { return TOKEN_MINUS; }
\*              { return TOKEN_MULTIPLY; }
\/              { return TOKEN_DIVIDE; }
    /* Operators */
%%
int yywrap()    { return 1; } /* Requirement of flex, function that indicates end of file so scanner stops scanning */
