// #include <stdio.h>	// printf
// #include <stdlib.h>	// exit
// #include <errno.h>	// errno
// #include <unistd.h>	// write
// #include <string.h>	// strlen

#include "../include/minishell.h"

extern int	exit_status;
// int	exit_status;

// void	ft_putchar_fd(char c, int fd)
// {
// 	write(fd, &c, 1);
// }

// static void	ft_putstr_fd(char *s, int fd)
// {
// 	int	i;

// 	i = 0;
// 	if (!s || fd < 0)
// 		return ;
// 	while (s[i])
// 	{
// 		write(fd, &s[i], 1);
// 		i++;
// 	}
// 	return ;
// }

static int	is_option(char *str)
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

// int	ft_perror(char *str, char letter)
// {
// 		ft_putstr_fd("bash: ", 2);
// 		ft_putstr_fd(str, 2);
// 		ft_putstr_fd(": -", 2);
// 		ft_putchar_fd(letter, 2);
// 		ft_putstr_fd(": invalid option\n", 2);	
// 		ft_putstr_fd(str, 2);
// 		ft_putstr_fd(": usage: ", 2);
// 		ft_putstr_fd(str, 2);
// //		if (!ft_strcmp(str, "unset"))
// 		if (!strcmp(str, "./a.out"))
// 			ft_putstr_fd("\n", 2);
// 		if (!strcmp(str, "unset"))
// 			ft_putstr_fd("[name ...]\n", 2);
// 		if (!strcmp(str, "export"))
// 			ft_putstr_fd("[name[=value] ...]\n", 2);
// 		if (!strcmp(str, "export"))
// 			ft_putstr_fd("[name[=value] ...]\n", 2);
// 		if (!strcmp(str, "cd"))
// 			ft_putstr_fd("[dir]\n", 2);
// 		exit_status = 2;
// 		exit (exit_status);
// }

int	ft_argc(char **argv)
{
	int	counter = 0;
	if (!argv)
		return (0);
	while (argv[counter] != NULL)
		counter++;
//	printf("counter: %d\n", counter);
	return (counter);
}

int	perror_cd(void)
{
		ft_putstr_fd("bash: cd: too many arguments\n", 2);
		exit_status = 1;
//		exit (exit_status);
		exit (exit_status);
}

static size_t	pointer_array_len(char **pointers)
{
	size_t	len = 0;

	while (pointers[len])
		len++;	
//	printf("number of env var: %d\n", len);
	return (len);
}

static size_t	find_key_env_index(char *argv, char **env, size_t index)
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

// static char	*extract_value(char *str)
// {
// 	if (!*str)
// 		return (0); // confirm if is the correct one
// 	while (*str != '=')
// 	{		
// //			printf("%c\n", *str);
// 			str++;
		
// 	}
// 	str++;
// 		return (char *)str;		// similar to exam ramk 2 ft_strpbrk
// }

// char	*ft_strjoin(char const *s1, char const *s2)
// {
// 	char	*result;
// 	size_t	i;
// 	size_t	j;

// 	result = malloc(sizeof(char) * (strlen(s1) +strlen(s2) + 1));
// 	if (!result)
// 		return (NULL);
// 	i = 0;
// 	while (s1[i])
// 	{
// 		result[i] = s1[i];
// 		i++;
// 	}
// 	j = 0;
// 	while (s2[j])
// 	{
// 		result[i + j] = s2[j];
// 		j++;
// 	}
// 	result[i + j] = '\0';
// 	return (result);
// }


static void	env_print(char **env)
{
	size_t	key_index = 0;
	
	while (env[key_index])
	{
		printf("%s\n", env[key_index]);
		key_index++;	
	}
}

//	char	**update_env(char **env_cpy, char *final_path)
char	**update_env(char **env_cpy)
{
	char	*new_PWD_str = NULL;
	char	*new_OLDPWD_str = NULL;
	char	*temp_pwd;
	char	*temp_oldpwd;
	size_t	env_index;
	
	printf("\n\nupdate\n\n");
//	env_print(env_cpy);
//	printf("final_path: %s\n", final_path);
	env_index = find_key_env_index("PWD", env_cpy, 3);
//	printf("\nenv_index PWD: %zu\n", env_index);
	temp_pwd = env_cpy[env_index];
	char *temp = getcwd(NULL, 0);
	new_PWD_str = ft_strjoin("PWD=", temp);
	free(temp);
	env_cpy[env_index] = new_PWD_str;
//	printf("new PWD: %s\n", new_PWD_str);
	env_index = find_key_env_index("OLDPWD", env_cpy, 6);
//	printf("\nenv_index OLDPWD: %zu\n", env_index);
	temp_oldpwd = env_cpy[env_index];
//	printf("temp_oldpwd: %s\n", temp_oldpwd);
	new_OLDPWD_str = ft_strjoin("OLDPWD=", my_getenv(temp_pwd, env_cpy));
//	printf("new_OLDPWD before: %s\n", new_OLDPWD_str);
	env_cpy[env_index] = new_OLDPWD_str;
//	printf("new OLDPWD after: %s\n", new_OLDPWD_str);
	env_print(env_cpy);
//	printf("fim do env_copy\n");
	if (temp_pwd)
		free(temp_pwd);
	if (temp_oldpwd)
		free(temp_oldpwd);
	return (env_cpy);
}

// static char	**env_copy(char **env)
// {
// 	char **env_copy;
// 	size_t	key_index = 0;
// 	size_t	value_index = 0;

// //	while (env[key_index])
// //		key_index++;
// 	env_copy = (char**)malloc(sizeof(char*) * (pointer_array_len(env) + 2));
// 	if (!env_copy)
// 		return NULL;
// 	env_copy[pointer_array_len(env) ] = '\0';
// 	env_copy[pointer_array_len(env)  + 1] = '\0';
// //	key_index = 0;
// 	while (env[key_index])
// 	{
// 		env_copy[key_index] = (char *)malloc(sizeof(char) * (strlen(env[key_index]) + 1));
// 		if (!env_copy[key_index])
// 			return NULL;
// 		env_copy[key_index][strlen(env[key_index])] = '\0';
// 		while (value_index < strlen(env[key_index]))
// 		{
// 			env_copy[key_index][value_index] = env[key_index][value_index] ;
// 			value_index++;
// 		}
// 		value_index = 0;
// 		key_index++;	
// 	}
// //	printf("env_copy len: %d\n", pointer_array_len(env_copy));
// 	return (env_copy);
// }

int	cd_buildin(char **argv, char **env_cpy)
{
//	size_t	env_index ;
//	char	*final_path = NULL;
	
//	env_index = find_key_env_index(argv, env, index);

//	printf("argv[1]: %s\n", argv[1]);
	if (ft_argc(argv) > 2)
		perror_cd();
	else if ((ft_argc(argv) == 2) && !argv[1][0])
	{
		exit_status = 0;
		return (0);
	}
	else if ((ft_argc(argv) == 2) && is_option(argv[1]))
		ft_perror(argv[0], is_option(argv[1]));
//	if (!argv[1][0] || !strcmp(argv[1], "~"))
	else if ((ft_argc(argv) == 1) || !strcmp(argv[1], my_getenv("HOME", env_cpy)))
	{
		if (find_key_env_index("HOME", env_cpy, 4) != pointer_array_len(env_cpy))
		{
			char str[100];
			
			printf("current pwd: %s\n", getcwd(str, 100));
			exit_status = chdir(my_getenv("HOME", env_cpy));
			if (exit_status)
				ft_putstr_fd("error in changing directory", 2);
	//		str = extract_value(env_cpy[find_key_env_index("HOME", env_cpy, 4)]);
			printf("to be in chdir; %s\n", my_getenv("HOME", env_cpy));
			getcwd(str,100);
			printf("current pwd: %s\n", str);
//			exit_status = 0;
//			return (exit_status);
		}
		else
		{
			printf("bash: cd: HOME not set\n");
			exit_status = 1;
//			return (exit_status);
		}
	}

	else if (!strcmp(argv[1], "-"))
	{
		if (find_key_env_index("OLDPWD", env_cpy, 6) != pointer_array_len(env_cpy))
		{
			char str[100];
			
			printf("current pwd: %s\n", getcwd(str, 100));
		//	exit_status = chdir("-");
		//	chdir(extract_value(env_cpy[find_key_env_index("OLDPWD", env_cpy, 6)]));
			exit_status = chdir(my_getenv("OLDPWD", env_cpy));
			if (exit_status)
				ft_putstr_fd("error in changing directory\n", 2);
			printf("exit_status: %d\n", exit_status);
	//		str = extract_value(env_cpy[find_key_env_index("HOME", env_cpy, 4)]);
			printf("to be in chdir; %s\n", my_getenv("OLDPWD", env_cpy));
			getcwd(str,100);
			printf("current pwd: %s\n", str);
	//		exit_status = 0;
//			return (exit_status);
		}
		else
		{
			printf("bash: cd: OLDPWD not set\n");
			exit_status = 1;
			return (exit_status);
		}	
	}
/*
	else if (!strncmp(argv[1], "..", 2))
	{
		printf("inside if\n");
		char str[100];	
		printf("current pwd: %s\n", getcwd(str, 100));
		printf("argv[1]: %s\n", argv[1]);
		exit_status = chdir(argv[1]);
//		exit_status = chdir("..");
		printf("exit_status antes: %d\n", exit_status);
		if (exit_status)
		{
			errno = ENOENT;
			perror("bash: cd");
			exit_status = 1;
			printf("exit_status depois: %d\n", exit_status);
		// panic	exit();
		}	
	//	str = extract_value(env_cpy[find_key_env_index("HOME", env_cpy, 4)]);
		getcwd(str,100);
		printf("current pwd: %s\n", str);
	//	exit_status = 0;
		return (exit_status);
	}
*/
	else
	{
//		printf("inside else\n");
		char str[100];	
		printf("current pwd: %s\n", getcwd(str, 100));
		printf("argv[1]: %s\n", argv[1]);
		exit_status = chdir(argv[1]);
//		exit_status = chdir("..");
		printf("exit_status antes: %d\n", exit_status);
		if (exit_status)
		{
//			errno = ENOENT;
			perror("bash: cd");
			exit_status = 1;
			printf("exit_status depois: %d\n", exit_status);
		// panic	exit();
		}	
	//	str = extract_value(env_cpy[find_key_env_index("HOME", env_cpy, 4)]);
		getcwd(str,100);
		printf("current pwd: %s\n", str);
	//	exit_status = 0;
//		return (exit_status);
	}
//	final_path = argv[1];	
	update_env(env_cpy);

	return (exit_status);
}

// int	main(int argc, char **argv, char **env)
// {
// 	(void)argc;
// 	char	**copy;

// 	copy = env_copy(env);
// 	cd_buildin(argv, copy);
// }
