/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   var_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fernacar <fernacar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/06 20:30:14 by aamaral-          #+#    #+#             */
/*   Updated: 2023/11/19 23:38:57 by fernacar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	key_length(char *str)
{
	size_t	i;

	i = 0;
	if (!str[i])
		return (0);
	while (i < ft_strlen(str))
	{
		if (str[i] == '=' || str[i] == ' ' || str[i] == '\0')
			return (i);
		i++;
	}
	return (i);
}

size_t	pointer_array_len(char **pointers)
{
	size_t	len;

	len = 0;
	while (pointers && pointers[len])
		len++;
	return (len);
}

char	*extract_key(char *str)
{
	char	*key;
	int		length;
	int		i;

	if (!str)
		return (0);
	length = key_length(str);
	key = (char *)malloc(sizeof(char) * (length + 1));
	i = 0;
	while (i < length)
	{
		key[i] = str[i];
		i++;
	}
	key[i] = '\0';
	return (key);
}

size_t	find_key_env_index(char *arg, char **env, int index)
{
	int	i_str;
	int	i_env;
	int	len_env;

	i_str = 0;
	i_env = 0;
	len_env = 0;
	len_env = pointer_array_len(env);
	while (env[i_env])
	{
		while (i_str < index && arg[i_str] == env[i_env][i_str])
			i_str++;
		if (env[i_env][i_str] == '=' && index == i_str)
			return (i_env);
		i_str = 0;
		i_env++;
	}
	return (len_env);
}

char	**env_copy(char **env)
{
	char	**copy;
	size_t	key_index;

	key_index = 0;
	copy = (char **)malloc(sizeof(char *) * (pointer_array_len(env) + 3));
	if (!copy)
		return (NULL);
	copy[pointer_array_len(env)] = NULL;
	copy[pointer_array_len(env) + 1] = NULL;
	copy[pointer_array_len(env) + 2] = NULL;
	while (env && env[key_index])
	{
		copy[key_index] = ft_strdup(env[key_index]);
		key_index++;
	}
	return (copy);
}
