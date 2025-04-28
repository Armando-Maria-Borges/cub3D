/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   aux.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lnzila <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 09:19:39 by lnzila            #+#    #+#             */
/*   Updated: 2025/03/18 09:19:41 by lnzila           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

int	create_window(t_data *data)
{
	data->win = mlx_new_window(data->mlx, NOVA_LARGURA, NOVA_ALTURA, "Cub3D");
	if (!data->win)
	{
		printf("Error!\nAo criar a janela\n");
		return (1);
	}
	return (0);
}

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
				cor = 0x00008C;
			mlx_pixel_put(data->mlx, data->win, x, y, cor);
			x++;
		}
		y++;
	}
}

void	pintar_chao_teto_aux(t_data *data, int *x, int *y, int *floor_color)
{
	while ((*y) < NOVA_ALTURA)
	{
		(*x) = 0;
		while ((*x) < NOVA_LARGURA)
		{
			my_mlx_pixel_put(data, (*x), (*y), (*floor_color));
			(*x)++;
		}
		(*y)++;
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
	pintar_chao_teto_aux(data, &x, &y, &floor_color);
}
