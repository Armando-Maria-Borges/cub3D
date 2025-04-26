/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   algdda.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lnzila <lnzila@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/03 08:47:49 by lnzila            #+#    #+#             */
/*   Updated: 2025/04/23 08:14:49 by lnzila           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

// Executa o algoritmo DDA

void	exec_dda(t_data *data, t_ray *ray)
{
	while (ray->hit == 0)
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
		if (ray->map_y >= 0 && ray->map_y < data->map_height && ray->map_x >= 0
			&& ray->map_x < data->map_width
			&& data->mapa[ray->map_y][ray->map_x] == '1')
			ray->hit = 1;
	}
}
