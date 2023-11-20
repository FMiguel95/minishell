/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_node_exec.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fernacar <fernacar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/16 22:00:49 by fernacar          #+#    #+#             */
/*   Updated: 2023/11/20 19:17:53 by fernacar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static int	child_buildins(char **args, t_minishell *data)
{
	if (!ft_strcmp(args[0], "echo") || !ft_strcmp(args[0], "env")
		|| !ft_strcmp(args[0], "pwd") || !ft_strcmp(args[0], "export")
		|| !ft_strcmp(args[0], "unset") || !ft_strcmp(args[0], "cd")
		|| !ft_strcmp(args[0], "exit"))
	{
		if (!ft_strcmp(args[0], "echo"))
			echo_buildin(args, data->exit_status);
		else if (!ft_strcmp(args[0], "env"))
			env_buildin(args, data->env, data->exit_status);
		else if (!ft_strcmp(args[0], "pwd"))
			pwd_buildin(args, data->exit_status);
		else if (!ft_strcmp(args[0], "export") && !args[1])
			export_buildin(args, &data->env, &data->uninit, data->exit_status);
		else if (!ft_strcmp(args[0], "unset") && !args[1])
			unset_buildin(args, &data->env, &data->uninit, data->exit_status);
		return (1);
	}
	return (0);
}

static char	*build_full_path(char *command, char **env)
{
	char	*ret;
	char	*temp;
	char	**paths;
	int		i;

	ret = NULL;
	temp = my_getenv("PATH", env);
	if (!temp)
		return (NULL);
	paths = ft_split(temp, ':');
	i = 0;
	while (paths[i] != NULL)
	{
		temp = ft_strjoin(paths[i], "/");
		ret = ft_strjoin(temp, command);
		free(temp);
		if (access(ret, X_OK) == 0)
			break ;
		free(ret);
		ret = NULL;
		i++;
	}
	free_split(paths);
	return (ret);
}

static char	*get_full_path(char **args, t_minishell *data)
{
	char	*full_path;

	full_path = NULL;
	if (ft_strchr(args[0], '/'))
	{
		full_path = ft_strdup(args[0]);
		if (access(full_path, X_OK) != 0)
		{
			perror(full_path);
			free(full_path);
			exit(127);
		}
	}
	else
	{
		if (args[0][0])
			full_path = build_full_path(args[0], data->env);
		if (full_path == NULL)
		{
			ft_putstr_fd(args[0], STDERR_FILENO);
			ft_putendl_fd(": command not found", STDERR_FILENO);
			exit(127);
		}
	}
	return (full_path);
}

static void	execute_command(char **args, t_minishell *data)
{
	char	*full_path;

	if (!child_buildins(args, data))
	{
		full_path = get_full_path(args, data);
		if (!fork1())
		{
			execve(full_path, args, data->env);
			perror(full_path);
			free(full_path);
			free_split(args);
			exit(126);
		}
		waitpid(0, data->exit_status, 0);
		*data->exit_status = WEXITSTATUS(*data->exit_status);
		free(full_path);
	}
}

void	execute_node_exec(t_tnode_exec *exec_node, t_minishell *data)
{
	if (!(exec_node->argv[0] == 0))
		execute_command(exec_node->argv, data);
}
