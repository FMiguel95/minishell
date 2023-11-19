/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_tokens.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fernacar <fernacar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/16 21:41:13 by fernacar          #+#    #+#             */
/*   Updated: 2023/11/19 22:00:51 by fernacar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static void	newstr_append(char *token, char **new_str, int *sub_start, int *i)
{
	char	*substr;
	char	*temp;

	substr = ft_substr(token, *sub_start, *i - *sub_start);
	temp = ft_strjoin(*new_str, substr);
	if (*new_str)
		free(*new_str);
	if (substr)
		free(substr);
	*new_str = temp;
}

static char	*handle_token_quotes(char *token)
{
	char	*new_str;
	int		i;
	int		sub_start;
	char	quote_type;

	new_str = ft_strdup("");
	quote_type = 0;
	i = 0;
	sub_start = 0;
	while (token[i])
	{
		if ((!quote_type && (token[i] == '\"' || token[i] == '\''))
			|| token[i] == quote_type)
		{
			newstr_append(token, &new_str, &sub_start, &i);
			if (!quote_type)
				quote_type = token[i];
			else
				quote_type = 0;
			sub_start = i + 1;
		}
		i++;
	}
	newstr_append(token, &new_str, &sub_start, &i);
	return (new_str);
}

void	handle_tokens(char **token_list, char **env, int exit_status)
{
	char	*temp;

	while (*token_list)
	{
		temp = *token_list;
		*token_list = expand_token_var(*token_list, env, exit_status, '\0');
		free(temp);
		temp = *token_list;
		*token_list = handle_token_quotes(*token_list);
		free(temp);
		token_list++;
	}
}
