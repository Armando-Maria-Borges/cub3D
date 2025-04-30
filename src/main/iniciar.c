/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   iniciar.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aborges <aborges@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/13 20:50:08 by aborges           #+#    #+#             */
/*   Updated: 2025/04/30 16:23:17 by aborges          ###   ########.fr       */
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

size_t	print_original_map(t_data *data)
{
	int		y;
	size_t	maior_linha;

	y = 0;
	maior_linha = 0;
	printf("Mapa carregado:\n");
	while (y < data->map_height)
	{
		if (!data->mapa[y])
			break;
		printf("%s\n", data->mapa[y]);
		if (maior_linha < ft_strlen(data->mapa[y]))
			maior_linha = ft_strlen(data->mapa[y]);
		y++;
	}
	return (maior_linha);
}

int	print_map(t_data *data, t_map_data *map_data)
{
	size_t	maior_linha;
	char	**nova;

	maior_linha = print_original_map(data);
	nova = criar_nova_matriz(data, maior_linha);
	substituir_espacos_por_x(nova);
	completar_linhas_com_x(nova, maior_linha);
	if (!validar_conexoes_mapa(nova, map_data))
		return (0);
	if (!map_data->map_iniciado)
	{
		printf("Error\nErro ao carregar o mapa!");
		return (0);
	}
	//liberar_matriz(nova);
	return (1);
}

void	start_loop(t_data *data)
{
	mlx_loop(data->mlx);
}

void	init_data(t_data *data, t_map_data *map_data)
{
	map_data->map_iniciado = 1;
	ft_memset(data, 0, sizeof(t_data));
}
