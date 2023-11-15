/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build_tree.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fernacar <fernacar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/28 19:34:11 by fernacar          #+#    #+#             */
/*   Updated: 2023/11/13 19:31:33 by fernacar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static void	get_redir_node(t_tnode **node, char ***tokens,
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

static t_tnode	*parse_redir(t_tnode *node, char ***tokens, int *invalid_syntax)
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

static void	insert_redir_node(char ***tokens, t_tnode **ret_node,
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

static t_tnode	*parse_exec(char ***tokens, int *invalid_syntax)
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

char	get_token_type(char *token)
{
	if (!token)
		return ('\0');
	if (strcmp(token, "|") == 0)
		return ('|');
	else if (strcmp(token, "<") == 0)
		return ('<');
	else if (strcmp(token, ">") == 0)
		return ('>');
	else if (strcmp(token, "<<") == 0)
		return ('-');
	else if (strcmp(token, ">>") == 0)
		return ('+');
	else
		return ('a');
}

static t_tnode	*parse_pipe(char ***tokens, int *invalid_syntax)
{
	t_tnode	*node;

	node = parse_exec(tokens, invalid_syntax);
	if (**tokens != NULL && ft_strcmp(**tokens, "|") == 0)
	{
		(*tokens)++;
		if (**tokens == NULL)
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
