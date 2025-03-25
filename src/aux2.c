/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   aux2.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lnzila <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 09:33:02 by lnzila            #+#    #+#             */
/*   Updated: 2025/03/18 09:33:07 by lnzila           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

void	my_mlx_pixel_put(t_data *data, int x, int y, int color)
{
	char	*dst;

	if (x >= 0 && x < NOVA_LARGURA && y >= 0 && y < NOVA_ALTURA)
	{
		dst = data->addr + (y * data->line_length + x
				* (data->bits_per_pixel / 8));
		*(unsigned int *)dst = color;
	}
}

void	encontrar_jogador(t_data *data)
{
	int	y;
	int	x;

	y = 0;
	while (data->mapa[y] != NULL)
	{
		x = 0;
		while (data->mapa[y][x] != '\0')
		{
			if (data->mapa[y][x] == 'N' || data->mapa[y][x] == 'W' ||
				data->mapa[y][x] == 'S' || data->mapa[y][x] == 'E')
			{
				data->player.x = x + 0.5;
				data->player.y = y + 0.5;
				if (data->mapa[y][x] == 'N')
					data->player.angle = 3 * M_PI / 2;
				else if (data->mapa[y][x] == 'S')
					data->player.angle = M_PI / 2;
				else if (data->mapa[y][x] == 'E')
					data->player.angle = 0;
				else if (data->mapa[y][x] == 'W')
					data->player.angle = M_PI;
				return ;
			}
			x++;
		}
		y++;
	}
	printf("Jogador não encontrado no mapa!\n");
}

void	rotacionar_jogador(t_data *data)
{
	double	rotation_speed;

	rotation_speed = 0.05;
	if (data->keys.left)
		data->player.angle -= rotation_speed;
	if (data->keys.right)
		data->player.angle += rotation_speed;
	if (data->player.angle < 0)
		data->player.angle += 2 * M_PI;
	if (data->player.angle > 2 * M_PI)
		data->player.angle -= 2 * M_PI;
}
