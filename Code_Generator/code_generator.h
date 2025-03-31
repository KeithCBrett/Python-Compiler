#ifndef code_generator_h
#define code_generator_h
#include "../Parser/parser.h"


typedef struct StNode{
	int contents;
	size_t lexeme;
	TreeNode *node;
	struct StNode *next;
} StNode;


// Input: Nothing.
// Output: Returns an array of linked lists (empty). This is the basis of our
// symbol table.
StNode **st_spawn_table();


// Input: An array of linked lists spawned by st_spawn_table().
// Output: Nothing, returns malloced memory back to heap with free.
void st_kill_table(StNode **);


// Input: A symbol table spawned by st_spawn_table(), a AST node (typically of
// type TOKEN_IDENTIFIER) and some integer we wish to associate with said
// identifer.
// Output: Symbol table updated with new identifier and integer association.
StNode **st_insert(StNode **, TreeNode *, int);
int st_search(StNode **, TreeNode *);
void st_delete(StNode **, TreeNode *);
StNode *st_spawn_node(TreeNode *, int);
size_t st_convert_string(const char *, int);


void label(TreeNode *, TreeNode *, int *, FILE *, StNode **);
void generate_asm(size_t, TreeNode *, int *, FILE *, StNode **);
void tile(TreeNode *, TreeNode *, int *, FILE *, StNode **);
bool is_leftside(TreeNode *, TreeNode *);
size_t fib_hash(size_t);
TreeNode *get_arg_array(TreeNode *);


#endif
