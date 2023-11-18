/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build_tree_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fernacar <fernacar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/16 21:10:33 by fernacar          #+#    #+#             */
/*   Updated: 2023/11/16 21:14:41 by fernacar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	get_redir_node(t_tnode **node, char ***tokens,
		char token_type, int *invalid_syntax)
{
	if (get_token_type(**tokens) != 'a')
	{
		*invalid_syntax = 1;
		return ;
	}
	if (token_type == '<')
		*node = construct_redir(*node, **tokens, O_RDONLY, STDIN_FILENO);
	else if (token_type == '>')
		*node = construct_redir(*node, **tokens,
				O_WRONLY | O_CREAT | O_TRUNC, STDOUT_FILENO);
	else if (token_type == '+')
		*node = construct_redir(*node, **tokens,
				O_CREAT | O_WRONLY | O_APPEND, STDOUT_FILENO);
	else if (token_type == '-')
		*node = construct_heredoc(*node, **tokens);
	(*tokens)++;
}

void	insert_redir_node(char ***tokens, t_tnode **ret_node,
		t_tnode *exec_node, int *invalid_syntax)
{
	t_tnode			*newest_node;
	static t_tnode	*prev_node;

	if ((*ret_node)->type == EXEC)
	{
		*ret_node = parse_redir(exec_node, tokens, invalid_syntax);
		prev_node = *ret_node;
		return ;
	}
	newest_node = parse_redir(exec_node, tokens, invalid_syntax);
	if (prev_node->type == REDIR)
		((t_tnode_redir *)prev_node)->node = newest_node;
	else if (prev_node->type == HEREDOC)
		((t_tnode_heredoc *)prev_node)->node = newest_node;
	prev_node = newest_node;
	return ;
}
