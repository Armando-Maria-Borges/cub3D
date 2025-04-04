/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_aux1.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aborges <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/04 16:07:46 by aborges           #+#    #+#             */
/*   Updated: 2025/04/04 16:07:49 by aborges          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

int	parse_rgb(const char *str, int *r, int *g, int *b)
{
	int	i;
	int	num[3];
	int	n;
	int	val;

	i = 0;
	n = 0;
	while (n < 3)
	{
		val = 0;
		while (str[i] == ' ')
			i++;
		if (!ft_isdigit(str[i]))
			return (0);
		while (ft_isdigit(str[i]))
		{
			val = val * 10 + (str[i] - '0');
			i++;
		}
		if (val < 0 || val > 255)
			return (0);
		num[n++] = val;
		if (n < 3)
		{
			if (str[i] != ',')
				return (0);
			i++;
		}
	}
	while (str[i] == ' ')
		i++;
	// Se ainda houver conteúdo inválido depois
	if (str[i] != '\0' && str[i] != '\n')
		return (0);
	*r = num[0];
	*g = num[1];
	*b = num[2];
	return (1);
}

size_t	ft_strcspn(const char *s, const char *reject)
{
	size_t i, j;
	i = 0;
	while (s[i])
	{
		j = 0;
		while (reject[j])
		{
			if (s[i] == reject[j])
				return (i);
			j++;
		}
		i++;
	}
	return (i);
}
