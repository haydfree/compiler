#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define LEXEME_STR_SIZE 64
#define NUM_LEXEMES 14

typedef struct Lexeme
{
    char *str;
    size_t str_len, line, col;
    struct Lexeme *prev, *next;
} Lexeme;

char *lexemes[32] = {
    "i8", "i16", "i32", "i64",
    "u8", "u16", "u32", "u64",
    "ptr",
    "(", ")", "{", "}", ";", "=",
};

Lexeme*
lexeme_new(void)
{
    Lexeme *lexeme;
    lexeme = malloc(sizeof(Lexeme));
    if (!lexeme)
    {
        printf("error\n");
        exit(1);
    }

    lexeme->str = malloc(LEXEME_STR_SIZE);
    if (!lexeme->str)
    {
        printf("error\n");
        exit(1);
    }

    memset(lexeme->str, 0, LEXEME_STR_SIZE);
    lexeme->str_len = 0;

    return lexeme;
}

int
lexeme_is_match(char *str)
{
    size_t i;
    for (i = 0; i < NUM_LEXEMES; i++)
    {
        if (strcmp(str, lexemes[i]) == 0)
        {
            return 1;
        }
    }

    return 0;
}

void
lexeme_str_append(Lexeme *lexeme, char c)
{
    if (lexeme->str_len < LEXEME_STR_SIZE- 1)
    {
        lexeme->str[lexeme->str_len++] = c;
    }
}

void
lexeme_push(Lexeme *lexeme, Lexeme **arr, size_t *count, size_t line, size_t col)
{
    lexeme->str[lexeme->str_len] = '\0';
    lexeme->line = line;
    lexeme->col = col;
    if (*count > 0)
    {
        arr[*count-1]->next = lexeme;
        lexeme->prev = arr[*count-1];
    }
    arr[*count] = lexeme;
    (*count)++;
}

Lexeme**
lexeme_lexemize(char *input)
{
    char *ptr;
    Lexeme *lexeme, **arr;
    size_t count, line, col; 

    arr = malloc(sizeof(Lexeme*) * 256);
    if (!arr)
    {
        printf("error\n");
        exit(1);
    }

    count = 0, line = 1, col = 1;
    lexeme = lexeme_new();
    for (ptr = input; *ptr; ptr++, col++)
    {
        char c = *ptr;
        if (c == '\n')
        {
            line++;
            col = 1;
            continue;
        } else if (strchr(" \t\r", c))
        {
            continue;
        } else if (strchr("(){};=", c))
        {
            lexeme_str_append(lexeme, c);
            lexeme_push(lexeme, arr, &count, line, col);
            lexeme = lexeme_new();
            continue;
        }

        lexeme_str_append(lexeme, c);
        if (lexeme_is_match(lexeme->str))
        {
            lexeme_push(lexeme, arr, &count, line, col);
            lexeme = lexeme_new();
        }
    }
    
    arr[count] = lexeme;
    return arr;
}

int
main(void)
{
    Lexeme **arr;
    size_t i;

    arr = lexeme_lexemize(" i8 num = 0; ");
    for (i = 0; i < 6; i++)
    {
        printf("%s\n", arr[i]->str);
    }
    return 0;
}
