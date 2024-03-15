/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   files.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <bomboclat@bidol.juis>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/28 13:41:23 by craimond          #+#    #+#             */
/*   Updated: 2024/03/15 16:48:39 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../headers/minishell.h"

int16_t	open_p(const char *const path, const int32_t flags, const uint16_t mode)
{
	const int16_t	fd = open(path, flags, mode);

	if (fd == -1 && is_bad_errno(errno))
		panic(ERR_FD, NULL);
	return (fd);
}

void	close_p(const int16_t fd)
{
	if (close(fd) == -1 && is_bad_errno(errno))
		panic(ERR_FD, NULL);
}

void	reset_fd(int16_t *const fd)
{
	if (*fd >= 0)
		close_p(*fd);
	*fd = -42;
}
