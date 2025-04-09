/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: strodrig <strodrig@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/09 14:54:07 by strodrig          #+#    #+#             */
/*   Updated: 2025/04/09 15:24:55 by strodrig         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	builtin_env(char **envp)
{
	for (int i = 0; envp[i]; i++)
        printf("%s\n", envp[i]);
    g_data_ptr->exit_status = 0;
}
