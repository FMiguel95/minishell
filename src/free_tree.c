/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_tree.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fernacar <fernacar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/28 19:38:14 by fernacar          #+#    #+#             */
/*   Updated: 2023/11/13 19:03:49 by fernacar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	free_node(t_tnode *node)
{
	if (node && node->type == EXEC)
	{
		free(((t_tnode_exec *)node)->argv);
		free(((t_tnode_exec *)node)->eargv);
		free(((t_tnode_exec *)node));
	}
	else if (node && node->type == REDIR)
	{
		free_node(((t_tnode_redir *)node)->node);
		free(((t_tnode_redir *)node));
	}
	else if (node && node->type == PIPE)
	{
		free_node(((t_tnode_pipe *)node)->branch_left);
		free_node(((t_tnode_pipe *)node)->branch_right);
		free(((t_tnode_pipe *)node));
	}
	else if (node && node->type == HEREDOC)
	{
		free_node(((t_tnode_heredoc *)node)->node);
		free(((t_tnode_heredoc *)node)->input);
		free(((t_tnode_heredoc *)node));
	}
}
