#include "lexer.h"
#include "../Parser/parser.h"
#include "common.h"


Lexer lex;
// Stores program as a string to be lexed
char *get_source_from_file (FILE *file) {
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


void initialize_lexer(const char *source){
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


Token spawn_token(TokenType type) {
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
	if (is_at_end()) {
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
	if (is_at_end()) {
		return false;
	}
	if (lex.current_char[0] != check) {
		return false;
	}
	return true;
}


// Breaks out of loop when non whitespace is encountered
static void skip_whitespace(bool was_newline) {
	for (;;) {
		char c = peak_once();
		switch (c) {
			case '\t':
			case '\r':
				lex.current_char++;
				break;
			case ' ':
				// If last token was newline, this space is
				// acting as indentation, so we dont skip.
				if (was_newline == true)
				{
					// get_next_token () will handle this one.
					return;
				}
				// Otherwise, skip as usual.
				else
				{
					lex.current_char++;
					break;
				}
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
	while (is_digit(peak_once())) {
		consume_char();
	}
	if (peak_once() == '.') {
		consume_char();
		while (is_digit(peak_once())) {
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


static Token spawn_identifier() {
	// This switch is for single character identifiers, i.e. n = 6
	switch (lex.current_char[-1]) {
		case '\n': case '\0': case ' ':
			lex.current_char--;
			return spawn_token(TOKEN_IDENTIFIER);
		default:
			break;
	}
	while ((is_letter(lex.current_char[0])) ||
	(is_digit(lex.current_char[0]))) {
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
	if (is_digit(peak_once())) {
		lex.current_char -= count;
		return false;
	}
	if (is_letter(peak_once())) {
		lex.current_char -= count;
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
			if (bool_check_keyword("s")) {
				// Check for TOKEN_ABS
				return spawn_token(TOKEN_ABS);
			} else {
				return spawn_identifier();
			}
		case 'i':
			if (bool_check_keyword("ter")) {
				// Check for TOKEN_AITER
				return spawn_token(TOKEN_AITER);
			} else {
				return spawn_identifier();
			}
		case 'l':
			if (bool_check_keyword("l")) {
				// Check for TOKEN_ALL
				return spawn_token(TOKEN_ALL);
			} else {
				return spawn_identifier();
			}
		case 'n':
			if (bool_check_keyword("d")) {
				// Check for TOKEN_AND
				return spawn_token(TOKEN_AND);
			} else if (bool_check_keyword("y")) {
				// Check for TOKEN_ANY
				return spawn_token(TOKEN_ANY);
			} else if (bool_check_keyword("ext")) {
				// Check for TOKEN_ANEXT
				return spawn_token(TOKEN_ANEXT);
			} else {
				return spawn_identifier();
			}
		case 's':
			if (bool_check_keyword("")) {
				// Check for TOKEN_AS
				return spawn_token(TOKEN_AS);
			} else if (bool_check_keyword("sert")) {
				// Check for TOKEN_ASSERT
				return spawn_token(TOKEN_ASSERT);
			} else if (bool_check_keyword("ync")) {
				// Check for TOKEN_ASYNC
				return spawn_token(TOKEN_ASYNC);
			} else if (bool_check_keyword("cii")) {
				// Check for TOKEN_ASCII
				return spawn_token(TOKEN_ASCII);
			} else {
				return spawn_identifier();
			}
		case 'w':
			if (bool_check_keyword("ait")) {
				// Check for TOKEN_AWAIT
				return spawn_token(TOKEN_AWAIT);
			} else {
				return spawn_identifier();
			}
		default:
			lex.current_char--;
			return spawn_identifier();
	}
}


// This is the 'b' branch for our keyword/default function trie
static Token check_b_branch() {
	char c = consume_char();
	switch (c) {
		case 'i':
			if (bool_check_keyword("n")) {
				// Check for TOKEN_BIN
				return spawn_token(TOKEN_BIN);
			} else {
				return spawn_identifier();
			}
		case 'o':
			if (bool_check_keyword("ol")) {
				// Check for TOKEN_BOOL
				return spawn_token(TOKEN_BOOL);
			} else {
				return spawn_identifier();
			}
		case 'r':
			if (bool_check_keyword("eakpoint")) {
				// Check for TOKEN_BREAKPOINT
				return spawn_token(TOKEN_BREAKPOINT);
			} else if (bool_check_keyword("eak")) {
				// Check for TOKEN_BREAK
				return spawn_token(TOKEN_BREAK);
			} else {
				return spawn_identifier();
			}
		case 'y':
			if (bool_check_keyword("tearray")) {
				// Check for TOKEN_BYTEARRAY
				return spawn_token(TOKEN_BYTEARRAY);
			} else if (bool_check_keyword("tes")) {
				// Check for TOKEN_BYTES
				return spawn_token(TOKEN_BYTES);
			} else {
				return spawn_identifier();
			}
		default:
			lex.current_char--;
			return spawn_identifier();
	}
}


static Token check_c_branch() {
	char c = consume_char();
	switch (c) {
		case 'a':
			if (bool_check_keyword("llable")) {
				return spawn_token(TOKEN_CALLABLE);
			} else {
				return spawn_identifier();
			}
		case 'h':
			if (bool_check_keyword("r")) {
				return spawn_token(TOKEN_CHR);
			} else {
				return spawn_identifier();
			}
		case 'l':
			if (bool_check_keyword("assmethod")) {
				return spawn_token(TOKEN_CLASSMETHOD);
			} else if (bool_check_keyword("ass")) {
				return spawn_token(TOKEN_CLASS);
			} else {
				return spawn_identifier();
			}
		case 'o':
			if (bool_check_keyword("mpile")) {
				return spawn_token(TOKEN_COMPILE);
			} else if (bool_check_keyword("mplex")) {
				return spawn_token(TOKEN_COMPLEX);
			} else if (bool_check_keyword("ntinue")){
				return spawn_token(TOKEN_CONTINUE);
			} else {
				return spawn_identifier();
			}
		default:
			lex.current_char--;
			return spawn_identifier();
	}
}


static Token check_d_branch() {
	char c = consume_char();
	switch (c) {
		case 'e':
			if (bool_check_keyword("lattr")) {
				return spawn_token(TOKEN_DELATTR);
			} else if (bool_check_keyword("l")) {
				return spawn_token(TOKEN_DEL);
			} else if (bool_check_keyword("f")) {
				return spawn_token(TOKEN_DEF);
			} else {
				return spawn_identifier();
			}
		case 'i':
			if (bool_check_keyword("ct")) {
				return spawn_token(TOKEN_DICT);
			} else if (bool_check_keyword("r")) {
				return spawn_token(TOKEN_DIR);
			} else if (bool_check_keyword("vmod")) {
				return spawn_token(TOKEN_DIVMOD);
			} else {
				return spawn_identifier();
			}
		default:
			lex.current_char--;
			return spawn_identifier();
	}
}


static Token check_e_branch() {
	char c = consume_char();
	switch (c) {
		case 'l':
			if (bool_check_keyword("if")) {
				return spawn_token(TOKEN_ELIF);
			} else if (bool_check_keyword("se")) {
				return spawn_token(TOKEN_ELSE);
			} else {
				return spawn_identifier();
			}
		case 'n':
			if (bool_check_keyword("umerate")) {
				return spawn_token(TOKEN_ENUMERATE);
			} else {
				return spawn_identifier();
			}
		case 'v':
			if (bool_check_keyword("al")) {
				return spawn_token(TOKEN_EVAL);
			} else {
				return spawn_identifier();
			}
		case 'x':
			if (bool_check_keyword("ec")) {
				return spawn_token(TOKEN_EXEC);
			} else if (bool_check_keyword("cept")) {
				return spawn_token(TOKEN_EXCEPT);
			} else {
				return spawn_identifier();
			}
		default:
			lex.current_char--;
			return spawn_identifier();
	}
}


static Token check_f_branch() {
	char c = consume_char();
	switch (c) {
		case 'i':
			if (bool_check_keyword("lter")) {
				return spawn_token(TOKEN_FILTER);
			} else if (bool_check_keyword("nally")) {
				return spawn_token(TOKEN_FINALLY);
			} else {
				return spawn_identifier();
			}
		case 'l':
			if (bool_check_keyword("oat")) {
				return spawn_token(TOKEN_FLOAT_FUNC);
			} else {
				return spawn_identifier();
			}
		case 'o':
			if (bool_check_keyword("rmat")) {
				return spawn_token(TOKEN_FORMAT);
			} else if (bool_check_keyword("r")) {
				return spawn_token(TOKEN_FOR);
			} else {
				return spawn_identifier();
			}
		case 'r':
			if (bool_check_keyword("ozenset")) {
				return spawn_token(TOKEN_FROZENSET);
			} else if (bool_check_keyword("om")) {
				return spawn_token(TOKEN_FROM);
			} else {
				return spawn_identifier();
			}
		default:
			lex.current_char--;
			return spawn_identifier();
	}
}


static Token check_g_branch() {
	char c = consume_char();
	switch (c) {
		case 'e':
			if (bool_check_keyword("tattr")) {
				return spawn_token(TOKEN_GETATTR);
			} else {
				return spawn_identifier();
			}
		case 'l':
			if (bool_check_keyword("obals")) {
				return spawn_token(TOKEN_GLOBALS);
			} else if (bool_check_keyword("obal")) {
				return spawn_token(TOKEN_GLOBAL);
			} else {
				return spawn_identifier();
			}
		default:
			lex.current_char--;
			return spawn_identifier();
	}
}


static Token check_h_branch() {
	char c = consume_char();
	switch (c) {
		case 'a':
			if (bool_check_keyword("sattr")) {
				return spawn_token(TOKEN_HASATTR);
			} else if (bool_check_keyword("sh")) {
				return spawn_token(TOKEN_HASH);
			} else {
				return spawn_identifier();
			}
		case 'e':
			if (bool_check_keyword("lp")) {
				return spawn_token(TOKEN_HELP);
			} else if (bool_check_keyword("x")) {
				return spawn_token(TOKEN_HEX);
			} else {
				return spawn_identifier();
			}
		default:
			lex.current_char--;
			return spawn_identifier();
	}
}


static Token check_i_branch() {
	char c = consume_char();
	switch (c) {
		case 'd':
			if (bool_check_keyword("")) {
				return spawn_token(TOKEN_ID);
			} else {
				return spawn_identifier();
			}
		case 'm':
			if (bool_check_keyword("port")) {
				return spawn_token(TOKEN_IMPORT);
			} else {
				return spawn_identifier();
			}
		case 'n':
			if (bool_check_keyword("")) {
				return spawn_token(TOKEN_IN);
			} else if (bool_check_keyword("put")) {
				return spawn_token(TOKEN_INPUT);
			} else if (bool_check_keyword("t")) {
				return spawn_token(TOKEN_INT);
			} else {
				return spawn_identifier();
			}
		case 's':
			if (bool_check_keyword("instance")) {
				return spawn_token(TOKEN_ISINSTANCE);
			} else if (bool_check_keyword("subclass")) {
				return spawn_token(TOKEN_ISSUBCLASS);
			} else if (bool_check_keyword("")) {
				return spawn_token(TOKEN_IS);
			} else {
				return spawn_identifier();
			}
		case 'f':
			if (bool_check_keyword("")) {
				return spawn_token(TOKEN_IF);
			} else {
				return spawn_identifier();
			}
		case 't':
			if (bool_check_keyword("er")) {
				return spawn_token(TOKEN_ITER);
			} else {
				return spawn_identifier();
			}
		default:
			lex.current_char--;
			return spawn_identifier();
	}
}


static Token check_l_branch() {
	char c = consume_char();
	switch (c) {
		case 'a':
			if (bool_check_keyword("mbda")) {
				return spawn_token(TOKEN_LAMBDA);
			} else {
				return spawn_identifier();
			}
		case 'e':
			if (bool_check_keyword("n")) {
				return spawn_token(TOKEN_LEN);
			} else {
				return spawn_identifier();
			}
		case 'i':
			if (bool_check_keyword("st")) {
				return spawn_token(TOKEN_LIST);
			} else {
				return spawn_identifier();
			}
		case 'o':
			if (bool_check_keyword("cals")) {
				return spawn_token(TOKEN_LOCALS);
			} else {
				return spawn_identifier();
			}
		default:
			lex.current_char--;
			return spawn_identifier();
	}
}


static Token check_m_branch() {
	char c = consume_char();
	switch (c) {
		case 'a':
			if (bool_check_keyword("p")) {
				return spawn_token(TOKEN_MAP);
			} else if (bool_check_keyword("x")) {
				return spawn_token(TOKEN_MAX);
			} else {
				return spawn_identifier();
			}
		case 'e':
			if (bool_check_keyword("moryview")) {
				return spawn_token(TOKEN_MEMORYVIEW);
			} else {
				return spawn_identifier();
			}
		case 'i':
			if (bool_check_keyword("n")) {
				return spawn_token(TOKEN_MIN);
			} else {
				return spawn_identifier();
			}
		default:
			lex.current_char--;
			return spawn_identifier();
	}
}


static Token check_n_branch() {
	char c = consume_char();
	switch (c) {
		case 'e':
			if (bool_check_keyword("xt")) {
				return spawn_token(TOKEN_NEXT);
			} else {
				return spawn_identifier();
			}
		case 'o':
			if (bool_check_keyword("nlocal")) {
				return spawn_token(TOKEN_NONLOCAL);
			} else if (bool_check_keyword("t")) {
				return spawn_token(TOKEN_NOT_KEYWORD);
			} else {
				return spawn_identifier();
			}
		default:
			lex.current_char--;
			return spawn_identifier();
	}
}


static Token check_o_branch() {
	char c = consume_char();
	switch (c) {
		case 'b':
			if (bool_check_keyword("ject")) {
				return spawn_token(TOKEN_OBJECT);
			} else {
				return spawn_identifier();
			}
		case 'r':
			if (bool_check_keyword("")) {
				return spawn_token(TOKEN_OR_KEYWORD);
			} else if (bool_check_keyword("d")) {
				return spawn_token(TOKEN_ORD);
			} else {
				return spawn_identifier();
			}
		case 'c':
			if (bool_check_keyword("t")) {
				return spawn_token(TOKEN_OCT);
			} else {
				return spawn_identifier();
			}
		case 'p':
			if (bool_check_keyword("en")) {
				return spawn_token(TOKEN_OPEN);
			} else {
				return spawn_identifier();
			}
		default:
			lex.current_char--;
			return spawn_identifier();
	}
}


static Token check_p_branch() {
	char c = consume_char();
	switch (c) {
		case 'a':
			if (bool_check_keyword("ss")) {
				return spawn_token(TOKEN_PASS);
			} else {
				return spawn_identifier();
			}
		case 'o':
			if (bool_check_keyword("w")) {
				return spawn_token(TOKEN_POW);
			} else {
				return spawn_identifier();
			}
		case 'r':
			if (bool_check_keyword("int")) {
				return spawn_token(TOKEN_PRINT);
			} else if (bool_check_keyword("operty")) {
				return spawn_token(TOKEN_PROPERTY);
			} else {
				return spawn_identifier();
		}
		default:
			lex.current_char--;
			return spawn_identifier();
	}
}


static Token check_r_branch() {
	char c = consume_char();
	switch (c) {
		case 'a':
			if (bool_check_keyword("ise")) {
				return spawn_token(TOKEN_RAISE);
			} else if (bool_check_keyword("nge")) {
				return spawn_token(TOKEN_RANGE);
			} else {
				return spawn_identifier();
			}
		case 'e':
			if (bool_check_keyword("pr")) {
				return spawn_token(TOKEN_REPR);
			} else if (bool_check_keyword("turn")) {
				return spawn_token(TOKEN_RETURN);
			} else if (bool_check_keyword("versed")) {
				return spawn_token(TOKEN_REVERSED);
			} else {
				return spawn_identifier();
			}
		case 'o':
			if (bool_check_keyword("und")) {
				return spawn_token(TOKEN_ROUND);
			} else {
				return spawn_identifier();
			}
		default:
			lex.current_char--;
			return spawn_identifier();
	}
}


static Token check_s_branch() {
	char c = consume_char();
	switch (c) {
		case 'e':
			if (bool_check_keyword("tattr")) {
				return spawn_token(TOKEN_SETATTR);
			} else if (bool_check_keyword("t")) {
				return spawn_token(TOKEN_SET);
			} else {
				return spawn_identifier();
			}
		case 'l':
			if (bool_check_keyword("ice")) {
				return spawn_token(TOKEN_SLICE);
			} else {
				return spawn_identifier();
			}
		case 'o':
			if (bool_check_keyword("rted")) {
				return spawn_token(TOKEN_SORTED);
			} else {
				return spawn_identifier();
			}
		case 't':
			if (bool_check_keyword("aticmethod")) {
				return spawn_token(TOKEN_STATICMETHOD);
			} else if (bool_check_keyword("r")) {
				return spawn_token(TOKEN_STR);
			} else {
				return spawn_identifier();
			}
		case 'u':
			if (bool_check_keyword("m")) {
				return spawn_token(TOKEN_SUM);
			} else if (bool_check_keyword("per")) {
				return spawn_token(TOKEN_SUPER);
			} else {
				return spawn_identifier();
			}
		default:
			lex.current_char--;
			return spawn_identifier();
	}
}


static Token check_t_branch() {
	char c = consume_char();
	switch (c) {
		case 'u':
			if (bool_check_keyword("ple")) {
				return spawn_token(TOKEN_TUPLE);
			} else {
				return spawn_identifier();
			}
		case 'r':
			if (bool_check_keyword("y")) {
				return spawn_token(TOKEN_TRY);
			} else {
				return spawn_identifier();
			}
		case 'y':
			if (bool_check_keyword("pe")) {
				return spawn_token(TOKEN_TYPE);
			} else {
				return spawn_identifier();
			}
		default:
			lex.current_char--;
			return spawn_identifier();
	}
}


static Token check_v_branch() {
	char c = consume_char();
	switch (c) {
		case 'a':
			if (bool_check_keyword("rs")) {
				return spawn_token(TOKEN_VARS);
			} else {
				return spawn_identifier();
			}
		default:
			lex.current_char--;
			return spawn_identifier();
	}
}


static Token check_w_branch() {
	char c = consume_char();
	switch (c) {
		case 'h':
			if (bool_check_keyword("ile")) {
				return spawn_token(TOKEN_WHILE);
			} else {
				return spawn_identifier();
			}
		case 'i':
			if (bool_check_keyword("th")) {
				return spawn_token(TOKEN_WITH);
			} else {
				return spawn_identifier();
			}
		default:
			lex.current_char--;
			return spawn_identifier();
	}
}


static Token check_y_branch() {
	char c = consume_char();
	switch (c) {
		case 'i':
			if (bool_check_keyword("eld")) {
				return spawn_token(TOKEN_YIELD);
			} else {
				return spawn_identifier();
			}
		default:
			lex.current_char--;
			return spawn_identifier();
	}
}


static Token check_z_branch() {
	char c = consume_char();
	switch (c) {
		case 'i':
			if (bool_check_keyword("p")) {
				return spawn_token(TOKEN_ZIP);
			} else {
				return spawn_identifier();
			}
		default:
			lex.current_char--;
			return spawn_identifier();
	}
}


static Token check_underscore_branch() {
	char c = consume_char();
	switch (c) {
		case '_':
			if (bool_check_keyword("import__")) {
				return spawn_token(TOKEN_IMPORT_FUNC);
			} else {
				return spawn_identifier();
			}
		default:
			lex.current_char--;
			return spawn_identifier();
	}
}


// Checks if string of characters is a keyword and returns the corresponding token.
// If not keyword, capture lexeme and return identifier token.
static Token identifier_or_keyword() {
	switch (lex.current_char[-1]) {
		case 'F':
			if (bool_check_keyword("alse")) {
				// Check for TOKEN_FALSE
				return spawn_token(TOKEN_FALSE);
			} else {
				return spawn_identifier();
			}
		case 'N':
			if (bool_check_keyword("one")) {
				// Check for TOKEN_NONE
				return spawn_token(TOKEN_NONE);
			} else {
				return spawn_identifier();
			}
		case 'T':
			if (bool_check_keyword("rue")) {
				// Check for TOKEN_TRUE
				return spawn_token(TOKEN_TRUE);
			} else {
				return spawn_identifier();
			}
		case 'a':
			return check_a_branch();
		case 'b':
			return check_b_branch();
		case 'c':
			return check_c_branch();
		case 'd':
			return check_d_branch();
		case 'e':
			return check_e_branch();
		case 'f':
			return check_f_branch();
		case 'g':
			return check_g_branch();
		case 'h':
			return check_h_branch();
		case 'i':
			return check_i_branch();
		case 'l':
			return check_l_branch();
		case 'm':
			return check_m_branch();
		case 'n':
			return check_n_branch();
		case 'o':
			return check_o_branch();
		case 'p':
			return check_p_branch();
		case 'r':
			return check_r_branch();
		case 's':
			return check_s_branch();
		case 't':
			return check_t_branch();
		case 'v':
			return check_v_branch();
		case 'w':
			return check_w_branch();
		case 'y':
			return check_y_branch();
		case 'z':
			return check_z_branch();
		case '_':
			return check_underscore_branch();
		default:
			return spawn_identifier();
	}
}


// Main logic, call this repeatedly in a loop until EOF token reached
// in order to lex a program. (Be sure to initialize Lexer with source
// code first with initialize_lexer() and get_source_from_file()).
Token get_next_token(bool was_newline) {
	skip_whitespace(was_newline);
	lex.start_char = lex.current_char;

	if (is_at_end()) {
		return spawn_token(TOKEN_EOF);
	}

	char c = consume_char();

	if (is_digit(c)) {
		return number();
	}

	// Handle keywords and identifiers
	if (is_letter(c)) {
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
		// In order to make things straight forward, we strictly inforce PEP8 4 space indent.
		case ' ':	return spawn_token(TOKEN_TAB);
		case '\t':	return spawn_token(TOKEN_TAB);
		// Single lookahead tokens (one or two characters)
		case '=':       
			return spawn_token(
				look_ahead('=') ? TOKEN_EQUAL_EQUAL : TOKEN_EQUALS);
		case '+':       
			return spawn_token(
				look_ahead('=') ? TOKEN_ADD_ASSIGN : TOKEN_ADD);
		case '%':       
			return spawn_token(
				look_ahead('=') ? TOKEN_MODULUS_ASSIGN : TOKEN_MODULUS);
		case '&':       
			return spawn_token(
				look_ahead('=') ? TOKEN_AMPERSAND_ASSIGN : TOKEN_AMPERSAND);
		case '@':       
			return spawn_token(
				look_ahead('=') ? TOKEN_MATRIX_MULT_ASSIGN : TOKEN_MATRIX_MULT);
		case '^':       
			return spawn_token(
				look_ahead('=') ? TOKEN_XOR_ASSIGN : TOKEN_XOR);
		case ':':       
			return spawn_token(
				look_ahead('=') ? TOKEN_BNF : TOKEN_COLON);
		case '!':       
			return spawn_token(
				look_ahead('=') ? TOKEN_NOT_EQUAL : TOKEN_NOT);
		case '|':       
			return spawn_token(
				look_ahead('=') ? TOKEN_OR_ASSIGN : TOKEN_OR);
		case '-':       
			if (look_ahead('>')) {
				return spawn_token(TOKEN_ARROW);
			} else if (look_ahead('=')) {
				return spawn_token(TOKEN_MINUS_ASSIGN);
			} else {
				return spawn_token(TOKEN_MINUS);
			}
			// Double lookahead tokens (one - three characters)
		case '*':
			if ((peak_once() == '*') && (peak_twice() == '=')) {
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
