#ifndef code_generator_h
#define code_generator_h

#include "../Parser/parser.h"


typedef struct TileRule{
	size_t contents;
	size_t left;
	size_t right;
} TileRule;


// for temp registers.
typedef struct IntllNode {
	int contents;
	struct IntllNode *next;
} IntllNode;
void IntllEnqueue(int, IntllNode **);
IntllNode *IntllDequeue(IntllNode **);
IntllNode *spawn_IntllNode(int);
void init_regnames(IntllNode **);
void init_reginuse(IntllNode **);
int get_reg(IntllNode **, IntllNode **);


TileRule *spawn_tile_rule(size_t);
void label(TreeNode *, int *);
void generate_asm(size_t, TreeNode *, int *);
int **init_array(int **);
void tile(TreeNode *, int *);




#endif
