#ifndef code_generator_h
#define code_generator_h
#include "../Parser/parser.h"


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
	VASM_XOR
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
	size_t label_num;
	struct VasmInstruction *next;
}
VasmInstruction;


VasmInstruction *
spawn_vasm_op (VasmOperation, size_t, size_t, bool, bool);


VasmInstruction *
spawn_vasm_label (size_t);


VasmInstruction *
insert_vasm_instruction (VasmInstruction *, VasmInstruction *);


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
label (TreeNode *, TreeNode *, size_t *, FILE *, StNode **, int *);

VasmInstruction *
generate_vasm(size_t, TreeNode *, size_t *, StNode **, int *,
		VasmInstruction *);

void
tile (TreeNode *, TreeNode *, size_t *, FILE *, StNode **, int *);

bool
is_leftside (TreeNode *, TreeNode *);

TreeNode *
get_arg_array (TreeNode *);


#endif
