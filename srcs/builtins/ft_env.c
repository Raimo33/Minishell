/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <bomboclat@bidol.juis>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/13 16:29:39 by egualand          #+#    #+#             */
/*   Updated: 2024/02/19 14:55:25 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/minishell.h"

void ft_env(char **args)
{
	if (args[1])
	{
		ft_putstr_fd("env: too many arguments\n", STDERR_FILENO);
		g_status = EXIT_FAILURE;
		return ;
	}
	print_envp_table(false);
	g_status = EXIT_SUCCESS;
}