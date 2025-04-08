/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   desenhar_mapa.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lnzila <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 12:56:36 by lnzila            #+#    #+#             */
/*   Updated: 2025/03/27 17:05:23 by lnzila           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	desenhar_mapa(t_data *data)
{
	int	x;
	int	y;
	int	i;
	int	j;

	y = 0;
	while (data->mapa[y] != NULL)
	{
		x = 0;
		while (data->mapa[y][x] != '\0')
		{
			//ciclo_desenhar_mapa(data, &j, &x, &y);
			i = 0;
			while (i < TILE_SIZE)
			{
				j = 0;
				while (j < TILE_SIZE)
				{
					if (x * TILE_SIZE + i >= 0 && x
						* TILE_SIZE + i < NOVA_LARGURA
						&& y * TILE_SIZE + j >= 0 && TILE_SIZE
						+ j < NOVA_ALTURA)
					{
						if (data->mapa[y][x] == '1')
							my_mlx_pixel_put(data, x
								* TILE_SIZE + i, y
								* TILE_SIZE + j, 0xFFFFFF);
						else if (data->mapa[y][x] == '0')
							my_mlx_pixel_put(data, x
								* TILE_SIZE + i, y
								* TILE_SIZE + j, 0x808080);
					}
					j++;
				}
				i++;
			}
			x++;
		}
		y++;
	}
	i = 0;
	while (i < TILE_SIZE / 2)
	{
		j = 0;
		while (j < TILE_SIZE / 2)
		{
			my_mlx_pixel_put(data, data->player.x * TILE_SIZE + i,
				data->player.y * TILE_SIZE + j, 0xFFA500);
			j++;
		}
		i++;
	}
}

int	render_scene(void *param)
{
	t_data	*data;

	data = (t_data *)param;
	pintar_chao_teto(data);
	raycast(data);
	mlx_put_image_to_window(data->mlx, data->win, data->img, 0, 0);
	mover_jogador(data);
	desenhar_mapa(data);
	return (0);
}

void	debug_print_map(t_data *data)
{
	int	y;

	y = 0;
	printf("Imprimindo mapa:\n");
	while (y < data->map_height)
	{
		printf("[%d] %s\n", y, data->mapa[y]);
		y++;
	}
}
