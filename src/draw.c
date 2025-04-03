/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lnzila <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/03 08:56:01 by lnzila            #+#    #+#             */
/*   Updated: 2025/04/03 08:56:04 by lnzila           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

// Desenha a parede com textura
void draw_wall(t_data *data, int x, t_ray *ray)
{
    int y;
    int tex_y;

    y = ray->start;
    while (y < ray->end)
    {
        int d;

        d = y * 256 - NOVA_ALTURA * 128 + ray->wall_height * 128;
        tex_y = ((d * ray->texture->height) / ray->wall_height) / 256;
        if (tex_y < 0)
            tex_y = 0;
        if (tex_y >= ray->texture->height)
            tex_y = ray->texture->height - 1;
        my_mlx_pixel_put(data, x, y, get_pixel(ray->texture, ray->tex_x, tex_y));
        y++;
    }
}

// Desenha o teto
void draw_ceiling(t_data *data, int x, int start)
{
    int y;

    y = 0;
    while (y < start)
    {
        my_mlx_pixel_put(data, x, y, data->ceiling_color);
        y++;
    }
}

// Desenha o chão
void draw_floor(t_data *data, int x, int end)
{
    int y;
    unsigned int    r;
    unsigned int    g;
    unsigned int    b;

    r = (data->floor_color >> 16) & 0xFF;
    g = (data->floor_color >> 8) & 0xFF;
    b = data->floor_color & 0xFF;
    y = end;
    while (y < NOVA_ALTURA)
    {
        my_mlx_pixel_put(data, x, y, cria_trgb(0, r, g, b));
        y++;
    }
}

// Desenha o raio completo
void draw_ray(t_data *data, int x, t_ray *ray)
{
    draw_wall(data, x, ray);
    draw_ceiling(data, x, ray->start);
    draw_floor(data, x, ray->end);
}