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
	VASM_JE,
	VASM_JMP,
	VASM_INC,
	VASM_LABEL_START,
	VASM_LABEL_END,
}
VasmOperation;


typedef struct
VasmInstruction
{
	VasmOperation op;
	size_t regl;
	size_t regr;
	bool regl_reg;
	bool regr_reg;
	bool label;
	size_t label_num;
	struct VasmInstruction *next;
}
VasmInstruction;


VasmInstruction *
spawn_vasm_op (VasmOperation, size_t, size_t, bool, bool, bool);


VasmInstruction *
insert_vasm_instruction (VasmInstruction *, VasmInstruction *);


void
output_vasm_file (FILE *, VasmInstruction *);


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
label (TreeNode *, TreeNode *, size_t *, StNode **, size_t *,
		VasmInstruction **);

void
generate_vasm (size_t, TreeNode *, size_t *, StNode **, size_t *,
		VasmInstruction **);

void
tile (TreeNode *, TreeNode *, size_t *, StNode **, size_t *,
		VasmInstruction **);

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


#endif
