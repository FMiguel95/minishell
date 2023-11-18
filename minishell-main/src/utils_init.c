/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_init.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fernacar <fernacar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/06 16:59:22 by aamaral-          #+#    #+#             */
/*   Updated: 2023/11/14 23:41:23 by fernacar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	is_option(char *str)
{
	char	letter;

	if (!str)
		return (0);
	if (!str[0])
		return (-1);
	letter = str[1];
	if (*str == '-')
	{
		if (*(++str) != '-' && *str != '\0')
			return (letter);
	}
	return (0);
}

int	is_option_n(char *str)
{
	if (!str)
		return (0);
	if (*str == '-')
	{
		if (*(str +1) == '\0')
			return (0);
		while (*(++str) == 'n')
			;
		if (!*str)
			return (1);
	}
	return (0);
}

int	ft_argc(char **argv)
{
	int	counter;

	counter = 0;
	if (!argv)
		return (0);
	while (argv[counter] != NULL)
		counter++;
	return (counter);
}

int	key_alnum_under(char *s, size_t index)
{
	size_t	i;

	i = 0;
	while (i < index || s[i] == ' ')
	{
		if (!((s[i] >= 'a' && s[i] <= 'z') || (s[i] >= 'A' && s[i] <= 'Z') \
			|| (s[i] >= '0' && s[i] <= '9') || s[i] == '_' || s[i] == '='))
			return (0);
		i++;
	}
	return (1);
}

void	init_env(char **env)
{
	size_t	env_index;
	char	*asci_env_value;
	int		dec_env_value;
	char	*new_env_str;

	env_index = 0;
	dec_env_value = 0;
	asci_env_value = NULL;
	new_env_str = NULL;
	if (!my_getenv("SHLVL", env))
		env[pointer_array_len(env)] = ft_strdup("SHLVL=1");
	else
	{
		env_index = find_key_env_index("SHLVL", env, 5);
		asci_env_value = my_getenv("SHLVL", env);
		dec_env_value = ft_atoi(asci_env_value);
		asci_env_value = ft_itoa(dec_env_value + 1);
		new_env_str = ft_strjoin("SHLVL=", asci_env_value);
		free(asci_env_value);
		free(env[env_index]);
		env[env_index] = new_env_str;
	}
}
