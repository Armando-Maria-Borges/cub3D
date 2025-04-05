/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aborges <aborges@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 12:17:15 by aborges           #+#    #+#             */
/*   Updated: 2025/03/17 18:14:31 by lnzila           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

unsigned int	get_pixel(t_texture *texture, int x, int y)
{
	char	*dst;

	dst = texture->addr + (y * texture->line_length + x * (texture->bpp / 8));
	return (*(unsigned int *)dst);
}

unsigned int	cria_trgb(int t, int r, int g, int b)
{
	/* if (r + 60 > 255)
			r = 255;
		else
			r = r + 50;
		if (g > 24)
			g = g - 24;
		else
			g = 0;
		if (b > 24)
			b = b - 24;
		else
			b = 0;*/
	return ((t << 0) | (r << 0) | (g << 0) | b);
}

// Inicializa a estrutura t_data
static void	init_data(t_data *data)
{
	memset(data, 0, sizeof(t_data));
}

// Exibe informações de cores
static void	print_colors(t_data *data)
{
	printf("Cor do Chão (F): #%06X\n", data->floor_color);
	printf("Cor do Teto (C): #%06X\n", data->ceiling_color);
}

// Inicializa o MLX
static int	init_mlx(t_data *data)
{
	data->mlx = mlx_init();
	if (!data->mlx)
	{
		printf("Error!\nAo iniciar o MLX\n");
		return (1);
	}
	return (0);
}

// Exibe o mapa carregado
static void	print_map(t_data *data)
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

// Encontra a posição do jogador
static int	find_player(t_data *data)
{
	if (!encontrar_jogador(data))
		return (1);
	printf("Jogador encontrado em: %.2f, %.2f, ângulo: %.2f\n", data->player.x,
		data->player.y, data->player.angle);
	return (0);
}

// Inicia o loop principal do MLX
static void	start_loop(t_data *data)
{
	mlx_loop(data->mlx);
}

// Função principal
int	main(int argc, char **argv)
{
	t_data data;

	init_data(&data);
	if (check_args(argc, argv) || load_map(&data, argv[1]))
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
	print_map(&data);
	if (find_player(&data))
		return (1);
	setup_hooks(&data);
	start_loop(&data);
	return (0);
}