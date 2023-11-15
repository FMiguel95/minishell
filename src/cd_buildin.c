/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_buildin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fernacar <fernacar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/06 17:22:03 by aamaral-          #+#    #+#             */
/*   Updated: 2023/11/14 23:51:31 by fernacar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static char	**update_env(char **env_cpy)
{
	char	*new_pwd_str;
	char	*new_oldpwd_str;
	char	*temp_pwd;
	size_t	env_index;
	char	*temp;

	env_index = find_key_env_index("OLDPWD", env_cpy, 6);
	temp_pwd = env_cpy[env_index];
	if (temp_pwd)
		free(temp_pwd);
	new_oldpwd_str = ft_strjoin("OLDPWD=", my_getenv("PWD", env_cpy));
	env_cpy[env_index] = new_oldpwd_str;
	temp = getcwd(NULL, 0);
	new_pwd_str = ft_strjoin("PWD=", temp);
	if (temp)
		free(temp);
	env_index = find_key_env_index("PWD", env_cpy, 3);
	temp_pwd = env_cpy[env_index];
	free(temp_pwd);
	env_cpy[env_index] = new_pwd_str;
	return (env_cpy);
}

static void	change_home(char ***env_cpy, int *exit_status)
{
	char	str[PATH_MAX];

	if (find_key_env_index("HOME", *env_cpy, 4) != pointer_array_len(*env_cpy))
	{
		*exit_status = chdir(my_getenv("HOME", *env_cpy));
		if (*exit_status)
			ft_putendl_fd("error in changing directory", 2);
		getcwd(str, PATH_MAX);
	}
	else
	{
		ft_putendl_fd("minishell: cd: HOME not set", 2);
		*exit_status = 1;
	}
}

static void	change_old(char ***env_cpy, int *exit_status)
{
	char	str[PATH_MAX];

	if (find_key_env_index("OLDPWD", *env_cpy, 6) != \
		pointer_array_len(*env_cpy))
	{
		*exit_status = chdir(my_getenv("OLDPWD", *env_cpy));
		if (*exit_status)
			ft_putstr_fd("error in changing directory\n", 2);
		getcwd(str, PATH_MAX);
		printf("%s\n", str);
	}
	else
	{
		ft_putendl_fd("minishell: cd: OLDPWD not set", 2);
		*exit_status = 1;
	}
}

static void	change_others(char *argv, char ***env_cpy, int *exit_status)
{
	char	str[PATH_MAX];

	*exit_status = chdir(argv);
	if (exit_status)
	{
		perror(argv);
		*exit_status = 1;
	}
	getcwd(str, PATH_MAX);
}

int	cd_buildin(char **argv, char ***env_cpy, int *exit_status)
{
	char	str[PATH_MAX];

	*exit_status = EXIT_SUCCESS;
	if ((ft_argc(argv) == 2) && !argv[1][0])
		*exit_status = 0;
	else if ((ft_argc(argv) == 2) && is_option(argv[1]))
		ft_perror(argv[0], is_option(argv[1]), exit_status);
	else if (ft_argc(argv) > 2)
		perror_cd(exit_status);
	else if (!argv[1] || !ft_strcmp(argv[1], "~") || !ft_strcmp(argv[1], "--"))
		change_home(env_cpy, exit_status);
	else if (!strcmp(argv[1], "-"))
		change_old(env_cpy, exit_status);
	else
		change_others(argv[1], env_cpy, exit_status);
	update_env(*env_cpy);
	return (*exit_status);
}
