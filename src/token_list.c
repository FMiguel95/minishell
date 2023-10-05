/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_list.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fernacar <fernacar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/05 21:28:15 by fernacar          #+#    #+#             */
/*   Updated: 2023/10/05 22:23:28 by fernacar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static int	get_token_size(char *token)
{
	
}

static int	count_tokens(char *input)
{
	int		count;
	char	quote_type;
	int		i;

	count = 0;
	i = 0;
	while (input[i])
	{
		if (!ft_isspace(input[i]))
		{
			count++;
			quote_type = '\0';
			while (input[i] && !ft_isspace(input[i]))
			{
				if (input[i] == '\"' || input[i] == '\'')
				{
					quote_type = input[i];
					i++;
					while (input[i] && input[i] != quote_type)
						i++;
					if (input[i])
						i++;
				}
				i++;
			}
		}
		i++;
	}
	return (count);
}

char	**make_token_list(char *input)
{
	char	**token_list;
	int		token_count;
	
	token_count = count_tokens(input);
	token_list = malloc(sizeof(char *) * (token_count + 1));
	if (!token_list)
		panic("token allocation error");
	token_list[token_count] = NULL;

}
