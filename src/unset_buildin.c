/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset_buildin.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fernacar <fernacar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/06 16:50:22 by aamaral-          #+#    #+#             */
/*   Updated: 2023/11/20 22:18:17 by fernacar         ###   ########.fr       */
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

	if (!*uninit)
		return ;
	index = 0;
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

void	unset_buildin(char **argv, char ***copy, char ***uninit,
			int *exit_status)
{
	int	i; 

	*exit_status = EXIT_SUCCESS;
	i = 1;
	if (!argv[1])
		return ;
	if (is_option(argv[i]))
		perror_option_identifier(argv[i - 1], argv[i], exit_status);
	while (argv && argv[i] && is_option(argv[1]) == 0)
	{
		if (!(ft_isalpha(argv[i][0]) || argv[i][0] == '_'))
		{
			perror_identifier(argv[0], argv[i], exit_status);
			i++;
			continue ;
		}
		if (!ft_strcmp(argv[i], "_"))
			;
		else
		{
			remove_env(argv[i], copy, ft_strlen(argv[i]));
			remove_uninit(argv[i], uninit);
		}
		i++;
	}
}
