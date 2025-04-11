/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lnzila <lnzila@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 09:38:18 by lnzila            #+#    #+#             */
/*   Updated: 2025/04/11 08:36:21 by lnzila           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

char	**ler_mapa(char *arquivo, t_data *data, t_map_data *map_data)
{
	map_data->f = open(arquivo, O_RDONLY);
	if (map_data->f == -1)
	{
		perror("Error\n Erro ao abrir o arquivo");
		return (NULL);
	}
	map_data->mapa = NULL;
	ft_memset(map_data->flags, 0, sizeof(map_data->flags));
	//obs
	if (!map_data->f)
	{
		printf("Error\nErro ao abrir o mapa");
		return (NULL);
	}
	if (!processar_primeira_passagem(data, map_data))
	{
		close(map_data->f);
		return (NULL);
	}
	if (!ler_mapa1(arquivo, data, map_data))
		return (NULL);
	return (map_data->mapa);
}

int	ler_mapa1(char *arquivo, t_data *data, t_map_data *map_data)
{
	map_data->mapa = validar_e_alocar(data, map_data);
	if (!map_data->mapa)
	{
		close(map_data->f);
		return (0);
	}
	if (!processar_segunda_passagem(data, map_data, arquivo))
	{
		liberar_mapa(map_data->mapa, data->map_height);
		close(map_data->f);
		return (0);
	}
	if (data->map_height > 0)
	{
		data->map_width = strlen(map_data->mapa[0]);
	}
	printf("Mapa carregado com %d linhas e largura %d\n", data->map_height,
		data->map_width);
	close(map_data->f);
	return (1);
}

char	**validar_e_alocar(t_data *data, t_map_data *map_data)
{
	char	**mapa;

	if (map_data->config_count < 6)
	{
		printf("Configurações faltando\n");
		close(map_data->f);
		return (NULL);
	}
	if (!map_data->map_iniciado || data->map_height <= 0)
	{
		printf("Mapa inválido\n");
		close(map_data->f);
		return (NULL);
	}
	mapa = malloc(sizeof(char *) * (data->map_height + 1));
	if (!mapa)
	{
		printf("Error, Erro de alocação");
		close(map_data->f);
		return (NULL);
	}
	return (mapa);
}

void	liberar_mapa(char **mapa, int altura)
{
	int	i;

	i = 0;
	while (i < altura)
	{
		free(mapa[i]);
		i++;
	}
	free(mapa);
}
