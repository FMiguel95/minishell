/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fernacar <fernacar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/28 19:38:14 by fernacar          #+#    #+#             */
/*   Updated: 2023/10/17 20:25:03 by fernacar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../include/minishell.h"

// checks if the next non white space character is in tokens and advances the input buffer
int	peek(char **start, char *end, char *tokens)
{
	char	*str;

	str = *start;
	while (str < end && ft_strchr(" \t\r\v\n", *str))
		str++;
	*start = str;
	return (*str && ft_strchr(tokens, *str));
}

void	parse_quotes(char **str, char type)
{
	(*str)++;
	while (**str && **str != type)
		(*str)++;
	if (**str == type)
		**str = ' ';
}

// return the code for the next token and advances the input buffer
char	get_token(char **token_start, char *token_end, char **q, char **eq)
{
	char 	*str;
	char	result;

	str = *token_start;
	// skip white space at start
	while (str < token_end && ft_strchr(" \t\r\n\v", *str))
		str++;
	if (q)
		*q = str;
	result = *str;
	// check what the token is
	if (*str && *str == '|')
		str++;
	else if (*str && *str == '>')
	{
		str++;
		if (*str == '>')
		{
			result = '+';
			str++;
		}
	}
	else if (*str && *str == '<')
	{
		str++;
		if (*str == '<')
		{
			result = '-';
			str++;
		}
	}
	else if (*str)
	{
		result = 'a';
		// handle quotes
		if (*str == '\"' || *str == '\'')
		{
			if (q)
				(*q)++;
			parse_quotes(&str, *str);
		}
		// skip to the end of the current token
		while (str < token_end && !ft_strchr(" \t\r\n\v", *str)
				&& !ft_strchr("<|>", *str))
			str++;
	}
	if (eq)
		*eq = str;
	// skip to the start of the next token
	while (str < token_end && ft_strchr(" \t\r\n\v", *str))
		str++;
	*token_start = str;
	return (result);
}

t_tnode *nulterminate(t_tnode *node)
{
	t_tnode_exec	*exec_node;
	t_tnode_pipe	*pipe_node;
	t_tnode_redir	*redir_node;
	t_tnode_heredoc	*heredoc_node;

	if (node == NULL)
		return (NULL);
	if (node->type == EXEC)
	{
		exec_node = (t_tnode_exec*)node;
		int	i = 0;
		while (exec_node->argv[i] != NULL)
		{
			*exec_node->eargv[i] = '\0';
			i++;
		}
	}
	else if (node->type == REDIR)
	{
		redir_node = (t_tnode_redir*)node;
		nulterminate(redir_node->node);
		// *redir_node->efile = '\0';
	}
	else if (node->type == HEREDOC)
	{
		heredoc_node = (t_tnode_heredoc*)node;
		nulterminate(heredoc_node->node);
		*heredoc_node->delm_end = '\0';
	}
	else if (node->type == PIPE)
	{
		pipe_node = (t_tnode_pipe *)node;
		nulterminate(pipe_node->branch_left);
		nulterminate(pipe_node->branch_right);
	}
	return (node);
}
