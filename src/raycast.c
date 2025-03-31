/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycast.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lnzila <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 12:51:05 by lnzila            #+#    #+#             */
/*   Updated: 2025/03/24 12:51:33 by lnzila           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

void raycast(t_data *data)
{
    int x;
    double delta_angle = FOV / (double)NUM_RAYS;

    for (x = 0; x < NUM_RAYS; x++)
    {
        double ray_angle = data->player.angle - (FOV / 2) + x * delta_angle;
        double dir_x = cos(ray_angle);
        double dir_y = sin(ray_angle);

        int map_x = (int)data->player.x;
        int map_y = (int)data->player.y;

        double delta_dist_x = (dir_x == 0) ? 1e30 : fabs(1 / dir_x);
        double delta_dist_y = (dir_y == 0) ? 1e30 : fabs(1 / dir_y);
        double side_dist_x, side_dist_y;

        int step_x = (dir_x < 0) ? -1 : 1;
        int step_y = (dir_y < 0) ? -1 : 1;

        side_dist_x = (dir_x < 0) ? (data->player.x - map_x) * delta_dist_x : (map_x + 1.0 - data->player.x) * delta_dist_x;
        side_dist_y = (dir_y < 0) ? (data->player.y - map_y) * delta_dist_y : (map_y + 1.0 - data->player.y) * delta_dist_y;

        int hit = 0, side;
        double perp_wall_dist;

        while (!hit)
        {
            if (side_dist_x < side_dist_y)
            {
                side_dist_x += delta_dist_x;
                map_x += step_x;
                side = 0; // Parede vertical
            }
            else
            {
                side_dist_y += delta_dist_y;
                map_y += step_y;
                side = 1; // Parede horizontal
            }

            if (map_y >= 0 && map_y < data->map_height && map_x >= 0 && map_x < data->map_width &&
                data->mapa[map_y][map_x] == '1')
            {
                hit = 1;
                if (side == 0)
                    perp_wall_dist = (map_x - data->player.x + (1 - step_x) / 2.0) / dir_x;
                else
                    perp_wall_dist = (map_y - data->player.y + (1 - step_y) / 2.0) / dir_y;
                if (perp_wall_dist < 0.1)
                    perp_wall_dist = 0.1;
            }
        }

        // Cálculo da altura da parede a ser desenhada
        // perp_wall_dist = perp_wall_dist * cos(data->player.angle - ray_angle);
        int wall_height = (int)(NOVA_ALTURA / (perp_wall_dist * cos(data->player.angle - ray_angle)));

        // Determina os pontos de início e fim da linha da parede
        int start = (NOVA_ALTURA / 2) - (wall_height / 2);
        int end = (NOVA_ALTURA / 2) + (wall_height / 2);
        if (start < 0) start = 0;
        if (end >= NOVA_ALTURA) end = NOVA_ALTURA - 1;

        // Calcula a posição onde o raio atingiu a parede
        double wall_x;
        if (side == 0)
            wall_x = data->player.y + perp_wall_dist * dir_y;
        else
            wall_x = data->player.x + perp_wall_dist * dir_x;
        wall_x -= floor(wall_x);

        // Determina a coordenada x da textura
        int tex_x = (int)(wall_x * (double)(data->textures[0].width));
        if (tex_x < 0)
            tex_x = 0;
        if (tex_x >= data->textures[0].width)
            tex_x = data->textures[0].width - 1;

        // Ajusta tex_x para a orientação correta da textura
        if (side == 0 && dir_x > 0)
            tex_x = data->textures[0].width - tex_x - 1;
        if (side == 1 && dir_y < 0)
            tex_x = data->textures[0].width - tex_x - 1;

        // Seleciona a textura correta com base na direção
        t_texture *texture;
        if (side == 0 && step_x < 0)
            texture = &data->textures[0]; // Parede Norte
        else if (side == 0 && step_x > 0)
            texture = &data->textures[1]; // Parede Sul
        else if (side == 1 && step_y < 0)
            texture = &data->textures[2]; // Parede Oeste
        else
            texture = &data->textures[3]; // Parede Leste

        // Desenha os pixels da parede com a textura
        for (int y = start; y < end; y++)
        {
            int d = y * 256 - NOVA_ALTURA * 128 + wall_height * 128;
            int tex_y = ((d * texture->height) / wall_height) / 256;
            if (tex_y < 0)
                tex_y = 0;
            if (tex_y >= texture->height)
                tex_y = texture->height - 1;

            unsigned int color = get_pixel(texture, tex_x, tex_y);
            my_mlx_pixel_put(data, x, y, color);
        }

        // Desenha o teto acima da parede usando a cor já armazenada
        for (int y = 0; y < start; y++)
        {
            unsigned int ceiling_color = data->ceiling_color;
            my_mlx_pixel_put(data, x, y, ceiling_color);
        }

        // Desenha o chão abaixo da parede
        for (int y = end; y < NOVA_ALTURA; y++)
        {
            // Extração dos componentes RGB a partir do inteiro floor_color
            unsigned int r = (data->floor_color >> 16) & 0xFF;
            unsigned int g = (data->floor_color >> 8) & 0xFF;
            unsigned int b = data->floor_color & 0xFF;
            unsigned int floor_color = cria_trgb(0, r, g, b);
            my_mlx_pixel_put(data, x, y, floor_color);
        }
    }
}