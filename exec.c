/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tborges- <tborges-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 14:48:54 by tborges-          #+#    #+#             */
/*   Updated: 2025/02/04 19:41:56 by tborges-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

/**
 * Verifica se o comando passado como argumento é um PATH (se tiver uma '/' signifca que o utilizador quer
 * usar um comando numa determinada localização) ou se é um comando simples (nesse caso atribuimos o PATH
 * com o getenv e depois usamos o strtok para dividir o comando e os seus argumentos).
 *
 * Esta função não executa comandos, apenas verifica se o comando passado como argumento é um PATH ou não e
 * deixa o comando pronto para ser executado.
 */
char	*find_executable(char *cmd)
{
	char	*path;
	char	*path_copy;
	char	*token;
	char	full_path[1024];

	if (strchr(cmd, '/'))
	{
		if (access(cmd, X_OK) == 0)
			return strdup(cmd);
		return (NULL);
	}
	path = getenv("PATH");
	if (!path)
		return (NULL);
	path_copy = strdup(path);
	if (!path_copy)
		return (NULL);
	token = strtok(path_copy, ":");
	while (token)
	{
		snprintf(full_path, sizeof(full_path), "%s/%s", token, cmd);
		if (access(full_path, X_OK) == 0)
		{
			free(path_copy);
			return (strdup(full_path));
		}
		token = strtok(NULL, ":");
	}
	free(path_copy);
	return (NULL);
}

/**
 * Executa o comando passado como argumento.
 *
 * Cria um processo filho com o fork e executa o comando no processo filho.
 */
void	execute_command(char *cmd, char **args, char **envp)
{
	char	*exec_path;
	pid_t	pid;

	exec_path = find_executable(cmd);
	if (!exec_path)
	{
		fprintf(stderr, "minishell: command not found: %s\n", cmd);
		return ;
	}
	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		return ;
	}
	if (pid == 0)
	{
		if (execve(exec_path, args, envp) == -1)
		{
			perror("execve");
			exit(EXIT_FAILURE);
		}
	}
	else
		waitpid(pid, NULL, 0);
	free(exec_path);
}



/**
 * main só para testar
 */
int main(int argc, char **argv, char **envp)
{
    if (argc < 2)
    {
        fprintf(stderr, "Usage: %s <command> [args...]\n", argv[0]);
        return 1;
    }
    execute_command(argv[1], &argv[1], envp);
    return 0;
}
