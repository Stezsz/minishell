/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: strodrig <strodrig@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/06 18:20:44 by tborges-          #+#    #+#             */
/*   Updated: 2025/04/09 15:13:42 by strodrig         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * Verifica se o comando passado é um builtin.
 */
int is_builtin(char *cmd)
{
    return (ft_strcmp(cmd, "echo") == 0 || ft_strcmp(cmd, "cd") == 0 ||
            ft_strcmp(cmd, "pwd") == 0 || ft_strcmp(cmd, "export") == 0 ||
            ft_strcmp(cmd, "unset") == 0 || ft_strcmp(cmd, "env") == 0 ||
            ft_strcmp(cmd, "exit") == 0);
}

/**
 * Executa o builtin passado como argumento.
 */
void execute_builtin(char **args, char **envp)
{
    if (ft_strcmp(args[0], "echo") == 0)
        builtin_echo(args);
    else if (ft_strcmp(args[0], "cd") == 0)
        builtin_cd(args);
    else if (ft_strcmp(args[0], "pwd") == 0)
        builtin_pwd();
    else if (ft_strcmp(args[0], "export") == 0)
        builtin_export(args);
    else if (ft_strcmp(args[0], "unset") == 0)
        builtin_unset(args);
    else if (ft_strcmp(args[0], "env") == 0)
        builtin_env(envp);
    else if (ft_strcmp(args[0], "exit") == 0)
        builtin_exit(args);
}

