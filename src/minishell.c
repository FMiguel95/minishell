/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fernacar <fernacar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/06 21:55:33 by fernacar          #+#    #+#             */
/*   Updated: 2023/10/30 23:46:22 by fernacar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int exit_status;

void	handle_sigint(int signal)
{
	exit_status = 130;
	rl_replace_line("", 0);
	write(1, "\n", 1);
	rl_on_new_line();
	rl_redisplay();
}

void	wait_signal(void)
{
	signal(SIGINT, handle_sigint);
	signal(SIGQUIT, SIG_IGN);
}

void panic(char *str)
{
	printf("%s\n", str);
	exit(1);
}

int	fork1(void)
{
	int	pid;

	pid = fork();
	if (pid == -1)
		panic("fork");
	return(pid);
}

// TODO: fix heredocs with pipes, heredocs with redirects leaks
// quote stuff (ls "<" Makefile  aaa)

int	main(int ac, char **av, char **envp)
{
	char	*input;
	char	**tokens;
	t_tnode	*tree_root;
	int		pid;
	char	**env_cpy;
	char	**uninit;

	exit_status = 0;
	env_cpy = env_copy(envp);
	uninit = NULL;
	init_env(env_cpy);
	// printf("-----------main\n");
	// printf("%s\n", env_cpy[0]);
	//print_list((char**)env_copy);
	wait_signal();
	while (1)
	{
		// printf("============================main\n");
		// print_list(env_cpy);
		input = readline("\001\e[1;37m\002minis\001\e[31;5m\002hell\001\e[0m\002> ");
		if (!input)
			break ;
		if (input[0] == '\0')
		{
			free(input);
			continue ;
		}
		add_history(input);


		tokens = make_token_list(input);
		tree_root = build_tree(tokens);
		if (tree_root->type == EXEC
			&& (ft_strcmp(((t_tnode_exec *)tree_root)->argv[0], "unset") == 0 
			|| (ft_strcmp(((t_tnode_exec *)tree_root)->argv[0], "export") == 0
			&& ((t_tnode_exec *)tree_root)->argv[1] != NULL)))
		{
			if (ft_strcmp(((t_tnode_exec *)tree_root)->argv[0], "unset") == 0)
				unset_buildin(((t_tnode_exec *)tree_root)->argv, &env_cpy);
			else if (ft_strcmp(((t_tnode_exec *)tree_root)->argv[0], "export") == 0)
				export_buildin(((t_tnode_exec *)tree_root)->argv, &env_cpy, &uninit);
		}
		else if (!(pid = fork1()))
		{
			//print_node(tree_root);
			execute_node(tree_root, &env_cpy, uninit);
			free(input);
			free_split(tokens);
			free_node(tree_root);
			// if (unlink(".heredoc") < 0)
			// {
			// 	perror(".heredoc");
			// }
			exit(exit_status);
		}
		free(input);
		free_split(tokens);
		free_node(tree_root);
		waitpid(pid, &exit_status, 0);
		unlink(".heredoc");
		exit_status = WEXITSTATUS(exit_status);
	}
	rl_clear_history();
	printf("🔥exit🔥\n");
	printf("exit_status:%d\n", exit_status);
	free_split(env_cpy);
	return (exit_status);
}
