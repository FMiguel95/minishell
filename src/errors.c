/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fernacar <fernacar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/06 19:41:47 by aamaral-          #+#    #+#             */
/*   Updated: 2023/11/20 21:40:56 by fernacar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	ft_perror(char *str, char letter, int *exit_status)
{
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(str, 2);
	ft_putstr_fd(": -", 2);
	ft_putchar_fd(letter, 2);
	ft_putendl_fd(": invalid option", 2);
	ft_putstr_fd(str, 2);
	ft_putstr_fd(": usage: ", 2);
	ft_putstr_fd(str, 2);
	if (!ft_strcmp(str, "pwd"))
		ft_putchar_fd('\n', 2);
	if (!ft_strcmp(str, "unset"))
		ft_putendl_fd("[name ...]", 2);
	if (!ft_strcmp(str, "export"))
		ft_putendl_fd("[name[=value] ...]", 2);
	if (!ft_strcmp(str, "cd"))
		ft_putendl_fd(" [dir]", 2);
	*exit_status = 2;
	return (*exit_status);
}

int	perror_identifier(char *command, char *str, int *exit_status)
{
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(command, 2);
	ft_putstr_fd(": `", 2);
	ft_putstr_fd(str, 2);
	ft_putendl_fd("' not a valid identifier", 2);
	*exit_status = 1;
	return (*exit_status);
}

void	perror_option_identifier(char *command, char *arg, int *exit_status)
{
	if (is_option(arg) != -1)
		ft_perror(command, is_option(arg), exit_status);
	else
		perror_identifier(command, arg, exit_status);
}

int	perror_cd(int *exit_status)
{
	ft_putendl_fd("minishell: cd: too many arguments", 2);
	*exit_status = 1;
	return (*exit_status);
}
