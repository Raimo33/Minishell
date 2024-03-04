/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <bomboclat@bidol.juis>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/07 17:37:20 by marvin            #+#    #+#             */
/*   Updated: 2024/03/04 15:12:32 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/minishell.h"

static void silent_mode(const int32_t signo);
static void interactive_mode(const int32_t signo);
static void heredoc_mode(const int32_t signo);
static void command_mode(const int32_t signo);
static void death_mode(const int32_t signo, siginfo_t *const info, void *const context);
static void safe_exit(const int32_t signo);

void    set_signals(const uint8_t mode, const bool is_main)
{
    struct sigaction            sa;
    static const __sighandler_t sig_handler[] = {&interactive_mode, &heredoc_mode, &command_mode, &silent_mode}; //order has to be the same as the defines in the header file

    signal_p(SIGINT, sig_handler[mode]);
    signal_p(SIGQUIT, sig_handler[mode]);
    if (!is_main)
        signal_p(SIGTERM, &safe_exit);
    else
    {
        signal_p(SIGTERM, SIG_IGN);
        sa.sa_sigaction = death_mode;
        sa.sa_flags = SA_SIGINFO | SA_RESTART;
        sigemptyset(&sa.sa_mask);
        sigaction_p(SIGUSR1, &sa, NULL);
        sigaction_p(SIGUSR2, &sa, NULL);
    }
}

static void safe_exit(const int32_t signo)
{
    (void)signo;
    free_data();
    exit(0);
}

static void death_mode(const int32_t signo, siginfo_t *const info, void *const context)
{
    static pid_t    pid = -1;
    static uint8_t  id = 0;
    static uint8_t  n_signals = 0;

    (void)context;
    if (pid == -1)
        pid = info->si_pid;
    else if (pid != info->si_pid)
        return ;
    if (signo == SIGUSR1)
        id |= 0x01 << n_signals++;
    else if (signo == SIGUSR2)
        n_signals++;
    if (n_signals == (sizeof(id) * 8))
    {
        kill(-(get_data()->main_pid), SIGTERM);
        free_data();
        exit(id);
    }
}

static void silent_mode(const int32_t signo)
{
    if (signo == SIGINT)
        g_status = 130;
    else if (signo == SIGQUIT)
        g_status = 131;
}

static void interactive_mode(const int32_t signo)
{
    if (signo == SIGINT)
    {
        g_status = 130;
        ft_putstr_fd("\n", STDOUT_FILENO);
        rl_on_new_line();
        rl_replace_line("", 0);
        rl_redisplay();
    }
    else if (signo == SIGQUIT)
        ft_putstr_fd("\b\b  \b\b", STDOUT_FILENO);
}

static void heredoc_mode(const int32_t signo)
{
    if (signo == SIGINT)
    {
        ft_putstr_fd("\n", STDOUT_FILENO);
        exit(130);
    }
    else if (signo == SIGQUIT)
        ft_putstr_fd("\b\b  \b\b", STDOUT_FILENO);
}

static void command_mode(const int32_t signo)
{
    if (signo == SIGINT)
    {
        g_status = 130;
        ft_putstr_fd("\n", STDOUT_FILENO);
        exit(130);  
    }
    else if (signo == SIGQUIT)
    {
        g_status = 131;
        ft_putstr_fd("\b\b  \b\b", STDOUT_FILENO); //non identico a bash ma pie' bello
        ft_putstr_fd("Quit (core dumped)\n", STDOUT_FILENO);
        exit(131);
    }
}

