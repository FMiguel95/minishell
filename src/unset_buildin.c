#include <stdio.h>	// printf
#include <stdlib.h>	// exit
#include <errno.h>	// errno
#include <unistd.h>	// write
#include <string.h>	// strlen

int	exit_status;

void	ft_putstr_fd(char *s, int fd)
{
	int	i;

	i = 0;
	if (!s || fd < 0)
		return ;
	while (s[i])
	{
		write(fd, &s[i], 1);
		i++;
	}
	return ;
}

int	ft_strcmp(const char *s1, const char *s2)
{
	int	i;

	i = 0;
	while (s1[i] == s2[i] && s1[i] != '\0' && s2[i] != '\0')
	{
		i++;
	}
	return (s1[i] - s2[i]);
}

int	is_option(char *str)
{
	char	letter = str[1];
	
	if (!str)
		return (0);
	if (*str == '-')	
		if (*(++str) != '-' && *str != '\0')   // ver se a segunda condicao faz sentido, acho que sim
			return (letter);
	return (0);
}

int	ft_perror(char *str, char letter)
{
		ft_putstr_fd("bash: ", 2);
		ft_putstr_fd(str, 2);
		ft_putstr_fd(": -", 2);
		ft_putstr_fd(&letter, 2);
		ft_putstr_fd(": invalid option\n", 2);	
		ft_putstr_fd(str, 2);
		ft_putstr_fd(": usage: ", 2);
		ft_putstr_fd(str, 2);
//		if (!ft_strcmp(str, "unset"))
		if (!ft_strcmp(str, "./a.out"))
			ft_putstr_fd("\n", 2);
		if (!ft_strcmp(str, "unset"))
			ft_putstr_fd("[name ...]\n", 2);
		if (!ft_strcmp(str, "export"))
			ft_putstr_fd("[name[=value] ...]\n", 2);
		exit_status = 2;
		exit (exit_status);
}

size_t	key_length(char *str)		
{
	size_t	i = 0;
	
//	printf("strlen in keylen: %zu\n", strlen(str));
	if (!str[i])
		return (0); // confirm if is the correct one
	while (i < strlen(str))
	{
		if (str[i] == '=' || str[i] == ' ')
			return (i);
		i++;
	}
//	printf("saida do key_len: %zu \n", i);
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
	while (env[i_env])
	{
		while (i_str < index && argv[i_str] == env[i_env][i_str])	
			i_str++;
//		printf("%d  i %d   %s, \n", i_env, i_str, env[i_env]);
		if (env[i_env][i_str] == '=' && index == i_str)
			return (i_env);
		i_str = 0;
		i_env++;
	}
//	printf("i_env %d\n", i_env);
	return (len_env);
}

int	pointer_array_len(char **pointers)
{
	int	len = 0;

	while (pointers[len])
		len++;	
//	printf("number of env var: %d\n", len);
	return (len);
}

void	env_print(char **env)
{
	size_t	key_index = 0;
	size_t	value_index = 0;
	
	while (env[key_index])
	{
		while (value_index < strlen(env[key_index]))
			value_index++;
		printf("%s\n", env[key_index]);
		key_index++;	
	}
}

char	**env_copy(char **env)
{
	char **copy;
	size_t	key_index = 0;
	size_t	value_index = 0;

//	while (env[key_index])
//		key_index++;
	copy = (char**)malloc(sizeof(char*) * (pointer_array_len(env) + 2));
	if (!copy)
		return NULL;
	copy[pointer_array_len(env) ] = '\0';
	copy[pointer_array_len(env)  + 1] = '\0';
//	key_index = 0;
	while (env[key_index])
	{
		copy[key_index] = (char *)malloc(sizeof(char) * (strlen(env[key_index]) + 1));
		if (!copy[key_index])
			return NULL;
		copy[key_index][strlen(env[key_index])] = '\0';
		while (value_index < strlen(env[key_index]))
		{
			copy[key_index][value_index] = env[key_index][value_index] ;
			value_index++;
		}
		value_index = 0;
		key_index++;	
	}
//	printf("env_copy len: %d\n", pointer_array_len(env_copy));
	return (copy);
}

char	**unset_buildin(char **argv, char **copy)
{
	int	i = 1; 
	int	key_index = 0;
	int	env_index = 0;
	char	**new_copy;

	if (is_option(argv[i]))
		ft_perror(argv[i -1], is_option(argv[i]));
//	while(i < argc)
	while(argv[i])
	{
		key_index = strlen(argv[i]);
//		printf("index passed %d\n", key_index);
		env_index = find_key_env_index(argv[i], copy, key_index);
//		printf("env_index %d\n", env_index);
//		free(env_copy[env_index]);
		while (copy[env_index])
		{	
//			printf("env[%d] %s", env_index, env_copy[env_index]);
			copy[env_index] = copy[env_index + 1];
//			printf("env[%d] %s\n", env_index, copy[env_index]);
			env_index++;
		}
		copy[env_index] = NULL;
		new_copy = env_copy(copy);
//		free_split(copy);
		i++;
	}
	exit_status = EXIT_SUCCESS;
	return (new_copy);
}



/*
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
//	printf("i_env %d\n", i_env);
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
//		printf("env_index %d\n", env_index);
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
*/
int	main(int argc, char **argv, char **env)
{
	(void)argc;
	char	**new_env;
	
	new_env = unset_buildin(argv, env);
	printf("\n\n\nbefor print env\n\n\n");
	env_print(new_env);
}


