/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tborges- <tborges-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/06 18:29:34 by tborges-          #+#    #+#             */
/*   Updated: 2025/02/07 13:12:29 by tborges-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <readline/readline.h>
#include <readline/history.h>

// exec.c
char *find_executable(char *cmd);
void handle_redirections(char **args);
void execute_command(char **args, char **envp);
void execute_piped_commands(char **cmd1, char **cmd2, char **envp);

//builtins
void builtin_echo(char **args);
void builtin_cd(char **args);
void builtin_pwd();
void builtin_export(char **args);
void builtin_unset(char **args);
void builtin_env(char **envp);
void builtin_exit();

int is_builtin(char *cmd);
void execute_builtin(char **args, char **envp);


// environment variables
int g_exit_status = 0;
char *expand_variable(char *arg);
void expand_args(char **args);
