#include "Lexer/lexer.h"
#include "Parser/parser.h"
#include "Code_Generator/code_generator.h"
#include "Testing/testing.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int main(int argc, char **argv){
	if (argc < 3) {
		printf ("Error, output file ommitted. Usage: pyco inputfile.py");
		printf (" outputfile.py\n");
		return 1;
	}
	if (argc < 2) {
		printf ("Error, input file ommitted. Usage: pyco inputfile.py");
		printf ("outputfile.py\n");
		return 1;
	}
	// Input file (python source to compile)
	FILE *ifp = fopen(argv[1], "r");

	// Output file (x86-64 assembly source if all goes well)
	FILE *ofp = fopen(argv[2], "w");

	// Store input file in string.
	char *source = get_source_from_file(ifp);
	// Done with input file, free.
	fclose(ifp);

	initialize_lexer(source);

	//test_parser();
	TreeNode *root = NULL;
	bool was_newline = false;
	root = parse(Prec_Start, root, was_newline);


	// For virtual registers. We start at 100 because the first 100 are
	// reserved for various things. For one, 0-4 are reserved for rax,
	// rbx, rcx, rdx. We probably don't need all of them, but we definitely
	// need atleast rdx and rax for division, even in vasm. The other
	// reserved names are for primitive functions and helper functions
	// like print, abs, and whatnot.
	size_t regcount = 5;
	size_t *p_regcount = &regcount;
	size_t loopcount = 1;
	size_t *p_loopcount = &loopcount;

	// Symbol table for variables.
	StNode **symbol_table = st_spawn_table();
	VasmInstruction *vasm = NULL;
	VasmInstruction **p_vasm = &vasm;

	// Perform instruction selection using virtual registers.
	tile (root, root, p_regcount, symbol_table, p_loopcount, p_vasm);
	output_vasm_file (ofp, vasm);

	kill_tree (root);
	root = NULL;

	// CONVERT VIRTUAL REGISTERS TO REAL ONES HERE.
	// pass

	// Assembly generated, close output file.
	fclose (ofp);
	// Tree done, discard source.
	free (source);

	/*
	for (size_t i = 0; i < tree_size; i++) {
		printf("%.*s ", array[i]->contents.length, array[i]->contents.first_char);
		printf("%s", print_type(array[i]->contents.type));
	}
	printf("\n");
	*/
}
