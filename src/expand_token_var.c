/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_token_var.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fernacar <fernacar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/16 21:28:42 by fernacar          #+#    #+#             */
/*   Updated: 2023/11/20 19:44:04 by fernacar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	skip_var(char *token, int *i)
{
	if (token[*i] == '?')
		*i = *i + 1;
	else
	{
		while (ft_isalnum(token[*i]) || token[*i] == '_')
			*i = *i + 1;
	}
}

char	*get_var_val(char *token, int i, char **env, int exit_status)
{
	char	*key;
	char	*val;
	int		j;

	if (token[i] == '?')
		return (ft_itoa(exit_status));
	j = i;
	while (ft_isalnum(token[j]) || token[j] == '_')
		j++;
	key = ft_calloc(j - i + 1, sizeof(char));
	j--;
	while (j >= i)
	{
		key[j - i] = *(token + j);
		j--;
	}
	val = my_getenv(key, env);
	free(key);
	if (!val)
		return (ft_strdup(""));
	return (ft_strdup(val));
}

int	do_single_quotes(char *token, char	**expanded_token, int *i)
{
	char	*temp;

	if (token[*i] == '\'')
	{
		temp = *expanded_token;
		*expanded_token = ft_strjoin(temp, (char [2]){token[*i], '\0'});
		free(temp);
		*i = *i + 1;
		while (token[*i] && token[*i] != '\'')
		{
			temp = *expanded_token;
			*expanded_token = ft_strjoin(temp, (char [2]){token[*i], '\0'});
			free(temp);
			*i = *i + 1;
		}
		if (token[*i])
		{
			temp = *expanded_token;
			*expanded_token = ft_strjoin(temp, (char [2]){token[*i], '\0'});
			free(temp);
			*i = *i + 1;
		}
		return (1);
	}
	return (0);
}

void	handle_quote_type(char quote, char *quote_type)
{
	if (quote == '\"' || quote == '\'')
	{
		if (*quote_type == quote)
			*quote_type = '\0';
		else if (*quote_type == '\0')
			*quote_type = quote;
	}
}

char	*expand_token_var(char *token, char **env, int exit_status,
		char quote_type)
{
	char	*expanded_token;
	char	*temp;
	int		i;

	expanded_token = ft_strdup("");
	i = 0;
	while (token[i])
	{
		handle_quote_type(token[i], &quote_type);
		if (quote_type != '\"' && do_single_quotes(token, &expanded_token, &i))
			continue ;
		if (token[i] == '$' && (ft_isalpha(token[i + 1])
				|| token[i + 1] == '_' || token[i + 1] == '?') && (i++ || i))
		{
			expanded_token = strjoin_free(expanded_token,
					get_var_val(token, i, env, exit_status));
			skip_var(token, &i);
			continue ;
		}
		temp = expanded_token;
		expanded_token = ft_strjoin(temp, (char [2]){token[i], '\0'});
		free(temp);
		i++;
	}
	return (expanded_token);
}
