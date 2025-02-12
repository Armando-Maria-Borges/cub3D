/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aborges <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/15 18:59:05 by aborges           #+#    #+#             */
/*   Updated: 2024/05/15 18:59:07 by aborges          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"

void	*ft_memcpy(void *dst, const void *src, size_t n)
{
	char	*cd;
	char	*co;
	size_t	a;

	a = 0;
	cd = (char *)dst;
	co = (char *)src;
	while (a < n)
	{
		cd[a] = co[a];
		a++;
	}
	return (dst);
}
