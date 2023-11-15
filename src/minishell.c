/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fernacar <fernacar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/06 21:55:33 by fernacar          #+#    #+#             */
/*   Updated: 2023/11/15 00:14:38 by fernacar         ###   ########.fr       */
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

void	free_commands(t_minishell *data)
{
	if (data->token_list)
	{
		free_split(data->token_list);
		data->token_list = NULL;
	}
	if (data->tree_root)
	{
		free_node(data->tree_root);
		data->tree_root = NULL;
	}
}

void	free_data(t_minishell *data)
{
	free_commands(data);
	if (data->env)
	{
		free_split(data->env);
		data->env = NULL;
	}
	if (data->uninit)
	{
		free_split(data->uninit);
		data->uninit = NULL;
	}
}

static int	prompt_stuff(char **input)
{
	*input = NULL;
	while (1)
	{
		*input = readline(PROMPT);
		if (!*input)
			return (0);
		if ((*input)[0] == '\0')
		{
			free(*input);
			continue ;
		}
		add_history(*input);
		return (1);
	}
}

void	parent_buildins(t_minishell *d)
{
	if (d->tree_root->type == EXEC
		&& (ft_strcmp(((t_tnode_exec *)d->tree_root)->argv[0], "unset") == 0 
			|| (ft_strcmp(((t_tnode_exec *)d->tree_root)->argv[0],
					"export") == 0
				&& ((t_tnode_exec *)d->tree_root)->argv[1] != NULL)
			|| (ft_strcmp(((t_tnode_exec *)d->tree_root)->argv[0], "cd") == 0)
			|| (ft_strcmp(((t_tnode_exec *)d->tree_root)->argv[0],
					"exit") == 0)))
	{
		if (ft_strcmp(((t_tnode_exec *)d->tree_root)->argv[0], "unset") == 0)
			unset_buildin(((t_tnode_exec *)d->tree_root)->argv, &d->env,
				&d->uninit, &d->exit_status);
		else if (ft_strcmp(((t_tnode_exec *)d->tree_root)->argv[0],
				"export") == 0)
			export_buildin(((t_tnode_exec *)d->tree_root)->argv, &d->env,
				&d->uninit, &d->exit_status);
		else if (ft_strcmp(((t_tnode_exec *)d->tree_root)->argv[0], "cd") == 0)
			cd_buildin(((t_tnode_exec *)d->tree_root)->argv, &d->env,
				&d->exit_status);
		else if (ft_strcmp(((t_tnode_exec *)d->tree_root)->argv[0],
				"exit") == 0)
			exit_buildin(((t_tnode_exec *)d->tree_root)->argv, d->exit_status);
	}
}

void	run_prompt(char *input, t_minishell *data)
{
	int		pid;

	data->token_list = make_token_list(input, data->env, data->exit_status);
	data->tree_root = build_tree(data->token_list, &data->exit_status);
	if (data->tree_root)
	{
		parent_buildins(data);
		pid = fork1();
		if (pid == 0)
		{
			check_heredocs(data->tree_root, data);
			execute_node(data->tree_root, data);
			free(input);
			free_data(data);
			exit(data->exit_status);
		}
		waitpid(pid, &data->exit_status, 0);
		data->exit_status = WEXITSTATUS(data->exit_status);
		unlink(".heredoc");
	}
	free_commands(data);
	free(input);
}

// TO FIX

// check if exit status is correct everywhere

// export and unset : gives error message twice

int	main(int ac, char **av, char **envp)
{
	char		*input;
	t_minishell	data;

	data = (t_minishell){0};
	data.env = env_copy(envp);
	init_env(data.env);
	wait_signal();
	while (1)
	{
		if (!prompt_stuff(&input))
			break ;
		run_prompt(input, &data);
	}
	rl_clear_history();
	printf("🔥exit🔥\n");
	free_data(&data);
	return (data.exit_status);
}
