/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <bomboclat@bidol.juis>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/03 20:15:35 by craimond          #+#    #+#             */
/*   Updated: 2024/03/05 17:27:27 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../headers/minishell.h"

char	*ft_strnstr(const char *big, const char *little, const uint16_t len)
{
	uint16_t		i;
	uint16_t		n;
	const uint16_t	little_len = ft_strlen(little);

	i = 0;
	n = 0;
	if (little[0] == '\0')
		return ((char *)big);
	while (i < len && big[i])
	{
		while (big[i + n] == little[n] && n + i < len)
		{
			if (n >= little_len - 1)
				return ((char *)&big[i]);
			n++;
		}
		n = 0;
		i++;
	}
	return (0);
}
