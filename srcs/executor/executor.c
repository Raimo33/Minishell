/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <bomboclat@bidol.juis>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/19 17:46:08 by craimond          #+#    #+#             */
/*   Updated: 2024/03/15 16:53:16 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/minishell.h"

static void		launch_commands(const t_tree *const node, const int8_t prev_type, int16_t fds[5]);
static void		launch_builtin_cmd(const t_tree *const node, const int8_t prev_type, int16_t fds[5]);
static void		launch_standard_cmd(const t_tree *const node, const int8_t prev_type, int16_t fds[5]);
static t_tree	*skip_till_semicolon(const t_tree *const node);
static void		child(const t_tree *const node, const int16_t fds[5], const int8_t prev_type);
static void		parent(const t_tree *const node, int16_t fds[5]);

void	executor(const t_tree *const parsed_params)
{
	const uint8_t	original_status = g_status;
	uint8_t			heredoc_status;
	int16_t			fds[5] = {-42, -42, -42, -42, -42};//pipe read, pipe write, prev_output, original stdin, original stdout

	set_signals(S_COMMAND);
	fds[3] = dup_p(STDIN_FILENO);
	fds[4] = dup_p(STDOUT_FILENO);
	heredoc_status = 0;
	create_heredocs(parsed_params, &heredoc_status);
	dup2_p(fds[3], STDIN_FILENO);
	if (heredoc_status != 0)
	{
		if (heredoc_status == 130)
			g_status = 130;
		return ;
	}
	g_status = (uint8_t)original_status;
	launch_commands(parsed_params, -1, fds);
	dup2_p(fds[3], STDIN_FILENO);
}

static void	launch_commands(const t_tree *const node, const int8_t prev_type, int16_t fds[5])
{
	t_parser	*elem;
	t_parser	*left_elem;

	if (!node)
		return ;
	elem = (t_parser *)node->content;
	if (elem->type != CMD)
	{
		left_elem = (t_parser *)node->left->content;
		if (elem->type == PIPELINE)
			pipe_p(fds);
		if (left_elem->cmd && is_builtin(left_elem->cmd->cmd_str))
			launch_builtin_cmd(node, prev_type, fds);
		else
			launch_standard_cmd(node, prev_type, fds);
		if ((elem->type == AND && g_status != 0) || (elem->type == OR && g_status == 0))
			launch_commands(skip_till_semicolon(node), -1, fds);
		else
			launch_commands(node->right, elem->type, fds);
		return ;
	}
	child(node, fds, prev_type);
}

static void	launch_builtin_cmd(const t_tree *const node, const int8_t prev_type, int16_t fds[5])
{
	const t_parser *const	elem = (t_parser *)node->content;

	if (elem->type == PIPELINE)
	{
		dup2_p(fds[1], STDOUT_FILENO);
		reset_fd(&fds[1]);
	}
	fds[2] = fds[0];
	launch_commands(node->left, prev_type, fds);
	dup2_p(fds[4], STDOUT_FILENO);
}

static void	launch_standard_cmd(const t_tree *const node, const int8_t prev_type, int16_t fds[5])
{
	const t_parser *const	elem = (t_parser *)node->content;
	const pid_t				pid = fork_p();

	if (pid == 0)
	{
		signal_p(SIGTERM, &safe_exit);
		if (elem->type == PIPELINE)
		{
			dup2_p(fds[1], STDOUT_FILENO);
			reset_fd(&fds[0]);
			reset_fd(&fds[1]);
		}
		launch_commands(node->left, prev_type, fds);
		release_resources();
		exit(g_status);
	}
	else
		parent(node, fds);
}

static t_tree	*skip_till_semicolon(const t_tree *const node)
{
	t_parser	*elem;

	if (!node)
		return (NULL);
	elem = (t_parser *)node->content;
	if (elem->type == SEMICOLON)
		return (node->right);
	return (skip_till_semicolon(node->right));
}

static void	child(const t_tree *const node, const int16_t fds[5], const int8_t prev_type)
{
	const t_parser *const	elem = (t_parser *)node->content;

	if (prev_type == PIPELINE)
		dup2_p(fds[2], STDIN_FILENO);
	replace_env_vars(&elem->cmd->cmd_str, false);
	replace_wildcards(&elem->cmd->cmd_str);
	if (exec_redirs(elem->cmd->redirs) == -1)
		return ;
	exec(ft_getenv("PATH="), elem->cmd->cmd_str);
}

static void	parent(const t_tree *const node, int16_t fds[5])
{
	const t_parser *const	elem = (t_parser *)node->content;
	int32_t					status;

	reset_fd(&fds[1]);
	fds[2] = fds[0];
	if (elem->type != PIPELINE)
	{
		while (waitpid_p(0, &status, 0) > 0)
			g_status = WEXITSTATUS(status);
		reset_fd(&fds[0]);
	}
}
