#include "Lexer/lexer.h"
#include "Parser/parser.h"
#include "Code_Generator/code_generator.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int main(int argc, char **argv){
	FILE *fp = fopen("inputfile.py", "r");
	char *source = get_source_from_file(fp);
	fclose(fp);

	initialize_lexer(source);

	TreeNode *root = NULL;
	root = parse(Prec_Outcomes, root);

	size_t tree_size = count_tree_nodes(root);
	TreeNode **array = postorder(root, tree_size);

	int regcount = 0;
	int *p_regcount = &regcount;


	tile(root, root, p_regcount);

	/*
	for (size_t i = 0; i < tree_size; i++) {
		printf("%.*s ", array[i]->contents.length, array[i]->contents.first_char);
		printf("%s", print_type(array[i]->contents.type));
	}
	printf("\n");
	*/
}
