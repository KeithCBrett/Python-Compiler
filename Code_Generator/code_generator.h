#ifndef code_generator_h
#define code_generator_h
#include "../Parser/parser.h"


// Even though its vasm, due to div we need some way to refer these.
#define REG_RAX 0
#define REG_RBX 1
#define REG_RCX 2
#define REG_RDX 3
// We also need to reserve space for primitive python functions like print.
#define VASM_FUNC_PRINTSTR 4
#define VASM_FUNC_CAT_ARGS 5


typedef struct
LineInfo
{
	size_t prev;
	size_t curr;
}
LineInfo;


// Here we define the different error codes (like lack of indented block after
// for loop). We will insert them into spawn_python_error function whenever a
// incorrect tree pattern is found.
typedef enum
ErrorCode
{
	ERROR_INDENTATION = 0,
}
ErrorCode;


typedef struct
StNode {
	int contents;
	size_t lexeme;
	TreeNode *node;
	struct StNode *next;
}
StNode;


typedef enum
VasmOperation
{
	VASM_MOV = 0,
	VASM_ADD,
	VASM_SUB,
	VASM_MUL,
	VASM_DIV,
	VASM_XOR,
	VASM_CALL,
	VASM_PUSH,
	VASM_DEC,
	VASM_CMP,
	VASM_INC = 10,
	VASM_JE,
	VASM_JMP,
	VASM_FOR_ENTRY_LABEL,
	VASM_FOR_BODY_LABEL,
	VASM_FOR_EXIT_LABEL
}
VasmOperation;


typedef enum
LabelType
{
	LT_FOR = 0,
}
LabelType;


typedef enum
LabelSubtype
{
	LST_FOR_ENTRY = 0,
	LST_FOR_BODY,
	LST_FOR_EXIT
}
LabelSubtype;


typedef struct
LabelData
{
	size_t type;
	size_t subtype;
	size_t id;
}
LabelData;


typedef struct
LoopCounts
{
	// We format our loops with a beginning, middle and end.
	// We will need individual counts for each so that we can
	// be sure to assign labels unique names.
	size_t beginning_count;
	size_t middle_count;
	size_t end_count;
}
LoopCounts;


typedef struct
VasmInstruction
{
	VasmOperation op;
	size_t regl;
	size_t regr;
	bool regl_reg;
	bool regr_reg;
	LabelData label_data;
	size_t line_number;
	struct VasmInstruction *next;
}
VasmInstruction;


VasmInstruction *
spawn_vasm_op (VasmOperation, size_t, size_t, bool, bool, bool, size_t);


VasmInstruction *
insert_vasm_instruction (VasmInstruction *, VasmInstruction *);


void
output_vasm_file (FILE *, VasmInstruction *);


bool
left_side_register (VasmInstruction *);


bool
right_side_register (VasmInstruction *);


bool
both_side_register (VasmInstruction *);


bool
no_registers (VasmInstruction *);


// Input: Nothing.
// Output: Returns an array of linked lists (empty). This is the basis of our
// symbol table.
StNode **
st_spawn_table ();


// Input: An array of linked lists spawned by st_spawn_table().
// Output: Nothing, returns malloced memory back to heap with free.
void
st_kill_table (StNode **);


// Input: A symbol table spawned by st_spawn_table(), a AST node (typically of
// type TOKEN_IDENTIFIER) and some register we wish to associate with said
// identifer.
// Output: Symbol table updated with new identifier and register association.
StNode **
st_insert (StNode **, TreeNode *, int);

size_t
st_search (StNode **, TreeNode *);

void
st_delete (StNode **, TreeNode *);

StNode *
st_spawn_node (TreeNode *, int);

size_t
st_convert_string (const char *, int);

size_t
fib_hash (size_t);


void
label (TreeNode *, TreeNode *, size_t *, StNode **, LoopCounts *,
		VasmInstruction **, size_t *, bool *);

void
generate_vasm (size_t, TreeNode *, size_t *, StNode **, LoopCounts *,
		VasmInstruction **, size_t *);

void
tile (TreeNode *, TreeNode *, size_t *, StNode **, LoopCounts *,
		VasmInstruction **, size_t *, bool *);

/*
 * is_leftside (TreeNode *AST, TreeNode *node)
 * Input: AST, node in AST.
 * Output: Boolean representing whether we are assigning some variable a value,
 * like x in 'x=6', or whether its like y in 'x=y'.
 */
bool
is_leftside (TreeNode *, TreeNode *);

/*
 * get_arg_array (TreeNode *ast)
 * Input: AST, rooted at a comma.
 * Output: Gets all args and stuffs them in array. Used for concatenation
 * within print.
 */
TreeNode *
get_arg_array (TreeNode *);

/*
 * convert_constant (int length, char *first_char)
 * Input: First character of string and strings length.
 * Output: String cast to a size_t (we only input strings composed of numerics).
 */
size_t
convert_constant (int, const char *);

bool
is_indent_level_dropped (size_t, size_t);

LoopCounts *
spawn_loopcounts ();

// Returns the token type of input node. Used for readability/simpler if
// statements. Same can be said for function check_right_branch ().
TokenType
check_left_branch (TreeNode *);

TokenType
check_right_branch (TreeNode *);

// Takes newline node as input, returns type of token on next line (excluding
// newline). Used mainly for checking if next line is indented (usually
// returns TOKEN_TAB if program is written correctly).
TokenType
check_next_line (TreeNode *);

void
spawn_python_error (ErrorCode, size_t);


#endif
