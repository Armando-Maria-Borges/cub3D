/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   desenhar_mapa.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lnzila <lnzila@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 12:56:36 by lnzila            #+#    #+#             */
/*   Updated: 2025/04/29 23:51:29 by lnzila           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

int	render_scene(void *param)
{
	t_data	*data;

	data = (t_data *)param;
	pintar_chao_teto(data);
	mover_jogador(data);
	raycast(data);
	mlx_put_image_to_window(data->mlx, data->win, data->img, 0, 0);
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
