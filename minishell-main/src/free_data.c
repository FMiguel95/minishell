/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_data.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fernacar <fernacar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/16 20:44:55 by fernacar          #+#    #+#             */
/*   Updated: 2023/11/16 20:45:07 by fernacar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	free_commands(t_minishell *data)
{
	if (data->token_list)
	{
		free_split(data->token_list);
		data->token_list = NULL;
	}
	if (data->tree_root)
	{
		free_node(data->tree_root);
		data->tree_root = NULL;
	}
}

void	free_data(t_minishell *data)
{
	free_commands(data);
	if (data->env)
	{
		free_split(data->env);
		data->env = NULL;
	}
	if (data->uninit)
	{
		free_split(data->uninit);
		data->uninit = NULL;
	}
}
