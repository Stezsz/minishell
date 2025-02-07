/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_var.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tborges- <tborges-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/06 19:37:14 by tborges-          #+#    #+#             */
/*   Updated: 2025/02/07 13:10:56 by tborges-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

char *expand_variable(char *arg)
{
    if (strcmp(arg, "$?") == 0)
    {
        char *status = malloc(12);
        if (status)
            snprintf(status, 12, "%d", g_exit_status);
        return status;
    }
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
