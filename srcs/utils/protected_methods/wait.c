/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wait.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <bomboclat@bidol.juis>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/28 13:38:07 by craimond          #+#    #+#             */
/*   Updated: 2024/03/14 16:41:01 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../headers/minishell.h"

pid_t	wait_p(int32_t *const status)
{
	pid_t	ret;

	ret = wait(status);
	if (ret == -1 && is_bad_errno(errno))
		panic(ERR_WAIT, NULL);
	return (ret);
}

pid_t	waitpid_p(const pid_t pid, int32_t *const status, const int32_t options)
{
	pid_t	ret;

	ret = waitpid(pid, status, options);
	if (ret == -1 && is_bad_errno(errno))
		panic(ERR_WAIT, NULL);
	return (ret);
}
