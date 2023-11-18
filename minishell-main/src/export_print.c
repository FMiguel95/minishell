/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_print.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fernacar <fernacar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/12 17:38:50 by aamaral-          #+#    #+#             */
/*   Updated: 2023/11/14 23:49:00 by fernacar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static void	bubble_sort_array(char **copy)
{
	char	*temp;
	int		sorted;
	int		i;

	sorted = 0;
	while (!sorted)
	{
		sorted = 1;
		i = 0;
		while (copy[i + 1])
		{
			if (strncmp(copy[i], copy[i + 1], key_length(copy[i])) > 0)
			{
				temp = NULL;
				temp = copy[i];
				copy[i] = copy[i + 1];
				copy[i + 1] = temp;
				sorted = 0;
			}
			i++;
		}
	}
}

static char	**ft_strjoin_var(char **s1, char **s2)
{
	char	**result;
	size_t	i;
	size_t	j;

	result = malloc(sizeof(char *) * \
		(pointer_array_len(s1) + pointer_array_len(s2) + 1));
	if (!result)
		return (NULL);
	result[pointer_array_len(s1) + pointer_array_len(s2)] = NULL;
	i = 0;
	while (s1[i])
	{
		result[i] = ft_strdup(s1[i]);
		i++;
	}
	j = 0;
	while (s2[j])
	{
		result[i + j] = ft_strdup(s2[j]);
		j++;
	}
	return (result);
}

static void	print_export_print(char **copy, char **var_sorted)
{
	size_t	index;
	char	*key;
	char	*val;

	index = 0;
	while (var_sorted[index])
	{
		key = extract_key(var_sorted[index]);
		val = my_getenv(key, copy);
		if (var_sorted[index][ft_strlen(key)] == '=')
		{
			if (ft_strcmp(key, "_"))
				printf("declare -x %s=\"%s\"\n", key, val);
		}
		else
			printf("declare -x %s\n", key);
		free(key);
		index++;
	}
}

void	print_export(char **copy, char **uninitialized, int *exit_status)
{
	char	**var_sorted;

	if (uninitialized)
		var_sorted = ft_strjoin_var(copy, uninitialized);
	else
		var_sorted = env_copy(copy);
	bubble_sort_array(var_sorted);
	print_export_print(copy, var_sorted);
	free_split(var_sorted);
	*exit_status = EXIT_SUCCESS;
}
