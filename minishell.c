/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: strodrig <strodrig@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 12:20:04 by strodrig          #+#    #+#             */
/*   Updated: 2025/01/27 12:20:04 by strodrig         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	main(int ac, char **av, char **env) //env recupera as variaveis de ambiente
{
	char	*input;

	(void)ac;
	(void)av;
	while (1)
	{
		// Exibe o prompt e aguarda a entrada do usuário
		display_prompt();
		input = readline("minishell> ");
		// Se o usuário pressionar Ctrl+D, saia do loop
		if (!input)
		{
			printf("exit\n");
			break ;
		}
		// Se a entrada não for vazia, execute o comando
		if (*input)
		{
			add_history(input); // Adiciona a entrada ao histórico
			execute_command(input, env);
		}
		// Libera a memória alocada para a entrada
		free(input);
	}
}
