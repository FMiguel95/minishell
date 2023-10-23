/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_list.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fernacar <fernacar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/05 21:28:15 by fernacar          #+#    #+#             */
/*   Updated: 2023/10/23 19:44:23 by fernacar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static int	is_delimiter(char c)
{
	return (c == ' ' || c == '\t' || c == '|' || c == '<' || c == '>');
}

static int	get_token_size(char *token)
{
	return (0);
}

static int	count_tokens(char *input)
{
	int		count;
	char	quote_type;
	int		i;

	count = 0;
	i = 0;
	while (input[i])
	{
		if (input[i] != ' ' && input[i] != '\t')
		{
			count++;
			if (input[i] == '|' || input[i] == '<' || input[i] == '>')
			{
				if (input[i + 1] && (input[i] == '<' || input[i] == '>') && input[i] == input[i + 1])
					i++;
				i++;
				continue ;
			}
			quote_type = 0;
			while (input[i] && !is_delimiter(input[i]))
			{
				if (input[i] == '\"' || input[i] == '\'')
				{
					quote_type = input[i];
					i++;
					while (input[i] && input[i] != quote_type)
						i++;
				}
				i++;
			}
			continue ;
		}
		i++;
	}
	return (count);
}

static void	fill_list(char **token_list, char *input)
{
	char	quote_type;
	int		i;
	int		token_start_pos;
	int		token_end_pos;
	int		token_count;

	token_count = 0;
	i = 0;
	while (input[i])
	{
		if (input[i] != ' ' && input[i] != '\t')
		{
			token_start_pos = i;
			if (input[i] == '|' || input[i] == '<' || input[i] == '>')
			{
				if (input[i + 1] && (input[i] == '<' || input[i] == '>') && input[i] == input[i + 1])
					i++;
				token_end_pos = i;
				token_list[token_count++] = ft_substr(input, token_start_pos, token_end_pos - token_start_pos + 1);
				i++;
				continue ;
			}
			quote_type = 0;
			while (input[i] && !is_delimiter(input[i]))
			{
				if (input[i] == '\"' || input[i] == '\'')
				{
					quote_type = input[i];
					i++;
					while (input[i] && input[i] != quote_type)
						i++;
				}
				i++;
			}
			token_list[token_count++] = ft_substr(input, token_start_pos, i - token_start_pos);
			continue ;
		}
		i++;
	}
}

static char	*handle_quotes_str(char *token)
{
	char	*new_str;
	char	*temp;
	char	*substr;
	int		i;
	int		sub_start;
	char	quote_type;

	new_str = ft_strdup("");
	quote_type = 0;
	i = 0;
	sub_start = 0;
	while (token[i])
	{
		if (token[i] == '$' && quote_type != '\'')	// expand variables (it found a $ char and it's not inside a ' quote block)
		{
			// add to the str what came before
			substr = ft_substr(token, sub_start, i - sub_start);
			temp = ft_strjoin(new_str, substr);
			free(new_str);
			free(substr);
			new_str = temp;
			sub_start = i + 1; // mark the start of the variable name
			// lets advance until the end of the variable name (until it finds a space, quote, or the token ends)
			while (token[i] && !ft_isspace(token[i]) && token[i] != '\'' && token[i] != '\"')
				i++;
			// create a char* with just the variable name
			substr = ft_calloc(i - sub_start + 1, 1);
			ft_strlcat(substr, token + sub_start, i - sub_start + 1);
			// get the variable value with getenv and add it to the str
			temp = ft_strjoin(new_str, getenv(substr));
			free(new_str);
			free(substr);
			new_str = temp;
			sub_start = i;
			continue ;
		}
		if ((!quote_type && (token[i] == '\"' || token[i] == '\'')) || token[i] == quote_type)
		{
			substr = ft_substr(token, sub_start, i - sub_start);
			temp = ft_strjoin(new_str, substr);
			free(new_str);
			free(substr);
			new_str = temp;
			if (!quote_type)
				quote_type = token[i];
			else
				quote_type = 0;
			sub_start = i + 1;
		}
		i++;
	}
	substr = ft_substr(token, sub_start, i - sub_start);
	temp = ft_strjoin(new_str, substr);
	free(new_str);
	free(substr);
	new_str = temp;
	return (new_str);
}

static void	handle_quotes(char **token_list)
{
	char	*temp;

	while (*token_list)
	{
		temp = *token_list;
		*token_list = handle_quotes_str(temp);
		free(temp);
		token_list++;
	}
}

char	**make_token_list(char *input)
{
	char	**token_list;
	int		token_count;
	
	token_count = count_tokens(input);
	token_list = malloc(sizeof(char *) * (token_count + 1));
	if (!token_list)
		panic("token allocation error");
	token_list[token_count] = NULL;
	fill_list(token_list, input);
	// print_list(token_list);
	handle_quotes(token_list);
	// print_list(token_list);
	return (token_list);
}

void print_list(char **list)
{
	int i = 0;
	while (list[i] != NULL)
	{
		printf("%s$\n", list[i]);
		i++;
	}
}
