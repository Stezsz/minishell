/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tborges- <tborges-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/03 17:24:29 by tborges-          #+#    #+#             */
/*   Updated: 2025/02/03 18:01:27 by tborges-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <readline/history.h>
#include <readline/readline.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>

void	handle_signal(int sig)
{
	if (sig == SIGINT)
	{
		printf("\n");
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
}

void	setup_signals(void)
{
	signal(SIGINT, handle_signal);
	signal(SIGQUIT, SIG_IGN);
}

void	minishell_loop(void)
{
	char	*input;

	setup_signals();
	while (1)
	{
		input = readline("minishell$ ");
		if (!input) // Ctrl+D
		{
			printf("exit\n");
			rl_clear_history();
			break ;
		}
		if (*input)
			add_history(input);
		printf("You entered: %s\n", input); // Apenas para debug
		free(input);
	}
}

int	main(void)
{
	minishell_loop();
	return (0);
}
