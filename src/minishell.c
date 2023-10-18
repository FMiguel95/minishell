/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fernacar <fernacar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/06 21:55:33 by fernacar          #+#    #+#             */
/*   Updated: 2023/10/17 20:43:35 by fernacar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	exit_status;

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

int	main(int ac, char **av, char **envp)
{
	char	*input;
	char	**tokens;
	char	current_dir[1024];
	int		pid;


	wait_signal();
	getcwd(current_dir, sizeof(current_dir));
	// handle_sigint(SIGINT);
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



		tokens = make_token_list(input);
		free(input);

		if (!(pid = fork1()))
		{
			execute_node(build_tree(tokens), envp);
			exit(0);
		}
		waitpid(pid, &exit_status, 0);

		free_split(tokens);
	}
	rl_clear_history();
	printf("🔥exit🔥\n");
	return (EXIT_SUCCESS);
}
