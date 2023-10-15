#include <stdio.h>	// printf
#include <stdlib.h>	// exit
#include <errno.h>	// errno
#include <limits.h>
#include <unistd.h>

/*
I've saw our colleagues using different solutions, more complicated ones
based on youtube video of code vault about environment variables
at the end use the following command to compare the differences between the original env and the env buildin
_= is a special bash parameter https://www.cherryservers.com/blog/how-to-set-list-and-manage-linux-environment-variables
declare and set commands include shell functions that we are not taking into account for this simplified shell
*/


/*
Tests to check adherence to original bash

in minishell env doesn't accept no options nor arguments

- run	./a.out env ; $?
- run	./a.out env "more arguments" ; $?

*/


int	exit_status; // in the original will be a global variable

void	env_buildin(int argc, char **argv, char **env_copy)
{
	int	i = 0;			// index variable to goes through the arrays of pointers of each envp string pointer 
	char	*env_var = envp[0];	// declare and initialize there is always at least one pointer; the NULL since envp is nullterminated by standard request

				
	// run grep ARG_MAX /usr/include/linux/limits.h  ; getconf ARG_MAX https://unix.stackexchange.com/questions/120642/what-defines-the-maximum-size-for-a-command-single-argument
	
	(void) argv;			// instead use argc but must be a parameter, we can also count locally argv string
	if(argc > 2)			// a test number, it will be changed to 1 in original minishell 
	{
		errno = EINVAL;		// assign the error number 22 ( other possibility - E2BIG 7 see link above about ARG_MAX)for perror function()  on CLI run errno -l to see all the errors list; I would prefer too many arguments error but not implemented
		perror("minishell buildin specific");	// print the errono with a personal message also
		exit_status = 2;	// find the best exit status number
		exit(exit_status);
	}
	while (env_var)
	{
		printf("%s\n", env_var);
		i++;
		env_var = envp[i];
	}
	exit_status = EXIT_SUCCESS;
}

int	main(int argc, char **argv, char **envp) // not all OS allow to use envp. it is null terminated containing the envvariomental variables as string or we can use the external variable environ
{
	env_buildin(argc, argv, envp);
//	it will never give error since is nullterminated and will give null at the worst case cenario
	return 0;	// in the original function no need to return it will be void
}
