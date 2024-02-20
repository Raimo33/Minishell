/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <bomboclat@bidol.juis>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/13 15:08:20 by egualand          #+#    #+#             */
/*   Updated: 2024/02/19 15:44:02 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/minishell.h"

static char	*getcwd_p(char *buf, size_t size);

void ft_cd(char **args)
{
	char	*cwd;

	if (!args[1])
		return ;
	if (args[2])
	{
		ft_putstr_fd("cd: too many arguments\n", STDERR_FILENO);
		g_status = EXIT_FAILURE;
		return ;
	}
	cwd = getcwd_p(NULL, 0);
	if (!cwd)
		return ;
	if (chdir(args[1]) == -1)
	{
		perror("cd");
		g_status = EXIT_FAILURE;
		free(cwd);
		return ;
	}
	ft_setenv("OLDPWD", cwd, true);
	free(cwd);
	cwd = getcwd_p(NULL, 0);
	if (!cwd)
		return ;
	ft_setenv("PWD", cwd, true);
	free(cwd);
	g_status = EXIT_SUCCESS;
}

static char	*getcwd_p(char *buf, size_t size)
{
	char	*cwd;

	cwd = getcwd(buf, size);
	if (!cwd)
	{
		perror("cd");
		g_status = EXIT_FAILURE;
	}
	return (cwd);
}