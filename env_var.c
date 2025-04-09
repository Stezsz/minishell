/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_var.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: strodrig <strodrig@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/06 19:37:14 by tborges-          #+#    #+#             */
/*   Updated: 2025/04/09 14:48:50 by strodrig         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char *expand_variable(char *arg)
{
    if (strcmp(arg, "$?") == 0)
    {
        char *status = malloc(12);
        if (status)
            snprintf(status, 12, "%d", g_data_ptr->exit_status);
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



/* cd home
cd "32 32"

echo -n -n -nnnnnn
echo -nnnnnnn

export
unset

exit code

exit dsadas
numeric argument required

<<a << a */
