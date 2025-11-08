/***********************************************************************
 *  File name   : main.c
 *  Description : Entry point for the Lexical Analyzer Project.
 *                Reads an input C source file, tokenizes it line by line,
 *                and generates a summary report containing token counts.
 *
 *                Responsibilities:
 *                - Open input and output files
 *                - Read source code line by line
 *                - Tokenize each line using tokenize_line()
 *                - Track statistics for various token categories
 *                - Write summary results to output.txt
 *
 *                Functions:
 *                - main()
 * 
 ***********************************************************************/

#include "main.h"

int main(int argc, char **argv, char **envp)
{
    FILE *fptr_input, *fptr_output;
    char line_buffer[1000];
    TokenStats token_stats = {0};    // Initialize all token counters to zero

    // Validate and open input/output files
    if (argc < 2)
    {
        fprintf(stderr, "Usage: %s <input_file.c>\n", argv[0]);
        return 1;
    }

    fptr_input = fopen(argv[1], "r");
    fptr_output = fopen("output.txt", "w");

    if (fptr_input == NULL || fptr_output == NULL)
    {
        perror("Error opening file");
        return 1;
    }

    // Read and process each line from input file
    while (fgets(line_buffer, sizeof(line_buffer), fptr_input))
    {
        line_buffer[strcspn(line_buffer, "\n")] = '\0';  // Remove trailing newline
        tokenize_line(line_buffer, fptr_output, &token_stats);
    }

    // Write token summary to output file
    fprintf(fptr_output, "\nSummary:\n");
    fprintf(fptr_output, "Keywords: %d\n", token_stats.keyword);
    fprintf(fptr_output, "Identifiers: %d\n", token_stats.identifiers);
    fprintf(fptr_output, "Operators: %d\n", token_stats.operators);
    fprintf(fptr_output, "Integers: %d\n", token_stats.integers);
    fprintf(fptr_output, "Real Numbers: %d\n", token_stats.real_numbers);
    fprintf(fptr_output, "String Literals: %d\n", token_stats.string_literals);
    fprintf(fptr_output, "Delimiters: %d\n", token_stats.delimiters);
    fprintf(fptr_output, "Preprocessor Directives: %d\n", token_stats.preprocessor);
    fprintf(fptr_output, "Hex/Octal Numbers: %d\n", token_stats.hex_octal);
    fprintf(fptr_output, "Character Literals: %d\n", token_stats.character_literals);
    fprintf(fptr_output, "Single Line Comments: %d\n", token_stats.single_line);
    fprintf(fptr_output, "Multi Line Comments: %d\n", token_stats.multi_line);

    // Close files
    fclose(fptr_input);
    fclose(fptr_output);

    return 0;
}
