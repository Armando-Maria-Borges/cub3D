/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   checar_colisao.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aborges <aborges@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/13 20:50:54 by aborges           #+#    #+#             */
/*   Updated: 2025/04/22 11:45:14 by aborges          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

double	clamp(double value, double min, double max)
{
	if (value < min)
		return (min);
	if (value > max)
		return (max);
	return (value);
}

void	calcular_minimos(double new_x, double new_y, int *min_x, int *min_y)
{
	*min_x = (int)floor(new_x) - 1;
	*min_y = (int)floor(new_y) - 1;
}

void	calcular_maximos(double new_x, double new_y, int *max_x, int *max_y)
{
	*max_x = (int)floor(new_x) + 1;
	*max_y = (int)floor(new_y) + 1;
}

int	verificar_colisao_celula(t_data *data, double new_x, double new_y, int *var)
{
	int		i;
	int		j;
	double	collision_radius;
	double	distance;

	i = var[0];
	j = var[1];
	collision_radius = 0.2;
	if (data->mapa[i][j] == '1')
	{
		distance = calcular_distancia(new_x, new_y, i, j);
		if (distance < collision_radius)
			return (1);
	}
	return (0);
}

int	check_collision(t_data *data, double new_x, double new_y)
{
	int	var[2];
	int	min_x;
	int	min_y;
	int	max_x;
	int	max_y;

	calcular_minimos(new_x, new_y, &min_x, &min_y);
	calcular_maximos(new_x, new_y, &max_x, &max_y);
	var[0] = min_y;
	while (var[0] <= max_y)
	{
		var[1] = min_x;
		while (var[1] <= max_x)
		{
			if (var[0] >= 0 && var[1] >= 0 && var[0] < data->map_height
				&& var[1] < data->map_width)
				if (verificar_colisao_celula(data, new_x, new_y, var))
					return (1);
			var[1]++;
		}
		var[0]++;
	}
	return (0);
}
