/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mapa.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lnzila <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 08:32:29 by lnzila            #+#    #+#             */
/*   Updated: 2025/03/25 08:36:23 by lnzila           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

int	contar_linhas_mapa(int fd, t_data *data, int *config_count)
{
	char	linha[1024];
	int		bytes;
	int		result;

	result = 1;
	data->map_height = 0;
	bytes = ft_read_line(fd, linha, 1024);
	while (bytes > 0 && result)
	{
		ft_trim_newline(linha);
		result = validar_config(linha, data, config_count);
		if (!result && *config_count >= 6 && linha[0] != '\0')
		{
			data->map_height++;
			result = 1;
		}
		bytes = ft_read_line(fd, linha, 1024);
	}
	if (bytes < 0)
		return (0);
	return (result);
}

char	**alocar_mapa(int height)
{
	char	**mapa;

	mapa = malloc(sizeof(char *) * (height + 1));
	if (!mapa)
		write(2, "Erro ao alocar memória para o mapa\n", 34);
	return (mapa);
}

int	eh_config_ou_vazio(char *linha)
{
	int	resultado;

	resultado = 0;
	if (ft_strncmp(linha, "NO ", 3) == 0)
		resultado = 1;
	else if (ft_strncmp(linha, "SO ", 3) == 0)
		resultado = 1;
	else if (ft_strncmp(linha, "WE ", 3) == 0)
		resultado = 1;
	else if (ft_strncmp(linha, "EA ", 3) == 0)
		resultado = 1;
	else if (ft_strncmp(linha, "F ", 2) == 0)
		resultado = 1;
	else if (ft_strncmp(linha, "C ", 2) == 0)
		resultado = 1;
	else if (linha[0] == '\0')
		resultado = 1;
	return (resultado);
}

int	carregar_linhas_mapa(int fd, char **mapa, int height)
{
	char	linha[1024];
	int		i;
	int		bytes;

	i = 0;
	bytes = ft_read_line(fd, linha, 1024);
	while (bytes > 0 && i < height)
	{
		ft_trim_newline(linha);
		if (eh_config_ou_vazio(linha))
			bytes = ft_read_line(fd, linha, 1024);
		else
		{
			mapa[i] = substituir_tabs(linha);
			if (!mapa[i])
				return (0);
			i++;
			bytes = ft_read_line(fd, linha, 1024);
		}
	}
	mapa[i] = NULL;
	if (bytes < 0)
		return (0);
	if (i != height)
		return (0);
	return (1);
}
