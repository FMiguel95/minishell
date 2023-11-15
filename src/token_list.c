/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_list.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fernacar <fernacar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/05 21:28:15 by fernacar          #+#    #+#             */
/*   Updated: 2023/11/14 23:25:13 by fernacar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static int	is_delimiter(char c)
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

static void	fill_list(char **token_list, char *input)
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

static char	*handle_token_quotes(char *token, char **env, int exit_status)
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

static int	handle_single_quotes(char *token, char	**expanded_token, int *i)
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
		temp = *expanded_token;
		*expanded_token = ft_strjoin(temp, (char [2]){token[*i], '\0'});
		free(temp);
		return (1);
	}
	return (0);
}

static char	*get_var_val(char *token, int i, char **env, int exit_status)
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

static void	skip_var(char *token, int *i)
{
	if (token[*i] == '?')
		*i = *i + 1;
	else
	{
		while (ft_isalnum(token[*i]) || token[*i] == '_')
			*i = *i + 1;
	}
}

static char	*strjoin_free(char *s1, char *s2)
{
	char	*res;

	res = ft_strjoin(s1, s2);
	free(s1);
	free(s2);
	return (res);
}

static char	*expand_token_var(char *token, char **env, int exit_status)
{
	char	*expanded_token;
	char	*temp;
	int		i;

	expanded_token = ft_strdup("");
	i = 0;
	while (token[i])
	{
		if (handle_single_quotes(token, &expanded_token, &i))
			continue ;
		if (token[i] == '$' && (ft_isalpha(token[i + 1])
				|| token[i + 1] == '_' || token[i + 1] == '?'))
		{
			i++;
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

static void	handle_tokens(char **token_list, char **env, int exit_status)
{
	char	*temp;

	while (*token_list)
	{
		temp = *token_list;
		*token_list = expand_token_var(*token_list, env, exit_status);
		free(temp);
		temp = *token_list;
		*token_list = handle_token_quotes(*token_list, env, exit_status);
		free(temp);
		token_list++;
	}
}

char	**make_token_list(char *input, char **env, int exit_status)
{
	char	**token_list;
	int		token_count;

	token_count = count_tokens(input);
	token_list = ft_calloc(token_count + 1, sizeof(char *));
	if (!token_list)
		panic("token allocation error");
	fill_list(token_list, input);
	handle_tokens(token_list, env, exit_status);
	return (token_list);
}

// void print_list(char **list)
// {
// 	int i = 0;
// 	while (list && list[i] != NULL)
// 	{
// 		printf("%s$\n", list[i]);
// 		i++;
// 	}
// }
