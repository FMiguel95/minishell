/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fernacar <fernacar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/06 21:55:33 by fernacar          #+#    #+#             */
/*   Updated: 2023/10/26 20:34:26 by fernacar         ###   ########.fr       */
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
	char	**env_copy;

	exit_status = 0;
	wait_signal();
	while (1)
	{
		input = readline("\001\e[1;37m\002minis\001\e[31;5m\002hell\001\e[0m\002> ");
		if (!input)
			break ;
		if (input[0] == '\0')
		{
			free(input);
			continue ;
		}
		add_history(input);


		if (!(pid = fork1()))
		{
			tokens = make_token_list(input);
			tree_root = build_tree(tokens);
			//print_node(tree_root);
			execute_node(tree_root, envp);
			free(input);
			free_split(tokens);
			free_node(tree_root);
			exit(exit_status);
		}
		waitpid(pid, &exit_status, 0);
		free(input);
	}
	rl_clear_history();
	printf("🔥exit🔥\n");
	printf("exit_status:%d\n", exit_status);
	return (exit_status);
}
