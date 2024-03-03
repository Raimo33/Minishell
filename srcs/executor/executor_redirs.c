/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_redirs.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <bomboclat@bidol.juis>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/28 00:39:47 by craimond          #+#    #+#             */
/*   Updated: 2024/03/03 00:11:16 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/minishell.h"

static void     exec_dup(t_redir *redir);
static uint16_t open_redir_file(t_redir *redir);

void exec_redirs(t_list *redirs)
{
    t_redir *redir;

    while (redirs)
    {
        redir = (t_redir *)redirs->content;
        if (redir->filename)
        {
            replace_env_vars(&redir->filename, false);
            replace_wildcards(&redir->filename);
            clear_quotes(&redir->filename);
        }
        exec_dup(redir);
        redirs = redirs->next;
    }
}

static void exec_dup(t_redir *redir)
{
    uint16_t    file_fd;

    if (!redir->filename)
        return ;
    file_fd = open_redir_file(redir);
    if (redir->type == REDIR_INPUT || redir->type == REDIR_HEREDOC)
        dup2_p(file_fd, STDIN_FILENO);
    else if (redir->type == REDIR_OUTPUT || redir->type == REDIR_APPEND)
        dup2_p(file_fd, STDOUT_FILENO);
    reset_fd((int16_t *)&file_fd);
}

static uint16_t  open_redir_file(t_redir *redir)
{
    uint16_t     fd;
    char         *heredoc_filename;

    if (redir->type == REDIR_INPUT)
        fd = open_p(redir->filename, O_RDONLY, 0644);
    else if (redir->type == REDIR_HEREDOC)
    {
        heredoc_filename = get_heredoc_filename(redir->heredoc_fileno); 
        fd = open_p(heredoc_filename, O_RDONLY, 0644);
        free_and_null((void **)&heredoc_filename);
    }
    else if (redir->type == REDIR_OUTPUT)
        fd = open_p(redir->filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    else
        fd = open_p(redir->filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
    return (fd);
}