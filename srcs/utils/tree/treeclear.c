/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   treeclear.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: egualand <egualand@student.42firenze.it    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/05 15:42:45 by craimond          #+#    #+#             */
/*   Updated: 2024/02/20 17:29:23 by egualand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../headers/minishell.h"

void	treeclear(t_tree **tree, void (*del)(void *))
{
    if (!tree || !*tree)
        return ;
    treeclear(&(*tree)->left, del);
    treeclear(&(*tree)->right, del);
    del(*tree);
    free(*tree);
    *tree = NULL;
}