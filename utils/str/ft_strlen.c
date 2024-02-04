/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlen.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: egualand <egualand@student.42firenze.it    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/03 19:33:17 by craimond          #+#    #+#             */
/*   Updated: 2024/02/04 16:45:43 by egualand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/minishell.h"

size_t     ft_strlen(char *str)
{
    size_t     i;

    if (!str)
        return (0);
    i = 0;
    while (str[i])
        i++;
    return (i);
}
