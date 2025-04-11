/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   imagens.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lnzila <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/04 16:26:05 by lnzila            #+#    #+#             */
/*   Updated: 2025/04/04 16:26:07 by lnzila           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

int	create_image(t_data *data)
{
	data->img = mlx_new_image(data->mlx, NOVA_LARGURA, NOVA_ALTURA);
	if (!data->img)
	{
		printf("Error!\nAo criar a imagem\n");
		return (1);
	}
	return (0);
}

int	get_image_addr(t_data *data)
{
	data->addr = mlx_get_data_addr(data->img, &data->bits_per_pixel,
			&data->line_length, &data->endian);
	if (!data->addr)
	{
		printf("Error!\nAo obter endereço da imagem\n");
		return (1);
	}
	printf("Image created: bpp=%d, line_length=%d, endian=%d\n",
		data->bits_per_pixel, data->line_length, data->endian);
	return (0);
}
