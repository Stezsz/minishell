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

t_data	*g_data_ptr; // Ponteiro global para a estrutura de dados

// Inicializa a lista de sinais com descrições
void	init_sig_list(t_data *data)
{
	char	**sys_siglist;

	sys_siglist = data->siglist;
	memset(sys_siglist, 0, NSIG * sizeof(char));
	sys_siglist[SIGHUP] = "Hangup";
	sys_siglist[SIGQUIT] = "Quit";
	sys_siglist[SIGILL] = "Illegal instruction";
	sys_siglist[SIGTRAP] = "Trace/BPT trap";
	sys_siglist[SIGABRT] = "Abort trap";
	sys_siglist[SIGFPE] = "Floating point exception";
	sys_siglist[SIGKILL] = "Killed";
	sys_siglist[SIGBUS] = "Bus error";
	sys_siglist[SIGSEGV] = "Segmentation fault";
	sys_siglist[SIGSYS] = "Bad system call";
	sys_siglist[SIGTERM] = "Terminated";
	sys_siglist[SIGURG] = "Urgent I/O condition";
	sys_siglist[SIGSTOP] = "Stopped (signal)";
	sys_siglist[SIGTSTP] = "Stopped";
	sys_siglist[SIGTTIN] = "Stopped (tty input)";
	sys_siglist[SIGTTOU] = "Stopped (tty output)";
	sys_siglist[SIGIO] = "I/O ready";
	sys_siglist[SIGXCPU] = "Cputime limit exceeded";
	sys_siglist[SIGXFSZ] = "Filesize limit exceeded";
	sys_siglist[SIGUSR1] = "User defined signal 1";
	sys_siglist[SIGUSR2] = "User defined signal 2";
}

// Mascara sinais para serem bloqueados
void	mask_signals(t_sigaction *sa)
{
	sigemptyset(&sa->sa_mask);
	sigaddset(&sa->sa_mask, SIGINT);
	sigaddset(&sa->sa_mask, SIGQUIT);
}

// Define ações para sinais específicos
void	catch_sig(t_sigaction *sa)
{
	sigaction(SIGINT, sa, NULL);
	sigaction(SIGQUIT, sa, NULL);
}

// Manipulador de sinais
void	signals_handler(int sig, siginfo_t *siginfo, void *ptr)
{
	(void) ptr;
	(void) siginfo;
	if (sig == SIGINT)
	{
		if (g_data_ptr->shell_state != SH_READING)
			return ;
		write(STDOUT_FILENO, "\n", 1);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
		g_data_ptr->exit_status = 1;
	}
}

// Inicializa sinais
void	init_signals(t_data *data)
{
	t_sigaction	sa;

	g_data_ptr = data;
	init_sig_list(data);
	memset(&sa, 0, sizeof(sa));
	sa.sa_sigaction = &signals_handler;
	sa.sa_flags = SA_SIGINFO;
	rl_catch_signals = 0;
	data->exit_status = 0;
	mask_signals(&sa);
	catch_sig(&sa);
}
