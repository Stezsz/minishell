/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: strodrig <strodrig@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/05 10:41:43 by strodrig          #+#    #+#             */
/*   Updated: 2025/04/09 14:48:30 by strodrig         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Compare two strings
int	ft_strcmp(const char *s1, const char *s2)
{
    while (*s1 && *s2 && *s1 == *s2)
    {
        s1++;
        s2++;
    }
    return (*(unsigned char *)s1 - *(unsigned char *)s2);
}

// Get the length of a string
int	ft_strlen(const char *s)
{
    int	i = 0;

    while (s[i])
        i++;
    return (i);
}

// Check if a string contains only spaces
int	ft_strisspace(char *str)
{
    int	i = 0;

    while (str[i])
    {
        if (str[i] != ' ')
            return (0);
        i++;
    }
    return (1);
}

// Check if the input string has unclosed quotes
int	has_unclosed_quotes(const char *input)
{
    int	single_quote = 0;
    int	double_quote = 0;

    for (int i = 0; input[i]; i++)
    {
        if (input[i] == '\'' && double_quote == 0)
            single_quote = !single_quote;
        else if (input[i] == '\"' && single_quote == 0)
            double_quote = !double_quote;
    }
    return (single_quote || double_quote);
}

// Prompt for additional input if quotes are unclosed
char	*handle_unclosed_quotes(char *input)
{
    char	*line = NULL;
    char	*temp = NULL;

    while (has_unclosed_quotes(input))
    {
        write(1, "quote> ", 7);
        line = readline("");
        if (!line) // Handle Ctrl+D
        {
            free(input);
            return (NULL);
        }
        temp = input;
        input = malloc(strlen(temp) + strlen(line) + 2);
        if (!input)
            return (NULL);
        sprintf(input, "%s\n%s", temp, line);
        free(temp);
        free(line);
    }
    return (input);
}
