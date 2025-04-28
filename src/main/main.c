/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aborges <aborges@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 12:17:15 by aborges           #+#    #+#             */
/*   Updated: 2025/04/28 12:42:00 by aborges          ###   ########.fr       */
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

/*
void	free_data(t_data *data)
{
	int	i;

	// Liberar texturas da MLX (se tiver imagens carregadas)
	for (i = 0; i < 4; i++)
	{
		if (data->textures[i].img)
			mlx_destroy_image(data->mlx, data->textures[i].img);
	}

	// Destruir imagem principal (se existir)
	if (data->img)
		mlx_destroy_image(data->mlx, data->img);

	// Destruir janela (se existir)
	if (data->win)
		mlx_destroy_window(data->mlx, data->win);

	// Destruir display (Linux)
	if (data->mlx)
		mlx_destroy_display(data->mlx);

	// Liberar mlx (Linux)
	free(data->mlx);

	// Liberar caminhos das texturas (se alocados dinamicamente)
	for (i = 0; i < 4; i++)
	{
		if (data->texture_paths[i])
			free(data->texture_paths[i]);
	}

	// Liberar mapa (char **mapa)
	if (data->mapa)
	{
		i = 0;
		while (data->mapa[i])
			free(data->mapa[i++]);
		free(data->mapa);
	}
	free(data->mlx);
}
*/


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

	//free_data(&data);
	return (0);
}
