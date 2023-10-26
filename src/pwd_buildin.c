// #include <limits.h>	// PATH_MAX
// #include <stdio.h>	// printf
// #include <unistd.h>	// getcwd
// #include <errno.h>	// errno
// #include <string.h>	// strerror
// #include <stdlib.h>	// EXIT_FAILURE EXIT_SUCCESS

#include "../include/minishell.h"

//int	exit_status;
extern int exit_status;

// void	ft_putstr_fd(char *s, int fd)
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

// int	ft_strcmp(const char *s1, const char *s2)
// {
// 	int	i;

// 	i = 0;
// 	while (s1[i] == s2[i] && s1[i] != '\0' && s2[i] != '\0')
// 	{
// 		i++;
// 	}
// 	return (s1[i] - s2[i]);
// }

static int	is_option(char *str)
{
	
	if (!str)
		return (0);
	char	letter = str[1];
	if (*str == '-')
		if (*(++str) != '-' && *str != '\0')   // ver se a segunda condicao faz sentido, acho que sim
			return (letter);
	return (0);
}



// int	ft_perror(char letter)
// {
// 		ft_putstr_fd("bash: pwd: -", 2);
// 		ft_putchar_fd(letter, 2);
// 		ft_putstr_fd(": invalid option\npwd: usage: pwd\n", 2);
// 		exit_status = 2;
// 		exit (exit_status);	// talvez seja melhor usar um perror o exit vai ser 1????
// }

void	pwd_buildin(char **argv)
{
	char	curr_work_dir[PATH_MAX];
	int	i = 1;
	
	if (is_option(argv[i]))
		ft_perror(argv[i -1], is_option(argv[i]));
		//ft_perror(is_option(argv[i]));
	if(getcwd(curr_work_dir, PATH_MAX) == NULL) 
	{
	//use panic to print the error message
	//panic("pwd"); 
		perror("pwd error:");
		exit_status = 2;
	}	
	
	else
	{
		printf("%s\n", curr_work_dir);
		exit_status = EXIT_SUCCESS;
	}
	exit(exit_status);
}

// int	main(int argc, char **argv, char **envp)
// {
// 	(void)argc;
// 	(void)envp;
// 	pwd_buildin(argv);
// 	//ft_perror("export", 'k');
	
// }

