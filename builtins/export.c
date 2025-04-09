/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: strodrig <strodrig@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/09 14:54:12 by strodrig          #+#    #+#             */
/*   Updated: 2025/04/09 14:54:12 by strodrig         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	builtin_export(char **args)
{
	char	*key;
	char	*value;

	if (!args[1])
	{
		fprintf(stderr, "minishell: export: missing argument\n");
		g_data_ptr->exit_status = 1;
		return;
	}
	key = strtok(args[1], "=");
	value = strtok(NULL, "");
	if (!value)
	{
		fprintf(stderr, "minishell: export: invalid format\n");
		g_data_ptr->exit_status = 1;
		return;
	}
	// Adicionar ou atualizar a variável de ambiente manualmente
	t_env_elem *new_elem = malloc(sizeof(t_env_elem));
	new_elem->key = strdup(key);
	new_elem->value = strdup(value);
	new_elem->next = g_data_ptr->ev->head;
	g_data_ptr->ev->head = new_elem;
	g_data_ptr->exit_status = 0;
}
