/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   program.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: strodrig <strodrig@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/04 10:50:25 by strodrig          #+#    #+#             */
/*   Updated: 2025/04/09 14:46:20 by strodrig         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Função para escrever o prompt do shell
void	write_prompt(void)
{
	char	*cwd;

	cwd = getcwd(NULL, 0); // Obter o diretório de trabalho atual
	if (!cwd)
		return ;
	write(1, CYAN, 5);
	write(1, cwd, strlen(cwd));
	write(1, MAGENTA, 5);
	write(1, "$ ", 2);
	write(1, RESET_COLOR, 4);
	free(cwd);
}

// Função para ler uma linha da entrada do usuário
int	ft_readline(char **line)
{
	write_prompt();
	*line = readline("");
	if (!*line)
	{
		write(1, "exit\n", 5);
		exit(1);
	}
	if (ft_strcmp(*line, "") == EQUAL || ft_strisspace(*line))
	{
		free(*line);
		return (1);
	}
	*line = handle_unclosed_quotes(*line);
	if (!*line) // Handle Ctrl+D during continuation
	{
		write(1, "exit\n", 5);
		exit(1);
	}
	if (ft_strlen(*line) > 0)
		add_history(*line);
	return (0);
}

// Função para definir dados globais (atualmente não faz nada)
void	set_g_data(char **env)
{
	g_data_ptr->env = env;
	//g_data_ptr.ev = create_env(env); criar create_env
	g_data_ptr->tree = init_tree(g_data_ptr->tree);
	g_data_ptr->shell_state = SH_READING;
}

// Função para executar comandos encadeados por pipes
void execute_piped(char **cmds[], int num_cmds, char **envp)
{
	int pipe_fd[2]; // Descritores de arquivo para o pipe
	int prev_fd = -1; // Descritor de arquivo do comando anterior

	for (int i = 0; i < num_cmds; i++)
	{
		if (i < num_cmds - 1)
		{
			// Criar um pipe para comunicação entre processos
			if (pipe(pipe_fd) == -1)
			{
				perror("pipe");
				exit(EXIT_FAILURE);
			}
		}

		pid_t pid = fork(); // Criar um novo processo
		if (pid == -1)
		{
			perror("fork");
			exit(EXIT_FAILURE);
		}

		if (pid == 0) // Código do processo filho
		{
			if (prev_fd != -1)
			{
				dup2(prev_fd, STDIN_FILENO); // Redirecionar entrada padrão
				close(prev_fd);
			}
			if (i < num_cmds - 1)
			{
				close(pipe_fd[0]);
				dup2(pipe_fd[1], STDOUT_FILENO); // Redirecionar saída padrão
				close(pipe_fd[1]);
			}

			if (is_builtin(cmds[i][0]))
				execute_builtin(cmds[i], envp); // Executar comando builtin
			else
				execute_command(cmds[i], envp); // Executar comando externo
			exit(g_data_ptr->exit_status);
		}
		else // Código do processo pai
		{
			if (prev_fd != -1)
				close(prev_fd);
			if (i < num_cmds - 1)
			{
				close(pipe_fd[1]);
				prev_fd = pipe_fd[0]; // Guardar descritor de leitura do pipe
			}
		}
	}

	for (int i = 0; i < num_cmds; i++)
		wait(NULL); // Esperar todos os processos filhos terminarem
}

// Função principal para lançar o shell
void launch_shell(char **env)
{
	char *line;
	line = NULL;

	while (1)
	{
		set_g_data(env); // Definir dados globais
		if (ft_readline(&line))
			continue;

		char *cmds[1024];
		int num_cmds = 0;
		char *token = strtok(line, "|"); // Dividir a linha em comandos separados por '|'
		while (token)
		{
			cmds[num_cmds++] = token;
			token = strtok(NULL, "|");
		}

		char **args[num_cmds];
		for (int i = 0; i < num_cmds; i++)
		{
			args[i] = malloc(sizeof(char *) * 1024);
			int j = 0;
			token = strtok(cmds[i], " "); // Dividir cada comando em argumentos
			while (token)
			{
				args[i][j++] = token;
				token = strtok(NULL, " ");
			}
			args[i][j] = NULL;
			expand_args(args[i]); // Expandir variáveis de ambiente nos argumentos
		}
		if (num_cmds == 1 && args[0][0] && is_builtin(args[0][0]))
			execute_builtin(args[0], env);
		else
			execute_piped(args, num_cmds, env);
		for (int i = 0; i < num_cmds; i++)
			free(args[i]); // Liberar memória alocada para os argumentos

		free(line); // Liberar memória alocada para a linha de comando
	}
}


