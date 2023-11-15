/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_buildin.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fernacar <fernacar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/05 21:38:34 by fernacar          #+#    #+#             */
/*   Updated: 2023/11/14 23:14:54 by fernacar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static int	validate_numeric(char *arg)
{
	int	i;

	if (arg == NULL || arg[0] == '\0')
		return (0);
	i = 0;
	while (arg[i])
	{
		if (!ft_isdigit(arg[i]))
			return (0);
		i++;
	}
	return (1);
}

void	exit_buildin(char **argv, int exit_status)
{
	int	exit_code;

	exit_code = exit_status;
	ft_putendl_fd("🔥exit🔥", STDERR_FILENO);
	if (argv[1])
	{
		if (validate_numeric(argv[1]))
			exit_code = ft_atoi(argv[1]);
		else
		{
			ft_putstr_fd("minishell: exit: ", STDERR_FILENO);
			ft_putstr_fd(argv[1], STDERR_FILENO);
			ft_putendl_fd(": numeric argument required", STDERR_FILENO);
		}
	}
	exit(exit_code);
}
