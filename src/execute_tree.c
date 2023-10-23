/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_tree.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fernacar <fernacar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/03 18:36:37 by fernacar          #+#    #+#             */
/*   Updated: 2023/10/23 19:53:36 by fernacar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

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

static void	execute_command(char **args, char **envp)
{
	char	*full_path;

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
			dprintf(2, "%s: command not found\n", args[0]);
			// free_split(args);
			exit(127);
		}
	}
	if (!fork1())
	{
		execve(full_path, args, envp);
		free(full_path);
		free_split(args);
		panic("execve error");
	}
	wait(0);
	free(full_path);
	// free_split(args);
}

void	execute_node(t_tnode *node, char** envp)
{
	int pid;
	int	p[2];
	t_tnode_exec	*exec_node;
	t_tnode_pipe	*pipe_node;
	t_tnode_redir	*redir_node;
	t_tnode_heredoc	*heredoc_node;

	if (node == NULL)
		panic("null node");
	if (node->type == EXEC)
	{
		// ft_putstr_fd("EXEC\n", 2);
		exec_node = (t_tnode_exec*)node;
		// if (!(pid = fork1()))
		// {
			if (exec_node->argv[0] == 0)
				exit(1);
			execute_command(exec_node->argv, envp);
		// }
		// waitpid(pid, 0, 0);
	}
	else if (node->type == REDIR)
	{
		// ft_putstr_fd("REDIR\n", 2);
		redir_node = (t_tnode_redir*)node;
		close(redir_node->fd);
		if (open(redir_node->file, redir_node->flags, 0666) < 0)
		{
			perror(redir_node->file);
			exit(1);
		}
		if (redir_node->node)
			execute_node(redir_node->node, envp);
	}
	else if (node->type == PIPE)
	{
		// ft_putstr_fd("PIPE\n", 2);
		pipe_node = (t_tnode_pipe*)node;
		if (pipe(p) < 0)
			panic("pipe error");
		if (fork1() == 0)
		{
			dup2(p[1], STDOUT_FILENO);
			close(p[1]);
			close(p[0]);
			if (pipe_node->branch_left)
				execute_node(pipe_node->branch_left, envp);
		}
		if (fork1() == 0)
		{
			close(p[1]);
			dup2(p[0], STDIN_FILENO);
			close(p[0]);
			if (pipe_node->branch_right)
				execute_node(pipe_node->branch_right, envp);
		}
		close(p[0]);
		close(p[1]);
		wait(0);
		wait(0);
	}
	else if (node->type == HEREDOC)
	{
		// ft_putstr_fd("HEREDOC\n", 2);
		heredoc_node = (t_tnode_heredoc*)node;
		char *line;

		int fd = open(".heredoc", O_WRONLY | O_CREAT | O_TRUNC, 0600);
		while (1)
		{
			line = readline("heredoc> ");
			if (!line || ft_strcmp(line, heredoc_node->delm) == 0)
			{
				if (line)
					free(line);
				break ;
			}
			ft_putstr_fd(line, fd);
			ft_putstr_fd("\n", fd);
		}
		//close(fd_temp);

		// close(STDIN_FILENO);
		// fd_temp = open(".heredoc", O_RDONLY, 0400);
		if (dup2(fd, STDIN_FILENO) == -1)
			panic("dup2 error");
		close(fd);
		if (((t_tnode_exec *)(heredoc_node->node))->argv[0] != 0)
			execute_node(heredoc_node->node, envp);
		unlink(".heredoc");
	}
	else
		panic("invalid node type");
	// exit(0);
}
