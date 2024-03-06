/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   treeclear.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/05 15:42:45 by craimond          #+#    #+#             */
/*   Updated: 2024/03/06 19:12:48 by codespace        ###   ########.fr       */
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
