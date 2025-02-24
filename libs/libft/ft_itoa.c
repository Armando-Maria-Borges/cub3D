/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lnzila <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/15 19:22:06 by lnzila            #+#    #+#             */
/*   Updated: 2024/05/15 19:22:09 by lnzila           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_intlen(long int n)
{
	if (n < 0)
		return (ft_intlen(n * -1) + 1);
	else if (n >= 10)
		return (ft_intlen(n / 10) + 1);
	else if (n >= 0 && n < 10)
		return (1);
	return (0);
}

char	*ft_itoa(int n)
{
	char		*str;
	int			size;
	long int	i;

	i = (long int)n;
	size = ft_intlen(i);
	str = ft_calloc(size + 1, sizeof(char));
	if (!str)
		return (NULL);
	if (i < 0)
	{
		i *= -1;
		str[0] = '-';
	}
	while (size && !str[--size])
	{
		str[size] = (i % 10) + '0';
		i /= 10;
	}
	return (str);
}
