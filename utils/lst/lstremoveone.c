/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lstremoveone.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <bomboclat@bidol.juis>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/04 15:28:08 by egualand          #+#    #+#             */
/*   Updated: 2024/02/05 11:43:26 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/minishell.h"

void	lstremoveone(t_list *to_remove, void (*del)(void *))
{
    t_list *prev;
    t_list *next;

    prev = to_remove->prev;
    next = to_remove->next;
    prev->next = next;
    next->prev = prev;
    to_remove->next = NULL;
    to_remove->prev = NULL;
    lstdelone(to_remove, del);
}