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
CountArray {
	size_t header_count;
	size_t regcount;
	size_t footer_count;
}
CountArray;


typedef struct
StNode {
	int contents;
	size_t lexeme;
	TreeNode *node;
	struct StNode *next;
}
StNode;


typedef enum
AsmRule
{
	ASM_IDENTIFIER = 0,
	ASM_DEFER,
	ASM_CONSTANT,
	ASM_ADDITION,
	ASM_MULTIPLICATION,
	ASM_EQUALS,
	ASM_SUBTRACTION,
	ASM_DIVISION,
	ASM_IDENTIFIER_RS,
	ASM_PRINT,
	ASM_COMMA = 10,
	ASM_FORLOOPID,
	ASM_IN,
	ASM_FOR,
	ASM_CLOSE_LOOP
}
AsmRule;


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
	VASM_LOOP_CONST
}
VasmOperation;


typedef enum
LabelType
{
	LOOP_CONST = 0,
}
LabelType;


typedef struct
LabelData
{
	LabelType type;
	size_t id;
}
LabelData;


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


/*
 * spawn_vasm_op ()
 * Input:
 * 	VasmOperation	<- Some operation in the VASM language. Typically
 * 			has a corresponding X64 operation.
 *
 * 	size_t		<- Register number/id of left register, if its a
 * 			register. If the VasmOperation is something to do
 * 			with loops, we enter a loop id instead.
 *
 * 	size_t		<- Same as above but for the right register.
 *
 * 	bool		<- A boolean describing whether or not the left
 * 			operand is a register or something else.
 *
 * 	bool		<- Same as above but for the right operand.
 *
 * 	bool		<- Some boolean describing whether or not the
 * 			instruction is a label.
 *
*/
VasmInstruction *
spawn_vasm_op (VasmOperation, size_t, size_t, bool, bool, bool);


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


/*
 * label ()
 * Input:
 * 	TreeNode *		<- Some node in our A.S.T., this is the root
 * 				of the subtree we will try to pattern match.
 *
 * 	TreeNode *		<- The root of A.S.T. proper.
 *
 *	size_t *		<- Register count for assigning virtual
 *				register ids.
 *
 *	StNode **		<- Symbol table.
 *
 *	size_t *		<- Loop count for unique VASM label names.
 *
 *	VasmInstruction **	<- Linked list of VASM instructions.
 *
 *	bool *			<- Boolean describing whether or not an error
 *				occured.
*/
void
label (TreeNode *, TreeNode *, StNode **, VasmInstruction **, bool *,
		CountArray *);

/*
 * generate_vasm ()
 * Input:
 * 	size_t			<- Code generation rule derived from label ()
 * 				tree pattern matching.
 *
 * 	TreeNode *		<- Root of subtree passed to label. Useful for
 * 				getting lexemes and line numbers of tokens that
 * 				caused errors.
 *
 * 	TreeNode *		<- Root of A.S.T. proper.
 *
 *	size_t *		<- Register counter for assigning unique virtual
 *				register I.D.'s.
 *
 *	StNode **		<- Symbol table for symbol table things.
 *
 *	size_t *		<- Loop count for generating vasm level loop
 *				labels.
 *
 *	VasmInstruction **	<- Linked list of VASM instructions. This is
 *				is out implicit output.
 * Output:
 * 	We implicitly output vasm instructions to the linked list entered in
 * 	the VasmInstruction ** slot.
*/
void
generate_vasm (size_t, TreeNode *, TreeNode *, StNode **, VasmInstruction **,
		CountArray *);

/*
 * tile ()
 * Input:
 * 	TreeNode *		<- Node in A.S.T. to tile.
 *
 * 	TreeNode *		<- Root of A.S.T..
 *
 * 	size_t *		<- Register count used for assigning virtual
 * 				registers in VASM.
 *
 * 	StNode **		<- Symbol table for symbol table things.
 *
 * 	size_t *		<- Loop count for creating assembly labels that
 * 				relate to loops.
 *
 * 	VasmInstruction **	<- Empty linked list of vasm instructions. We
 * 				implicitly output here.
 *
 * 	size_t *		<- Line number of "Node in A.S.T to tile". We
 * 				use this if errors occur in our users program.
 *
 *	bool *			<- Boolean describing whether or not an error
 *				occured in our users program.
 *
 * Output:
 * 	No explicit output, we implicitly output to VasmInstruction **. As a
 * 	side ~NOTE~ make this output explicit.
 * 	tile is responsible for walking the A.S.T. (visiting all nodes in a
 * 	post-order traversal). It is mainly responsible for calling label ()
 * 	which performs tree pattern matching to derive a code generation rule.
*/
void
tile (TreeNode *, TreeNode *, StNode **, VasmInstruction **, size_t *, bool *,
		CountArray *);

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

/*
 * indent_level_dropped ()
 * Input:
 * 	size_t		<- The indent level of the previous node in the
 * 			A.S.T., i.e. the parent of the node we are checking.
 * 	size_t		<- The indent level of the node we want to check.
 * Output:
 * 	bool		<- A boolean describing whether or not there was a
 * 			drop in the indent level.
*/
bool
indent_level_dropped (size_t, size_t);

// Returns the token type of input node. Used for readability/simpler if
// statements. Same can be said for function check_right_branch ().
TokenType
check_left_branch (TreeNode *);

// Same as check_left_branch () but ignores tabs.
TokenType
check_left_branch_tabless (TreeNode *);

TokenType
check_right_branch (TreeNode *);

// Takes newline node as input, returns type of token on next line (excluding
// newline). Used mainly for checking if next line is indented (usually
// returns TOKEN_TAB if program is written correctly).
TokenType
check_next_line (TreeNode *);

void
spawn_python_error (ErrorCode, size_t);

/*
 * loop_nested ()
 * Input:
 * 	TreeNode *	<- Root of A.S.T..
 *
 * 	TreeNode *	<- Node within the A.S.T. of type newline. This is the
 * 			node we are checking.
 * Output:
 * 	bool		<- A boolean describing whether or not the node we are
 * 			checking is a loop nested within another loop. If this
 * 			is the case, we have to generate code accordingly.
*/
bool
loop_nested (TreeNode *, TreeNode *);


/*
 * get_nesting_level ()
 * Input:
 * 	TreeNode *	<- Node of type TOKEN_FOR. We return how deep the
 * 			nesting is within the loop. I.e. if there are no
 * 			nested loops within TreeNode *, we return 0.
 * Output:
 * 	size_t		<- Some integer representing how deep the nesting is.
 * 			We need this number to generate the correct number of
 * 			assembly labels.
*/
size_t
get_nesting_level (TreeNode *, TreeNode *);


/*
 * get_parent()
 * Input:
 * 	TreeNode *	<- A node in our A.S.T., The return will be a pointer
 * 			to this node's parent.
 * 	TreeNode *	<- The root of our A.S.T.
 * Output:
 * 	TreeNode *	<- A pointer to input node's parent.
*/
TreeNode *
get_parent (TreeNode *, TreeNode *);


/*
 * is_for_loop ()
 * Input:
 * 	TreeNode *	<- Root of tree to check. We use this function on
 * 			TOKEN_NEWLINEs in order to close loops.
 * Output:
 * 	bool		<- A boolean describing whether or not the left branch
 * 			is equal to TOKEN_FOR.
*/
bool
is_for_loop (TreeNode *);


/*
 * check_indent_level ()
 * Input:
 * 	TreeNode *	<- Some node of type TOKEN_NEWLINE. This node
 * 			typically has a left branch of type TOKEN_TAB, which
 * 			we would like to ascertain the level of.
 * Output:
 * 	size_t		<- The node's indent level. We use this in
 * 			get_nesting_level () to check if a for loop is within
 * 			another's body.
*/
size_t
check_indent_level (TreeNode *);


/*
 * spawn_count_array ()
 * Input:
 * 	size_t		<- Init header count.
 * 	size_t		<- Init register count.
 * 	size_t		<- Init footer count.
 * Output:
 * 	CountArray *	<- Wrapper for all of our counters.
*/
CountArray *
spawn_count_array (size_t, size_t, size_t);
#endif
