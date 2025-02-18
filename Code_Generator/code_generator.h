#ifndef code_generator_h
#define code_generator_h
#include "../Parser/parser.h"


void label(TreeNode *, TreeNode *, int *, FILE *);
void generate_asm(size_t, TreeNode *, int *, FILE *);
void tile(TreeNode *, TreeNode *, int *, FILE *);
bool is_leftside(TreeNode *, TreeNode *);
TreeNode *get_arg_array(TreeNode *);


#endif
