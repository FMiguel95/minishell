/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fernacar <fernacar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/16 20:34:59 by fernacar          #+#    #+#             */
/*   Updated: 2023/11/16 21:36:13 by fernacar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	panic(char *str)
{
	ft_putendl_fd(str, STDERR_FILENO);
	exit(1);
}

int	fork1(void)
{
	int	pid;

	pid = fork();
	if (pid == -1)
		panic("fork");
	return (pid);
}

char	*my_getenv(char *var, char **env)
{
	char	*result;

	result = NULL;
	while (*env)
	{
		if (ft_strncmp(var, *env, ft_strlen(var)) == 0
			&& (*env)[ft_strlen(var)] == '=')
			return (*env + ft_strlen(var) + 1);
		env++;
	}
	return (result);
}

char	get_token_type(char *token)
{
	if (!token)
		return ('\0');
	if (strcmp(token, "|") == 0)
		return ('|');
	else if (strcmp(token, "<") == 0)
		return ('<');
	else if (strcmp(token, ">") == 0)
		return ('>');
	else if (strcmp(token, "<<") == 0)
		return ('-');
	else if (strcmp(token, ">>") == 0)
		return ('+');
	else
		return ('a');
}

char	*strjoin_free(char *s1, char *s2)
{
	char	*res;

	res = ft_strjoin(s1, s2);
	free(s1);
	free(s2);
	return (res);
}
