/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build_tree.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fernacar <fernacar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/28 19:34:11 by fernacar          #+#    #+#             */
/*   Updated: 2023/10/11 20:48:26 by fernacar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

// t_tnode	*parse_pipe(char **token_start, char *token_end)
// {
// 	t_tnode	*node;

// 	node = parse_exec(token_start, token_end);
// 	if (peek(token_start, token_end, "|")){
// 		get_token(token_start, token_end, NULL, NULL);
// 		node = construct_pipe(node,
// 				parse_pipe(token_start, token_end));
// 	}
// 	return (node);
// }

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

// t_tnode	*parse_exec(char **token_start, char *token_end)
// {
// 	char	*q, *eq;
// 	char	token;
// 	int		argc;
// 	t_tnode_exec *node;
// 	t_tnode *ret_node;

// 	ret_node = construct_exec();
// 	node = (t_tnode_exec*)ret_node;

// 	argc = 0;
// 	ret_node = parse_redir(ret_node, token_start, token_end);
// 	while (!peek(token_start, token_end, "|"))
// 	{
// 		if ((token = get_token(token_start, token_end, &q, &eq)) == 0)
// 			break ;
// 		if (token != 'a')
// 			panic("syntax ??");
// 		node->argv[argc] = q;
// 		node->eargv[argc] = eq;
// 		argc++;
// 		ret_node = parse_redir(ret_node, token_start, token_end);
// 	}
// 	node->argv[argc] = NULL;
// 	node->eargv[argc] = NULL;
// 	return (ret_node);
// }

t_tnode	*parse_exec(char ***tokens)
{
	char	*q, *eq;
	char	token;
	int		argc;
	t_tnode_exec *node;
	t_tnode *ret_node;

	ret_node = construct_exec();
	node = (t_tnode_exec*)ret_node;

	argc = 0;
	ret_node = parse_redir(ret_node, tokens);
	while (**tokens != NULL && ft_strcmp(**tokens, "|") != 0)
	{
		if (get_token_type(**tokens) != 'a')
			panic("syntax ??");
		node->argv[argc] = **tokens;
		node->eargv[argc] = NULL;
		argc++;
		(*tokens)++;
		ret_node = parse_redir(ret_node, tokens);
	}
	node->argv[argc] = NULL;
	node->eargv[argc] = NULL;
	return (ret_node);
}

// t_tnode	*parse_redir(t_tnode *node, char **token_start, char *token_end)
// {
// 	int	token;
// 	char *q, *eq;

// 	while (peek(token_start, token_end, "<>"))
// 	{
// 		token = get_token(token_start, token_end, NULL, NULL);
// 		if (get_token(token_start, token_end, &q, &eq) != 'a')
// 			panic("missing file for redirection");
// 		if (token == '<')
// 			node = construct_redir(node, q, eq, O_RDONLY, STDIN_FILENO);
// 		else if (token == '>')
// 			node = construct_redir(node, q, eq, O_WRONLY | O_CREAT | O_TRUNC, STDOUT_FILENO);
// 		else if (token == '+')
// 			node = construct_redir(node, q, eq, O_CREAT | O_WRONLY | O_APPEND, STDOUT_FILENO);
// 		else if (token == '-') // heredoc stuff
// 			node = construct_heredoc(node, q, eq);
// 			// panic("heredoc TBA"); // maybe change to its own parse_heredoc :think:
// 	}
// 	return (node);
// }

char	get_token_type(char *token)
{
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
			node = construct_redir(node, **tokens, NULL, O_RDONLY, STDIN_FILENO);
		else if (token_type == '>')
			node = construct_redir(node, **tokens, NULL, O_WRONLY | O_CREAT | O_TRUNC, STDOUT_FILENO);
		else if (token_type == '+')
			node = construct_redir(node, **tokens, NULL, O_CREAT | O_WRONLY | O_APPEND, STDOUT_FILENO);
		else if (token_type == '-') // heredoc stuff
			node = construct_heredoc(node, **tokens, NULL);
			// panic("heredoc TBA"); // maybe change to its own parse_heredoc :think:
		(*tokens)++;
	}
	return (node);
}

// t_tnode *build_tree(char *input)
// {
// 	char	*token_end;
// 	t_tnode	*node;

// 	token_end = input + ft_strlen(input);
// 	// creates the tree and returns the root node
// 	node = parse_pipe(&input, token_end);
// 	// checks if the whole input was analysed
// 	peek(&input, token_end, "");
// 	// if not then it prints whats left and calls panic
// 	if(input != token_end){
// 		printf("leftovers: %s\n", input);
// 		panic("syntax");
// 	}
// 	// replaces all the program names and stuff with NULL char for all nodes
// 	nulterminate(node);
// 	return (node);
// }

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
