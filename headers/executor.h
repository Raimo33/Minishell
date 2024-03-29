/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <bomboclat@bidol.juis>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/19 17:46:26 by craimond          #+#    #+#             */
/*   Updated: 2024/03/15 16:53:03 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTOR_H
# define EXECUTOR_H

# define EXEC_FAILURE   1
# define CMD_NOT_FOUND  127

void	executor(const t_tree *const parsed_params);
void	create_heredocs(const t_tree *const tree, uint8_t *const status);
char	*get_heredoc_filename(uint16_t id);
int8_t	exec_redirs(const t_list *redirs);
char	*get_cmd_path(const char *const path, const char *const cmd);
char	**get_cmd_args(char *cmd_str);
void	replace_env_vars(char **const str, const bool ignore_quotes);
void	replace_wildcards(char **str);
void	exec_simple_cmd(const char *const path, char *cmd_str);
void	exec(const char *const path, char *cmd_str);

#endif
