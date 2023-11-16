/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build_tree.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fernacar <fernacar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/28 19:34:11 by fernacar          #+#    #+#             */
/*   Updated: 2023/11/16 21:17:17 by fernacar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

t_tnode	*parse_redir(t_tnode *node, char ***tokens, int *invalid_syntax)
{
	char	token_type;

	if (**tokens == NULL)
		return (node);
	token_type = get_token_type(**tokens);
	if (token_type != 'a')
	{
		(*tokens)++;
		get_redir_node(&node, tokens, token_type, invalid_syntax);
	}
	return (node);
}

t_tnode	*parse_exec(char ***tokens, int *invalid_syntax)
{
	int				argc;
	t_tnode_exec	*node;
	t_tnode			*ret_node;
	t_tnode			*exec_node;

	exec_node = construct_exec();
	ret_node = exec_node;
	node = (t_tnode_exec *)ret_node;
	argc = 0;
	while (**tokens != NULL && ft_strcmp(**tokens, "|") != 0)
	{
		if (get_token_type(**tokens) != 'a')
		{
			insert_redir_node(tokens, &ret_node, exec_node, invalid_syntax);
			continue ;
		}
		node->argv[argc] = **tokens;
		argc++;
		(*tokens)++;
	}
	node->argv[argc] = NULL;
	node->eargv[argc] = NULL;
	return (ret_node);
}

t_tnode	*parse_pipe(char ***tokens, int *invalid_syntax)
{
	t_tnode	*node;

	node = parse_exec(tokens, invalid_syntax);
	if (**tokens != NULL && ft_strcmp(**tokens, "|") == 0)
	{
		(*tokens)++;
		if (**tokens == NULL || ft_strcmp(**tokens, "|") == 0)
			*invalid_syntax = 1;
		node = construct_pipe(node,
				parse_pipe(tokens, invalid_syntax));
	}
	return (node);
}

t_tnode	*build_tree(char **tokens, int *exit_status)
{
	t_tnode	*node;
	int		invalid_syntax;

	invalid_syntax = 0;
	if (get_token_type(*tokens) == '|')
		invalid_syntax = 1;
	node = parse_pipe(&tokens, &invalid_syntax);
	if (invalid_syntax)
	{
		ft_putendl_fd("bad syntax", STDERR_FILENO);
		free_node(node);
		*exit_status = 2;
		return (NULL);
	}
	return (node);
}
