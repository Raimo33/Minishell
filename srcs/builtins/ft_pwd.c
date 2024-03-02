/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pwd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <bomboclat@bidol.juis>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/13 14:59:42 by egualand          #+#    #+#             */
/*   Updated: 2024/02/28 20:24:27 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/minishell.h"

void	ft_pwd(char **args)
{
	char	*pwd;

	(void)args;
	pwd = getcwd_p(NULL, 0);
	if (!pwd)
	{
		ft_putstr_fd("pwd: PWD environvment variable not set", STDERR_FILENO);
		g_status = EXIT_FAILURE;
		return ;
	}
	ft_putstr_fd(pwd, STDOUT_FILENO);
	ft_putstr_fd("\n", STDOUT_FILENO);
	g_status = EXIT_SUCCESS;
}
