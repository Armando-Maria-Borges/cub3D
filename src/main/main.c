/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aborges <aborges@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 12:17:15 by aborges           #+#    #+#             */
/*   Updated: 2025/04/14 01:47:58 by aborges          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

unsigned int	get_pixel(t_texture *texture, int x, int y)
{
	char	*dst;

	dst = texture->addr + (y * texture->line_length + x * (texture->bpp / 8));
	return (*(unsigned int *)dst);
}

unsigned int	cria_trgb(int t, int r, int g, int b)
{
	t = t;
	if (r + 60 > 255)
		r = 24;
	else
		r = r + 50;
	if (g > 24)
		g = g - 16;
	else
		g = 0;
	if (b > 24)
		b = b - 8;
	else
		b = 0;
	return (t | r | g | b);
}

static void	print_colors(t_data *data)
{
	printf("Cor do Chão (F): #%06X\n", data->floor_color);
	printf("Cor do Teto (C): #%06X\n", data->ceiling_color);
}

static int	encontrar_player(t_data *data)
{
	if (!encontrar_jogador(data))
		return (1);
	printf("Jogador encontrado em: %.2f, %.2f, ângulo: %.2f\n", data->player.x,
		data->player.y, data->player.angle);
	return (0);
}

int	main(int argc, char **argv)
{
	t_data		data;
	t_map_data	map_data;

	init_data(&data);
	if (check_args(argc, argv) || load_map(argv[1], &data, &map_data))
		return (1);
	if (validate_positions(&data) || validate_characters(&data))
		return (1);
	if (validate_textures(&data))
		return (1);
	print_colors(&data);
	if (init_mlx(&data) || create_window(&data))
		return (1);
	if (create_image(&data) || get_image_addr(&data))
		return (1);
	if (load_textures(&data))
		return (1);
	if (!print_map(&data))
		return (1);
	if (encontrar_player(&data))
		return (1);
	setup_hooks(&data);
	start_loop(&data);
	return (0);
}
