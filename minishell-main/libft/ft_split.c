/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fernacar <fernacar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/13 20:22:41 by fernacar          #+#    #+#             */
/*   Updated: 2023/04/13 21:20:44 by fernacar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	get_word_count(const char *s, char c)
{
	int	total;
	int	i;

	total = 0;
	i = 0;
	while (s[i])
	{
		if ((i == 0 && s[i] != c) || (s[i] != c && s[i - 1] == c))
			total++;
		i++;
	}
	return (total);
}

static int	get_word_length(const char *s, char c)
{
	int	i;

	i = 0;
	while (s[i] && s[i] != c)
		i++;
	return (i);
}

static char	*copy_word(const char *s, char c)
{
	char	*word;
	int		i;

	word = malloc(sizeof(char *) * (get_word_length(s, c) + 1));
	if (!word)
		return (NULL);
	i = 0;
	while (s[i] && s[i] != c)
	{
		word[i] = s[i];
		i++;
	}
	word[i] = '\0';
	return (word);
}

char	**ft_split(const char *s, char c)
{
	char	**result;
	int		index;
	int		i;

	result = malloc(sizeof(char *) * (get_word_count(s, c) + 1));
	if (!result)
		return (NULL);
	index = 0;
	i = 0;
	while (s[i])
	{
		if (s[i] == c)
			i++;
		else
		{
			result[index] = copy_word(&s[i], c);
			index++;
			i += get_word_length(&s[i], c);
		}
	}
	result[index] = NULL;
	return (result);
}
/*
int main(){
	int i = 0;
	char **r = ft_split("    hello bye    lel   ", ' ');
	while (r[i] != NULL)
	{
		printf("%s\n", r[i]);
		i++;
	}
}*/