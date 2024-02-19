/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <bomboclat@bidol.juis>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/19 17:46:08 by craimond          #+#    #+#             */
/*   Updated: 2024/02/19 15:33:58 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/minishell.h"

static void     launch_commands(t_tree *node, int8_t prev_type, int fds[3]);
static void     launch_standard_cmd(t_tree *node, int8_t prev_type, int fds[3]);
static void     launch_builtin_cmd(t_tree *node, int8_t prev_type, int fds[3]);
static void     child(t_tree *elem, int fds[3], int8_t prev_type);
static void     parent(pid_t pid, int fds[3], t_tree *node);
static void     exec_redirs(t_list *redirs);
static void     exec_fd_redirs(t_redir *redir);
static void     exec_file_redirs(t_redir *redir);
static int      open_redir_file(t_redir *redir);
static void     wait_for_children(t_tree *parsed_params);
static uint16_t get_n_pipelines(t_tree *parsed_params);

void    executor(t_tree *parsed_params)
{
    int     original_status;
    int     heredoc_status;
    int     fds[5] = {-42, -42, -42, -42, -42}; //pipe read, pipe write, prev_output, original stdin, original stdout

    fds[3] = dup_p(STDIN_FILENO);
    fds[4] = dup_p(STDOUT_FILENO);
    original_status = g_status;
    heredoc_status = 0;
    create_heredocs(parsed_params, &heredoc_status);
    dup2(fds[3], STDIN_FILENO); //altrimenti se heredoc e' terminato con segnale 130, il comando dopo continua a scrivere nell'fd dell'heredoc
    if (heredoc_status != 0)
    {
        if (heredoc_status == 130)
            g_status = 130;
        return ;
    }
    g_status = original_status; //fai fallire un comando, fai '<< here echo $?' scrivendo qualsiasi cosa nell heredoc, e vedi che echo $? ritorna l'errore del comando precedente (non 0 anche se heredoc e' stato eseguito correttamente)
    set_signals(S_COMMAND);
    launch_commands(parsed_params, -1, fds);
    wait_for_children(parsed_params);
    dup2(fds[3], STDIN_FILENO);
}

static t_tree   *skip_till_semicolon(t_tree *node)
{
    if (!node)
        return (NULL);
    if (node->type == SEMICOLON)
        return (node->right);
    return (skip_till_semicolon(node->right));
}

//Yes, when you use pipes within a subshell, such as in ( cmd1 | cmd2 | cmd3 ), 
//the subshell will wait for the entire pipeline of commands to complete before it returns 
//control to the rest of the script or command line. Each command in the pipeline 
//is executed in parallel in its own sub-process: cmd1's output is piped into
//cmd2, and cmd2's output is piped into cmd3. The subshell waits for the last
//command in the pipeline (cmd3 in this case) to complete, along with 
//all preceding commands in the pipeline, before it finishes.

static void launch_commands(t_tree *node, int8_t prev_type, int fds[3])
{
    if (!node)
        return ; //se e' exit non funziona sleep 2 | ls
    if (node->type != CMD)
    {
        if (node->type == PIPELINE)
            pipe_p(fds);
        if (node->left->cmd && is_builtin(node->left->cmd->cmd_str))
            launch_builtin_cmd(node, prev_type, fds);
        else
            launch_standard_cmd(node, prev_type, fds);
        if ((node->type == AND && g_status != 0) || (node->type == OR && g_status == 0))
            launch_commands(skip_till_semicolon(node), -1, fds);
        else
            launch_commands(node->right, node->type, fds);
        signal(SIGPIPE, SIG_DFL);
        return ;
    }
    child(node, fds, prev_type);
}

// Yes, you are correct. When Bash executes built-in commands,
// these commands run directly within the context of the current shell process,
// rather than in a new child process. This means that for a pipeline
// of built-in commands, the commands are executed sequentially, not in parallel
// as they would be if each command in the pipeline were an external command that
// required a new process to be spawned.
static void launch_builtin_cmd(t_tree *node, int8_t prev_type, int fds[3])
{
    if (node->type == PIPELINE)
    {
        dup2_p(fds[1], STDOUT_FILENO);
        reset_fd(&fds[1]);
    }
    fds[2] = fds[0];
    launch_commands(node->left, prev_type, fds);
    dup2_p(fds[4], STDOUT_FILENO);
}

static void launch_standard_cmd(t_tree *node, int8_t prev_type, int fds[3])
{
    pid_t   pid;

    pid = fork_p();
    if (pid == 0)
    {
        if (node->type == PIPELINE)
        {
            dup2_p(fds[1], STDOUT_FILENO);
            reset_fd(&fds[0]);
            reset_fd(&fds[1]);
        }
        launch_commands(node->left, prev_type, fds);
        wait_for_children(node->left); //deve stare qua, non su. in questo modo anche le subshells aspettano le pipe al loro interno VEDI SOPRA
        exit(g_status); //lui e' l'unico che deve uscire perche' e' il figlio
    }
    else
        parent(pid, fds, node);
}

static void child(t_tree *elem, int fds[3], int8_t prev_type)
{
    if (prev_type == PIPELINE)
        dup2_p(fds[2], STDIN_FILENO);
    exec_redirs(elem->cmd->redirs);
    elem->cmd->cmd_str = replace_env_vars(elem->cmd->cmd_str);
    elem->cmd->cmd_str = replace_wildcards(elem->cmd->cmd_str);
    exec(ft_getenv("PATH"), elem->cmd->cmd_str);
}

static void parent(pid_t pid, int fds[3], t_tree *node)
{
    reset_fd(&fds[1]);
    fds[2] = fds[0];
    if (node->type != PIPELINE)
    {
        waitpid_p(pid, &g_status, 0);
        g_status = WEXITSTATUS(g_status);
        reset_fd(&fds[0]); //se dopo non c'e' pipe chiude la pipeline
    }
}

static void exec_redirs(t_list *redirs)
{
    t_redir *redir;

    while (redirs)
    {
        redir = (t_redir *)redirs->content;
        exec_fd_redirs(redir);
        exec_file_redirs(redir);
        redirs = redirs->next;
    }
}

static void exec_fd_redirs(t_redir *redir)
{
    if (redir->fds[0] != -42)
        dup2_p(redir->fds[0], STDIN_FILENO);
    if (redir->fds[1] != -42)
        dup2_p(redir->fds[1], STDOUT_FILENO);
    reset_fd(&redir->fds[0]);
    reset_fd(&redir->fds[1]);
}

static void exec_file_redirs(t_redir *redir)
{
    int file_fd;
    int dup_target;

    file_fd = -42;
    if (redir->filename[0] == '$')
        redir->filename = replace_env_vars(redir->filename);
    file_fd = open_redir_file(redir);
    if (redir->type == REDIR_INPUT || redir->type == REDIR_INPUT_FD || redir->type == REDIR_HEREDOC)
        dup_target = STDIN_FILENO;
    else if (redir->type == REDIR_OUTPUT || redir->type == REDIR_OUTPUT_FD || redir->type == REDIR_APPEND_FD)
        dup_target = STDOUT_FILENO;
    dup2_p(file_fd, dup_target);
    reset_fd(&file_fd);
}

static int  open_redir_file(t_redir *redir)
{
    int     fd;

    fd = -1;
    if (redir->type == REDIR_INPUT)
        fd = open_p(redir->filename, O_RDONLY, 0644);
    else if (redir->type == REDIR_HEREDOC)
        fd = open_p(get_heredoc_filename(redir->heredoc_fileno), O_RDONLY, 0644);
    else if (redir->type == REDIR_OUTPUT)
        fd = open_p(redir->filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    else if (redir->type == REDIR_APPEND)
        fd = open_p(redir->filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
    return (fd);
}

static void wait_for_children(t_tree *node) //aspetta tutti i figli (apparte quelli che erano gia stati aspettati, ovvero ; | || e &&)
{
    uint16_t    n_to_wait;

    n_to_wait = get_n_pipelines(node); //numero di pipeline SULLO STESSO LAYER
    while (n_to_wait--)
    {
        waitpid_p(0, &g_status, 0);
        g_status = WEXITSTATUS(g_status);
        //funziona cosi', exit non e' immediato. basta guardare il caso 'sleep 3 && (sleep 2 | exit)'
    }
}

static uint16_t get_n_pipelines(t_tree *node)
{
    uint16_t    n;

    n = 0;
    if (!node)
        return (n);
    if (node->type == PIPELINE)
        if (node->left->type == CMD && !is_builtin(node->left->cmd->cmd_str))
            n++;
    n += get_n_pipelines(node->right); //va solo a destra per contare quelle sullo stesso layer
    return (n);
}

