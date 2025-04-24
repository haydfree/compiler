#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void
lexical_analysis(char *statement)
{
    char *ptr, **tokens;
    size_t i,j;
    tokens = malloc(255);
    if (!tokens) 
    { 
        fprintf(stderr, "%s:%d ERROR: malloc failed\n", __FILE__, __LINE__);
        exit(1);
    }
    for (ptr=strtok(statement, " "),i=0; ptr != NULL; ptr=strtok(NULL, " "),i++)
    {
        tokens[i] = strdup(ptr);
        printf("%s\n", tokens[i]);
    }
    for (j = 0; j < i; j++)
    {
        free(tokens[j]);
    }
    free(tokens);
}

int
main(void)
{
    lexical_analysis("hello buddy guy"); 
    return 0;
}
