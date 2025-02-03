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

void	init_data(t_data *data)
{
	data->exit_status = 0;
	data->which = 0;
	data->pid = 0;
	data-> tree = NULL;
	data->ev = NULL;
	data->alloc = 0;
}

int	main(int ac, char **av, char **env)
{
	(void)av;
	t_data data;

	if (ac != 1 || !*env)
		return (1);
	init_data(&data); // Inicializa a estrutura de dados para o MiniShell
	init_signals(&data); // Inicializa os sinais
	return (0);
}
