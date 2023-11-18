/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_list.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fernacar <fernacar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/05 21:28:15 by fernacar          #+#    #+#             */
/*   Updated: 2023/11/16 21:48:59 by fernacar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	is_delimiter(char c)
{
	return (c == ' ' || c == '\t' || c == '|' || c == '<' || c == '>');
}

static void	skip_to_token_end(char *input, int *i)
{
	int	quote_type;

	if (input[*i] == '|' || input[*i] == '<' || input[*i] == '>')
	{
		if (input[*i + 1] && (input[*i] == '<' || input[*i] == '>')
			&& input[*i] == input[*i + 1])
			*i = *i + 1;
		*i = *i + 1;
		return ;
	}
	quote_type = 0;
	while (input[*i] && !is_delimiter(input[*i]))
	{
		if (input[*i] == '\"' || input[*i] == '\'')
		{
			quote_type = input[*i];
			*i = *i + 1;
			while (input[*i] && input[*i] != quote_type)
				*i = *i + 1;
			if (input[*i])
				*i = *i + 1;
			return ;
		}
		*i = *i + 1;
	}
}

static int	count_tokens(char *input)
{
	int		count;
	int		i;

	count = 0;
	i = 0;
	while (input[i])
	{
		if (input[i] != ' ' && input[i] != '\t')
		{
			count++;
			skip_to_token_end(input, &i);
			continue ;
		}
		i++;
	}
	return (count);
}

char	**make_token_list(char *input, char **env, int exit_status)
{
	char	**token_list;
	int		token_count;

	token_count = count_tokens(input);
	token_list = ft_calloc(token_count + 1, sizeof(char *));
	if (!token_list)
		panic("token allocation error");
	fill_token_list(token_list, input);
	handle_tokens(token_list, env, exit_status);
	return (token_list);
}
