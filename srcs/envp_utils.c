/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   envp_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <bomboclat@bidol.juis>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/01 14:46:00 by craimond          #+#    #+#             */
/*   Updated: 2024/03/02 20:01:07 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/minishell.h"

void    envp_init(char **envp)
{
    uint16_t    size;
    t_data      *data;
    char        **matrix;
    t_tree      *tree;
    char        *str;

    data = get_data();
    tree = NULL;
    size = ft_matrixsize(envp);
    data->envp_size = size;
    matrix = (char **)malloc_p(sizeof(char *) * (size + 1));
    matrix[size] = NULL;
    while (size--)
    {
        str = strdup_p(envp[size]);
        tree = envp_tree_add(tree, str);
        matrix[size] = str;
    }
    data->envp_tree = tree;
    data->envp_matrix = matrix;
}

//NAME DEVE ESSERE COMPRESO DI '='
void	ft_setenv(char *name, char *value, bool replace)
{
    char        *str;
    t_data      *data;

    data = get_data();
    str = strjoin_p(name, value);
    if (replace && ft_getenv(name))
        ft_unsetenv(name);
    data->envp_size++;
    data->envp_tree = envp_tree_add(data->envp_tree, str);
    envp_matrix_add(str);
}

//NAME DEVE ESSERE COMPRESO DI '='
void	ft_unsetenv(char *name)
{
    uint16_t    name_len;
    t_data      *data;

    if (!ft_getenv(name))
        return ;
    name_len = ft_strlen(name);
    data = get_data();
    data->envp_size--;
    data->envp_tree = envp_tree_remove(data->envp_tree, name, name_len);
    envp_matrix_remove(name, name_len);
}

//NAME DEVE ESSERE COMPRESO DI '='
char	*ft_getenv(char *name)
{
    t_tree *found;
    char   *value;

    found = envp_tree_find(get_data()->envp_tree, name, ft_strlen(name));
    if (found)
    {
        value = ft_strchr((char *)found->content, '=');
        if (value)
            return (value + 1);
        else
            return (NULL);
    }
    return (NULL);
}