/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_heredoc.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <bomboclat@bidol.juis>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/24 14:34:01 by craimond          #+#    #+#             */
/*   Updated: 2024/03/11 13:01:15 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/minishell.h"

static uint8_t	fill_in_child(const char *const limiter, const uint16_t heredoc_fd);
static void		fill_heredoc(const char *const limiter, const uint16_t fd);

void	create_heredocs(const t_tree *const tree, uint8_t *const status)
{
	t_parser	*elem;
	t_list		*redirs;
	t_redir		*redir;
	char		*filename;
	uint16_t	fd;

	if (!tree || *status != 0)
		return ;
	elem = (t_parser *)tree->content;
	if (elem->type == CMD)
	{
		redirs = (t_list *)elem->cmd->redirs;
		while (redirs)
		{
			redir = (t_redir *)redirs->content;
			redirs = redirs->next;
			if (redir->type != REDIR_HEREDOC)
				continue ;
			filename = get_heredoc_filename(redir->heredoc_fileno);
			fd = open_p(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
			*status = fill_in_child(redir->filename, fd);
			if (*status != 0)
				return ;
		}
	}
	create_heredocs(tree->left, status);
	create_heredocs(tree->right, status);
}

char	*get_heredoc_filename(const uint16_t id)
{
	char				*filename;
	const t_data *const	data = get_data();
	const char *const	id_str = ft_itoa((uint16_t)id);
	const uint16_t		size = ft_strlen(data->starting_dir) + ft_strlen(id_str) + 16;

	filename = calloc_p(size, sizeof(char), TMP);
	if (!filename || !id_str)
		return (panic(ERR_MEM, "minishell: failed to allocate memory"), NULL);
	ft_strcpy(filename, data->starting_dir);
	ft_strcat(filename, "/tmp/.heredoc_");
	ft_strcat(filename, id_str);
	return (filename);
}

static uint8_t	fill_in_child(const char *const limiter, const uint16_t heredoc_fd)
{
	int32_t		status;
	const pid_t	pid = fork_p();

	status = 0;
	if (pid == 0)
	{
		signal_p(SIGTERM, &safe_exit);
		set_signals(S_HEREDOC);
		fill_heredoc(limiter, heredoc_fd);
	}
	else
	{
		signal_p(SIGQUIT, SIG_IGN);
		signal_p(SIGINT, SIG_IGN);
		waitpid_p(pid, &status, 0);
		status = WEXITSTATUS(status);
	}
	return ((uint8_t)status);
}

static void	fill_heredoc(const char *const limiter, const uint16_t fd)
{
	char		*str;
	uint16_t	str_len;

	str = NULL;
	while (true)
	{
		str = readline("> ");
		if (!str)
		{
			ft_putstr_fd("minishell: warning: here-document delimited by end-of-file (wanted `", STDERR_FILENO);
			ft_putstr_fd(limiter, STDERR_FILENO);
			ft_putstr_fd("')\n", STDERR_FILENO);
			break ;
		}
		str_len = ft_strlen(str);
		if (ft_strncmp(limiter, str, str_len + 1) == 0)
			break ;
		replace_env_vars(&str, true);
		ft_putstr_fd(str, fd);
		write(fd, "\n", 1);
	}
	release_resources();
	exit(0);
}
