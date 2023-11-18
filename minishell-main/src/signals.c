/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fernacar <fernacar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/09 23:12:13 by fernacar          #+#    #+#             */
/*   Updated: 2023/11/16 21:06:38 by fernacar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	handle_sigint(int signal)
{
	if (signal == SIGINT)
	{
		write(1, "\n", 1);
		rl_replace_line("", 0);
		rl_on_new_line();
		rl_redisplay();
	}
}

void	wait_signal(int *exit_status)
{
	signal(SIGINT, handle_sigint);
	*exit_status = 130;
	signal(SIGQUIT, SIG_IGN);
}
