/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <bomboclat@bidol.juis>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/06 11:58:14 by craimond          #+#    #+#             */
/*   Updated: 2024/03/10 19:43:34 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/minishell.h"

static uint16_t	skip_flags(const char *const *args, bool *is_n);

void	ft_echo(const char *const *args)
{
	uint16_t	i;
	bool		is_n;

	is_n = false;
	i = skip_flags(args, &is_n);
	while (args[i])
	{
		ft_putstr_fd(args[i], STDOUT_FILENO);
		if (args[i + 1])
			ft_putstr_fd(" ", STDOUT_FILENO);
		i++;
	}
	if (!is_n)
		ft_putstr_fd("\n", STDOUT_FILENO);
	g_status = EXIT_SUCCESS;
}

static uint16_t	skip_flags(const char *const *args, bool *is_n)
{
	uint16_t	i;
	uint16_t	j;

	i = 1;
	while (args[i] && args[i][0] == '-')
	{
		j = 1;
		while (args[i][j])
		{
			if (args[i][j] != 'n')
				return (i);
			j++;
		}
		*is_n = true;
		i++;
	}
	return (i);
}
