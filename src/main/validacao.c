/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validacao.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aborges <aborges@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/04 16:31:07 by lnzila            #+#    #+#             */
/*   Updated: 2025/04/30 18:10:21 by aborges          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

int	check_args(int ac, char **av)
{
	(void)av;
	if (ac != 2)
	{
		printf("Error\nUsage: ./cub3d <mapa>\n");
		return (1);
	}
	return (0);
}

int	load_map(char *arquivo, t_data *data, t_map_data *map_data)
{
	data->mapa = ler_mapa(arquivo, data, map_data);
	if (!data->mapa)
	{
		printf("Error\n ao carregar o mapa\n");
		return (1);
	}
	if (!map_data->map_iniciado)
		return (1);
	return (0);
}

int	validate_positions(t_data *data)
{
	int	number_position;

	number_position = check_number_position(data);
	if (number_position > 1)
	{
		printf("\n\nError!. there is %d position for player.\n\n",
			number_position);
		return (1);
	}
	return (0);
}

int	validate_characters(t_data *data)
{
	int	number_caracter;

	number_caracter = check_other_cracter(data);
	if (number_caracter > 0)
	{
		printf("\n\nError!\nThere is %d caracter not found.\n\n",
			number_caracter);
		return (1);
	}
	return (0);
}

int	validate_textures(t_data *data)
{
	int	i;

	i = 0;
	while (i < 4)
	{
		if (!data->texture_paths[i])
		{
			printf("Error!\nCaminho da textura %d não definido!\n", i);
			return (1);
		}
		printf("Textura %d lida do mapa: %s\n", i, data->texture_paths[i]);
		i++;
	}
	return (0);
}
