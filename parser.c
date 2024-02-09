/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <bomboclat@bidol.juis>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/06 17:58:27 by craimond          #+#    #+#             */
/*   Updated: 2024/02/09 19:52:13 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "headers/minishell.h"

static bool     is_empty_cmd(void *content);
static int8_t   check_syntax(t_list *lexered_params);
static void     merge_separators(t_list **lexered_params);
static void     merge_ampersands(t_list **head, t_list *node);
static void     merge_pipes(t_list **head, t_list *node);
static t_tree   *fill_tree(t_list *lexered_params, t_list *stop);
static t_list   *get_next_token(t_list *lexered_params, t_list *stop);
static t_list   *skip_parenthesis(t_list *params);
static t_list   *unskip_parenthesis(t_list *lexered_params);
static t_cmd    *init_cmd(char *cmd_str);
static void     clear_redirs(t_list *redirs, char *cmd_str);
static void     remove_fd_num(char *cmd_str, uint32_t idx_redir, int8_t before_after);
static uint32_t remove_filename(char *cmd_str, uint32_t i);
static t_list   *fill_redirs(char *cmd_str);
static void     fill_redir_input(t_list **redirs, char *str, uint32_t i, bool is_heredoc);
static void     fill_redir_output(t_list **redirs, char *str, uint32_t i, bool is_append);
static t_redir  *init_redir(void);
static int32_t  get_fd_num(char *str, uint32_t idx_redir, uint8_t before_after);

t_tree	*parser(t_list *lexered_params)
{
    t_tree   **parsed_params;
    t_lexer  *elem;

    lstdelif(&lexered_params, &is_empty_cmd, &del_content_lexer);
    check_syntax(lexered_params);
    merge_separators(&lexered_params);
    //merge_parenthesis(&lexered_params); rimuove le parentesi inutili tipo (cmd 1) oppure ((((cmd1))(((cmd2)))))
    parsed_params = (t_tree **)malloc_p(sizeof(t_tree *));
    if (lexered_params->next)
        *parsed_params = fill_tree(lexered_params->next, NULL);
    else
    {
        elem = (t_lexer *)lexered_params;
        *parsed_params = treenew_p(elem->token, init_cmd(elem->cmd_str));
    }
    return (*parsed_params);
}

static int8_t   check_syntax(t_list *lexered_params)
{
    //TODO 
    (void)lexered_params;
    return (0);
}

// static t_tree   *fill_tree(t_list *lexered_params, t_list *stop_node)
// {
//     t_tree  *node;
// 	t_lexer *elem;
//     t_lexer *next_elem;

//     node = NULL;
//     elem = (t_lexer *)lexered_params->content;
//     if (lexered_params->next == stop_node)
//         return (treenew_p(elem->token, init_cmd(elem->cmd_str)));
//     next_elem = (t_lexer *)lexered_params->next->content;
//     treeadd_below(&node, treenew_p(next_elem->token, init_cmd(next_elem->cmd_str)));
//     if (elem->token != SUBSHELL_END)
//         treeadd_below(&node, treenew_p(elem->token, init_cmd(elem->cmd_str)));
//     else
//         treeadd_below(&node, fill_tree(unskip_parenthesis(lexered_params), lexered_params));
//     treeadd_below(&node, fill_tree(skip_parenthesis(lexered_params->next->next), stop_node));
//     return (node);
// }

// static t_tree   *fill_tree(t_list *lexered_params)
// {
//     t_tree  *node;
// 	t_lexer *elem;
//     t_lexer *next_elem;

//     node = NULL;
//     if (!lexered_params)
//         return (NULL);
//     elem = (t_lexer *)lexered_params->content;
//     if (elem->token == SUBSHELL_START)
//         return (fill_tree(skip_parenthesis(lexered_params)));
//     if (!lexered_params->next)
//         return (treenew_p(elem->token, init_cmd(elem->cmd_str)));
//     next_elem = (t_lexer *)lexered_params->next->content;
//     if (next_elem->token == SUBSHELL_END && elem->token != SUBSHELL_END)
//         return (treenew_p(elem->token, init_cmd(elem->cmd_str)));
//     if (next_elem->token != SUBSHELL_END)
//         treeadd_below(&node, treenew_p(next_elem->token, init_cmd(next_elem->cmd_str)));
//     if (elem->token != SUBSHELL_END)
//         treeadd_below(&node, treenew_p(elem->token, init_cmd(elem->cmd_str)));
//     else
//         treeadd_below(&node, fill_tree(unskip_parenthesis(lexered_params)));
//     treeadd_below(&node, fill_tree(lexered_params->next->next));
//     return (node);
// }

// static t_tree   *fill_tree(t_list *lexered_params, t_list *stop)
// {
//     t_tree  *node;
//     t_lexer *elem;
//     t_lexer *prev_elem;
//     t_lexer *next_elem;

//     node = NULL;
//     prev_elem = (t_lexer *)lexered_params->prev->content;
//     if (!lexered_params || lexered_params == stop)
//         return (treenew_p(prev_elem->token, init_cmd(prev_elem->cmd_str)));
//     elem = (t_lexer *)lexered_params->content;
//     next_elem = (t_lexer *)lexered_params->next->content;
//     if (prev_elem->token == SUBSHELL_START)
//         return (fill_tree(skip_parenthesis(lexered_params->prev), stop));
//     node = treenew_p(elem->token, init_cmd(elem->cmd_str));
//     if (prev_elem->token == SUBSHELL_END)
//         treeadd_below(&node, fill_tree(unskip_parenthesis(lexered_params->prev), lexered_params->prev));
//     else
//         treeadd_below(&node, treenew_p(prev_elem->token, init_cmd(prev_elem->cmd_str)));
//     treeadd_below(&node, fill_tree(lexered_params->next->next, stop));
//     return (node);
// }

//da passargli lexered_params->next
//NON funziona il caso: cmd 1 ; ( cmd2 ; ( cmd3 ; cmd 4)) ; cmd 5
//NON funziona il caso con una parentesi che contiene solo un comando ma si gestisce prima tramite merge_parenthesis
static t_tree   *fill_tree(t_list *lexered_params, t_list *stop)
{
    t_tree  *node;
    t_lexer *elem;
    t_lexer *prev_elem;

    if (!lexered_params)
        return (NULL);
    node = NULL;
    elem = (t_lexer *)lexered_params->content;
    prev_elem = (t_lexer *)lexered_params->prev->content;
    if (elem->token == CMD || lexered_params->next == stop)
        return (treenew_p(elem->token, init_cmd(elem->cmd_str)));
    if (elem->token == SUBSHELL_START)
    {
        lexered_params = skip_parenthesis(lexered_params);
        elem = (t_lexer *)lexered_params->content;
        prev_elem = (t_lexer *)lexered_params->prev->content;
    }
    if (elem->token != SUBSHELL_END)
        node = treenew_p(elem->token, init_cmd(elem->cmd_str));
    if (prev_elem->token != SUBSHELL_END)
        treeadd_below(&node, treenew_p(prev_elem->token, init_cmd(prev_elem->cmd_str)));
    else
        return (fill_tree(unskip_parenthesis(lexered_params->prev), lexered_params->prev));
    treeadd_below(&node, fill_tree(get_next_token(lexered_params, stop), stop));
    return (node);
}

static t_list   *get_next_token(t_list *lexered_params, t_list *stop) //altrimenti ritorna il comando
{
    t_lexer *elem;
    t_list  *last;

    last = lexered_params;
    lexered_params = lexered_params->next;
    while (lexered_params && lexered_params != stop)
    {
        elem = (t_lexer *)lexered_params->content;
        if (elem->token)
            return (lexered_params);
        last = lexered_params;
        lexered_params = lexered_params->next;
    }
    return (last);
}

//TODO da mergiare con unskip_parenthesis
static t_list	*skip_parenthesis(t_list *lexered_params)
{
	int32_t	    n_open;
	t_lexer		*elem;

	n_open = 1;
	while (n_open && lexered_params->next) 
	{
        lexered_params = lexered_params->next;
		elem = (t_lexer *)lexered_params->content;
		if (elem->token == SUBSHELL_END)
			n_open--;
		else if (elem->token == SUBSHELL_START)
			n_open++;
	}
	return (lexered_params->next); //ritorna dopo la parentesi chiusa
}

static t_list   *unskip_parenthesis(t_list *lexered_params)
{
    int32_t    n_close;
    t_lexer    *elem;

    n_close = 1;
    while (n_close && lexered_params->prev)
    {
        lexered_params = lexered_params->prev;
        elem = (t_lexer *)lexered_params->content;
        if (elem->token == SUBSHELL_END)
            n_close++;
        else if (elem->token == SUBSHELL_START)
            n_close--;
    }
    return (lexered_params->next->next); //ritorna due dopo la parentesi aperta
}

static t_cmd    *init_cmd(char *cmd_str)
{
    t_cmd   *cmd;

    if (!cmd_str)
        return (NULL);
    cmd = malloc_p(sizeof(t_cmd));
    cmd->redirs = fill_redirs(cmd_str);
    clear_redirs(cmd->redirs, cmd_str);
    cmd->cmd_str = cmd_str;
    return (cmd);
}

 //rimpiazza anche tab e \n con spazi (cosi' dopo si potra' fare split con spazi)
static void     clear_redirs(t_list *redirs, char *cmd_str)
{
    uint32_t    i;
    t_redir     *redir;

    i = 0;
    while (cmd_str[i])
    {
        if (cmd_str[i] == '<' || cmd_str[i] == '>')
        {
            cmd_str[i++] = ' ';
            if (cmd_str[i] == '<' || cmd_str[i] == '>')
                cmd_str[i++] = ' ';
            redir = (t_redir *)redirs->content;
            if (redir->fds[1] != -42)
                remove_fd_num(cmd_str, i, AFTER);
            if (redir->fds[0] != -42)
                remove_fd_num(cmd_str, i, BEFORE);
            i = remove_filename(cmd_str, i);
        }
        if (is_shell_space(cmd_str[i])) //NON ELSE IF perche' remove filename incrementa la i uno di troppo
            cmd_str[i] = ' ';
        i++;
    }
}

static void     remove_fd_num(char *cmd_str, uint32_t idx_redir, int8_t before_after)
{
    uint32_t i;

    i = idx_redir + (before_after == AFTER) - (before_after == BEFORE);
    if (before_after == AFTER)
    {
        if (cmd_str[i] == '&')
            cmd_str[i++] = ' ';
        while (cmd_str[i] && ft_isdigit(cmd_str[i]))
            cmd_str[i++] = ' ';
    }
    else
    {
        while (cmd_str[i] && ft_isdigit(cmd_str[i]))
            cmd_str[i--] = ' ';
    }
}

static uint32_t  remove_filename(char *cmd_str, uint32_t i)
{
    while (cmd_str[i] && is_shell_space(cmd_str[i]))
        cmd_str[i++] = ' ';
    while (cmd_str[i] && !is_shell_space(cmd_str[i]))
        cmd_str[i++] = ' ';
    return (i);
}

static t_list  *fill_redirs(char *cmd_str)
{
    t_list      *redirs;
    uint32_t     i;

    i = 0;
    redirs = NULL;
    while (cmd_str[i])
    {
        if (cmd_str[i] == '<')
        {
            if (cmd_str[i + 1] == '<')
                fill_redir_input(&redirs, cmd_str, ++i, true);
            else
                fill_redir_input(&redirs, cmd_str, i, false);
        }
        if (cmd_str[i] == '>')
        {
            if (cmd_str[i + 1] == '>')
                fill_redir_output(&redirs, cmd_str, ++i, true);
            else
                fill_redir_output(&redirs, cmd_str, i, false);
        }
        i++;
    }
    lstreverse(&redirs);
    return (redirs);
}

static int32_t get_fd_num(char *str, uint32_t idx_redir, uint8_t before_after)
{
    uint32_t    i;
    int32_t     num;

    num = -42;
    i = idx_redir + (before_after == AFTER) - (before_after == BEFORE);
    if (before_after == AFTER)
    {
        if (str[i] == '&' && ft_isdigit(str[i + 1]))
            num = ft_atou(&str[i + 1]);
    }
    else
    {
        while (str[i] && ft_isdigit(str[i]))
            i--;
        if (is_shell_space(str[i]))
            num = ft_atou(&str[i + 1]);
    }
    return (num);
}

static char *get_filename(char *str, uint32_t idx_redir)
{
	char 		*filename;
	uint32_t 	len;
	uint32_t 	i;
	
	filename = NULL;
	i = idx_redir + 1;
	while (str[i] && is_shell_space(str[i]))
		i++;
	len = 0;
	while (str[i + len] && !is_shell_space(str[i + len]))
		len++;
	filename = calloc_p(len + 1, sizeof(char));
	ft_strlcpy(filename, &str[i], len + 1);
	return (filename);
}

static void    fill_redir_input(t_list **redirs, char *str, uint32_t i, bool is_heredoc)
{
    t_redir *node;

    node = init_redir();
    if (is_heredoc == false)
    {
        node->type = REDIR_INPUT_FD;
        node->fds[0] = get_fd_num(str, i, AFTER);
        if (node->fds[0] == -42)
        {
            node->type = REDIR_INPUT;
            node->filename = get_filename(str, i);
        }
    }
    else
    {
        node->type = REDIR_HEREDOC;
        node->filename = get_filename(str, i);
    }
    lstadd_front(redirs, lstnew_p(node));
}

static void     fill_redir_output(t_list **redirs, char *str, uint32_t i, bool is_append)
{
    t_redir *node;

    node = init_redir();
    if (is_append== false)
        node->type = REDIR_OUTPUT_FD;
    else
        node->type = REDIR_APPEND_FD;
    node->fds[1] = get_fd_num(str, i, AFTER);
    node->fds[0] = get_fd_num(str, i, BEFORE);
    if (node->fds[1] == -42)
    {
        if (is_append== false)
            node->type = REDIR_OUTPUT;
        else
            node->type = REDIR_APPEND;
        node->filename = get_filename(str, i);
    }
    if (node->fds[0] == -42)
        node->fds[0] = STDOUT_FILENO;
    lstadd_front(redirs, lstnew_p(node));
}

static t_redir *init_redir(void)
{
    t_redir *redir;

    redir = malloc_p(sizeof(t_redir));
    redir->fds[0] = -42;
    redir->fds[1] = -42;
    redir->filename = NULL;
    redir->type = -1;
    return (redir);
}

static void merge_separators(t_list **lexered_params)
{
    t_list  *node;
    t_lexer *elem;

    node = *lexered_params;
    while (node)
    {
        elem = (t_lexer *)node->content;
        if (elem->token)
        {
            if (elem->token == '&')
                merge_ampersands(lexered_params, node);
            else if (elem->token == '|')
                merge_pipes(lexered_params, node);
            else if (elem->token == '(')
                elem->token = SUBSHELL_START;
            else if (elem->token == ')')
                elem->token = SUBSHELL_END;
            else if (elem->token == ';')
                elem->token = SEMICOLON;
        }
        node = node->next;
    }
}

static void merge_ampersands(t_list **head, t_list *node)
{
    t_lexer *elem;
    t_lexer *next_elem;
    t_lexer *prev_elem;

    //vanno in segfault se non ci sono due elementi prima e dopo (quindi e' da fare attentamente il check_syntax())
    elem = (t_lexer *)node->content;
    next_elem = (t_lexer *)node->next->content;
    prev_elem = (t_lexer *)node->prev->content;
    if (next_elem->token == '&')
        elem->token = AND;
    else
        ft_strcat(prev_elem->cmd_str, "&");
    lstremoveone(head, node->next, &del_content_lexer);
}

static void merge_pipes(t_list **head, t_list *node)
{
    t_lexer *elem;
    t_lexer *next_elem;

    elem = (t_lexer *)node->content;
    next_elem = (t_lexer *)node->next->content;
    if (next_elem->token == '|')
    {
        elem->token = OR;
        lstremoveone(head, node->next, del_content_lexer);
    }
    else
        elem->token = PIPELINE;
}

void    del_content_parser(void *content)
{
    t_tree  *elem;

    elem = (t_tree *)content;
    lstclear(&elem->cmd->redirs, &del_content_redirs);
    free(elem->cmd->cmd_str);
    free(elem->cmd);
}

void    del_content_redirs(void *content)
{
    t_redir *elem;

    elem = (t_redir *)content;
    free(elem->filename);
}

static bool    is_empty_cmd(void *content)
{
    t_lexer *elem;

    elem = (t_lexer *)content;
    if (elem->token == 0 && is_empty_str(elem->cmd_str))
        return (true);
    return (false);
}
