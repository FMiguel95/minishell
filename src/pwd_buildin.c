#include <limits.h>	// PATH_MAX
#include <stdio.h>	// printf
#include <unistd.h>	// getcwd
#include <errno.h>	// errno
#include <string.h>	// strerror
#include <stdlib.h>	// EXIT_FAILURE EXIT_SUCCESS

/*to be substituted by the final function prototype
void	pwd_buildin(t_list words) or to build the binary and give the path to it*/
int	main(void)
{
	int	exit_status;
	/*function just to confirm how long the array can hold	 
	  stackoverflow question 9449241 where is PATH_MAX defined in Linux*/
	printf("max char that a path can hold: %d\n", PATH_MAX);

	char	curr_work_dir[PATH_MAX];

	if(getcwd(curr_work_dir, PATH_MAX) == NULL) // man getpwd return value on failure NULL pointer and errno is set
	{
	/*use panic to print the error message
	panic("pwd"); */
	perror("pwd: ");
	//just to check if the errno ATTENTION: errno and exit code DON'T match https://www.gnu.org/software/libc/manual/html_node/Exit-Status.html
	strerror(errno);
	
	exit_status = 127;  // https://tldp.org/LDP/abs/html/exitcodes.html   https://itsfoss.com/linux-exit-codes/
	/*just to see if the errno functions. check if in real bash exit code is the same*/
	printf("%d", exit_status);
	}	
	
	else
	{
		printf("%s\n", curr_work_dir);	// on success returns a pointer to string containing the path
		exit_status = EXIT_SUCCESS;	// man getpwd on success returns the pointer to the array containing the path
	}
}
