/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   teclas.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lnzila <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 12:24:01 by lnzila            #+#    #+#             */
/*   Updated: 2025/03/24 12:46:40 by lnzila           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

int	key_press(int keycode, void *param)
{
	t_data	*data;

	data = (t_data *)param;
	if (keycode == KEY_ESC)
	{
		mlx_destroy_window(data->mlx, data->win);
		exit(0);
	}
	else if (keycode == KEY_W)
		data->keys.w = 1;
	else if (keycode == KEY_A)
		data->keys.a = 1;
	else if (keycode == KEY_S)
		data->keys.s = 1;
	else if (keycode == KEY_D)
		data->keys.d = 1;
	else if (keycode == KEY_LEFT)
		data->keys.left = 1;
	else if (keycode == KEY_RIGHT)
		data->keys.right = 1;
	return (0);
}

int	key_release(int keycode, void *param)
{
	t_data	*data;

	data = (t_data *)param;
	if (keycode == KEY_W)
		data->keys.w = 0;
	else if (keycode == KEY_A)
		data->keys.a = 0;
	else if (keycode == KEY_S)
		data->keys.s = 0;
	else if (keycode == KEY_D)
		data->keys.d = 0;
	else if (keycode == KEY_LEFT)
		data->keys.left = 0;
	else if (keycode == KEY_RIGHT)
		data->keys.right = 0;
	return (0);
}
