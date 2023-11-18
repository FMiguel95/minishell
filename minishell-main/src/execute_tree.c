/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_tree.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fernacar <fernacar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/03 18:36:37 by fernacar          #+#    #+#             */
/*   Updated: 2023/11/16 22:26:40 by fernacar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	execute_node_redir(t_tnode_redir *redir_node, t_minishell *data)
{
	int	new_fd;

	new_fd = open(redir_node->file, redir_node->flags, 0666);
	if (new_fd < 0)
	{
		perror(redir_node->file);
		data->exit_status = 1;
		exit(data->exit_status);
	}
	if (dup2(new_fd, redir_node->fd) == -1)
		panic("dup2 error");
	close(new_fd);
	if (redir_node->node)
		execute_node(redir_node->node, data);
}

void	execute_node_heredoc(t_tnode_heredoc *heredoc_node, t_minishell *data)
{
	int	fd;

	fd = open(".heredoc", O_WRONLY | O_CREAT | O_TRUNC, 0600);
	write(fd, heredoc_node->input, ft_strlen(heredoc_node->input));
	close(fd);
	fd = open(".heredoc", O_RDONLY, 0400);
	if (dup2(fd, STDIN_FILENO) == -1)
		panic("dup2 error");
	close(fd);
	if (heredoc_node->node)
		execute_node(heredoc_node->node, data);
	unlink(".heredoc");
}

void	execute_node(t_tnode *node, t_minishell *data)
{
	if (node == NULL)
		panic("null node");
	if (node->type == EXEC)
		execute_node_exec((t_tnode_exec *)node, data);
	else if (node->type == REDIR)
		execute_node_redir((t_tnode_redir *)node, data);
	else if (node->type == PIPE)
		execute_node_pipe((t_tnode_pipe *)node, data);
	else if (node->type == HEREDOC)
		execute_node_heredoc((t_tnode_heredoc *)node, data);
	else
		panic("invalid node type");
}
