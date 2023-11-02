/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_tree.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fernacar <fernacar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/28 19:38:14 by fernacar          #+#    #+#             */
/*   Updated: 2023/11/02 17:22:17 by fernacar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../include/minishell.h"

void	free_node(t_tnode *node)
{
	t_tnode_exec	*exec_node;
	t_tnode_pipe	*pipe_node;
	t_tnode_redir	*redir_node;
	t_tnode_heredoc	*heredoc_node;

	if (node && node->type == EXEC)
	{
		exec_node = (t_tnode_exec*)node;
		free(exec_node->argv);
		free(exec_node->eargv);
		free(exec_node);
	}
	else if (node && node->type == REDIR)
	{
		redir_node = (t_tnode_redir*)node;
		free_node(redir_node->node);
		free(redir_node);
	}
	else if (node && node->type == PIPE)
	{
		pipe_node = (t_tnode_pipe*)node;
		free_node(pipe_node->branch_left);
		free_node(pipe_node->branch_right);
		free(pipe_node);
	}
	else if (node && node->type == HEREDOC)
	{
		heredoc_node = (t_tnode_heredoc*)node;
		free_node(heredoc_node->node);
		free(heredoc_node->input);
		free(heredoc_node);
	}
}

void	print_node(t_tnode *node)
{
	t_tnode_exec	*exec_node;
	t_tnode_pipe	*pipe_node;
	t_tnode_redir	*redir_node;
	t_tnode_heredoc	*heredoc_node;

	if (node->type == EXEC)
	{
		exec_node = (t_tnode_exec*)node;
		printf("exec: %s\n", exec_node->argv[0]);
	}
	else if (node->type == REDIR)
	{
		redir_node = (t_tnode_redir*)node;
		printf("redir: %s\n", redir_node->file);
		print_node(redir_node->node);
	}
	else if (node->type == PIPE)
	{
		pipe_node = (t_tnode_pipe*)node;
		printf("pipe\n");
		print_node(pipe_node->branch_left);
		print_node(pipe_node->branch_right);
	}
	else if (node->type == HEREDOC)
	{
		heredoc_node = (t_tnode_heredoc*)node;
		printf("heredoc: %s\n", heredoc_node->delm);
		print_node(heredoc_node->node);
	}
}
