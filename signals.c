/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: strodrig <strodrig@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/03 13:18:33 by strodrig          #+#    #+#             */
/*   Updated: 2025/02/03 13:18:33 by strodrig         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	init_sig_list(t_data *data)
{
	char	**sys_siglist;

	sys_siglist = data->siglist;
	memset(sys_siglist, 0, NSIG * sizeof(char));
	sys_siglist[SIGHUP] = "Hangup";
	//Contiuar preenchendo a lista de sinais
}

void	init_signals(t_data *data)
{
	t_sigaction	sa;

	init_sig_list(data);
	memset(&sa, 0, sizeof(sa));
	sa.sa_sigaction = &signals_handler;
	sa.sa_flags = 0;
	rl_catch_signals = 0;
	data->exit_status = 0;
	mask_signals(&sa);
	catch_sig(&sa);
}
