/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   iniciar.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aborges <aborges@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/13 20:50:08 by aborges           #+#    #+#             */
/*   Updated: 2025/04/13 20:55:52 by aborges          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

int	init_mlx(t_data *data)
{
	data->mlx = mlx_init();
	if (!data->mlx)
	{
		printf("Error!\nAo iniciar o MLX\n");
		return (1);
	}
	return (0);
}

void	print_map(t_data *data)
{
	int	y;

	printf("Mapa carregado:\n");
	y = 0;
	while (y < data->map_height)
	{
		printf("%s\n", data->mapa[y]);
		y++;
	}
}

void	start_loop(t_data *data)
{
	mlx_loop(data->mlx);
}

void	init_data(t_data *data)
{
	ft_memset(data, 0, sizeof(t_data));
}
