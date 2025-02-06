/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_var.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tborges- <tborges-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/06 19:37:14 by tborges-          #+#    #+#             */
/*   Updated: 2025/02/06 19:38:14 by tborges-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

char *expand_variable(char *arg)
{
    if (arg[0] != '$')
        return strdup(arg);

    char *value = getenv(arg + 1);
    if (!value)
        return strdup("");
    return strdup(value);
}

void expand_args(char **args)
{
    for (int i = 0; args[i]; i++)
    {
        if (args[i][0] == '$')
        {
            char *expanded = expand_variable(args[i]);
            free(args[i]);
            args[i] = expanded;
        }
    }
}
