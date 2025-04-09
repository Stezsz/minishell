/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: strodrig <strodrig@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/09 14:54:17 by strodrig          #+#    #+#             */
/*   Updated: 2025/04/09 14:54:18 by strodrig         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	builtin_unset(char **args)
{
	t_env_elem	*current;
	t_env_elem	*prev;

	if (!args[1])
	{
		fprintf(stderr, "minishell: unset: missing argument\n");
		g_data_ptr->exit_status = 1;
		return;
	}
	current = g_data_ptr->ev->head;
	prev = NULL;
	while (current)
	{
		if (ft_strcmp(current->key, args[1]) == 0)
		{
			if (prev)
				prev->next = current->next;
			else
				g_data_ptr->ev->head = current->next;
			free(current->key);
			free(current->value);
			free(current);
			g_data_ptr->exit_status = 0;
			return;
		}
		prev = current;
		current = current->next;
	}
	g_data_ptr->exit_status = 1;
}
