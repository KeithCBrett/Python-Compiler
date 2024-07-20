#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define BUFFER_SIZE 1024

/* Input grammar file, output number of characters in the file. */
/* |USECASE| */
/* Used for dynamic memory allocation and looping through grammars */
size_t kcb_get_buffer(FILE *input_file){
	long char_count = 0;
	int current_character;
	while ((current_character = fgetc(input_file)) != EOF) {
		char_count++;
	}
	return char_count;
}

/*
(kcb_store_file_as_array)
(Used to turn files contents into a character array so that
we can isolate terminals and non-terminals in a grammar
with array indexing.)
!!! FREE ARRAY WHEN DONE OR MEMORY LEAK WILL OCCUR !!!
*/
char *kcb_store_file_as_array(FILE *input_file){
	int c;
	size_t file_index = 0, buffer = BUFFER_SIZE;
	char *output_array = malloc(sizeof(*output_array) * buffer);
	if (!input_file) {
		fprintf(stderr, "Error: could not open file");
	}
	if (!output_array) {
		fprintf(stderr, "Error: malloc failed");
	}
	// Scan file
	while ((c = fgetc(input_file)) != EOF) {
		// Add character encountered in file to output_array
		output_array[file_index++] = c;
		// Allocate more memory if buffer size is wrong
		if (file_index == buffer) {
			void *temp = realloc(output_array, buffer + BUFFER_SIZE);
			if (!temp) {
				fprintf(stderr, "Error: Memory reallocation failed, out of memory");
				break;
			}
			output_array = temp;
			buffer += BUFFER_SIZE;
		}
	}
	output_array[file_index] = 0;
	fclose(input_file);
	return output_array;
}

// Returns 0 or 1 depending on whether its terminal or non-terminal
size_t kcb_term_or_nonterm(char *input_char){
	size_t is_it_terminal;

	return is_it_terminal;
}


// Isolates nonterminals so that we may exclude them from first set
void kcb_print_nonterminals(char *input_array){
	printf("\n");
	printf("Non-terminals:\n");
	for (size_t i = 0; i < strlen(input_array); i++) {
		if (input_array[i] == '.') {
			putchar(input_array[i+1]);
			if ((input_array[i+2]) == '\047') {
				putchar(input_array[i+2]);
			}
			printf("\n");
		}
	}
	printf("\n");
}


// Isolates terminals which will be apart of our first set.
void kcb_print_terminals(char *input_array){
	printf("\n");
	printf("Terminals:\n");
	for (size_t i = 0; i < strlen(input_array); i++) {
		if (input_array[i] == '>') {
			putchar(input_array[i+1]);
			printf("\n");
		}
	}
	printf("\n");
}


// Returns a array of characters representing our non-terminals
char *kcb_non_terminal_set(char *grammar){
	int c;
	size_t array_index = 0, buffer = BUFFER_SIZE;
	char *output_array = malloc(sizeof(*output_array) * buffer);
	if (!output_array) {
		fprintf(stderr, "Error: Malloc failed");
	}
	for (size_t i = 0; i < strlen(grammar); i++) {
		if (grammar[i] == '.') {
			output_array[array_index] = grammar[i+1];
			array_index++;
			if (grammar[i+2] == '\'') {
				output_array[array_index] = grammar[i+2];
				array_index++;
			}
		}
		if (array_index == buffer) {
			void *temp = realloc(output_array, buffer + BUFFER_SIZE);
			if (!temp) {
				fprintf(stderr, "Error: Realloc failed, out of memory");
				break;
			}
			output_array = temp;
			buffer += BUFFER_SIZE;
		}
	}
	output_array[array_index] = 0;
	return output_array; 
}


void kcb_print_char_array(char *input_array){
	for (size_t i = 0; i < strlen(input_array); i++) {
		putchar(input_array[i]);
	}
}


int main(int argc, char **argv){
	FILE *file = fopen(argv[1], "r");


	// Free this array when done
	char *grammar = kcb_store_file_as_array(file);
	char *non_terminals = kcb_non_terminal_set(grammar);


	char *non_terminals_array[BUFFER_SIZE];
	size_t non_term_index = 0;
	char word[BUFFER_SIZE];


	for (size_t i = 0; i < strlen(non_terminals); i++) {
		if (non_terminals[i] == '\'') {
			word = strcat(non_terminals[i-1], non_terminals[i]);
		}
	}







	free(grammar);
	free(non_terminals);

	return 0;
}
