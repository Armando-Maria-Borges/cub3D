/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   aux.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lnzila <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 09:19:39 by lnzila            #+#    #+#             */
/*   Updated: 2025/03/24 12:22:00 by lnzila           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

int	fechar_janela(void *param)
{
	t_data	*data;

	data = (t_data *)param;
	if (data->win)
		mlx_destroy_window(data->mlx, data->win);
	exit(0);
	return (0);
}

void	pintar_janela(t_data *data)
{
	int	x;
	int	y;
	int	cor;

	y = 0;
	if (!data->mlx || !data->win)
		return ;
	while (y < NOVA_ALTURA)
	{
		x = 0;
		while (x < NOVA_LARGURA)
		{
			if (y < NOVA_ALTURA / 2)
				cor = 0xFFD700;
			else
				cor = 0x00008B;
			mlx_pixel_put(data->mlx, data->win, x, y, cor);
			x++;
		}
		y++;
	}
}

void	pintar_chao_teto(t_data *data)
{
	int	x;
	int	y;
	int	floor_color;
	int	ceiling_color;

	y = 0;
	floor_color = data->floor_color;
	ceiling_color = data->ceiling_color;
	while (y < NOVA_LARGURA / 2)
	{
		x = 0;
		while (x < NOVA_LARGURA)
		{
			my_mlx_pixel_put(data, x, y, ceiling_color);
			x++;
		}
		y++;
	}
	y = NOVA_ALTURA / 2;
	while (y < NOVA_ALTURA)
	{
		x = 0;
		while (x < NOVA_LARGURA)
		{
			my_mlx_pixel_put(data, x, y, floor_color);
			x++;
		}
		y++;
	}
}
