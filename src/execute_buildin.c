// #include <limits.h>	
// #include <stdio.h>	
// #include <unistd.h>	
// #include <errno.h>	
// #include <string.h>	
// #include <stdlib.h>	

#include "../include/minishell.h"

//int	exit_status; 
extern int exit_status;

int	ft_perror(char *str, char letter)
{
	ft_putstr_fd("bash: ", 2);
	ft_putstr_fd(str, 2);
	ft_putstr_fd(": -", 2);
	ft_putchar_fd(letter, 2);
	ft_putstr_fd(": invalid option\n", 2);	
	ft_putstr_fd(str, 2);
	ft_putstr_fd(": usage: ", 2);
	ft_putstr_fd(str, 2);
	if (!ft_strcmp(str, "pwd"))
		ft_putstr_fd("\n", 2);
	if (!ft_strcmp(str, "unset"))
		ft_putstr_fd("[name ...]\n", 2);
	if (!ft_strcmp(str, "export"))
		ft_putstr_fd("[name[=value] ...]\n", 2);
	exit_status = 2;
	exit (exit_status);
}

static int	is_option_echo(char *str)
{
	if (!str)
		return (0);
	if (*str == '-')
	{
		while (*(++str) == 'n')
			;
		if (!*str)	
			return (10);
	}
	return (0);
	
}

// void	env_buildin(int argc, char **argv, char **env_copy)
// {
// 	int	i = 0;
// 	char	*env_var = env_copy[0];	
	
// 	(void) argv;			
// 	if(argc > 2)			
// 	{
// 		errno = EINVAL;		
// 		perror("minishell buildin specific");	
// 		exit_status = 2;	
// 		exit(exit_status);
// 	}
// 	while (env_var)
// 	{
// 		printf("%s\n", env_var);
// 		i++;
// 		env_var = env_copy[i];
// 	}
// 	exit_status = EXIT_SUCCESS;
// }

// void	echo_buildin(char **argv, char **env_copy)
// {
// 	int	i = 2;		
// 	int	flag_n_option = 0;

// 	while (is_option_echo(argv[i]) == 10)
// 	{
// 		if (is_option_echo(argv[i]) == 10)
// 			flag_n_option = 1;
// 		i++;
// 	}
// 	while(argv[i])
// 	{
// 		printf("%s", argv[i]);
// 			if (argv[i + 1])
// 				printf(" ");
// 		i++;
// 	}
// 	if (!flag_n_option)
// 		printf("\n");
// 	exit_status = EXIT_SUCCESS;
// }

// void	pwd_buildin(int argc, char **argv, char **env_copy)
// {
// 	char	curr_work_dir[PATH_MAX];

	
// 	while (*argv)
// 		if(is_option(*(++argv)))			
// 		{
// 			errno = 0;		
// 			perror("minishell buildin specific");	
// 			exit_status = 2;	
// 			exit(exit_status);
// 		}
// 	if(getcwd(curr_work_dir, PATH_MAX) == NULL) 
// 	{
// 		perror("pwd error:");	

// 		exit_status = 2;  
// 	}	
	
// 	else
// 	{
// 		printf("%s\n", curr_work_dir);	
// 		exit_status = EXIT_SUCCESS;	
// 	}

// }

char	**execute_builtin(char **argv, char **env_copy, char **uninit)
{
	if (!ft_strcmp(argv[0], "echo"))
		echo_buildin(argv);
	else if (!ft_strcmp(argv[0], "env"))
		env_buildin(argv, env_copy);
	else if (!ft_strcmp(argv[0], "pwd"))
		pwd_buildin(argv);
	else if (!ft_strcmp(argv[0], "export"))
		export_buildin(argv, env_copy, uninit);
	else if (!ft_strcmp(argv[0], "unset"))
		return (unset_buildin(argv, env_copy));
//	else if (!ft_strcmp(argv[1], "cd"))
//		cd_buildin(argc, argv, env_copy);
//	else if (!ft_strcmp(argv[1], "exit"))
//		exit_buildin(argc, argv, env_copy);
	return (env_copy);
}

// int	main(int argc, char **argv, char **envp)
// {
// 	execute_builtin(argc, argv, envp);
// }
