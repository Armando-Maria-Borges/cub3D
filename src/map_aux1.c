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

int	ciclo_verifica_linha_cor(const char *str, int *val, int *i)
{
	*val = 0;
	while (str[*i] == ' ')
		(*i)++;
	if (!ft_isdigit(str[*i]))
		return (0);
	while (ft_isdigit(str[*i]))
	{
		*val = (*val) * 10 + (str[*i] - '0');
		(*i)++;
	}
	if (*val < 0 || *val > 255)
		return (0);
	return (1);
}

int	verifica_linha_cor(const char *str, int *r, int *g, int *b)
{
	int	num[3];
	int var[3];

	var[0] = 0;
	var[1] = 0;
	while (var[1] < 3)
	{
		if (!ciclo_verifica_linha_cor(str, &var[2], &var[0]))
			return (0);
		num[var[1]++] = var[2];
		if (var[1] < 3)
		{
			if (str[var[0]] != ',')
				return (0);
			var[0]++;
		}
	}
	while (str[var[0]] == ' ')
		var[0]++;
	if (str[var[0]] != '\0' && str[var[0]] != '\n')
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
