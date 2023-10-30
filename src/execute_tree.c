/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_tree.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fernacar <fernacar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/03 18:36:37 by fernacar          #+#    #+#             */
/*   Updated: 2023/10/30 23:47:56 by fernacar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

extern int exit_status;

static char	*get_full_path(char *command)
{
	char	*ret;
	char	*temp;
	char	**paths;
	int		i;

	ret = NULL;
	paths = ft_split(getenv("PATH"), ':');
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

static void	execute_command(char **args, char ***env, char **uninit)
{
	char	*full_path;
	if (!ft_strcmp(args[0], "echo"))
		echo_buildin(args);
	else if (!ft_strcmp(args[0], "env"))
		env_buildin(args, *env);
	else if (!ft_strcmp(args[0], "pwd"))
		pwd_buildin(args);
	else if (!ft_strcmp(args[0], "export"))
		export_buildin(args, env, &uninit);
	else if (!ft_strcmp(args[0], "unset"))
		unset_buildin(args, env);
	else
	{
		if (ft_strchr(args[0], '/'))
		{
			full_path = args[0];
			if (access(full_path, X_OK) != 0)
			{
				perror(full_path);
				free(full_path);
				exit(127);
			}
		}
		else
		{
			full_path = get_full_path(args[0]);
			if (full_path == NULL)
			{
				ft_putstr_fd(args[0], STDERR_FILENO);
				ft_putstr_fd(": command not found\n", STDERR_FILENO);
				// free_split(args);
				exit(127);
			}
		}
		if (!fork1())
		{
			execve(full_path, args, *env);
			free(full_path);
			free_split(args);
			panic("execve error");
		}
		// printf("exit_status before:%d\n", WEXITSTATUS(exit_status));
		waitpid(0, &exit_status, 0);
		exit_status = WEXITSTATUS(exit_status);
		free(full_path);
		//exit(exit_status);
	}
}

void	execute_node_exec(t_tnode_exec *exec_node, char*** env, char **uninit)
{
	if (exec_node->argv[0] == 0)
		exit(1);
	execute_command(exec_node->argv, env, uninit);
}

void	execute_node_pipe(t_tnode_pipe *pipe_node, char*** envp, char **uninit)
{
	int	pid;
	int	p[2];

	if (pipe(p) < 0)
		panic("pipe error");
	if (fork1() == 0)
	{
		dup2(p[1], STDOUT_FILENO);
		close(p[1]);
		close(p[0]);
		if (pipe_node->branch_left->type != 0)
			execute_node(pipe_node->branch_left, envp, uninit);
	}
	if (fork1() == 0)
	{
		close(p[1]);
		dup2(p[0], STDIN_FILENO);
		close(p[0]);
		if (pipe_node->branch_right->type != 0)
			execute_node(pipe_node->branch_right, envp, uninit);
	}
	close(p[0]);
	close(p[1]);
	wait(0);
	wait(0);
}

void	execute_node_redir(t_tnode_redir *redir_node, char*** envp, char **uninit)
{
	close(redir_node->fd);
	if (open(redir_node->file, redir_node->flags, 0666) < 0)
	{
		perror(redir_node->file);
		exit(1);
	}
	if (redir_node->node)
		execute_node(redir_node->node, envp, uninit);
}

void	execute_node_heredoc(t_tnode_heredoc *heredoc_node, char*** envp, char **uninit)
{
	char	*line;
	int		fd = open(".heredoc", O_WRONLY | O_CREAT | O_TRUNC, 0600);
	while (1)
	{
		line = readline("heredoc> ");
		if (!line || ft_strcmp(line, heredoc_node->delm) == 0)
		{
			if (!line)
				free(line);
			break ;
		}
		ft_putstr_fd(line, fd);
		ft_putstr_fd("\n", fd);
		free(line);
	}
	// if (dup2(fd, STDIN_FILENO) == -1)
	// 	panic("dup2 error");
	// close(fd);
	close(fd);
	close(STDIN_FILENO);
	fd = open(".heredoc", O_RDONLY, 0400);
	if (((t_tnode_exec *)(heredoc_node->node))->argv[0] != 0)
		execute_node(heredoc_node->node, envp, uninit);
	close(fd);
	unlink(".heredoc");
}

void	execute_node(t_tnode *node, char*** envp, char **uninit)
{
	if (node == NULL)
		panic("null node");
	if (node->type == EXEC)
		execute_node_exec((t_tnode_exec*)node, envp, uninit);
	else if (node->type == REDIR)
		execute_node_redir((t_tnode_redir*)node, envp, uninit);
	else if (node->type == PIPE)
		execute_node_pipe((t_tnode_pipe*)node, envp, uninit);
	else if (node->type == HEREDOC)
		execute_node_heredoc((t_tnode_heredoc*)node, envp, uninit);
	else
		panic("invalid node type");
	exit(exit_status);
}
