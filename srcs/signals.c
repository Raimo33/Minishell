/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <bomboclat@bidol.juis>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/07 17:37:20 by marvin            #+#    #+#             */
/*   Updated: 2024/03/14 16:41:24 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/minishell.h"

static void	command_mode(const int32_t signo);
static void	interactive_mode(const int32_t signo);
static void	heredoc_mode(const int32_t signo);
static void	catch_panic(const int32_t signo, siginfo_t *const info, void *const context);

void	set_signals(const uint8_t mode)
{
	static const __sighandler_t	sig_handler[] = {&interactive_mode, &heredoc_mode, &command_mode};

	signal_p(SIGINT, sig_handler[mode]);
	signal_p(SIGQUIT, sig_handler[mode]);
}

void	init_signals(void)
{
	struct sigaction	sa;

	sa.sa_sigaction = &catch_panic;
	sa.sa_flags = SA_SIGINFO | SA_RESTART;
	sigemptyset(&sa.sa_mask);
	sigaction_p(SIGUSR1, &sa, NULL);
	sigaction_p(SIGUSR2, &sa, NULL);
}

void	safe_exit(const int32_t signo)
{
	(void)signo;
	release_resources();
	exit(0);
}

static void	catch_panic(const int32_t signo, siginfo_t *const info, void *const context)
{
	static pid_t	calling_pid = -1;
	static uint8_t	id = 0;
	static uint8_t	n_signals = 0;

	(void)context;
	if (calling_pid == -1)
		calling_pid = info->si_pid;
	else if (calling_pid != info->si_pid)
		return ;
	if (signo == SIGUSR1)
		id |= 0x01 << n_signals++;
	else if (signo == SIGUSR2)
		n_signals++;
	if (n_signals == (sizeof(id) * 8))
	{
		signal_p(SIGTERM, SIG_IGN);
		kill(-get_data()->main_pid, SIGTERM);
		signal_p(SIGTERM, &safe_exit);
		release_resources();
		while (waitpid_p(-1, NULL, WNOHANG) > 0)
			;
		exit(id);
	}
}

static void	command_mode(const int32_t signo)
{
	if (signo == SIGINT)
	{
		ft_putstr_fd("\n", STDOUT_FILENO);
		g_status = 130;
	}
	else if (signo == SIGQUIT)
	{
		ft_putstr_fd("Quit (core dumped)\n", STDOUT_FILENO);
		g_status = 131;
	}
}

static void	interactive_mode(const int32_t signo)
{
	if (signo == SIGINT)
	{
		g_status = 130;
		ft_putstr_fd("\n", STDOUT_FILENO);
		rl_replace_line("", 0);
		rl_on_new_line();
		rl_redisplay();
	}
	else if (signo == SIGQUIT)
		ft_putstr_fd("\b\b  \b\b", STDOUT_FILENO);
}

static void	heredoc_mode(const int32_t signo)
{
	if (signo == SIGINT)
	{
		ft_putstr_fd("\n", STDOUT_FILENO);
		release_resources();
		exit(130);
	}
	else if (signo == SIGQUIT)
		ft_putstr_fd("\b\b  \b\b", STDOUT_FILENO);
}
