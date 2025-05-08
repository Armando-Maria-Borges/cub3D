/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   desenhar_minimapa.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lnzila <lnzila@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/08 15:46:41 by lnzila            #+#    #+#             */
/*   Updated: 2025/05/08 15:59:27 by lnzila           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

static void	desenhar_tile(t_data *data, int x, int y, char tile)
{
	int	i;
	int	j;

	i = 0;
	while (i < TILE_SIZE)
	{
		j = 0;
		while (j < TILE_SIZE)
		{
			if (x * TILE_SIZE + i >= 0 && x * TILE_SIZE + i < NOVA_LARGURA && y
				* TILE_SIZE + j >= 0 && y * TILE_SIZE + j < NOVA_ALTURA)
			{
				if (tile == '1')
					my_mlx_pixel_put(data, x * TILE_SIZE + i, y * TILE_SIZE + j,
						0xFFFFFF);
				else if (tile == '0')
					my_mlx_pixel_put(data, x * TILE_SIZE + i, y * TILE_SIZE + j,
						0x808080);
			}
			j++;
		}
		i++;
	}
}

static void	desenhar_jogador(t_data *data)
{
	int	i;
	int	j;

	if (!data)
		return ;
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

void	desenhar_mapa(t_data *data)
{
	int	x;
	int	y;

	if (!data || !data->mapa)
	{
		printf("Error!\nMapa inválido em desenhar_mapa\n");
		return ;
	}
	y = 0;
	while (data->mapa[y])
	{
		x = 0;
		while (data->mapa[y][x])
		{
			if (data->mapa[y][x] == '1' || data->mapa[y][x] == '0')
				desenhar_tile(data, x, y, data->mapa[y][x]);
			x++;
		}
		y++;
	}
}

void	desenhar_minimapa(t_data *data)
{
	if (!data || !data->mapa)
	{
		printf("Error!\nDados inválidos para minimapa\n");
		return ;
	}
	desenhar_mapa(data);
	desenhar_jogador(data);
}
