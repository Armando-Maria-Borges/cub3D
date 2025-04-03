/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycast.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lnzila <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 12:51:05 by lnzila            #+#    #+#             */
/*   Updated: 2025/04/02 09:00:41 by lnzila           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

void calc_raio(t_data *data, int x, t_ray *ray)
{
    double  delta_angle;

    delta_angle = FOV / (double)NUM_RAYS;
    ray->ray_angle = data->player.angle - (FOV / 2) + x * delta_angle;
    ray->dir_x = cos(ray->ray_angle);
    ray->dir_y = sin(ray->ray_angle);
}

// Inicializa variáveis de mapa
void init_mapa(t_data *data, t_ray *ray)
{
    ray->map_x = (int)data->player.x;
    ray->map_y = (int)data->player.y;
}

// Inicializa passos
void init_passos(t_ray *ray)
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

// Seleciona a textura correta
void select_texture(t_data *data, t_ray *ray)
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

// Processa um único raio
void process_ray(t_data *data, int x)
{
    t_ray   ray;

    setup_ray_1(data, x, &ray);
    setup_ray_2(data, &ray);
    setup_ray_3(data, &ray);
    draw_ray(data, x, &ray);
}

// Função principal de raycasting
void    raycast(t_data *data)
{
    int x;

    x = 0;
    while (x < NUM_RAYS)
    {
        process_ray(data, x);
        x++;
    }
}
