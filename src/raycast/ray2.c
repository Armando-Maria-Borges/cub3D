/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ray2.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aborges <aborges@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/02 11:21:23 by lnzila            #+#    #+#             */
/*   Updated: 2025/04/25 09:59:40 by aborges          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

// Inicializa distâncias delta
void	init_delta_dist(t_ray *ray)
{
	double	div_x;
	double	div_y;

	div_x = 1 / ray->dir_x;
	div_y = 1 / ray->dir_y;
	if (ray->dir_x == 0)
		ray->delta_dist_x = 1e30;
	else
		ray->delta_dist_x = fabs(div_x);
	if (ray->dir_y == 0)
		ray->delta_dist_y = 1e30;
	else
		ray->delta_dist_y = fabs(div_y);
}

// Calcula distâncias laterais iniciais
void	calc_side_dist(t_data *data, t_ray *ray)
{
	double	px;
	double	py;

	px = data->player.x;
	py = data->player.y;
	if (ray->dir_x < 0)
		ray->side_dist_x = (px - ray->map_x) * ray->delta_dist_x;
	else
		ray->side_dist_x = (ray->map_x + 1.0 - px) * ray->delta_dist_x;
	if (ray->dir_y < 0)
		ray->side_dist_y = (py - ray->map_y) * ray->delta_dist_y;
	else
		ray->side_dist_y = (ray->map_y + 1.0 - py) * ray->delta_dist_y;
}

// Calcula a coordenada x da textura
void	calc_tex_x(t_data *data, t_ray *ray)
{
	ray->tex_x = (int)(ray->wall_x * (double)(data->textures[0].width));
	if (ray->tex_x < 0)
		ray->tex_x = 0;
	if (ray->tex_x >= data->textures[0].width)
		ray->tex_x = data->textures[0].width - 1;
	if (ray->side == 0 && ray->dir_x > 0)
		ray->tex_x = data->textures[0].width - ray->tex_x - 1;
	if (ray->side == 1 && ray->dir_y < 0)
		ray->tex_x = data->textures[0].width - ray->tex_x - 1;
}
