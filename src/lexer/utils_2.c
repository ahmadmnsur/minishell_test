#include "../../minishell.h"

char *remove_escaped_quotes(const char *str)
{
    int i;
    int j;

    i = 0;
    j = 0;
    // Allocate enough space (the result will never be longer than the original)
    char *result = malloc(ft_strlen(str) + 1);
    if (!result)
        return NULL;
    while (str[i])
    {
        if (str[i] == '\\' && str[i + 1] == '"')
        {
            result[j++] = '"';
            i += 2;
        }
        else
        {
            result[j++] = str[i++];
        }
    }
    result[j] = '\0';
    return (result);
}