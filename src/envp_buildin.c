#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
This is a simplified ft_split function
I had some problems that's why so many printfs
I saw a interesting thing that when we open inside bash another bash the $SHLVL env_var changes.
we are going to use this envp_build to make the necessary changes
*/
int	main(int argc, char **argv, char **envp)
{
	(void)argc;
	(void)argv;
	char **envp_buildin;	// we are going to copy the original envp and needs to be nullterminated
	size_t	key_index = 0;	// we need to count the number of env_var to be malloc the number of pointers to be created
	size_t	value_index = 0;
	
	
	while (envp[key_index])
		key_index++;
//	printf("number of env var: %zu\n", key_index);	
	envp_buildin = (char**)malloc(sizeof(char*) * (key_index + 1));
	if (!envp_buildin)
		return 1;	// after change to exit status and perror
	envp_buildin[key_index] = '\0';	// nullterminate, a documentation  request
	key_index = 0;	// reset the index to start of the array of pointers
	while (envp[key_index])
	{
//		printf("%zu  %s\n", strlen(envp[key_index]), envp[key_index]);
		envp_buildin[key_index] = (char *)malloc(sizeof(char) * (strlen(envp[key_index]) + 1));
		if (!envp_buildin[key_index])
			return 1;	// after change to exit status and perror
		envp_buildin[key_index][strlen(envp[key_index])] = '\0';
		while (value_index < strlen(envp[key_index]))
		{
			envp_buildin[key_index][value_index] = envp[key_index][value_index] ;
			value_index++;
		}
		value_index = 0;
//		printf("%zu  %s\n", strlen(envp_buildin[key_index]), envp_buildin[key_index]);
//		key_index++;	
//		printf("key_index %zu\n", key_index);
	//	printf("pointer %p\n", &envp_buildin[key_index]);
	//	printf("string %s\n", envp_buildin[key_index]);
	//	printf("char %c\n", *envp_buildin[key_index]);
		printf("%s\n", envp_buildin[key_index]); // to confirm that the envp_buid is a copy of the original run the following command line		env>final1 ; ./a.out>final2 ; diff final1 final2
		key_index++;	
	}
}
