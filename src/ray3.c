/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ray3.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lnzila <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/02 11:21:43 by lnzila            #+#    #+#             */
/*   Updated: 2025/04/03 09:23:15 by lnzila           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

void	calc_raio(t_data *data, int x, t_ray *ray)
{
	double	delta_angle;

	delta_angle = FOV / (double)NUM_RAYS;
	ray->ray_angle = data->player.angle - (FOV / 2) + x * delta_angle;
	ray->dir_x = cos(ray->ray_angle);
	ray->dir_y = sin(ray->ray_angle);
}

// Calcula a altura da parede corrigida
void	calc_wall_height(t_data *data, t_ray *ray)
{
	double	corrected_dist;

	corrected_dist = ray->perp_wall_dist * cos(data->player.angle
			- ray->ray_angle);
	ray->wall_height = (int)(NOVA_ALTURA / corrected_dist);
	ray->start = (NOVA_ALTURA / 2) - (ray->wall_height / 2);
	ray->end = (NOVA_ALTURA / 2) + (ray->wall_height / 2);
	if (ray->start < 0)
		ray->start = 0;
	if (ray->end >= NOVA_ALTURA)
		ray->end = NOVA_ALTURA - 1;
}

// Calcula a posição da parede atingida
void	calc_wall_x(t_data *data, t_ray *ray)
{
	if (ray->side == 0)
		ray->wall_x = data->player.y + ray->perp_wall_dist * ray->dir_y;
	else
		ray->wall_x = data->player.x + ray->perp_wall_dist * ray->dir_x;
	ray->wall_x -= floor(ray->wall_x);
}
