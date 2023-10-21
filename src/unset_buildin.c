#include <stdio.h>	// printf
#include <stdlib.h>	// exit
#include <errno.h>	// errno
#include <unistd.h>	// write
#include <string.h>	// strlen


int	exit_status; // in the original will be a global variable

int	is_option(char *str)
{
	char	letter = str[1];
	
	if (!str)
		return (0);
	if (*str == '-')
	{
		
		if (*(++str) != '-' && *str != '\0')   // ver se a segunda condicao faz sentido, acho que sim
			return (letter);
	}
	return (0);
}

int	perror_unset(char letter)
{
		write(2, "bash: unset: -", 14);
		write(2, &letter, 1);
		write(2, ": invalid option\nunset: usage: unset [name ...]\n", strlen(": invalid option\nunset: usage: unset [name ...]\n"));	// cannot use strlen()
		exit_status = 2;
		exit (exit_status);	// talvez seja melhor usar um perror o exit vai ser 1????
}


int	extract_key_index(char *str)		
{
	int	i = 0;
	
	if (!str[i])
		return (0); // confirm if is the correct one
	while (str[i] != '=')
	{		
//		printf("%c", str[i]);
		i++;
	}
//	printf("%d \n", i);
		return (i);
}


int	find_key_env_index(char *argv, char **env, int index)
{
	int	i_str = 0;
	int	i_env = 0;
	int	len_env = 0;

	while (env[i_env])
		i_env++;
	len_env = i_env;
	i_env = 0;
//	printf("number of env %d\n", len_env);
	while (env[i_env])
	{
		while (i_str < index && argv[i_str] == env[i_env][i_str])	
			i_str++;
//		printf("%d  i %d   %s, \n", i_env, i_str, env[i_env]);
		if (env[i_env][i_str] == '=' && index == i_str)
		{
			
			return (i_env);
		}
		i_str = 0;
		i_env++;
	}
	printf("i_env %d\n", i_env);
	return (len_env);
}

void	unset_buildin(int argc, char **argv, char **env_copy)
{
	int	i = 1;		// index variable to goes through the arrays of pointers of each argument string pointer 
	int	key_index = 0;
	int	env_index = 0;

	if (is_option(argv[i]))
		perror_unset(is_option(argv[i]));
	while(i < argc)
	{
		key_index = strlen(argv[i]);
//		printf("index passed %d\n", key_index);
		env_index = find_key_env_index(argv[i], env_copy, key_index);
		printf("env_index %d\n", env_index);
//		free(env_copy[env_index]);
		while (env_copy[env_index])
		{	
//			printf("env[%d] %s", env_index, env_copy[env_index]);
			env_copy[env_index] = env_copy[env_index + 1];
			printf("env[%d] %s\n", env_index, env_copy[env_index]);
			env_index++;
		}
		env_copy[env_index] = NULL;
		i++;
	}
	exit_status = EXIT_SUCCESS;
}

int	main(int argc, char **argv, char **env)
{
	unset_buildin(argc, argv, env);
}


