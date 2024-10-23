#ifndef parser_h
#define parser_h
#include <stdio.h>
#include "../Lexer/lexer.h"


// This parser is based on Vaughan R. Pratt's algorithm. For more information,
// I would recommend the links below:
// Original paper:
    // https://dl.acm.org/doi/pdf/10.1145/512927.512931
// Implementation in C can be seen in Robert Nystrom's book, Crafting Interpreters:
    // https://craftinginterpreters.com/compiling-expressions.html
// Nystrom also made one in Java:
    // https://journal.stuffwithstuff.com/2011/03/19/pratt-parsers-expression-parsing-made-easy
// Douglas Crockford has implemented a couple:
    // https://www.jslint.com/
    // https://crockford.com/javascript/tdop/tdop.html


// Semantic code our functions that build our AST. There our two types of
// semantic code (since symbols like (-) can be negation (as in -x) or
// subtraction (as in x - y)).
// These two types are denoted:
//      nud(): This is for prefix semantics, i.e. -x
//      led(): This is for infix semantics, i.e. x - y
typedef void (*SemanticCode)();


// Requirement for TDOP algorithm, now we know that (+ < *) since multiplication
// has higher precendence.
typedef enum Precedence{
    Outcomes = 0,
    Booleans,
    Graphs,
    AddSub,
    MultDiv
} Precedence;


typedef struct ParserToken{
    char *lexeme;
    Precedence precendence;
    TokenType type;
    SemanticCode nud; // prefix
    SemanticCode led; // infix
} ParserToken;


/*
 * spawn_parse_token()
 * Input: Takes in a token produced from our lexer module (get_next_token())
 * Output: New struct with more information needed for TDOP parser. Namely
 * this function accociates a precedence level, code to translate if token
 * is used as a prefix operator (nud()), or infix operator (led()).
 */
ParserToken spawn_parse_token(Token);
/*
 * print_parse_token()
 * Input: Parse token create by spawn_parse_token().
 * Output: Prints debugging information about parse token.
 */
void print_parse_token(ParserToken);
/*
 * associate_precedence()
 * Input: TokenType
 * Output: Precedence of token type
 */
Precedence associate_precedence(TokenType);

#endif
