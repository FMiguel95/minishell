/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fernacar <fernacar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/06 21:55:33 by fernacar          #+#    #+#             */
/*   Updated: 2023/10/05 22:09:00 by fernacar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	exit_status;

void	handle_sigint(int signal)
{
	exit_status = 130;
	write(1, "\n", 1);
	rl_replace_line("", 0);	
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

int	main(int ac, char **av, char **env)
{
	char	*input;
	char	current_dir[1024];
	int		pid;


	wait_signal();
	getcwd(current_dir, sizeof(current_dir));
	while (1)
	{
		
		input = readline("\033[1;37mminis\033[31;5mhell\033[0m> ");
		if (!input)
			break ;
		if (ft_strcmp(input, "") == 0)
		{
			free(input);
			continue ;
		}
		add_history(input);



		make_token_list(input);

		if (fork1() == 0)
			execute_node(build_tree(input));
		wait(0);
		free(input);

	}
	printf("🔥exit🔥\n");
	clear_history();
	return (EXIT_SUCCESS);
}
