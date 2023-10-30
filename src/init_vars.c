#include <stdio.h>	// printf
#include <stdlib.h>	// exit
#include <errno.h>	// errno
#include <unistd.h>	// write
#include <string.h>	// strlen

char	*ft_itoa(int number) 
{
	int ntemp = number ;
	int len = 0 ;

	if(number == -2147483648)
		return( "-2147483648\0" ) ;
	if ( number == 0 )
		return ( "0\0" ) ;
	if ( number < 0 )
		len += 1 ;
	while ( ntemp )
	{
		ntemp /= 10 ;
		len++ ;
	}
	char 	*str = malloc( len + 1 ) ;
	if ( str )
	{	
		str[ len ] = '\0';
		if ( number < 0 ) 
		{
			str[ 0 ] = '-';
			number *= -1;
		}
		while ( number ) 
		{
			len--;
			str[ len ] = number % 10 + '0' ;
			number /= 10 ;
		}
	}
	return str ;
}

size_t	ft_strlen(const char *s)
{
	size_t	i;

	i = 0;
	while (s[i])
		i++;
	return (i);
}

char	*ft_strjoin(char const *s1, char const *s2)
{
	char	*result;
	size_t	i;
	size_t	j;

	result = malloc(sizeof(char) * (ft_strlen(s1) +ft_strlen(s2) + 1));
	if (!result)
		return (NULL);
	i = 0;
	while (s1[i])
	{
		result[i] = s1[i];
		i++;
	}
	j = 0;
	while (s2[j])
	{
		result[i + j] = s2[j];
		j++;
	}
	result[i + j] = '\0';
	return (result);
}

size_t	pointer_array_len(char **pointers)
{
	size_t	len = 0;

	while (pointers[len])
		len++;	
//	printf("number of env var: %d\n", len);
	return (len);
}

size_t	find_key_env_index(char *argv, char **env, size_t index)
{
	size_t	i_str = 0;
	size_t	i_env = 0;
	size_t	len_env = 0;

	len_env = pointer_array_len(env);
//	printf("number of env %zu\n", len_env);
	while (env[i_env])
	{
		while (i_str < index && argv[i_str] == env[i_env][i_str])	
			i_str++;
//		printf("%zu  i %zu   %s, \n", i_env, i_str, env[i_env]);
		if (env[i_env][i_str] == '=' && index == i_str)
			return (i_env);
		i_str = 0;
		i_env++;
	}
//	printf("i_env %zu\n", i_env);
	return (len_env);
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

int	main(int argc, char **argv, char **env)
{
	(void)argv;
	(void)argc;
	char	**env_cpy;
	char	**uninitialized = malloc(sizeof(char*) * 2);
//	size_t	value_index = 0;
	size_t	env_index = 0;
	char	*asci_env_value = NULL;
	int	dec_env_value = 0;
	char	*new_env_str = NULL;
	
	env_cpy = env_copy(env);
	env_index = find_key_env_index("SHLVL", env_cpy, 5);
	asci_env_value = extract_value(env_cpy[env_index]);
//	printf("env_SHLVL: %s\n", asci_env_value);
	dec_env_value = atoi(asci_env_value);
	asci_env_value = ft_itoa(dec_env_value + 1);
//	printf("env_SHLVL: %s\n", asci_env_value);
	new_env_str = ft_strjoin("SHLVL=", asci_env_value);
//	printf("new_SHLVL_str: %s\n", new_env_str);
	env_cpy = substitute_env_str(new_env_str, env_cpy, env_index);
	if (find_key_env_index("OLDPWD", env_cpy, 6) == pointer_array_len(env_cpy))
		uninitialized[0] = strdup("OLDPWD");
	env_print(env_cpy);
}
