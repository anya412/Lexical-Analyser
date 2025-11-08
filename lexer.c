/***********************************************************************
 *  File name   : lexer.c
 *  Description : Implements lexical token classification and line tokenization
 *                logic for the Lexical Analyzer Project. This module identifies
 *                tokens such as keywords, identifiers, literals, operators,
 *                and comments, updating token statistics accordingly.
 *
 *                Responsibilities:
 *                - Validate and classify tokens
 *                - Handle multi-line and single-line comments
 *                - Track strings, character literals, and preprocessor directives
 *
 *                Functions:
 *                - is_integer()
 *                - is_delim()
 *                - is_hex_octal()
 *                - is_keyword()
 *                - is_operator()
 *                - is_real_number()
 *                - is_pre_processor()
 *                - is_identifier()
 *                - tokenize_line()
 * 
 ***********************************************************************/

#include "main.h"

/*----------------------------------------------------------------------
 *  Function Declarations
 *--------------------------------------------------------------------*/
int is_integer(char *);
int is_delim(char *);
int is_hex_octal(char *);
int is_keyword(char *);
int is_operator(char *);
int is_real_number(char *);
int is_pre_processor(char *);
int is_identifier(char *);

/*----------------------------------------------------------------------
 *  Check if token is an integer constant
 *--------------------------------------------------------------------*/
int is_integer(char *str)
{
    int i = 0;

    if (str == NULL || str[0] == '\0')
        return 0;

    if (str[i] == '+' || str[i] == '-')
        i++;

    if (str[i] == '\0')
        return 0;

    while (str[i] != '\0')
    {
        if (!isdigit(str[i]))
            return 0;
        i++;
    }
    return 1;
}

/*----------------------------------------------------------------------
 *  Check if token is a delimiter
 *--------------------------------------------------------------------*/
int is_delim(char *str)
{
    const char *delim[] = {",", ";", "(", ")", "[", "]", "{", "}", ":"};

    for (int i = 0; i < 9; i++)
    {
        if (strcmp(delim[i], str) == 0)
            return 1;
    }
    return 0;
}

/*----------------------------------------------------------------------
 *  Check if token is a hexadecimal or octal constant
 *--------------------------------------------------------------------*/
int is_hex_octal(char *str)
{
    if (str == NULL || str[0] == '\0')
        return 0;

    if (str[0] == '0' && (str[1] == 'x' || str[1] == 'X'))
    {
        for (int i = 2; str[i] != '\0'; i++)
        {
            if (!isxdigit(str[i]))
                return 0;
        }
        return 1;
    }

    if (str[0] == '0')
    {
        for (int i = 1; str[i] != '\0'; i++)
        {
            if (str[i] < '0' || str[i] > '7')
                return 0;
        }
        return 1;
    }

    return 0;
}

/*----------------------------------------------------------------------
 *  Check if token is a C keyword
 *--------------------------------------------------------------------*/
int is_keyword(char *str)
{
    const char *keywords[] = {
        "char", "int", "float", "double", "signed", "unsigned", "short",
        "long", "const", "volatile", "for", "while", "do", "goto", "break",
        "continue", "if", "else", "switch", "case", "default", "auto",
        "register", "static", "extern", "struct", "union", "enum",
        "typedef", "void", "return", "sizeof"};

    for (int i = 0; i < (int)(sizeof(keywords) / sizeof(keywords[0])); i++)
    {
        if (strcmp(str, keywords[i]) == 0)
            return 1;
    }
    return 0;
}

/*----------------------------------------------------------------------
 *  Check if token is an operator
 *--------------------------------------------------------------------*/
int is_operator(char *str)
{
    const char *operators[] = {
        "+", "-", "*", "/", "=", "==", "!=", "<", ">", "<=", ">=", "++",
        "--", "&&", "||", "&", "|", "^", "~", "!", "%", "<<", ">>",
        "+=", "-=", ".", "->"};

    int len = sizeof(operators) / sizeof(operators[0]);
    for (int i = 0; i < len; i++)
    {
        if (strcmp(str, operators[i]) == 0)
            return 1;
    }
    return 0;
}

/*----------------------------------------------------------------------
 *  Check if token is a real number constant
 *--------------------------------------------------------------------*/
int is_real_number(char *str)
{
    int i = 0, flag = 0, digit_count = 0;

    if (str[0] == '-' || str[0] == '+')
        i++;

    while (str[i] != '\0')
    {
        if (str[i] == '.')
        {
            flag++;
            if (flag > 1)
                return 0;
        }
        else if (isdigit(str[i]))
            digit_count++;
        else
            return 0;

        i++;
    }

    return (flag == 1 && digit_count > 0);
}

/*----------------------------------------------------------------------
 *  Check if token is a preprocessor directive
 *--------------------------------------------------------------------*/
int is_pre_processor(char *str)
{
    const char *preprocessors[] = {
        "#include", "#define", "#undef", "#ifdef", "#ifndef",
        "#endif", "#if", "#else", "#elif", "#error", "#pragma", "#line"};

    int len = sizeof(preprocessors) / sizeof(preprocessors[0]);

    for (int i = 0; i < len; i++)
    {
        if (strstr(str, preprocessors[i]) != NULL)
            return 1;
    }
    return 0;
}

/*----------------------------------------------------------------------
 *  Check if token is a valid identifier
 *--------------------------------------------------------------------*/
int is_identifier(char *str)
{
    if (!isalpha(str[0]) && str[0] != '_')
        return 0;

    for (int i = 1; str[i] != '\0'; i++)
    {
        if (!isalnum(str[i]) && str[i] != '_')
            return 0;
    }

    return 1;
}

/*----------------------------------------------------------------------
 *  Tokenize and classify tokens within a line of source code
 *--------------------------------------------------------------------*/
void tokenize_line(char *line, FILE *fptr_output, TokenStats *token_stats)
{
    int i = 0;
    char token[1000];
    static char buffer_multi[200];
    static int m_index = 0, s_index = 0;
    static int multi_flag = 0, string_flag = 0;

    while (line[i] != '\0')
    {
        if (isspace(line[i]))
        {
            i++;
            continue;
        }

        char delim_buf[2] = {line[i], '\0'};
        if (is_delim(delim_buf))
        {
            fprintf(fptr_output, "%c is a delimiter\n", line[i]);
            token_stats->delimiters++;
            i++;
            continue;
        }

        if (line[i] == '#')
        {
            int j = 0;
            token[j++] = line[i++];
            while (line[i] != '\0' && j < sizeof(token) - 1)
                token[j++] = line[i++];
            token[j] = '\0';

            if (is_pre_processor(token))
            {
                fprintf(fptr_output, "%s is a preprocessor directive\n", token);
                token_stats->preprocessor++;
            }
            continue;
        }

        if (line[i] == '"' || string_flag == 1)
        {
            buffer_multi[s_index++] = line[i++];
            while (line[i] != '"' && line[i] != '\0')
                buffer_multi[s_index++] = line[i++];

            if (line[i] == '\0')
            {
                string_flag = 1;
                return;
            }
            buffer_multi[s_index++] = line[i++];
            buffer_multi[s_index] = '\0';
            fprintf(fptr_output, "%s is a string literal\n", buffer_multi);
            s_index = 0;
            string_flag = 0;
            token_stats->string_literals++;
            continue;
        }

        if ((line[i] == '/' && line[i + 1] == '*') || multi_flag == 1)
        {
            while (line[i] != '\0' && !(line[i] == '*' && line[i + 1] == '/'))
                buffer_multi[m_index++] = line[i++];

            if (line[i] == '\0')
            {
                multi_flag = 1;
                return;
            }

            buffer_multi[m_index++] = line[i++];
            buffer_multi[m_index++] = line[i++];
            buffer_multi[m_index] = '\0';
            fprintf(fptr_output, "%s is a multi-line comment\n", buffer_multi);
            token_stats->multi_line++;
            m_index = 0;
            multi_flag = 0;
            continue;
        }

        if (line[i] == '/' && line[i + 1] == '/')
        {
            int j = 0;
            while (line[i] != '\0')
                token[j++] = line[i++];
            token[j] = '\0';
            fprintf(fptr_output, "%s is a single line comment\n", token);
            token_stats->single_line++;
            return;
        }

        if (isalpha(line[i]) || line[i] == '_')
        {
            int j = 0;
            while ((isalnum(line[i]) || line[i] == '_') && j < sizeof(token) - 1)
                token[j++] = line[i++];
            token[j] = '\0';

            if (is_keyword(token))
            {
                fprintf(fptr_output, "%s is a keyword\n", token);
                token_stats->keyword++;
            }
            else if (is_identifier(token))
            {
                fprintf(fptr_output, "%s is an identifier\n", token);
                token_stats->identifiers++;
            }
            continue;
        }

        if (isdigit(line[i]) || (line[i] == '+' || line[i] == '-'))
        {
            int j = 0;
            token[j++] = line[i++];
            while (line[i] != '\0' && j < sizeof(token) - 1 &&
                   (isalnum(line[i]) || line[i] == '.' || line[i] == 'x' || line[i] == 'X'))
                token[j++] = line[i++];

            token[j] = '\0';

            if (is_real_number(token))
            {
                fprintf(fptr_output, "%s is a real number\n", token);
                token_stats->real_numbers++;
            }
            else if (is_integer(token))
            {
                fprintf(fptr_output, "%s is an integer\n", token);
                token_stats->integers++;
            }
            else if (is_hex_octal(token))
            {
                fprintf(fptr_output, "%s is a hex or octal number\n", token);
                token_stats->hex_octal++;
            }
            continue;
        }

        if (line[i] == '\'')
        {
            int j = 0;
            token[j++] = line[i++];

            if (line[i] == '\\')
            {
                token[j++] = line[i++];
                if (line[i] != '\0')
                    token[j++] = line[i++];
            }
            else if (line[i] != '\0')
                token[j++] = line[i++];

            if (line[i] == '\'')
                token[j++] = line[i++];

            token[j] = '\0';
            fprintf(fptr_output, "%s is a character literal\n", token);
            token_stats->character_literals++;
            continue;
        }

        if (strchr("=<>!&|+-*/%.~->^", line[i]))
        {
            char op[3] = {line[i], line[i + 1], '\0'};

            if (is_operator(op))
            {
                fprintf(fptr_output, "%s is an operator\n", op);
                token_stats->operators++;
                i += 2;
                continue;
            }
            else
            {
                op[1] = '\0';
                if (is_operator(op))
                {
                    fprintf(fptr_output, "%s is an operator\n", op);
                    token_stats->operators++;
                    i++;
                    continue;
                }
            }
        }

        fprintf(fptr_output, "%c is unknown\n", line[i]);
        i++;
    }
}
