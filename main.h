/***********************************************************************
 *  File name   : main.h
 *  Description : Header file for the Lexical Analyzer Project.
 *                Declares data structures and function prototypes
 *                required for lexical tokenization and statistics tracking.
 *
 *                Responsibilities:
 *                - Define TokenStats structure for counting token categories
 *                - Declare tokenize_line() for per-line lexical analysis
 *
 *                Structures:
 *                - TokenStats
 *
 *                Functions:
 *                - tokenize_line()
 * 
 ***********************************************************************/

#ifndef MAIN_H
#define MAIN_H

#include <stdio.h>
#include <string.h>
#include <ctype.h>

/*----------------------------------------------------------------------
*  Structure: TokenStats
*  Description:
*      Holds counters for each token category identified
*      during lexical analysis of the input source code.
*--------------------------------------------------------------------*/
typedef struct
{
int keyword;
int identifiers;
int operators;
int integers;
int real_numbers;
int string_literals;
int character_literals;
int delimiters;
int preprocessor;
int hex_octal;
int single_line;
int multi_line;
} TokenStats;

/*----------------------------------------------------------------------
*  Function Prototypes
*--------------------------------------------------------------------*/

/* Tokenize one line of source code and update statistics */
void tokenize_line(char *line, FILE *fptr_output, TokenStats *token_stats);

#endif
