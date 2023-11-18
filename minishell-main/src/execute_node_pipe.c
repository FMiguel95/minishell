/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_node_pipe.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fernacar <fernacar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/16 22:26:11 by fernacar          #+#    #+#             */
/*   Updated: 2023/11/16 22:26:54 by fernacar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

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
