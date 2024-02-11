/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <bomboclat@bidol.juis>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/29 20:59:44 by craimond          #+#    #+#             */
/*   Updated: 2024/02/10 23:50:20 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "headers/minishell.h"

static char *get_env_name(char *str);

char    *replace_env_vars(char *str)
{
    uint32_t    env_name_len;
    char        *env_value;
    char        *env_name;
    char        *start;
    char        *tmp;
    uint32_t    len;
    uint32_t    i;

    i = 0;
    while (true)
    {
        start = ft_strdup(&str[i]);
        if (!start)
            ft_quit(ERR_MALLOC, "failed to allocate memory");
        tmp = ft_strchr(start, '$');
        if (!tmp)
            return (free(start), str);
        env_name = get_env_name(tmp + 1);
        env_name_len = ft_strlen(env_name);
        if (ft_strncmp(env_name, "?", 1) != 0)
            env_value = ft_getenv(env_name);
        else
        {
            env_name_len = 1;
            env_value = ft_utoa((uint32_t)g_status);
        }
        free(env_name);
        if (!env_value)
            env_value = "";
        len = ft_strlen(str);
        free(str);
        str = calloc_p(len - env_name_len + ft_strlen(env_value), sizeof(char)); //senza + 1 perche' c'e' gia' il + 1 del carattere $ che non e' considerato nella len
        ft_strlcpy(str, start, (size_t)(tmp - start + 1));
        ft_strcat(str, env_value);
        ft_strcat(str, tmp + 1 + env_name_len);
        i += (uint32_t)(tmp - start);
        free(start);
    }
}

static char *get_env_name(char *str)
{
    char	    *env_name;
    char        *end;

    if (!str || str[0] == '\0' || str[0] == '$' || is_shell_space(str[0]))
        return (NULL);
    env_name = ft_strdup(str);
    if (!env_name)
        ft_quit(ERR_MALLOC, "failed to allocate memory");
    end = ft_strchr(env_name, ' ');
    if (end)
        *end = '\0';
    return (env_name);
}