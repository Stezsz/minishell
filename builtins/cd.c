/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: strodrig <strodrig@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/09 14:53:57 by strodrig          #+#    #+#             */
/*   Updated: 2025/04/09 15:17:52 by strodrig         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	builtin_cd(char **args)
{
	if (!args[1])
	{
		fprintf(stderr, "minishell: cd: missing argument\n");  //cd sem argumento, ir home
		g_data_ptr->exit_status = 1;
		return;
	}
	if (chdir(args[1]) != 0)
	{
		perror("minishell: cd");
		g_data_ptr->exit_status = 1;
	}
	else
		g_data_ptr->exit_status = 0;
}
