/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fill_token_list.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fernacar <fernacar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/16 21:46:34 by fernacar          #+#    #+#             */
/*   Updated: 2023/11/16 21:47:26 by fernacar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static int	fill_list_operator(char **tkn_lst, char *input, int *i, int *start)
{
	if (input[*i] == '|' || input[*i] == '<' || input[*i] == '>')
	{
		if (input[*i + 1] && (input[*i] == '<' || input[*i] == '>')
			&& input[*i] == input[*i + 1])
			*i = *i + 1;
		while (*tkn_lst)
			tkn_lst++;
		*tkn_lst = ft_substr(input, *start, *i - *start + 1);
		*i = *i + 1;
		return (1);
	}
	return (0);
}

static void	fill_list_noun(char **tkn_lst, char *input, int *i, int *start)
{
	int	quote_type;

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
			continue ;
		}
		*i = *i + 1;
	}
	while (*tkn_lst)
		tkn_lst++;
	*tkn_lst = ft_substr(input, *start, *i - *start);
}

void	fill_token_list(char **token_list, char *input)
{
	int		i;
	int		token_start_pos;

	i = 0;
	while (input[i])
	{
		if (input[i] != ' ' && input[i] != '\t')
		{
			token_start_pos = i;
			if (fill_list_operator(token_list, input, &i, &token_start_pos))
				continue ;
			fill_list_noun(token_list, input, &i, &token_start_pos);
			continue ;
		}
		i++;
	}
}
