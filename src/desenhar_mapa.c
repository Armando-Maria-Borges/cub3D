/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   desenhar_mapa.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lnzila <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 12:56:36 by lnzila            #+#    #+#             */
/*   Updated: 2025/03/24 12:57:41 by lnzila           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

void desenhar_mapa(t_data *data)
{
    int x, y, i, j;

    for (y = 0; data->mapa[y] != NULL; y++)
    {
        for (x = 0; data->mapa[y][x] != '\0'; x++)
        {
            for (i = 0; i < TILE_SIZE; i++)
            {
                for (j = 0; j < TILE_SIZE; j++)
                {
                    if (x * TILE_SIZE + i >= 0 && x * TILE_SIZE + i < NOVA_LARGURA &&
                        y * TILE_SIZE + j >= 0 && y * TILE_SIZE + j < NOVA_ALTURA)
                    {
                        if (data->mapa[y][x] == '1')
                            my_mlx_pixel_put(data, x * TILE_SIZE + i, y * TILE_SIZE + j, 0xFFFFFF);
                        else if (data->mapa[y][x] == '0')
                            my_mlx_pixel_put(data, x * TILE_SIZE + i, y * TILE_SIZE + j, 0x808080);
                    }
                }
            }
        }
    }
    for (i = 0; i < TILE_SIZE / 2; i++)
    {
        for (j = 0; j < TILE_SIZE / 2; j++)
        {
            my_mlx_pixel_put(data, data->player.x * TILE_SIZE + i, data->player.y * TILE_SIZE + j, 0xFFA500);
        }
    }
}

int render_scene(void *param)
{
    t_data *data = (t_data *)param;

    pintar_chao_teto(data);
    raycast(data);
    mlx_put_image_to_window(data->mlx, data->win, data->img, 0, 0);
    mover_jogador(data);

    desenhar_mapa(data);

    return 0;
}

void debug_print_map(t_data *data)
{
    printf("Imprimindo mapa:\n");
    for (int y = 0; y < data->map_height; y++)
    {
        printf("[%d] %s\n", y, data->mapa[y]);
    }
}
