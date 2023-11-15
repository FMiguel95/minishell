/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset_buildin.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fernacar <fernacar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/06 16:50:22 by aamaral-          #+#    #+#             */
/*   Updated: 2023/11/14 23:46:09 by fernacar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	remove_env(char *argv, char ***copy, int key_index)
{
	int		env_index;
	char	*temp;
	char	**new_copy;

	if (!copy)
		return ;
	env_index = find_key_env_index(argv, *copy, key_index);
	temp = (*copy)[env_index];
	while ((*copy)[env_index])
	{
		(*copy)[env_index] = (*copy)[env_index + 1];
		env_index++;
	}
	free(temp);
	copy[env_index] = NULL;
	(*copy)[env_index] = NULL;
	new_copy = env_copy(*copy);
	free_split(*copy);
	*copy = new_copy;
}

void	remove_uninit(char *argv, char ***uninit)
{
	char	*temp;
	size_t	index;
	char	**new_uninit;

	index = 0;
	if (!*uninit)
		return ;
	while ((*uninit)[index] && ft_strcmp(argv, (*uninit)[index]))
		index++;
	if (index == pointer_array_len(*uninit))
		return ;
	else
	{
		temp = (*uninit)[index];
		while ((*uninit)[index])
		{
			(*uninit)[index] = (*uninit)[index + 1];
			index++;
		}
		free(temp);
		(*uninit)[index] = NULL;
		new_uninit = env_copy(*uninit);
		free_split(*uninit);
		*uninit = new_uninit;
	}
}

void	unset_buildin(char **arg, char ***copy, char ***uninit,
		int *exit_status)
{
	int	i; 

	i = 1;
	*exit_status = EXIT_SUCCESS;
	if (!arg[1])
		*exit_status = EXIT_SUCCESS;
	else if (is_option(arg[i]))
		perror_option_identifier(arg[i - 1], arg[i], exit_status);
	else
	{
		while (arg && arg[i])
		{
			if (!key_alnum_under(arg[i], key_length(arg[i])) \
				|| ft_isdigit(*arg[i]) || !arg[i][0]
				|| !key_length(arg[i]))
				perror_identifier(arg[0], arg[i], exit_status);
			if (!ft_strcmp(arg[i], "_"))
				;
			else
			{
				remove_env(arg[i], copy, ft_strlen(arg[i]));
				remove_uninit(arg[i], uninit);
			}
			i++;
		}
	}
}
