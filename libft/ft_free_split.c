/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_free_split.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fernacar <fernacar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/06 22:47:37 by fernacar          #+#    #+#             */
/*   Updated: 2023/11/02 19:17:04 by fernacar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	free_split(char **split_ptr)
{
	int	i;

	if (!split_ptr)
		return ;
	i = 0;
	while (split_ptr[i])
	{
		free(split_ptr[i]);
		i++;
	}
	free(split_ptr);
}
