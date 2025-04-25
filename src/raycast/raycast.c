/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycast.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aborges <aborges@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 12:51:05 by lnzila            #+#    #+#             */
/*   Updated: 2025/04/25 15:48:08 by aborges          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

// Inicializa variáveis de mapa
void	init_mapa(t_data *data, t_ray *ray)
{
	ray->map_x = (int)data->player.x;
	ray->map_y = (int)data->player.y;
}

// Inicializa passos
void	init_passos(t_ray *ray)
{
	if (ray->dir_x < 0)
		ray->step_x = -1;
	else
		ray->step_x = 1;
	if (ray->dir_y < 0)
		ray->step_y = -1;
	else
		ray->step_y = 1;
}

// Processa um único raio
void	process_ray(t_data *data, int x)
{
	t_ray	ray;

	setup_ray_1(data, x, &ray);
	setup_ray_2(data, &ray);
	setup_ray_3(data, &ray);
	draw_ray(data, x, &ray);
}

// Função principal de raycasting
void	raycast(t_data *data)
{
	int	x;

	x = 0;
	while (x < NUM_RAYS)
	{
		process_ray(data, x);
		x++;
	}
}
