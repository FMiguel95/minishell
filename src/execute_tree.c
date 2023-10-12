/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_tree.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fernacar <fernacar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/03 18:36:37 by fernacar          #+#    #+#             */
/*   Updated: 2023/10/03 22:10:41 by fernacar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static char	*get_full_path(char *command)
{
	char	*ret;
	char	*temp;
	char	**paths;

	ret = NULL;
	paths = ft_split(getenv("PATH"), ':');
	while (*paths != NULL)
	{
		temp = ft_strjoin(*paths, "/");
		ret = ft_strjoin(temp, command);
		free(temp);
		if (access(ret, X_OK) == 0)
 			break ;
		free(ret);
		ret = NULL;
		paths++;
	}
	return (ret);
}

static void	execute_command(char **args)
{
	char	*full_path;

	full_path = get_full_path(args[0]);
	if (full_path == NULL)
	{
		dprintf(2, "%s: command not found\n", args[0]);
		exit(0);
	}
	execve(full_path, args, NULL);
	dprintf(2, "exec %s failed :(\n", args[0]);
	free(full_path);
}

void	execute_node(t_tnode *node)
{
	int	p[2];
	t_tnode_exec	*exec_node;
	t_tnode_pipe	*pipe_node;
	t_tnode_redir	*redir_node;
	t_tnode_heredoc	*heredoc_node;

	if (node == NULL)
		panic("null node");
	if (node->type == EXEC)
	{
		if (fork1() == 0)
		{
			exec_node = (t_tnode_exec*)node;
			if (exec_node->argv[0] == 0)
				exit(1);
			execute_command(exec_node->argv);
		}
	}
	else if (node->type == REDIR)
	{
		redir_node = (t_tnode_redir*)node;
		close(redir_node->fd);
		if (open(redir_node->file, redir_node->flags, 0666) < 0)
		{
			perror(redir_node->file);
			exit(1);
		}
		execute_node(redir_node->node);
	}
	else if (node->type == PIPE)
	{
		pipe_node = (t_tnode_pipe*)node;
		if (pipe(p) < 0)
			panic("pipe error");
		if (fork1() == 0)
		{
			dup2(p[1], STDOUT_FILENO);
			close(p[1]);
			close(p[0]);
			execute_node(pipe_node->branch_left);
		}
		if (fork1() == 0)
		{
			close(p[1]);
			dup2(p[0], STDIN_FILENO);
			close(p[0]);
			execute_node(pipe_node->branch_right);
		}
		close(p[0]);
		close(p[1]);
		wait(0);
		wait(0);
	}
	else if (node->type == HEREDOC)
	{
		heredoc_node = (t_tnode_heredoc*)node;
		
		// char *text = ft_calloc(1, 1);
		// char *temp;
		// char *line = NULL;
		// char *limiter = heredoc_node->delm_start;
		// while (1)
		// {
		// 	line = readline("heredoc> ");
		// 	if (!line || ft_strcmp(line, limiter) == 0)
		// 	{
		// 		free(line);
		// 		break ;
		// 	}
		// 	temp = ft_strjoin(text, line);
		// 	free(text);
		// 	free(line);
		// 	text = ft_strjoin(temp, "\n");
		// 	free(temp);
		// }
		// ft_putstr_fd(text, STDIN_FILENO);
		execute_node(heredoc_node->node);
	}
	else
		panic("invalid node type");
	exit(0);
}