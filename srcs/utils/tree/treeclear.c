/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   treeclear.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <bomboclat@bidol.juis>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/05 15:42:45 by craimond          #+#    #+#             */
/*   Updated: 2024/03/08 16:26:05 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../headers/minishell.h"

void	treeclear(t_tree **const tree, void (*del)(void *const))
{
	if (!tree || !*tree)
		return ;
	if ((*tree)->left)
		treeclear(&(*tree)->left, del);
	if ((*tree)->right)
		treeclear(&(*tree)->right, del);
	if (del)
		del((*tree)->content);
	(*tree)->content = NULL;
	free(*tree);
	*tree = NULL;
}
