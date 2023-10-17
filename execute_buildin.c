#include <limits.h>	
#include <stdio.h>	
#include <unistd.h>	
#include <errno.h>	
#include <string.h>	
#include <stdlib.h>	

int	exit_status; 

int	is_option(char *str)
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

void	env_buildin(int argc, char **argv, char **env_copy)
{
	int	i = 0;			
	char	*env_var = env_copy[0];	
	
	(void) argv;			
	if(argc > 2)			
	{
		errno = EINVAL;		
		perror("minishell buildin specific");	
		exit_status = 2;	
		exit(exit_status);
	}
	while (env_var)
	{
		printf("%s\n", env_var);
		i++;
		env_var = env_copy[i];
	}
	exit_status = EXIT_SUCCESS;
}

void	echo_buildin(int argc, char **argv, char **env_copy)
{
	int	i = 2;		
	int	flag_n_option = 0;

	while (is_option(argv[i]) == 10)
	{
		if (is_option(argv[i]) == 10)
			flag_n_option = 1;
		i++;
	}
	while(argv[i])
	{
		printf("%s", argv[i]);
			if (argv[i + 1])
				printf(" ");
		i++;
	}
	if (!flag_n_option)
		printf("\n");
	exit_status = EXIT_SUCCESS;
}

void	pwd_buildin(int argc, char **argv, char **env_copy)
{
	char	curr_work_dir[PATH_MAX];

	
	while (*argv)
		if(is_option(*(++argv)))			
		{
			errno = 0;		
			perror("minishell buildin specific");	
			exit_status = 2;	
			exit(exit_status);
		}
	if(getcwd(curr_work_dir, PATH_MAX) == NULL) 
	{
		perror("pwd error:");	

		exit_status = 2;  
	}	
	
	else
	{
		printf("%s\n", curr_work_dir);	
		exit_status = EXIT_SUCCESS;	
	}

}

void	execute_builtin(int argc, char **argv, char **env_copy)
{
	if (!strcmp(argv[1], "echo"))
		echo_buildin(argc, argv, env_copy);
	else if (!strcmp(argv[1], "env"))
		env_buildin(argc, argv, env_copy);
	else if (!strcmp(argv[1], "pwd"))
		pwd_buildin(argc, argv, env_copy);
//	else if (!ft_strcmp(argv[1], "export"))
//		export_buildin(argc, argv, env_copy);
//	else if (!ft_strcmp(argv[1], "unset"))
//		unset_buildin(argc, argv, env_copy);
//	else if (!ft_strcmp(argv[1], "cd"))
//		cd_buildin(argc, argv, env_copy);
//	else if (!ft_strcmp(argv[1], "exit"))
//		exit_buildin(argc, argv, env_copy);
	else
		return ;
}

int	main(int argc, char **argv, char **envp)
{
	execute_builtin(argc, argv, envp);
}
