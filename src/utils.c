/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lnzila <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 08:07:43 by lnzila            #+#    #+#             */
/*   Updated: 2025/03/25 08:19:18 by lnzila           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

void	ft_trim_newline(char *str)
{
	int	len;

	len = ft_strlen(str);
	while (len > 0 && (str[len - 1] == '\n' || str[len - 1] == '\r'))
	{
		str[len - 1] = '\0';
		len--;
	}
}

int	ft_read_line(int fd, char *buffer, int size)
{
	int	bytes;

	bytes = read(fd, buffer, size - 1);
	if (bytes >= 0)
		buffer[bytes] = '\0';
	return (bytes);
}
