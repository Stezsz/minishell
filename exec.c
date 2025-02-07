/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tborges- <tborges-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 14:48:54 by tborges-          #+#    #+#             */
/*   Updated: 2025/02/07 16:22:56 by tborges-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * Verifica se o comando passado como argumento é um PATH (se tiver uma '/' signifca que o utilizador quer
 * usar um comando numa determinada localização) ou se é um comando simples (nesse caso atribuimos o PATH
 * com o getenv e depois usamos o strtok para dividir o comando e os seus argumentos).
 *
 * Esta função não executa comandos, apenas verifica se o comando passado como argumento é um PATH ou não e
 * deixa o comando pronto para ser executado.
 */
char *find_executable(char *cmd)
{
    if (strchr(cmd, '/'))
    {
        if (access(cmd, X_OK) == 0)
            return strdup(cmd);
        return NULL;
    }

    char *path = getenv("PATH");
    if (!path)
        return NULL;

    char *path_copy = strdup(path);
    if (!path_copy)
        return NULL;

    char *token = strtok(path_copy, ":");
    while (token)
    {
        char full_path[1024];
        snprintf(full_path, sizeof(full_path), "%s/%s", token, cmd);
        if (access(full_path, X_OK) == 0)
        {
            free(path_copy);
            return strdup(full_path);
        }
        token = strtok(NULL, ":");
    }
    free(path_copy);
    return NULL;
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
void execute_command(char **args, char **envp)
{
    char *exec_path = find_executable(args[0]);
    if (!exec_path)
    {
        fprintf(stderr, "minishell: command not found: %s\n", args[0]);
        g_data_ptr->exit_status = 127;
        return;
    }

    pid_t pid = fork();
    if (pid == -1)
    {
        perror("fork");
        return;
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
            g_data_ptr->exit_status = WEXITSTATUS(status);
        else
            g_data_ptr->exit_status = 1;
    }
    free(exec_path);
}

/**
 * Executa dois comandos em sequência.
 */
void execute_piped_commands(char **cmd1, char **cmd2, char **envp)
{
    int pipe_fd[2];
    if (pipe(pipe_fd) == -1)
    {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    pid_t pid1 = fork();
    if (pid1 == 0)
    {
        dup2(pipe_fd[1], STDOUT_FILENO);
        close(pipe_fd[0]);
        close(pipe_fd[1]);
        execve(find_executable(cmd1[0]), cmd1, envp);
        perror("execve");
        exit(EXIT_FAILURE);
    }

    pid_t pid2 = fork();
    if (pid2 == 0)
    {
        dup2(pipe_fd[0], STDIN_FILENO);
        close(pipe_fd[0]);
        close(pipe_fd[1]);
        execve(find_executable(cmd2[0]), cmd2, envp);
        perror("execve");
        exit(EXIT_FAILURE);
    }

    close(pipe_fd[0]);
    close(pipe_fd[1]);
    waitpid(pid1, NULL, 0);
    waitpid(pid2, NULL, 0);
}





//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// /**
//  * main antigo
//  */
// int main(int argc, char **argv, char **envp)
// {
//     if (argc < 2)
//     {
//         fprintf(stderr, "Usage: %s <command> [args...]\n", argv[0]);
//         return 1;
//     }
//     execute_command(argv, envp);
//     return 0;
// }

// /**
//  * main só para testar
//  */
// int main(int argc, char **argv, char **envp)
// {
//     int pipe_index = -1;
//     for (int i = 1; i < argc; i++)
//     {
//         if (strcmp(argv[i], "|") == 0)
//         {
//             pipe_index = i;
//             break;
//         }
//     }

//     if (pipe_index != -1)
//     {
//         argv[pipe_index] = NULL;
//         char **cmd1 = &argv[1];
//         char **cmd2 = &argv[pipe_index + 1];
//         execute_piped_commands(cmd1, cmd2, envp);
//     }
//     else
//     {
//         execute_command(&argv[1], envp);
//     }

//     return 0;
// }



// void	minishell_loop(void)
// {
// 	char	*input;

// 	// setup_signals();
// 	while (1)
// 	{
// 		input = readline("minishell$ ");
// 		if (!input) // Ctrl+D
// 		{
// 			printf("exit\n");
// 			clear_history();
// 			break ;
// 		}
// 		if (*input)
// 			add_history(input);
// 		printf("You entered: %s\n", input); // Apenas para debug
// 		free(input);
// 	}
// }

// int	main(void)
// {
// 	minishell_loop();
// 	return (0);
// }
/*void minishell_loop(char **envp)
{
    char *input;

    while (1)
    {
        input = readline("minishell$ ");
        if (!input) // Ctrl+D
        {
            printf("exit\n");
            clear_history();
            break;
        }
        if (*input)
            add_history(input);

        char *args[] = {input, NULL};
        expand_args(args);

        if (is_builtin(args[0]))
            execute_builtin(args, envp);
        else
            execute_command(args, envp);

        free(input);
    }
}

int main(int argc, char **argv, char **envp)
{
    (void)argc;
    (void)argv;
    minishell_loop(envp);
    return 0;
}*/
