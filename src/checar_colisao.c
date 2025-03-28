/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   checar_colisao.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lnzila <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 12:53:46 by lnzila            #+#    #+#             */
/*   Updated: 2025/03/24 12:54:34 by lnzila           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

static double	clamp(double value, double min, double max)
{
	if (value < min)
		return (min);
	if (value > max)
		return (max);
	return (value);
}

static void	calcular_minimos(double new_x, double new_y, int *min_x, int *min_y)
{
	*min_x = (int)floor(new_x) - 1;
	*min_y = (int)floor(new_y) - 1;
}

// Calcula os limites máximos das células
static void	calcular_maximos(double new_x, double new_y, int *max_x, int *max_y)
{
	*max_x = (int)floor(new_x) + 1;
	*max_y = (int)floor(new_y) + 1;
}

// Calcula a distância até a parede
static double	calcular_distancia(double new_x, double new_y, int i, int j)
{
	double	nearest_x;
	double	nearest_y;
	double	dx;
	double	dy;

	nearest_x = clamp(new_x, j, j + 1);
	nearest_y = clamp(new_y, i, i + 1);
	dx = new_x - nearest_x;
	dy = new_y - nearest_y;
	return (sqrt(dx * dx + dy * dy));
}

// Verifica se há colisão em uma célula específica
static int	verificar_colisao_celula(t_data *data, double new_x, double new_y, int i, int j)
{
	double	collision_radius;
	double	distance;

	collision_radius = 0.2;
	if (data->mapa[i][j] == '1')
	{
		distance = calcular_distancia(new_x, new_y, i, j);
		if (distance < collision_radius)
			return (1);
	}
	return (0);
}

// Função principal de verificação de colisão
int	check_collision(t_data *data, double new_x, double new_y)
{
	int	min_x, min_y;
	int	max_x, max_y;
	int	i;
	int	j;

	calcular_minimos(new_x, new_y, &min_x, &min_y);
	calcular_maximos(new_x, new_y, &max_x, &max_y);
	i = min_y;
	while (i <= max_y)
	{
		j = min_x;
		while (j <= max_x)
		{
			if (i >= 0 && j >= 0 && i < data->map_height && j < data->map_width)
				if (verificar_colisao_celula(data, new_x, new_y, i, j))
					return 1;
			j++;
		}
		i++;
	}
	return 0;
}
