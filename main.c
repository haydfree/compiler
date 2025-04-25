#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define LEXEME_STR_SIZE 64

typedef struct Lexeme
{
    char *str;
    size_t str_len, start_idx, end_idx, line, column;
    struct Lexeme *last, *next;
} Lexeme;

Lexeme*
init_lexeme(void)
{
    Lexeme *lexeme;
    lexeme = malloc(sizeof(Lexeme));
    if (!lexeme)
    {
        printf("error\n");
        exit(1);
    }

    lexeme->str = malloc(LEXEME_STR_SIZE * sizeof(char));
    if (!lexeme->str)
    {
        printf("error\n");
        exit(1);
    }

    memset(lexeme->str, 0, LEXEME_STR_SIZE);
    lexeme->str_len = 0;

    return lexeme;
}

void
append_lexeme_char(Lexeme *lexeme, char c)
{
    lexeme->str[lexeme->str_len] = c;
    lexeme->str_len++;
}

Lexeme**
lexemize(char *input)
{
    char *ptr;
    Lexeme *lexeme, **arr;
    size_t num_lexemes, line_num, column_num;

    arr = malloc(sizeof(Lexeme*) * 256);
    if (!arr)
    {
        printf("error\n");
        exit(1);
    }

    num_lexemes = 0;
    line_num = 1;
    column_num = 1;
    lexeme = init_lexeme();
    for (ptr = input; *ptr != '\0'; ptr++)
    {
        if (*ptr == '\n')
        {
            line_num++;
            column_num = 1;
        } else if (*ptr == ' ' || *ptr == '\t' || *ptr == '\r')
        {
            lexeme->line = line_num;
            lexeme->column = column_num - strlen(lexeme->str);
            arr[num_lexemes] = lexeme;
            num_lexemes++;
            lexeme = init_lexeme();
            continue;
        }

        append_lexeme_char(lexeme, *ptr);
        column_num++;
    }
    
    arr[num_lexemes] = lexeme;
    return arr;
}

int
main(void)
{
    Lexeme **arr;
    size_t i;

    arr = lexemize("int main(void) { return 0; }");
    for (i = 0; i < 6; i++)
    {
        printf("%s\n", arr[i]->str);
    }
    return 0;
}
