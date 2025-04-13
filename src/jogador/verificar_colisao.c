/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mover_jogador.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lnzila <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 12:55:17 by lnzila            #+#    #+#             */
/*   Updated: 2025/03/24 13:49:27 by lnzila           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

double	calcular_distancia(double new_x, double new_y, int i, int j)
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
