#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>

/*
tests to run and compare with bash original

run	echo			//original
run	./a.out echo

run	echo -n
run	./a.out echo -n

run	echo -p		// original doesn't accept -p as an option so considers as an regular argument
run	./a.out echo -n	// our minishell only works with the -n option, all other arguments starting with '-' will be considered invalid options, but we can change this lines 75-76

*/

extern int	exit_status; // in the original will be a global variable

int	is_option(char *str)
{
	if (!str)
		return (0);
	if (*str == '-')
	{
		if (*(str +1) == '\0')
			return (0);
		while (*(++str) == 'n')
			;
//		printf("%c\n", *str);
		if (!*str)	
			return (10);
	}
	return (0);
	
}

void	echo_buildin(char **argv)
{
	int	i;
	int	flag_n_option;

	i = 1;
	flag_n_option = 0;
	while (is_option(argv[i]) == 10)
	{
		if (is_option(argv[i]) == 10)
			flag_n_option = 1;
		i++;
	}
	while(argv[i])
	{
		printf("%s", argv[i]);
			if (argv[i + 1])
				printf(" ");
		i++;
	}
	if (!flag_n_option)
		printf("\n");
	exit_status = EXIT_SUCCESS;
	//printf("[echo end]");
	exit(exit_status);
}

/*
int	exit_status; // in the original will be a global variable

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

void	echo_buildin(int argc, char **argv, char **env_copy)
{
	int	i = 2;			// index variable to goes through the arrays of pointers of each argument string pointer 
	int	flag_n_option = 0;

	while (is_option(argv[i]) == 21 || is_option(argv[i]) == 10)
	{
		if(is_option(argv[i]) == 21)			// a test number, it will be changed to 1 in original minishell ; pwd only accepts options man page, any other argument will be ignored
		{
			errno = EINVAL;		// assign the error number 22 ( other possibility - E2BIG 7 see link above about ARG_MAX)for perror function()  on CLI run errno -l to see all the errors list; I would prefer too many arguments error but not implemented
			perror("minishell buildin specific");	// print the errono with a personal message also
			exit_status = 2;	// find the best exit status number https://www.baeldung.com/linux/standard-error-eodes#:~:text=This%20error%20code%20means%20that%20the%20argument%20passed,a%20valid%20and%20supported%20argument%20to%20the%20operation.
			exit(exit_status);
		}
		else if (is_option(argv[i]) == 10)
			flag_n_option = 1;
		i++;
	}
	while(argv[i])
	{
		printf("%s", argv[i]);
			if (argv[i + 1])
				printf(" ");
		i++;
	}
	if (!flag_n_option)
		printf("\n");
	exit_status = EXIT_SUCCESS;
}
*/
// int	main(int argc, char **argv, char **envp) // not all OS allow to use envp. it is null terminated containing the envvariomental variables as string or we can use the external variable environ
// {
// 	(void)argc;
// 	(void)envp;
// 	echo_buildin(argv);
// //	it will never give error since is nullterminated and will give null at the worst case cenario
// 	return 0;	// in the original function no need to return it will be void
// }

