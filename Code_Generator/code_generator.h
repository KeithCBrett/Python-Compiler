#ifndef code_generator_h
#define code_generator_h
#include "../Parser/parser.h"


typedef struct StNode{
	int contents;
	size_t lexeme;
	TreeNode *node;
	struct StNode *next;
} StNode;


StNode **st_spawn_table();
void st_kill_table(StNode **);
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
