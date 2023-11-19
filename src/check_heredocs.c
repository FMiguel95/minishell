/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_heredocs.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fernacar <fernacar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/16 21:52:53 by fernacar          #+#    #+#             */
/*   Updated: 2023/11/19 18:57:53 by fernacar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static char	*expand_token_var_heredoc(char *token, char **env, int exit_status)
{
	char	*expanded_token;
	char	*temp;
	int		i;

	expanded_token = ft_strdup("");
	i = 0;
	while (token[i])
	{
		if (token[i] == '$' && (ft_isalpha(token[i + 1])
				|| token[i + 1] == '_' || token[i + 1] == '?'))
		{
			i++;
			expanded_token = strjoin_free(expanded_token,
					get_var_val(token, i, env, exit_status));
			skip_var(token, &i);
			continue ;
		}
		temp = expanded_token;
		expanded_token = ft_strjoin(temp, (char [2]){token[i], '\0'});
		free(temp);
		i++;
	}
	return (expanded_token);
}

static void	get_heredoc(t_tnode_heredoc *heredoc_node, char **env,
		int exit_status)
{
	char	*line;
	char	*temp;

	while (1)
	{
		line = readline("heredoc> ");
		if (!line || ft_strcmp(line, heredoc_node->delm) == 0)
		{
			if (line)
				free(line);
			else
				ft_putendl_fd("minishell: warning: heredoc delimited by \
end-of-file", STDERR_FILENO);
			break ;
		}
		temp = expand_token_var_heredoc(line, env, exit_status);
		free(line);
		line = ft_strjoin(temp, "\n");
		free(temp);
		temp = ft_strjoin(heredoc_node->input, line);
		if (heredoc_node->input)
			free(heredoc_node->input);
		free(line);
		heredoc_node->input = temp;
	}
}

void	check_heredocs(t_tnode *node, t_minishell *data)
{
	if (node && node->type == PIPE)
	{
		check_heredocs(((t_tnode_pipe *)node)->branch_left, data);
		check_heredocs(((t_tnode_pipe *)node)->branch_right, data);
	}
	else if (node && node->type == REDIR)
	{
		check_heredocs(((t_tnode_redir *)node)->node, data);
	}
	else if (node && node->type == HEREDOC)
	{
		get_heredoc((t_tnode_heredoc *)node, data->env, data->exit_status);
		check_heredocs(((t_tnode_heredoc *)node)->node, data);
	}
}
