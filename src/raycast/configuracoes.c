/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   configuracoes.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lnzila <lnzila@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/03 09:03:23 by lnzila            #+#    #+#             */
/*   Updated: 2025/04/23 08:20:04 by lnzila           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

// Configuração inicial do raio
void	setup_ray_1(t_data *data, int x, t_ray *ray)
{
	calc_raio(data, x, ray);
	init_mapa(data, ray);
	init_delta_dist(ray);
	init_passos(ray);
}

// Configuração do DDA e distância
void	setup_ray_2(t_data *data, t_ray *ray)
{
	calc_side_dist(data, ray);
	ray->hit = 0;
	exec_dda(data, ray);
	calc_perp_dist(data, ray);
}

// Configuração da parede e textura
void	setup_ray_3(t_data *data, t_ray *ray)
{
	calc_wall_height(data, ray);
	calc_wall_x(data, ray);
	calc_tex_x(data, ray);
	select_texture(data, ray);
}

void	select_texture(t_data *data, t_ray *ray)
{
	if (ray->side == 0 && ray->step_x < 0)
		ray->texture = &data->textures[0];
	else if (ray->side == 0 && ray->step_x > 0)
		ray->texture = &data->textures[1];
	else if (ray->side == 1 && ray->step_y < 0)
		ray->texture = &data->textures[2];
	else
		ray->texture = &data->textures[3];
}
