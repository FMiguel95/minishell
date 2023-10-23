/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   constructors.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fernacar <fernacar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/28 18:55:28 by fernacar          #+#    #+#             */
/*   Updated: 2023/10/23 16:40:36 by fernacar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"


t_tnode	*construct_exec(void)
{
	t_tnode_exec	*node;

	node = malloc(sizeof(t_tnode_exec));
	if (!node)
		panic("allocation error");
	ft_bzero(node, sizeof(node));
	node->argv = malloc(sizeof(char *) * 1024);
	node->eargv = malloc(sizeof(char *) * 1024);
	if (!node->argv || !node->eargv)
		panic("allocation error");
	node->type = EXEC;
	return ((t_tnode *)node);
}

t_tnode	*construct_redir(t_tnode *subnode, char *file, int flags, int fd)
{
	t_tnode_redir	*node;

	node = malloc(sizeof(t_tnode_redir));
	if (!node)
		panic("allocation error");
	ft_bzero(node, sizeof(node));
	node->type = REDIR;
	node->node = subnode;
	node->file = file;
	node->flags = flags;
	node->fd = fd;
	return ((t_tnode *)node);
}

t_tnode	*construct_heredoc(t_tnode *subnode, char *delm)
{
	t_tnode_heredoc	*node;

	node = malloc(sizeof(t_tnode_heredoc));
	if (!node)
		panic("allocation error");
	ft_bzero(node, sizeof(node));
	node->type = HEREDOC;
	node->node = subnode;
	node->delm = delm;
	return ((t_tnode *)node);
}

t_tnode	*construct_pipe(t_tnode *left, t_tnode *right)
{
	t_tnode_pipe	*node;

	node = malloc(sizeof(t_tnode_pipe));
	if (!node)
		panic("allocation error");
	ft_bzero(node, sizeof(node));
	node->type = PIPE;
	node->branch_left = left;
	node->branch_right = right;
	return ((t_tnode *)node);
}
