/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <bomboclat@bidol.juis>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/06 17:58:27 by craimond          #+#    #+#             */
/*   Updated: 2024/01/17 16:30:29 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	replace_placeholders(t_list *parsed_params, t_data *data);
static void	handle_env(t_list *lexered_params, t_parser *content_par,
				unsigned int i, t_data *data);

t_list	*parser(t_list *lexered_params, t_data *data)
{
	t_parser		*content_par;
	t_list			*parsed_params;
	t_list			*node;
	size_t			size;
	t_lexer			*prev_cmd_elem;
	t_lexer			*content_lex;
	unsigned int	i;
	unsigned int	token_streak;

	if (!lexered_params)
		return (NULL);
	ft_lstdel_if(&lexered_params, &is_empty_cmd, &del_content_lexer);
	// TODO gestire caso dipo "> >" e "< <"
	node = lexered_params;
	content_par = new_elem(&size, node, data);
	parsed_params = NULL;
	prev_cmd_elem = NULL;
	i = 0;
	while (node)
	{
		content_lex = (t_lexer *)node->content;
		if (content_lex->type == CMD)
		{
			ft_strlcat(content_par->cmd_str, content_lex->str.cmd, size);
			prev_cmd_elem = content_lex;
			node = node->next;
		}
		else if (content_lex->type == TOKEN)
		{
			if (content_lex->str.token == PIPE)
			{
				ft_lstadd_back(&parsed_params, ft_lstnew(content_par));
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
				handle_env(node, content_par, i++, data);
			// TODO valutare se fare qualche eccezione per i token di fila;
			// TODO se ci sono piu token di fila fare un controllo. ad esempio non puo esserci un | subito dopo un >
			token_streak = check_token_streak(NULL, node);
			while (token_streak-- > 0)
				node = node->next;
		}
	}
	ft_lstadd_back(&parsed_params, ft_lstnew(content_par));
	ft_lstclear(&lexered_params, &del_content_lexer);
	data->lexered_params = NULL;
	// TODO replace placeholders e' come se non funzionasse
	replace_placeholders(parsed_params, data);
	return (parsed_params);
}

static void	replace_placeholders(t_list *parsed_params, t_data *data)
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
				if (redir->type == REDIR_INPUT || redir->type == REDIR_APPEND
					|| redir->type == REDIR_OUTPUT)
					remove_word(&content_par->cmd_str, i, RIGHT, data);
				else if (redir->type == REDIR_HEREDOC)
					remove_word(&content_par->cmd_str, i, RIGHT, data);
				else if (redir->type == REDIR_INPUT_FD
					|| redir->type == REDIR_OUTPUT_FD
					|| redir->type == REDIR_APPEND_FD)
					remove_num(&content_par->cmd_str, i, RIGHT, data);
			}
			else if (content_par->cmd_str[i] == PH_ENV) //entra troppe volte qui
				replace_env_var(&content_par->cmd_str, i, content_par->env_vars[j++], data);
			i++;
		}
		node = node->next;
	}
}

static void	handle_env(t_list *lexered_params, t_parser *content_par,
		unsigned int i, t_data *data)
{
	char				*path_name;
	unsigned int		j;
	static const char	ph_env = PH_ENV;
	t_lexer				*next_cmd_elem;

	// placeholder per il $
	next_cmd_elem = get_next_cmd_elem(lexered_params);
	path_name = ft_strdup(next_cmd_elem->str.cmd);
	if (!path_name)
		ft_quit(15, "failed to allocate memory", data);
	j = 0;
	while (path_name[j] && !is_shell_space(path_name[j]))
		j++;
	path_name[j] = '\0';
	content_par->env_vars[i] = getenv(path_name);
	ft_strlcat(content_par->cmd_str, &ph_env, ft_strlen(content_par->cmd_str) + 2);
	free(path_name);
}
