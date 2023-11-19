/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals1.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fernacar <fernacar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/09 23:12:13 by fernacar          #+#    #+#             */
/*   Updated: 2023/11/19 21:04:36 by fernacar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static void	handle_sigint_main(int sig)
{
	if (sig == SIGINT)
	{
		write(1, "\n", 1);
		rl_replace_line("", 0);
		rl_on_new_line();
		rl_redisplay();
	}
}

void	wait_signal_main(void)
{
	signal(SIGINT, handle_sigint_main);
	signal(SIGQUIT, SIG_IGN);
}

static void	handle_sigint_child(int sig)
{
	if (sig == SIGINT)
	{
		printf("\n");
		return ;
	}
	else if (sig == SIGQUIT)
	{
		printf("Quit (core dumped)\n");
	}
}

void	wait_signal_child(void)
{
	signal(SIGINT, handle_sigint_child);
	signal(SIGQUIT, handle_sigint_child);
}
