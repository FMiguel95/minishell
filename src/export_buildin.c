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

int	perror_export(char letter)
{
		write(2, "bash: export: -", 15);
		write(2, &letter, 1);
		write(2, ": invalid option\nexport: usage: export [name[=value] ...]\n", strlen(": invalid option\nexport: usage: export [name[=value] ...]\n"));	// cannot use strlen()
		exit_status = 2;
		exit (exit_status);	// talvez seja melhor usar um perror o exit vai ser 1????
}

char	*extract_value(char *str)
{
	if (!*str)
		return (0); // confirm if is the correct one
	while (*str != '=')
	{		
//			printf("%c\n", *str);
			str++;
		
	}
	str++;
		return (char *)str;		// similar to exam ramk 2 ft_strpbrk
}

int	pointer_array_len(char **pointers)
{
	int	len = 0;

	while (pointers[len])
		len++;	
	printf("number of env var: %d\n", len);
	return (len);
}

int	key_length(char *str)		
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
/*
char	*extract_key(char *str)		
{
	int	length;
	char	*key;
	
	if (!str)
		return (0); // confirm if is the correct one
	length = key_length(str);
	key = (char *)malloc(sizeof(char) * (length + 1));
	strncpy(key, str, length);
	printf("key to be passed: %s\n", key);
	return (key);
}
*/
int	find_key_env_index(char *argv, char **env, int index)
{
	int	i_str = 0;
	int	i_env = 0;
	int	len_env = 0;

	len_env = pointer_array_len(env);
//	printf("number of env %d\n", len_env);
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
	printf("i_env %d\n", i_env);
	return (len_env);
}

void	ft_swap(char **env1, char **env2)	
{
	char	*temp = NULL;
	
	temp = *env1;
	*env1 = *env2;
	*env2 = temp;
}

char	**env_copy(char **env)
{
	char **env_copy;
	size_t	key_index = 0;
	size_t	value_index = 0;

//	while (env[key_index])
//		key_index++;
	env_copy = (char**)malloc(sizeof(char*) * (pointer_array_len(env) + 1));
	if (!env_copy)
		return NULL;
	env_copy[key_index] = '\0';
//	key_index = 0;
	while (env[key_index])
	{
		env_copy[key_index] = (char *)malloc(sizeof(char) * (strlen(env[key_index]) + 1));
		if (!env_copy[key_index])
			return NULL;
		env_copy[key_index][strlen(env[key_index])] = '\0';
		while (value_index < strlen(env[key_index]))
		{
			env_copy[key_index][value_index] = env[key_index][value_index] ;
			value_index++;
		}
		value_index = 0;
		printf("%s\n", env_copy[key_index]);
		key_index++;	
	}
	return (env_copy);
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

//void	bubble_sort_array(char **env_copy, char **unitialized)
void	bubble_sort_array(char **env_copy)
{
//	(void)unitialized;
	int	sorted;
	int	index;

	sorted = 0;
	while (!sorted)
	{
		sorted = 1;
		index = 0;
		while (env_copy[index + 1])
		{
			if (strncmp(env_copy[index], env_copy[index + 1], key_length(env_copy[index])) > 0)
			{
				ft_swap(&env_copy[index], &env_copy[index + 1]);
				sorted = 0;
			}
			index++;
		}
	}

}

void	print_export(char **env_copy)
{
	size_t	index = 0;
	
	while(env_copy[index])
	{
		printf("declare -x =\"%s\"\n", extract_value(env_copy[index]));
		index++;
	}
}

void	export_buildin(int argc, char **argv, char **env_copy, char **unitialized)
{
	(void)argc;
	(void)env_copy;
	(void)unitialized;

	int	i = 1;		// index variable to goes through the arrays of pointers of each argument string pointer 
//	int	key_index = 0;
//	int	env_index = 0;

	if (argc == 1)		//for test purpose only
		exit(0);
//	if (argc == 2)		// has no options or arguments
//		export_print(env_copy, unitialized);
	if (is_option(argv[i]))
		perror_export(is_option(argv[i]));

/*
		while (i_str < index && argv[i_str] == env[i_env][i_str])	
			i_str++;
//		printf("%d  i %d   %s, \n", i_env, i_str, env[i_env]);
		if (env[i_env][i_str] == '=' && index == i_str)
			return (i_env);
*/
/*
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
*/	exit_status = EXIT_SUCCESS;
}

int	main(int argc, char **argv, char **env)
{
	(void)argc;
	(void)argv;
	(void)env;
	char	**copy;

	copy = env_copy(env);
//	env_print(copy);
	bubble_sort_array(copy);
	printf("\n\ndepois do sort\n\n");
//	env_print(copy);
	print_export(copy);
//	printf("key to be passed: %s\n", extract_key("isto e um teste=hxbndtsdk"));
	
//	ft_swap(&env[0], &env[1]);
//	env_print(env);
}
