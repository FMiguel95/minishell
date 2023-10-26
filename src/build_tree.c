/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build_tree.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fernacar <fernacar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/28 19:34:11 by fernacar          #+#    #+#             */
/*   Updated: 2023/10/25 22:51:53 by fernacar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

t_tnode	*parse_pipe(char ***tokens)
{
	t_tnode	*node;

	node = parse_exec(tokens);
	if (**tokens != NULL && ft_strcmp(**tokens, "|") == 0)
	{
		(*tokens)++;
		node = construct_pipe(node,
				parse_pipe(tokens));
	}
	return (node);
}

t_tnode	*parse_exec(char ***tokens)
{
	int				argc;
	t_tnode_exec	*node;
	t_tnode 		*ret_node;
	t_tnode 		*newest_node;
	t_tnode 		*prev_node;
	t_tnode 		*exec_node;

	exec_node = construct_exec();
	ret_node = exec_node;
	node = (t_tnode_exec*)ret_node;

	argc = 0;
	// ret_node = parse_redir(ret_node, tokens);
	while (**tokens != NULL && ft_strcmp(**tokens, "|") != 0)
	{
		if (get_token_type(**tokens) != 'a')
		{
			if (ret_node->type == EXEC)
			{
				ret_node = parse_redir(exec_node, tokens);
				prev_node = ret_node;
				continue ;
			}
			newest_node = parse_redir(exec_node, tokens);
			if (prev_node->type == REDIR)
				((t_tnode_redir *)prev_node)->node = newest_node;
			else if (prev_node->type == HEREDOC)
				((t_tnode_heredoc *)prev_node)->node = newest_node;
			prev_node = newest_node;
			continue ;
		}
		// while (**tokens != NULL && get_token_type(**tokens) != 'a' && get_token_type(**tokens) != '|')
		// 	ret_node = parse_redir(ret_node, tokens);
		// if (get_token_type(**tokens) != 'a')
		// 	panic("syntax ??");
		node->argv[argc] = **tokens;
		argc++;
		(*tokens)++;
		// ret_node = parse_redir(ret_node, tokens);
	}
	node->argv[argc] = NULL;
	node->eargv[argc] = NULL;
	return (ret_node);
}

char	get_token_type(char *token)
{
	if (!token)
		return ('\0');
	if (strcmp(token, "<") == 0)
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

t_tnode	*parse_redir(t_tnode *node, char ***tokens)
{
	char	token_type;

	if (**tokens == NULL)
		return (node);
	token_type = get_token_type(**tokens);
	if (token_type != 'a')
	{
		(*tokens)++;
		if (get_token_type(**tokens) != 'a')
			panic("missing file for redirection");
		if (token_type == '<')
			node = construct_redir(node, **tokens, O_RDONLY, STDIN_FILENO);
		else if (token_type == '>')
			node = construct_redir(node, **tokens, O_WRONLY | O_CREAT | O_TRUNC, STDOUT_FILENO);
		else if (token_type == '+')
			node = construct_redir(node, **tokens, O_CREAT | O_WRONLY | O_APPEND, STDOUT_FILENO);
		else if (token_type == '-')
			node = construct_heredoc(node, **tokens);
		(*tokens)++;
	}
	return (node);
}

t_tnode *build_tree(char **tokens)
{
	t_tnode	*node;

	// creates the tree and returns the root node
	node = parse_pipe(&tokens);
	// checks if the whole input was analysed
	// if not then it prints whats left and calls panic
	if(*tokens != NULL){
		printf("leftovers:\n");
		print_list(tokens);
		panic("syntax");
	}
	return (node);
}
