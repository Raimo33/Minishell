/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <bomboclat@bidol.juis>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/06 17:58:27 by craimond          #+#    #+#             */
/*   Updated: 2024/01/13 15:09:30 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void replace_placeholders(t_list *parsed_params, t_data *data);
static void	handle_env(t_lexer *prev_cmd_elem, t_parser *content_par, unsigned int i, t_data *data);

t_list	*parser(t_list *lexered_params, t_data *data)
{
	t_parser		*content_par;
	t_list			*parsed_params;
	t_list			*node;
	size_t			size;
	t_lexer			*prev_cmd_elem;
	t_lexer			*content_lex;
	char			*cmd_str;
	unsigned int	i;
	unsigned int	token_streak;
	t_redir_type	redir_type;
	t_token			next_token;

	node = lexered_params;
	content_par = new_elem(&size, node, data);
	prev_cmd_elem = NULL;
	i = 0;
	while (node)
	{
		content_lex = (t_lexer *)node->content;
		if (content_lex->type == CMD)
		{
			ft_strlcat(content_par->cmd_str, content_lex->str.cmd, size);
			prev_cmd_elem = content_lex;
		}
		else if (content_lex->type == TOKEN)
		{
			if (content_lex->str.token == PIPE)
			{
				ft_lstadd_back(parsed_params, ft_lstnew(content_par));
				i = 0;
				content_par = new_elem(&size, node->next, data);
				node = node->next;
				continue ;
			}
			else if (content_lex->str.token == REDIR_L)
				handle_redir_l(node, prev_cmd_elem, content_par, data);
			else if (content_lex->str.token == REDIR_R)
				handle_redir_r(node, prev_cmd_elem, content_par, data);
			else if (content_lex->str.token == ENV)
				handle_env(prev_cmd_elem, content_par, i++, data);
			//TODO valutare se fare qualche eccezione per i token di fila;
			token_streak = check_token_streak(NULL, node);
			while (token_streak-- > 0)
				node = node->next;
		}
		node = node->next;
	}
	ft_lstclear(lexered_params, &del_content);
	replace_placeholders(parsed_params, data);
	return (parsed_params);
}

static void replace_placeholders(t_list *parsed_params, t_data *data)
{
	t_list			*node;
	t_parser		*content_par;
	t_redir			*redir;
	unsigned int	i;
	unsigned int	j;

	node = parsed_params;
	while (node)
	{
		content_par = (t_parser *)node->content;
		i = 0;
		j = 0;
		while (content_par->cmd_str[i] != '\0')
		{
			if (content_par->cmd_str[i] == PH_REDIR)
			{
				redir = (t_redir *)content_par->redirs->content;
				if (redir->type == REDIR_APPEND || redir->type == REDIR_OUTPUT)
					remove_num(&content_par->cmd_str, i, LEFT, data);
				if (redir->type == REDIR_INPUT || redir->type == REDIR_APPEND || redir->type == REDIR_OUTPUT)
					remove_word(&content_par->cmd_str, i, RIGHT, data);
				else if (redir->type == REDIR_HEREDOC)
					remove_word(&content_par->cmd_str, i, RIGHT, data);
				else if (redir->type == REDIR_INPUT_FD || redir->type == REDIR_OUTPUT_FD || redir->type == REDIR_APPEND_FD)
					remove_num(&content_par->cmd_str, i, RIGHT, data); //rimuove numeri a piu cifre e il carattere '&'
			}
			else if (content_par->cmd_str[i] == PH_ENV)
				replace_env_var(&content_par->cmd_str, content_par->env_vars[j++], i, data);
			i++;
		}
		node = node->next;
	}
}

static void	handle_env(t_lexer *prev_cmd_elem, t_parser *content_par, unsigned int i, t_data *data)
{
	char				*path_name;
	unsigned int		i;
	//placeholder per il $
	static const char	ph_env
		= PH_ENV;

	path_name = ft_strdup(prev_cmd_elem);
	if (!path_name)
		ft_quit(15, "failed to allocate memory", data);
	i = 0;
	while (!is_shell_space(path_name[i]))
		i++;
	path_name[i] = '\0';
	content_par->env_vars[i] = getenv(path_name);
	ft_strlcat(content_par->cmd_str, &ph_env, ft_strlen(content_par->cmd_str) + 1);
	free(path_name);
}
