/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mover_jogador.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aborges <aborges@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 12:55:17 by lnzila            #+#    #+#             */
/*   Updated: 2025/04/25 14:59:58 by aborges          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static void	rotate_player(t_data *data, double rotation_speed)
{
	if (data->keys.left)
		data->player.angle -= rotation_speed;
	if (data->keys.right)
		data->player.angle += rotation_speed;
	if (data->player.angle < 0)
		data->player.angle += 2 * M_PI;
	if (data->player.angle > 2 * M_PI)
		data->player.angle -= 2 * M_PI;
}

static void	mover_frente_tras(t_data *data, double move_speed)
{
	double	new_x;
	double	new_y;

	if (data->keys.w)
	{
		new_x = data->player.x + cos(data->player.angle) * move_speed;
		new_y = data->player.y + sin(data->player.angle) * move_speed;
		if (!check_collision(data, new_x, new_y))
		{
			data->player.x = new_x;
			data->player.y = new_y;
		}
	}
	if (data->keys.s)
	{
		new_x = data->player.x - cos(data->player.angle) * move_speed;
		new_y = data->player.y - sin(data->player.angle) * move_speed;
		if (!check_collision(data, new_x, new_y))
		{
			data->player.x = new_x;
			data->player.y = new_y;
		}
	}
}

static void	mover_lados(t_data *data, double move_speed)
{
	double	new_x;
	double	new_y;

	if (data->keys.d)
	{
		new_x = data->player.x + cos(data->player.angle + M_PI_2) * move_speed;
		new_y = data->player.y + sin(data->player.angle + M_PI_2) * move_speed;
		if (!check_collision(data, new_x, new_y))
		{
			data->player.x = new_x;
			data->player.y = new_y;
		}
	}
	if (data->keys.a)
	{
		new_x = data->player.x - cos(data->player.angle + M_PI_2) * move_speed;
		new_y = data->player.y - sin(data->player.angle + M_PI_2) * move_speed;
		if (!check_collision(data, new_x, new_y))
		{
			data->player.x = new_x;
			data->player.y = new_y;
		}
	}
}

void	mover_jogador(t_data *data)
{
	double	move_speed;
	double	rotation_speed;

	move_speed = 0.04;
	rotation_speed = 0.04;
	rotate_player(data, rotation_speed);
	mover_frente_tras(data, move_speed);
	mover_lados(data, move_speed);
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
