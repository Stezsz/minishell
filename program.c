/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   program.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: strodrig <strodrig@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/04 10:50:25 by strodrig          #+#    #+#             */
/*   Updated: 2025/02/04 10:50:25 by strodrig         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Conta o número de comandos na árvore de sintaxe abstrata (AST)
int	cmd_count(t_ast_node *ptr)
{
	static int	count;

	if (ptr->type == CMD)
		count++;
	else
	{
		cmd_count(ptr->content->pipe->right);
		cmd_count(ptr->content->pipe->left);
	}
	return (count);
}

// Espera pelos processos filhos
void	ft_wait(void)
{
	int	status;

	status = 0;
	while (1)
	{
		g_data_ptr->pid = waitpid(-1, &status, 0);
		if (g_data_ptr->pid == -1 && errno == EINTR)
			continue ;
		if (g_data_ptr->pid == -1)
			break ;
	}
	if (WIFSIGNALED(status))
	{
		g_data_ptr->exit_status = WTERMSIG(status) + 128;
		if (WTERMSIG(status) == SIGINT)
			write(STDOUT_FILENO, "\n", 1);
	}
	if (WIFSIGNALED(status) && WTERMSIG(status) == SIGQUIT)
		printf("%s: %i\n", g_data_ptr->siglist[status], status);
	g_data_ptr->which = 0;
}

// Configura os dados globais
void	set_g_data(char **env)
{
	g_data_ptr->env = env;
	g_data_ptr->ev = create_env(env);
	g_data_ptr->tree = init_tree(g_data_ptr->tree);
	g_data_ptr->shell_state = SH_READING;
}

// Lê a linha de comando
int	ft_readline(char **line)
{
	*line = readline("[minishell][:)]~> ");
	g_data_ptr->shell_state = SH_EXECUTING;
	if (!*line)
	{
		printf("exit\n");
		exit(1);
	}
	if (strcmp(*line, "") == EQUAL || ft_strisspace(*line))
		return (1);
	if (strlen(*line) > 0)
		add_history(*line);
	return (0);
}

// Inicializa o MiniShell
void	lunch_shell(char **env)
{
	char	*line;

	line = NULL;
	while (1)
	{
		set_g_data(env);
		if (ft_readline(&line))
			continue ;
		g_data_ptr->lex = lexer(line);
		if (!syntax_errors(g_data_ptr->lex))
		{
			if (parse(&g_data_ptr->tree, g_data_ptr->lex->head, g_data_ptr->ev)
				== EXIT_FAILURE)
				continue ;
			execute(g_data_ptr->tree->root, &env, cmd_count(g_data_ptr->tree->root),
				g_data_ptr->lex->head);
			free_lexer_parser(g_data_ptr->lex, g_data_ptr->tree);
			ft_wait();
		}
		else
			free_lexer(g_data_ptr->lex);
		del_env(g_data_ptr->ev);
	}
}
