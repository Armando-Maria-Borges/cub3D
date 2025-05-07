/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lnzila <lnzila@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 12:17:15 by aborges           #+#    #+#             */
/*   Updated: 2025/05/07 13:15:53 by lnzila           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

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

int	setup(t_data *data, t_map_data *map_data, int argc, char **argv)
{
	init_data(data, map_data);
	if (check_args(argc, argv) || load_map(argv[1], data, map_data))
	{
		liberar_tudo(data);
		return (1);
	}
	if (validate_positions(data) || validate_characters(data))
		return (1);
	if (validate_textures(data))
		return (1);
	print_colors(data);
	return (0);
}

int	start_graphics(t_data *data)
{
	if (init_mlx(data) || create_window(data))
		return (1);
	if (create_image(data) || get_image_addr(data))
		return (1);
	if (load_textures(data))
		return (1);
	return (0);
}

int	main(int argc, char **argv)
{
	t_data		data;
	t_map_data	map_data;

	if (setup(&data, &map_data, argc, argv))
		return (1);
	if (!print_map(&data) || encontrar_player(&data))
		return (1);
	if (start_graphics(&data))
		return (1);
	setup_hooks(&data);
	start_loop(&data);
	free_texturas(&data);
	mlx_destroy_image(data.mlx, data.img);
	return (0);
}
