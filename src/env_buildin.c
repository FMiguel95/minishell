#include <stdio.h>	// printf



/*it will be substituted by the env_buildin(char **envp)
I've saw our colleagues using different solutions, more complicated ones
based on youtube video of code vault about environment variables
at the end use the following command to compare the differences between the original env and the env buildin
_= is a special bash parameter https://www.cherryservers.com/blog/how-to-set-list-and-manage-linux-environment-variables
declare and set commands include shell functions that we are not taking into account for this simplified shell
*/
int	main(int argc, char **argv, char **envp) // not all OS allow to use envp. it is null terminated containing the envvariomental variables as string
{
	(void)argc;	// in the buildin function they will not be used
	(void)argv;	// in the buildin function they will not be used
//	int	exit_status; // in the original will be a global variable
	int	i = 0;
	char	*env_var = envp[0]; // there is always at least one pointer; the NULL since envp is nullterminated
	
	while (env_var)
	{
		printf("%s\n", env_var);
		i++;
		env_var = envp[i];
	}
//	exit_status = 0; // gives me an error that i don't understand why, since I initialize it at line 7  
//	it will never give error since is nullterminated and will give null at the worst case cenario
	return 0;	// in the original function no need to return it will be void
}
