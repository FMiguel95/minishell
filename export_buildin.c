#include <stdio.h>	// printf
#include <stdlib.h>	// exit
#include <errno.h>	// errno
#include <unistd.h>	// write
#include <string.h>	// strlen

int	exit_status; // in the original will be a global variable

int	ft_isdigit(int c)
{
	return (c >= '0' && c <= '9');
}

int	key_length(char *str)		
{
	size_t	i = 0;
	
//	printf("strlen in keylen: %zu\n", strlen(str));
	if (!str[i])
		return (0); // confirm if is the correct one
	while (i < strlen(str))
	{
		if (str[i] == '=' || str[i] == ' ' || str[i] == '\0')
			return (i);
		i++;
	}
//	printf("saida do key_len: %zu \n", i);
	return (i);
}

int	key_isalnum_underscore(char *str, size_t index)
{
	size_t	i = 0;
	
	while (i < index)
	{
		if (!((str[i] >= 'a' && str[i] <= 'z') || (str[i] >= 'A' && str[i] <= 'Z')
			|| (str[i] >= '0' && str[i] <= '9') || str[i] == '_' || str[i] == '='))
			return (0);
		i++;
	}
//	if (str[i] == '\0')
		return (1);
//	printf("inside keyalpha char %c\n", str[i]);
//	return (str[i]);
}

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

int	perror_identifier(char *str)
{
		write(2, "bash: export: `", 15);
		write(2, str, strlen(str));
		write(2, "' not a valid identifier\n", 25);	// cannot use strlen()
		exit_status = 1;
//		exit (exit_status);
		return (exit_status);
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

size_t	pointer_array_len(char **pointers)
{
	size_t	len = 0;

	while (pointers[len])
		len++;	
//	printf("number of env var: %d\n", len);
	return (len);
}


char	*extract_key(char *str)		
{
	int	length;
	char	*key;
	
	if (!str)
		return (0); // confirm if is the correct one
	length = key_length(str);
	key = (char *)malloc(sizeof(char) * (length + 1));	// must be freed
	strncpy(key, str, length);
//	printf("key to be passed: %s\n", key);
	return (key);
}



/*
char	*extract_key(char *str)		
{
	int	length;


	if (!str)
		return (0); // confirm if is the correct one
	length = key_length(str);
	str[length] = '\0';		
	return (key);
}
*/

size_t	find_key_env_index(char *argv, char **env, int index)
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
//	printf("i_env %d\n", i_env);
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
	env_copy = (char**)malloc(sizeof(char*) * (pointer_array_len(env) + 2));
	if (!env_copy)
		return NULL;
	env_copy[pointer_array_len(env) ] = '\0';
	env_copy[pointer_array_len(env)  + 1] = '\0';
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
		key_index++;	
	}
//	printf("env_copy len: %d\n", pointer_array_len(env_copy));
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
void	bubble_sort_array(char **copy)
{
//	(void)unitialized;
	int	sorted;
	int	index;

	sorted = 0;
	while (!sorted)
	{
		sorted = 1;
		index = 0;
		while (copy[index + 1])
		{
			if (strncmp(copy[index], copy[index + 1], key_length(copy[index])) > 0)
			{
				ft_swap(&copy[index], &copy[index + 1]);
				sorted = 0;
			}
			index++;
		}
	}
}

char	**ft_strjoin_var(char **s1, char **s2)
{
	char	**result;
	size_t	i;
	size_t	j;

//	printf("copy len: %zu\n", pointer_array_len(s1));
//	printf("copy len: %zu\n", pointer_array_len(s2));
	result = malloc(sizeof(char *) * (pointer_array_len(s1) + pointer_array_len(s2) + 1));
	if (!result)
		return (NULL);
	result[pointer_array_len(s1) + pointer_array_len(s2)] = NULL;
	i = 0;
	while (s1[i])
	{
		result[i] = s1[i];
//		printf("pointer[%zu] = %p\n", i ,result[i]);
		i++;
	}
//	if (!pointer_array_len(s2))
//		return (result);
//	else
//	{
		j = 0;
		while (s2[j])
		{
			result[i + j] = s2[j];
			j++;
		}
//	}
//	printf("all_var len: %zu\n", pointer_array_len(result));
	return (result);
}

void	print_export(char **copy, char **uninitialized)
//void	print_export(char **copy)
{
	size_t	index = 0;
//	char	*all_var;
	char	**var_sorted = malloc(sizeof(char*));
//	size_t	all_len = pointer_array_len(copy) + pointer_array_len(uninitialized);

//	all_var = ft_strjoin(*copy, *uninitialized);	
	var_sorted = ft_strjoin_var(copy, uninitialized);
//	env_print(var_sorted);
	
//	env_sorted = var_copy(copy);
	bubble_sort_array(var_sorted);
//	env_print(var_sorted);
	while(var_sorted[index])
	{
		if (var_sorted[index][key_length(var_sorted[index])] == '=')
		{
			if (strcmp(extract_key(var_sorted[index]), "_"))
				printf("declare -x %s=\"%s\"\n", extract_key(var_sorted[index]), extract_value(var_sorted[index]));
		}
		else
			printf("declare -x %s\n", extract_key(var_sorted[index]));	
		index++;
	}
//	free(var_sorted);
}

char	**substitute_env_str(char *arg, char **new_copy, size_t env_index)
{
	char	*new_env_value = NULL;

	new_env_value = malloc(strlen(arg) + 1);
	new_env_value = arg;
	if (!new_env_value)
		return NULL; // PANIC
	new_copy[env_index] = new_env_value;
//	free(copy[env_index]);	//probably not necessary free_split(copy)
	return (new_copy);
}

char	**append_env(char *arg, char **copy)
{
	char	**new_copy;
	size_t	key_len;
	size_t	env_index;
//	size_t	arg_len;
	
	key_len = key_length(arg);
	env_index = find_key_env_index(arg, copy, key_len);
//	printf("string on copy - 1: %s\n", copy[pointer_array_len(copy) - 1]);
//	copy[pointer_array_len(copy)] = arg;
	new_copy = env_copy(copy);
//	new_env_copy[pointer_array_len(copy)] = arg;
	if (env_index == pointer_array_len(copy))
		new_copy[pointer_array_len(copy)] = arg;
	else
		substitute_env_str(arg, new_copy, env_index);
/*		
	else
		{
			env_index = find_key_env_index(arg, copy, key_len);
			new_env_value = malloc(arg_len + 1);
			new_env_value = arg;
			if (!new_env_value)
				return NULL; // PANIC
			new_copy[env_index] = new_env_value;
			free(copy[env_index]);
		}
*/	
//	printf("string on copy: %s\n", copy[pointer_array_len(copy)]);
//	printf("string on new: %s\n", new_env_copy[pointer_array_len(copy)]);
//	env_print(new_copy);
//	free_split(copy);
	return (new_copy);
}

char	**append_uninitialized(char *arg, char **uninitialized, char **copy)
{
	char	**new_uninitialized;
//	size_t	env_index;
//	size_t	key_len;
//		size_t	key_len;
	size_t	index = 0;
	size_t	key_len;
	size_t	env_index;
	
	key_len = key_length(arg);
	env_index = find_key_env_index(arg, copy, key_len);	
//	key_len = key_length(arg);
//	env_index = find_key_env_index(arg, uninitialized, key_len);
//	printf("env_index %zu\n", env_index);
//	printf("string on copy - 1: %s\n", copy[pointer_array_len(copy) - 1]);
//	copy[pointer_array_len(copy)] = arg;
	new_uninitialized = env_copy(uninitialized);
//	new_uninitialized[pointer_array_len(uninitialized)] = arg;
//	printf("arg %s\n", arg);
//	printf("uninitialized[] %s\n", new_uninitialized[0]);

//	printf("\n\nenv index: %zu\n\n", env_index);
//	printf("\n\ncopy len: %zu\n\n", pointer_array_len(copy));
	if (env_index != pointer_array_len(copy))
		return (new_uninitialized);
	while (index < pointer_array_len(uninitialized))
	{
		if (!strcmp(arg, new_uninitialized[index]))
			return (new_uninitialized);
		index++;
//		printf("index %zu\n", index);
	}
//	if (index == pointer_array_len(uninitialized))
	new_uninitialized[pointer_array_len(uninitialized)] = arg;
	return (new_uninitialized);
//		substitute_env_str(arg, new_copy, env_index);
//	printf("string on copy: %s\n", copy[pointer_array_len(copy)]);
//	printf("string on new: %s\n", new_env_copy[pointer_array_len(copy)]);
//	env_print(new_uninitialized);
}
/*
char	**append_uninitialized(char *arg, char **uninitialized)
{
	char	**new_uninitialized;
	size_t	env_index;
	size_t	key_len;
//		size_t	key_len;
	size_t	index =0;
	
	key_len = key_length(arg);
	env_index = find_key_env_index(arg, uninitialized, key_len);
	printf("env_index %zu\n", env_index);
//	printf("string on copy - 1: %s\n", copy[pointer_array_len(copy) - 1]);
//	copy[pointer_array_len(copy)] = arg;
	new_uninitialized = env_copy(uninitialized);
//	new_uninitialized[pointer_array_len(uninitialized)] = arg;
	printf("arg %s\n", arg);
	printf("uninitialized[] %s\n", new_uninitialized[0]);

	while (index < pointer_array_len(uninitialized))
	{
		if (strcmp(arg, new_uninitialized[index]))
			index++;
		else
			return (new_uninitialized);
		printf("index %zu\n", index);
	}
	if (index == pointer_array_len(uninitialized))
		new_uninitialized[pointer_array_len(uninitialized)] = arg;
	return (new_uninitialized);
//		substitute_env_str(arg, new_copy, env_index);
//	printf("string on copy: %s\n", copy[pointer_array_len(copy)]);
//	printf("string on new: %s\n", new_env_copy[pointer_array_len(copy)]);
	env_print(new_uninitialized);
}
*/

void	export_buildin(int argc, char **argv, char **env_copy)
{
//	(void)argc;
//	(void)env_copy;
	char	**uninitialized = malloc(sizeof(char*) * 2);

	int	i = 1;		// index variable to goes through the arrays of pointers of each argument string pointer 
//	int	key_index = 0;
//	int	env_index = 0;
	
//	printf("\n\n\nindex of PWD: %zu\n\n\n", find_key_env_index("OLDPWD", env_copy, 6));
	if (find_key_env_index("OLDPWD", env_copy, 6) == pointer_array_len(env_copy))
		uninitialized[0] = strdup("OLDPWD");
//	if (argc == 1)		//for test purpose only
//		exit(0);
//	if (argc == 2)		// has no options or arguments
//		export_print(env_copy, unitialized);
//	if (!strcmp(argv[0], "export") && argc == 1)
	if (!strcmp(argv[0], "./a.out") && argc == 1)
	{
		print_export(env_copy, uninitialized);
//		print_export(env_copy);
		exit (0);
	}
	if (is_option(argv[i]))
		perror_export(is_option(argv[i]));
	while (argv[i])
	{
//		printf("key len: %d\n", key_length(argv[i]));
//		printf("alphanum_: %d\n\n", key_isalnum_underscore(argv[i], key_length(argv[i])));
		if (!key_isalnum_underscore(argv[i], key_length(argv[i])) || ft_isdigit(*argv[i]))
		{
			perror_identifier(argv[i]);
			i++;
			continue ;
		}
		if (strchr(argv[i], '='))
			env_copy = append_env(argv[i], env_copy);
		else
			uninitialized = append_uninitialized(argv[i], uninitialized, env_copy);
//		printf("after append: %d\n", pointer_array_len(env_copy));
		i++;
	}
	printf("++++++++\n\n");
	env_print(env_copy);
	printf("++++++++\n\n");
	env_print(uninitialized);
	exit_status = EXIT_SUCCESS;
}


/*
char	*ft_strpbrk(const char *s1, const char *s2)
{
	int		i = 0 ;
	
	if ( !s1 || !s2 )
		return 0 ;
	while ( *s1 )
	{
		i = 0 ;
	   	while ( s2[ i ] )
		{
			if ( *s1 == s2[ i ] )
				return (char *)s1 ;
			i++ ;
		}
		s1++ ;	
	}
}

int	ft_isdigit(int c)
{
	return (c >= '0' && c <= '9');
}

int	key_length(char *str)		// interesting function to see how information is stored in memory	
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

int	key_isalnum_underscore(char *str, size_t index)
{
	size_t	i = 0;
	
	while (i < index)
	{
		if (!((str[i] >= 'a' && str[i] <= 'z') || (str[i] >= 'A' && str[i] <= 'Z')
			|| (str[i] >= '0' && str[i] <= '9') || str[i] == '_' || str[i] == '='))
			return (0);
		i++;
	}
//	printf("inside keyalpha char %c\n", str[i]);
	return (str[i]);
}

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

int	perror_identifier(char *str)
{
		write(2, "bash: export: `", 15);
		write(2, str, strlen(str));
		write(2, "' not a valid identifier\n", 25);	// cannot use strlen()
		exit_status = 1;
		exit (exit_status);
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
//	printf("number of env var: %d\n", len);
	return (len);
}


char	*extract_key(char *str)		
{
	int	length;
	char	*key;
	
	if (!str)
		return (0); // confirm if is the correct one
	length = key_length(str);
	key = (char *)malloc(sizeof(char) * (length + 1));	// must be freed
	strncpy(key, str, length);
//	printf("key to be passed: %s\n", key);
	return (key);
}


char	*extract_key(char *str)		
{
	int	length;


	if (!str)
		return (0); // confirm if is the correct one
	length = key_length(str);
	str[length] = '\0';		
	return (key);
}


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
//	printf("i_env %d\n", i_env);
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
	env_copy = (char**)malloc(sizeof(char*) * (pointer_array_len(env) + 2));
	if (!env_copy)
		return NULL;
	env_copy[pointer_array_len(env) ] = '\0';
	env_copy[pointer_array_len(env)  + 1] = '\0';
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
		key_index++;	
	}
//	printf("env_copy len: %d\n", pointer_array_len(env_copy));
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

//void	print_export(char **env_dup, char **uninitialized)
void	print_export(char **env_dup)
{
//	void(uninitialized);
	size_t	index = 0;
//	char	**all_var;
	char	**env_sorted;
	
	env_sorted = env_copy(env_dup);
	bubble_sort_array(env_sorted);
	while(env_sorted[index])
	{
		printf("declare -x %s=\"%s\"\n", extract_key(env_sorted[index]), extract_value(env_sorted[index]));
		index++;
	}
}


char	**append_env(char *arg, char **copy)
{
	char	**new_env_copy;
	
//	printf("string on copy - 1: %s\n", copy[pointer_array_len(copy) - 1]);
//	copy[pointer_array_len(copy)] = arg;
	new_env_copy = env_copy(copy);
	new_env_copy[pointer_array_len(copy)] = arg;
//	printf("string on copy: %s\n", copy[pointer_array_len(copy)]);
//	printf("string on new: %s\n", new_env_copy[pointer_array_len(copy)]);
	env_print(new_env_copy);
	return (new_env_copy);
}

char	**append_unitialized(char *arg, char **uninitialized)
{
	char	**new_uninitialized;
	
//	printf("string on copy - 1: %s\n", copy[pointer_array_len(copy) - 1]);
//	copy[pointer_array_len(copy)] = arg;
	new_uninitialized = env_copy(uninitialized);
	new_uninitialized[pointer_array_len(uninitialized)] = arg;
//	printf("string on copy: %s\n", copy[pointer_array_len(copy)]);
//	printf("string on new: %s\n", new_env_copy[pointer_array_len(copy)]);
	env_print(new_uninitialized);
	return (new_uninitialized);
}

void	export_buildin(int argc, char **argv, char **env_copy)
{
//	(void)argc;
	(void)env_copy;
	char	**uninitialized = malloc(sizeof(char*));

	int	i = 2;		// index variable to goes through the arrays of pointers of each argument string pointer 
//	int	key_index = 0;
//	int	env_index = 0;

//	if (argc == 1)		//for test purpose only
//		exit(0);
//	if (argc == 2)		// has no options or arguments
//		export_print(env_copy, unitialized);
	if (!strcmp(argv[1], "export") && argc == 2)
	{
//		print_export(env_copy, uninitialized);
		print_export(env_copy);
		exit (0);
	}
	if (is_option(argv[i]))
		perror_export(is_option(argv[i]));
	while (argv[i])
	{
		printf("key len: %d\n", key_length(argv[i]));
		
//		if (!key_isalnum_underscore(argv[i], key_length(argv[i])) || ft_isdigit(*argv[i]))
//			perror_identifier(argv[i]);
		if (strchr(argv[i], '='))
			env_copy = append_env(argv[i], env_copy);
		else
			uninitialized = append_unitialized(argv[i], uninitialized);
//		printf("after append: %d\n", pointer_array_len(env_copy));
		i++;
	}
	exit_status = EXIT_SUCCESS;
}
*/
int	main(int argc, char **argv, char **env)
{
//	(void)argc;
//	(void)argv;
//	(void)env;
//	char	**copy;

//	copy = env_copy(env);
//	env_print(copy);
//	bubble_sort_array(copy);
//	printf("\n\ndepois do sort\n\n");
//	env_print(copy);
//	print_export(copy);
//	printf("key to be passed: %s\n", extract_key("isto e um teste=hxbndtsdk"));
//	printf("\n\n\n%s  \n\n\n\n", extract_key("USER=aamaral-"));	
//	ft_swap(&env[0], &env[1]);
//	env_print(env);
	export_buildin(argc, argv, env);

}
