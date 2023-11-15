/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_tree.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fernacar <fernacar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/03 18:36:37 by fernacar          #+#    #+#             */
/*   Updated: 2023/11/14 23:54:30 by fernacar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

char	*my_getenv(char *var, char **env)
{
	char	**split;
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

static int	child_buildins(char **args, t_minishell *data)
{
	if (!ft_strcmp(args[0], "echo") || !ft_strcmp(args[0], "env")
		|| !ft_strcmp(args[0], "pwd") || !ft_strcmp(args[0], "export")
		|| !ft_strcmp(args[0], "unset") || !ft_strcmp(args[0], "cd")
		|| !ft_strcmp(args[0], "exit"))
	{
		if (!ft_strcmp(args[0], "echo"))
			echo_buildin(args, &data->exit_status);
		else if (!ft_strcmp(args[0], "env"))
			env_buildin(args, data->env, &data->exit_status);
		else if (!ft_strcmp(args[0], "pwd"))
			pwd_buildin(args, &data->exit_status);
		else if (!ft_strcmp(args[0], "export"))
			export_buildin(args, &data->env, &data->uninit, &data->exit_status);
		else if (!ft_strcmp(args[0], "unset"))
			unset_buildin(args, &data->env, &data->uninit, &data->exit_status);
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
		waitpid(0, &data->exit_status, 0);
		data->exit_status = WEXITSTATUS(data->exit_status);
		free(full_path);
	}
}

void	execute_node_exec(t_tnode_exec *exec_node, t_minishell *data)
{
	if (exec_node->argv[0] == 0)
		exit(1);
	execute_command(exec_node->argv, data);
}

static void	pipe_child1(int p[2], t_tnode_pipe *pipe_node, t_minishell *data)
{
	dup2(p[1], STDOUT_FILENO);
	close(p[0]);
	close(p[1]);
	if (pipe_node->branch_left->type != 0)
		execute_node(pipe_node->branch_left, data);
	free_data(data);
	exit(data->exit_status);
}

static void	pipe_child2(int p[2], t_tnode_pipe *pipe_node, t_minishell *data)
{
	dup2(p[0], STDIN_FILENO);
	close(p[0]);
	close(p[1]);
	if (pipe_node->branch_right->type != 0)
		execute_node(pipe_node->branch_right, data);
	free_data(data);
	exit(data->exit_status);
}

void	execute_node_pipe(t_tnode_pipe *pipe_node, t_minishell *data)
{
	int	pid1;
	int	pid2;
	int	p[2];

	if (pipe(p) < 0)
		panic("pipe error");
	pid1 = fork1();
	if (pid1 == 0)
		pipe_child1(p, pipe_node, data);
	pid2 = fork1();
	if (pid2 == 0)
		pipe_child2(p, pipe_node, data);
	close(p[0]);
	close(p[1]);
	waitpid(pid1, &data->exit_status, 0);
	data->exit_status = WEXITSTATUS(data->exit_status);
	waitpid(pid2, &data->exit_status, 0);
	data->exit_status = WEXITSTATUS(data->exit_status);
}

void	execute_node_redir(t_tnode_redir *redir_node, t_minishell *data)
{
	int	new_fd;

	new_fd = open(redir_node->file, redir_node->flags, 0666);
	if (new_fd < 0)
	{
		perror(redir_node->file);
		data->exit_status = 1;
		exit(data->exit_status);
	}
	if (dup2(new_fd, redir_node->fd) == -1)
		panic("dup2 error");
	close(new_fd);
	if (redir_node->node)
		execute_node(redir_node->node, data);
}

void	execute_node_heredoc(t_tnode_heredoc *heredoc_node, t_minishell *data)
{
	int	fd;

	fd = open(".heredoc", O_WRONLY | O_CREAT | O_TRUNC, 0600);
	write(fd, heredoc_node->input, ft_strlen(heredoc_node->input));
	close(fd);
	fd = open(".heredoc", O_RDONLY, 0400);
	if (dup2(fd, STDIN_FILENO) == -1)
		panic("dup2 error");
	close(fd);
	if (heredoc_node->node)
		execute_node(heredoc_node->node, data);
	unlink(".heredoc");
}

static char	*get_var_val(char *token, int i, char **env, int exit_status)
{
	char	*key;
	char	*val;
	int		j;

	if (token[i] == '?')
		return (ft_itoa(exit_status));
	j = i;
	while (ft_isalnum(token[j]) || token[j] == '_')
		j++;
	key = ft_calloc(j - i + 1, sizeof(char));
	j--;
	while (j >= i)
	{
		key[j - i] = *(token + j);
		j--;
	}
	val = my_getenv(key, env);
	free(key);
	if (!val)
		return (ft_strdup(""));
	return (ft_strdup(val));
}

static void	skip_var(char *token, int *i)
{
	if (token[*i] == '?')
		*i = *i + 1;
	else
	{
		while (ft_isalnum(token[*i]) || token[*i] == '_')
			*i = *i + 1;
	}
}

static char	*strjoin_free(char *s1, char *s2)
{
	char	*res;

	res = ft_strjoin(s1, s2);
	free(s1);
	free(s2);
	return (res);
}

static char	*expand_token_var(char *token, char **env, int exit_status)
{
	char	*expanded_token;
	char	*temp;
	int		i;

	expanded_token = ft_strdup("");
	i = 0;
	while (token[i])
	{
		if (token[i] == '$' && (ft_isalpha(token[i + 1])
				|| token[i + 1] == '_' || token[i + 1] == '?'))
		{
			i++;
			expanded_token = strjoin_free(expanded_token,
					get_var_val(token, i, env, exit_status));
			skip_var(token, &i);
			continue ;
		}
		temp = expanded_token;
		expanded_token = ft_strjoin(temp, (char [2]){token[i], '\0'});
		free(temp);
		i++;
	}
	return (expanded_token);
}

void	get_heredoc(t_tnode_heredoc *heredoc_node, char **env, int exit_status)
{
	char	*line;
	char	*temp;

	while (1)
	{
		line = readline("heredoc> ");
		if (!line || ft_strcmp(line, heredoc_node->delm) == 0)
		{
			if (line)
				free(line);
			else
				ft_putendl_fd("minishell: warning: heredoc delimited by \
end-of-file", STDERR_FILENO);
			break ;
		}
		temp = expand_token_var(line, env, exit_status);
		free(line);
		line = ft_strjoin(temp, "\n");
		free(temp);
		temp = ft_strjoin(heredoc_node->input, line);
		if (heredoc_node->input)
			free(heredoc_node->input);
		free(line);
		heredoc_node->input = temp;
	}
}

void	check_heredocs(t_tnode *node, t_minishell *data)
{
	if (node && node->type == PIPE)
	{
		check_heredocs(((t_tnode_pipe *)node)->branch_left, data);
		check_heredocs(((t_tnode_pipe *)node)->branch_right, data);
	}
	else if (node && node->type == REDIR)
	{
		check_heredocs(((t_tnode_redir *)node)->node, data);
	}
	else if (node && node->type == HEREDOC)
	{
		get_heredoc((t_tnode_heredoc *)node, data->env, data->exit_status);
		check_heredocs(((t_tnode_heredoc *)node)->node, data);
	}
}

void	execute_node(t_tnode *node, t_minishell *data)
{
	if (node == NULL)
		panic("null node");
	if (node->type == EXEC)
		execute_node_exec((t_tnode_exec *)node, data);
	else if (node->type == REDIR)
		execute_node_redir((t_tnode_redir *)node, data);
	else if (node->type == PIPE)
		execute_node_pipe((t_tnode_pipe *)node, data);
	else if (node->type == HEREDOC)
		execute_node_heredoc((t_tnode_heredoc *)node, data);
	else
		panic("invalid node type");
}
