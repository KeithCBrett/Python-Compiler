#ifndef code_generator_h
#define code_generator_h
#include "../Parser/parser.h"


void label(TreeNode *, TreeNode *, int *);
void generate_asm(size_t, TreeNode *, int *);
void tile(TreeNode *, TreeNode *, int *);
bool is_leftside(TreeNode *, TreeNode *);


#endif
