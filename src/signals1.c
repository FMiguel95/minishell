/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals1.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fernacar <fernacar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/09 23:12:13 by fernacar          #+#    #+#             */
/*   Updated: 2023/11/20 22:18:26 by fernacar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static void	handle_sigint_main(int sig)
{
	if (sig == SIGINT)
	{
		g_exit = 130;
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

void	main_wait_signals(void)
{
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
}

static void	handle_sigint_child(int sig)
{
	if (sig == SIGINT)
	{
		g_exit = 130;
		printf("\n");
		return ;
	}
	else if (sig == SIGQUIT)
	{
		g_exit = 131;
		printf("Quit (core dumped)\n");
	}
}

void	wait_signal_child(void)
{
	signal(SIGINT, handle_sigint_child);
	signal(SIGQUIT, handle_sigint_child);
}
