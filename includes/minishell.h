/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: strodrig <strodrig@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 12:20:02 by strodrig          #+#    #+#             */
/*   Updated: 2025/01/27 12:20:02 by strodrig         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <limits.h>
# include <string.h>
# include <sys/wait.h>
# include <readline/readline.h>
# include <readline/history.h>

typedef struct s_cmd
{
	char	**args;
	char	*input_file;
	char	*output_file;
	int		is_append;
	struct s_cmd	*next;
}	t_cmd;

/* Exibe o prompt */
void	display_prompt(void);

/* Encontra o caminho do executável */
char	*find_executable_path(char *command, char **env);

/* Executa um comando */
void	execute_command(char *input, char **env);

/* Trata comandos built-in */
int	handle_builtin_commands(char **args, char **env);

/* Função auxiliar para dividir strings (implemente ou inclua libft) */
char	**ft_split(char const *s, char c);

#endif
