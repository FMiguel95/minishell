#include <limits.h>	// PATH_MAX
#include <stdio.h>	// printf
#include <unistd.h>	// getcwd
#include <errno.h>	// errno
#include <string.h>	// strerror
#include <stdlib.h>	// EXIT_FAILURE EXIT_SUCCESS

int	exit_status;

int	is_option(char *str)
{
	if (!str)
		return (0);
	if (*str == '-')
	{
		while (*(++str) == 'n')
			;
//		printf("%c\n", *str);
		if (!*str)	
			return (write(1, "option -n\n", 10));
		else
			return (write(1, "option other than -n\n", 21));
	}
	return (0);
	
}

void	pwd_buildin(int argc, char **argv, char **env_copy)
{
	/*function just to confirm how long the array can hold	 
	  stackoverflow question 9449241 where is PATH_MAX defined in Linux
	printf("max char that a path can hold: %d\n", PATH_MAX);*/

	char	curr_work_dir[PATH_MAX];

	
	while (*argv)
		if(is_option(*(++argv)))			// a test number, it will be changed to 1 in original minishell ; pwd only accepts options man page, any other argument will be ignored
		{
			errno = EINVAL;		// assign the error number 22 ( other possibility - E2BIG 7 see link above about ARG_MAX)for perror function()  on CLI run errno -l to see all the errors list; I would prefer too many arguments error but not implemented
			perror("minishell buildin specific");	// print the errono with a personal message also
			exit_status = 2;	// find the best exit status number  https://www.baeldung.com/linux/standard-error-eodes#:~:text=This%20error%20code%20means%20that%20the%20argument%20passed,a%20valid%20and%20supported%20argument%20to%20the%20operation.
			exit(exit_status);
		}
	if(getcwd(curr_work_dir, PATH_MAX) == NULL) // man getpwd return value on failure NULL pointer and errno is set
	{
	/*use panic to print the error message
	panic("pwd"); */
		perror("pwd error:");	//just to check if the errno ATTENTION: errno and exit code DON'T match https://www.gnu.org/software/libc/manual/html_node/Exit-Status.html

		exit_status = 2;  // https://tldp.org/LDP/abs/html/exitcodes.html   https://itsfoss.com/linux-exit-codes/
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

