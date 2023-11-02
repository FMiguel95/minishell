/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fernacar <fernacar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/06 21:55:33 by fernacar          #+#    #+#             */
/*   Updated: 2023/11/02 22:38:04 by fernacar         ###   ########.fr       */
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

// TO FIX

// when expanding env vars, token must end when next char is not alphanumeric or underscore

// expand $? to exit status

// check if exit status is correct everywhere

// unset SHLVL then run ./minishell : segfault
	// #0 0x4d1dab in ft_atoi (/nfs/homes/fernacar/Git/minishell/minishell+0x4d1dab)
    // #1 0x4cf7ac in init_env /nfs/homes/fernacar/Git/minishell/src/init_vars.c:195:18
    // #2 0x4c88e7 in main /nfs/homes/fernacar/Git/minishell/src/minishell.c:84:2

// export a then export a=1 : a is still in uninit

// export a then unset a : must remove a from uninit list 

// cd - : must print path

// unset _
// export _

// implement exit


// NOTES

// my_getenv behaves like getenv: it returns NULL if key nor found


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


		tokens = make_token_list(input, env_cpy);
		tree_root = build_tree(tokens);
		//print_node(tree_root);
		if (tree_root->type == EXEC
			&& (ft_strcmp(((t_tnode_exec *)tree_root)->argv[0], "unset") == 0 
			|| (ft_strcmp(((t_tnode_exec *)tree_root)->argv[0], "export") == 0
				&& ((t_tnode_exec *)tree_root)->argv[1] != NULL)
			|| (ft_strcmp(((t_tnode_exec *)tree_root)->argv[0], "cd") == 0)))
		{
			if (ft_strcmp(((t_tnode_exec *)tree_root)->argv[0], "unset") == 0)
				unset_buildin(((t_tnode_exec *)tree_root)->argv, &env_cpy);
			else if (ft_strcmp(((t_tnode_exec *)tree_root)->argv[0], "export") == 0)
				export_buildin(((t_tnode_exec *)tree_root)->argv, &env_cpy, &uninit);
			else if (ft_strcmp(((t_tnode_exec *)tree_root)->argv[0], "cd") == 0)
				cd_buildin(((t_tnode_exec *)tree_root)->argv, env_cpy);
		}
		// printf("===================================\n");
		// print_list(uninit);
		if (!(pid = fork1()))
		{
			//print_node(tree_root);
			check_heredocs(tree_root);
			execute_node(tree_root, &env_cpy, &uninit);
			free(input);
			free_split(tokens);
			free_node(tree_root);
			free_split(env_cpy);
			free_split(uninit);			
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
	free_split(uninit);
	return (exit_status);
}
