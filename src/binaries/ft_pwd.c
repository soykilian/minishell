/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pwd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mclerico <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/02 19:01:39 by mclerico          #+#    #+#             */
/*   Updated: 2022/02/22 18:59:54 by mclerico         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

void ft_pwd(int fd)
{
	t_list	**cpy;
	char *path;

	cpy = ft_calloc(1, sizeof(void *));
	path = getcwd(0, 0);
	if (path)
	{
		write(fd, path, ft_strlen(path));
		write(fd, "\n", 1);
	}
	free(cpy);
}
