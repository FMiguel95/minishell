#include <limits.h>	// PATH_MAX
#include <stdio.h>	// printf
#include <unistd.h>	// getcwd
#include <errno.h>	// errno
#include <string.h>	// strerror
#include <stdlib.h>	// EXIT_FAILURE EXIT_SUCCESS

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
	{
		
		if (*(++str) != '-' && *str != '\0')   // ver se a segunda condicao faz sentido, acho que sim
			return (letter);
	}
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
//		if (!ft_strcmp(str, "pwd"))
		if (!ft_strcmp(str, "./a.out"))
			ft_putstr_fd("\n", 2);
		if (!ft_strcmp(str, "unset"))
			ft_putstr_fd("[name ...]\n", 2);
		if (!ft_strcmp(str, "export"))
			ft_putstr_fd("[name[=value] ...]\n", 2);
		exit_status = 2;
		exit (exit_status);
}

void	pwd_buildin(char **argv)
{
	char	curr_work_dir[PATH_MAX];
	int	i = 1;
	
	if (is_option(argv[i]))
		ft_perror(argv[i -1], is_option(argv[i]));
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

}



/*
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
//		if (!ft_strcmp(str, "pwd"))
		if (!ft_strcmp(str, "./a.out"))
			ft_putstr_fd("\n", 2);
		if (!ft_strcmp(str, "unset"))
			ft_putstr_fd("[name ...]\n", 2);
		if (!ft_strcmp(str, "export"))
			ft_putstr_fd("[name[=value] ...]\n", 2);
		exit_status = 2;
		exit (exit_status);	// talvez seja melhor usar um perror o exit vai ser 1????
}

int	perror_pwd(char letter)
{
		write(2, "bash: pwd: -", 12);
		write(2, &letter, 1);
		write(2, ": invalid option\npwd: usage: pwd\n", strlen(": invalid option\npwd: usage: pwd\n"));	// cannot use strlen()
		exit_status = 2;
		exit (exit_status);	// talvez seja melhor usar um perror o exit vai ser 1????
}

void	pwd_buildin(char **argv)
{
	char	curr_work_dir[PATH_MAX];
	int	i = 1;
	
	if (is_option(argv[i]))
//		perror_pwd(is_option(argv[i]));
		ft_perror(argv[i -1], is_option(argv[i]));
	if(getcwd(curr_work_dir, PATH_MAX) == NULL) // man getpwd return value on failure NULL pointer and errno is set
	{
	//use panic to print the error message
	//panic("pwd"); 
		perror("pwd error:");
		exit_status = 2;
	}	
	
	else
	{
		printf("%s\n", curr_work_dir);	// on success returns a pointer to string containing the path
		exit_status = EXIT_SUCCESS;	// man getpwd on success returns the pointer to the array containing the path
	}

}
*/
int	main(int argc, char **argv, char **envp)
{
	(void)argc;
	(void)envp;
	pwd_buildin(argv);
	//ft_perror("export", 'k');
	
}

