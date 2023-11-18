/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo_env_pwd_buildin.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fernacar <fernacar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/06 16:19:52 by aamaral-          #+#    #+#             */
/*   Updated: 2023/11/14 23:48:19 by fernacar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	echo_buildin(char **argv, int *exit_status)
{
	int		i;
	int		flag_n_option;

	i = 1;
	flag_n_option = 0;
	*exit_status = EXIT_SUCCESS;
	while (is_option_n(argv[i]) == 1)
	{
		if (is_option_n(argv[i]) == 1)
			flag_n_option = 1;
		i++;
	}
	while (argv[i])
	{
		printf("%s", argv[i]);
		if (argv[i + 1])
			printf(" ");
		i++;
	}
	if (!flag_n_option)
		printf("\n");
	exit(*exit_status);
}

void	env_buildin(char **argv, char **env_copy, int *exit_status)
{
	int		i;
	char	*env_var;

	i = 0;
	env_var = env_copy[0];
	*exit_status = EXIT_SUCCESS;
	if (argv[0] && argv[1])
	{
		errno = EINVAL;
		perror("minishell buildin specific");
		*exit_status = 2;
		exit(*exit_status);
	}
	while (env_var)
	{
		printf("%s\n", env_var);
		i++;
		env_var = env_copy[i];
	}
	exit(*exit_status);
}

void	pwd_buildin(char **argv, int *exit_status)
{
	char	curr_work_dir[PATH_MAX];
	int		i;

	i = 1;
	if (is_option(argv[i]))
		ft_perror(argv[i - 1], is_option(argv[i]), exit_status);
	else if (getcwd(curr_work_dir, PATH_MAX) == NULL)
	{
		perror("pwd error:");
		*exit_status = 2;
	}
	else
	{
		printf("%s\n", curr_work_dir);
		*exit_status = EXIT_SUCCESS;
	}
	exit(*exit_status);
}
