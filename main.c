#include "Lexer/lexer.h"
#include "Parser/parser.h"
#include "Code_Generator/code_generator.h"
#include "Testing/testing.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int
main (int argc, char **argv)
{
	if (argc < 4)
	{
		if (argv[1][0] != '-')
		{
			fprintf (stderr,
			"Error, output type ommitted. Usage: pyco -asm");
			fprintf (stderr,
			" inputfile.py outputfile.asm\n");
			return 1;
		}
		else
		{
			fprintf (stderr,
			"Error, output file ommitted. Usage: pyco -asm");
			fprintf (stderr,
			" inputfile.py outputfile.asm\n");
			return 1;
		}
	}
	if (argc < 3)
	{
		fprintf (stderr,
		"Error, input file ommitted. Usage: pyco -asm ");
		fprintf (stderr,
		"inputfile.py outputfile.asm\n");
		return 1;

	}
	if (argc < 2)
	{
		fprintf (stderr,
		"Error, output type ommitted. Usage: pyco -asm");
		fprintf (stderr,
		" inputfile.py outputfile.asm\n");
		return 1;
	}

	// Input file (python source to compile)
	FILE *ifp = fopen (argv[2], "r");

	// Output file (x86-64 assembly source if all goes well)
	FILE *ofp = fopen (argv[3], "w");

	// Store input file in string.
	char *source = get_source_from_file (ifp);
	// Done with input file, free.
	fclose (ifp);

	initialize_lexer (source);

	TreeNode *root = NULL;
	bool was_newline = true;


	root = parse (Prec_Start, root, was_newline);

	// Now that we have our ast, we have to check a couple of invalid
	// roots. This mainly pertains to one line programs. For example,
	// if root is TOKEN_FOR, this means we have some for loop without
	// a body (because loops take up multiple lines, we need atleast one
	// TOKEN_NEWLINE which will always take precedence over TOKEN_FOR.
	
	bool error = false;
	bool *p_error = &error;

	// Handling for bodyless loop.
	if (root->contents.type == TOKEN_FOR)
	{
		spawn_python_error (ERROR_INDENTATION,
				root->contents.line_number);
		*p_error = true;
	}

	root->is_root = true;
	size_t indent_level = 0;
	size_t *p_indent_level = &indent_level;
	size_t line_num = 0;
	size_t *p_line_num = &line_num;

	// Symbol table for variables.
	StNode **symbol_table = st_spawn_table ();
	VasmInstruction *vasm = NULL;
	VasmInstruction **p_vasm = &vasm;

	// We start our virtual register count at 5. This is because 0-4 are
	// used to reference rax, rcx, etc.
	CountArray *count_array = spawn_count_array (2, 5, 2);

	// Perform instruction selection using virtual registers.
	tile
		(root, root, symbol_table, p_vasm, p_line_num, p_error,
		 count_array);
	run_tests();
	const char *vasm_flag = "-vasm";
	if ((strcmp (argv[1], vasm_flag)) == 0)
	{
		if (error == 0)
		{
			output_vasm_file (ofp, vasm);
		}
		else
		{
			// Delete emitted file, error occured so it is
			// probably unusable.
			int del = remove(argv[3]);
		}
	}


	kill_tree (root);
	root = NULL;

	// CONVERT VIRTUAL REGISTERS TO REAL ONES HERE.
	// pass

	// Assembly generated, close output file.
	fclose (ofp);
	// Tree done, discard source.
	free (source);

}
