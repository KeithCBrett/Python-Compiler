#include "common.h"
#include "lexer.h"
#include <stdio.h>


int main (int argc, char **argv) {
    FILE *fp = fopen("inputfile.py", "r");
    char *source = get_source_from_file(fp);
    fclose(fp);


    for (;;) {
        Token token = get_next_token();
    }


    free(source);
    return 0;
}


// Stores program as a string to be lexed
static char *get_source_from_file (FILE *file) {
    // Handle file error
    if (file == NULL) {
        fprintf(stderr, "Unable to open input file\n");
        exit(127);
    }


    // Get buffer size
    fseek(file, 0, SEEK_END);
    size_t source_length = ftell(file) - 1;
    rewind(file);


    // Store file in string
    char *source = malloc(source_length + 1);
    if (source == NULL) {
        fprintf(stderr, "Could not allocate enough memory\n");
        exit(127);
    }
    size_t buffer_size = fread(source, sizeof(char), source_length, file);
    if (buffer_size < source_length) {
        fprintf(stderr, "Could not read file to completion, fread failed\n");
        exit(127);
    }
    source[buffer_size] = '\0'; // Null terminate string
    return source;
}
