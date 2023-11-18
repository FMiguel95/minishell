/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_buildin.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fernacar <fernacar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/12 17:10:44 by aamaral-          #+#    #+#             */
/*   Updated: 2023/11/16 21:04:19 by fernacar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

char	**substitute_env_str(char *arg, char **new_copy, size_t env_index)
{
	free(new_copy[env_index]);
	new_copy[env_index] = ft_strdup(arg);
	return (new_copy);
}

static char	**append_env(char *arg, char **copy, char ***uninit)
{
	char	**new_copy;
	size_t	key_len;
	size_t	env_index;
	char	*temp_key;

	key_len = key_length(arg);
	env_index = find_key_env_index(arg, copy, key_len);
	new_copy = env_copy(copy);
	temp_key = extract_key(arg);
	if (env_index == pointer_array_len(copy))
		new_copy[pointer_array_len(copy)] = ft_strdup(arg);
	else
	{
		if (!ft_strcmp(temp_key, "_"))
			;
		else
			substitute_env_str(arg, new_copy, env_index);
	}
	remove_uninit(temp_key, uninit);
	if (temp_key)
		free(temp_key);
	free_split(copy);
	return (new_copy);
}

static void	append_uninitialized(char *arg, char ***uninitialized, char **copy)
{
	char	**new_uninitialized;
	size_t	index;

	index = 0;
	if (my_getenv(arg, copy) != NULL)
		return ;
	while (*uninitialized && (*uninitialized)[index])
	{
		if (!ft_strcmp(arg, (*uninitialized)[index]))
			return ;
		index++;
	}
	new_uninitialized = env_copy(*uninitialized);
	new_uninitialized[pointer_array_len(*uninitialized)] = ft_strdup(arg);
	free_split(*uninitialized);
	*uninitialized = new_uninitialized;
}

static void	append(char *argv, char ***env_copy, char ***uninit)
{
	if (strchr(argv, '='))
		*env_copy = append_env(argv, *env_copy, uninit);
	else
		append_uninitialized(argv, uninit, *env_copy);
}

void	export_buildin(char **arg, char ***env_copy, char ***uninit,
		int *exit_status)
{
	int	i;

	i = 1;
	*exit_status = EXIT_SUCCESS;
	if (!arg[1])
		print_export(*env_copy, *uninit, exit_status);
	else if (is_option(arg[i]))
		perror_option_identifier(arg[i - 1], arg[i], exit_status);
	else
	{
		while (arg[i])
		{
			if (!key_alnum_under(arg[i], key_length(arg[i])) \
				|| ft_isdigit(*arg[i]) || !arg[i][0]
				|| !key_length(arg[i]))
			{
				perror_identifier(arg[0], arg[i], exit_status);
				i++;
				continue ;
			}
			append(arg[i], env_copy, uninit);
			i++;
		}
	}
}
