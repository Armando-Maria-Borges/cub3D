/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ray4.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lnzila <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/03 08:47:49 by lnzila            #+#    #+#             */
/*   Updated: 2025/04/03 08:47:55 by lnzila           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

// Executa o algoritmo DDA
void exec_dda(t_data *data, t_ray *ray)
{
    while (!ray->hit)
    {
        if (ray->side_dist_x < ray->side_dist_y)
        {
            ray->side_dist_x += ray->delta_dist_x;
            ray->map_x += ray->step_x;
            ray->side = 0;
        }
        else
        {
            ray->side_dist_y += ray->delta_dist_y;
            ray->map_y += ray->step_y;
            ray->side = 1;
        }
        if (ray->map_y >= 0 && ray->map_y < data->map_height && ray->map_x >= 0 && ray->map_x < data->map_width && data->mapa[ray->map_y][ray->map_x] == '1')
            ray->hit = 1;
    }
}

// Calcula a distância perpendicular
void	calc_perp_dist(t_data *data, t_ray *ray)
{
	double	px;
	double	py;

	px = data->player.x;
	py = data->player.y;
	if (ray->side == 0)
		ray->perp_wall_dist = (ray->map_x - px + (1 - ray->step_x) / 2.0)
			/ ray->dir_x;
	else
		ray->perp_wall_dist = (ray->map_y - py + (1 - ray->step_y) / 2.0)
			/ ray->dir_y;
	if (ray->perp_wall_dist < 0.1)
		ray->perp_wall_dist = 0.1;
}

// Configuração do DDA e distância
void setup_ray_2(t_data *data, t_ray *ray)
{
    calc_side_dist(data, ray);
    ray->hit = 0;
    exec_dda(data, ray);
    calc_perp_dist(data, ray);
}