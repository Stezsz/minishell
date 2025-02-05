/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tborges- <tborges-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 14:48:54 by tborges-          #+#    #+#             */
/*   Updated: 2025/02/05 17:12:07 by tborges-         ###   ########.fr       */
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
 * trata dos redirecionamentos de output (> e >>)
 * e de input (< e <<).
 */
void handle_redirections(char **args)
{
    for (int i = 0; args[i]; i++)
    {
        if (strcmp(args[i], ">") == 0)
        {
            int fd = open(args[i + 1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
            if (fd == -1)
            {
                perror("open");
                exit(EXIT_FAILURE);
            }
            dup2(fd, STDOUT_FILENO);
            close(fd);
            args[i] = NULL;
        }
        else if (strcmp(args[i], ">>") == 0)
        {
            int fd = open(args[i + 1], O_WRONLY | O_CREAT | O_APPEND, 0644);
            if (fd == -1)
            {
                perror("open");
                exit(EXIT_FAILURE);
            }
            dup2(fd, STDOUT_FILENO);
            close(fd);
            args[i] = NULL;
        }
		else if (strcmp(args[i], "<") == 0)
        {
            int fd = open(args[i + 1], O_RDONLY);
            if (fd == -1)
            {
                perror("open");
                exit(EXIT_FAILURE);
            }
            dup2(fd, STDIN_FILENO);
            close(fd);
            args[i] = NULL;
        }
		else if (strcmp(args[i], "<<") == 0)
        {
            int pipe_fd[2];
            if (pipe(pipe_fd) == -1)
            {
                perror("pipe");
                exit(EXIT_FAILURE);
            }
            char *delimiter = args[i + 1];
            char buffer[1024];
            while (1)
            {
                printf("heredoc> ");
                fflush(stdout);
                if (!fgets(buffer, sizeof(buffer), stdin))
                    break;
                if (strncmp(buffer, delimiter, strlen(delimiter)) == 0 && buffer[strlen(delimiter)] == '\n')
                    break;
                write(pipe_fd[1], buffer, strlen(buffer));
            }
            close(pipe_fd[1]);
            dup2(pipe_fd[0], STDIN_FILENO);
            close(pipe_fd[0]);
            args[i] = NULL;
        }
    }
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
		handle_redirections(args);
		if (execve(exec_path, args, envp) == -1)
		{
			perror("execve");
			exit(EXIT_FAILURE);
		}
	}
	else
	{
		int status;
        waitpid(pid, &status, 0);
        if (WIFEXITED(status))
            printf("Process exited with status %d\n", WEXITSTATUS(status));
	}
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
