/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   verificar_colisao.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aborges <aborges@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 12:55:17 by lnzila            #+#    #+#             */
/*   Updated: 2025/04/22 11:44:59 by aborges          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

double	calcular_distancia(double new_x, double new_y, int i, int j)
{
	double	nearest_x;
	double	nearest_y;
	double	dx;
	double	dy;

	nearest_x = clamp(new_x, j, j + 1);
	nearest_y = clamp(new_y, i, i + 1);
	dx = new_x - nearest_x;
	dy = new_y - nearest_y;
	return (sqrt(dx * dx + dy * dy));
}
