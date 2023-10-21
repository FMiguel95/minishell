#include <limits.h>	// PATH_MAX
#include <stdio.h>	// printf
#include <unistd.h>	// getcwd
#include <errno.h>	// errno
#include <string.h>	// strerror
#include <stdlib.h>	// EXIT_FAILURE EXIT_SUCCESS

int	exit_status;

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
/*
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


int	ft_perror(char *str, char letter)
{
		write(2, "bash: ", 6);
		write(2, &str, strlen(str));
		write(2, ": -", 3);
		write(2, &letter, 1);
		write(2, ": invalid option\n", 17);	
		write(2, &str, strlen(str));
		write(2, ": usage: ", 9);
		write(2, &str, strlen(str));
		if (!ft_compare())
			write(2, ": usage: pwd\n", strlen(": invalid option\npwd: usage: pwd\n"));
		write(2, ": usage: pwd\n", strlen(": invalid option\npwd: usage: pwd\n"));
		// cannot use strlen()
		exit_status = 2;
		exit (exit_status);	// talvez seja melhor usar um perror o exit vai ser 1????
}

*/
int	perror_pwd(char letter)
{
		write(2, "bash: pwd: -", 12);
		write(2, &letter, 1);
		write(2, ": invalid option\npwd: usage: pwd\n", strlen(": invalid option\npwd: usage: pwd\n"));	// cannot use strlen()
		exit_status = 2;
		exit (exit_status);	// talvez seja melhor usar um perror o exit vai ser 1????
}

void	pwd_buildin(int argc, char **argv, char **env_copy)
{
	(void)env_copy;
	(void)argc;
	char	curr_work_dir[PATH_MAX];
	int	i = 1;
	
	if (is_option(argv[i]))
		perror_pwd(is_option(argv[i]));
	if(getcwd(curr_work_dir, PATH_MAX) == NULL) // man getpwd return value on failure NULL pointer and errno is set
	{
	/*use panic to print the error message
	panic("pwd"); */
		perror("pwd error:");
		exit_status = 2;
	}	
	
	else
	{
		printf("%s\n", curr_work_dir);	// on success returns a pointer to string containing the path
		exit_status = EXIT_SUCCESS;	// man getpwd on success returns the pointer to the array containing the path
	}

}

int	main(int argc, char **argv, char **envp)
{
	pwd_buildin(argc, argv, envp);
}

