/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: strodrig <strodrig@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/09 14:54:09 by strodrig          #+#    #+#             */
/*   Updated: 2025/04/09 15:12:21 by strodrig         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	builtin_exit(char **args)
{
	int	exit_code;

	printf("exit\n");
	if (args[1])
	{
		exit_code = atoi(args[1]);
		if (exit_code == 0 && ft_strcmp(args[1], "0") != 0)
		{
			fprintf(stderr, "minishell: exit: %s: numeric argument required\n", args[1]);
			exit(255);
		}
		exit(exit_code);
	}
	exit(g_data_ptr->exit_status);
}
