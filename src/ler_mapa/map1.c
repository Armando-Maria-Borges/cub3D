/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map1.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aborges <aborges@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 15:15:59 by aborges           #+#    #+#             */
/*   Updated: 2025/04/30 13:21:54 by aborges          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

int	ft_chao(t_data *data, t_map_data *map_data, char *linha)
{
	int	r;
	int	g;
	int	b;

	if (ft_strncmp(linha, "F ", 2) == 0)
	{
		if (map_data->flags[4]++)
		{
			printf("F duplicado\n");
			return (-1);
		}
		if (!verifica_linha_cor(linha + 2, &r, &g, &b))
		{
			printf("Cor F inválida\n");
			return (-1);
		}
		data->floor_color = (r << 16) | (g << 8) | b;
	}
	else
		return (0);
	return (1);
}

int	ft_teto(t_data *data, t_map_data *map_data, char *linha)
{
	int	r;
	int	g;
	int	b;

	if (ft_strncmp(linha, "C ", 2) == 0)
	{
		if (map_data->flags[5]++)
		{
			printf("C duplicado\n");
			return (-1);
		}
		if (!verifica_linha_cor(linha + 2, &r, &g, &b))
		{
			printf("Cor F inválida\n");
			return (-1);
		}
		data->ceiling_color = (r << 16) | (g << 8) | b;
	}
	else
		return (0);
	return (1);
}

int	chekagem_map_concluido(t_map_data *map_data, t_data *data)
{
	if (map_data->config_count < 6)
	{
		printf("Error\nMapa prematuro\n");
		return (0);
	}
	(map_data->map_iniciado) = 1;
	data->map_height++;
	return (1);
}

int	read_line(int fd, char *linha, int max_len)
{
	int		i;
	char	c;
	int		bytes_read;

	i = 0;
	bytes_read = read(fd, &c, 1);
	while (bytes_read > 0)
	{
		if (c == '\n' || i >= max_len - 1)
			break ;
		linha[i++] = c;
		bytes_read = read(fd, &c, 1);
	}
	linha[i] = '\0';
	return (bytes_read > 0 || i > 0);
}

/*
void	free_textures(char **texture)
{
	int i;

	if (!texture)
		return ;
	i = 0;
	while (i < 4)
	{
		free(texture[i]);
		i++;
	}
}
	*/

int	processar_primeira_passagem(t_data *data, t_map_data *map_data)
{
	char	linha[1024];
	int		return_cordenadas;

	(map_data->config_count) = 0;
	while (read_line(map_data->f, linha, 1024))
	{
		linha[ft_strcspn(linha, "\r\n")] = '\0';
		if (linha[0] == '\0')
			continue ;
		return_cordenadas = ft_return_cordenadas(data, map_data, linha);
		if (return_cordenadas)
		{
			if (return_cordenadas == -1)
			{
				//liberar_mapa(data->texture_paths, ft_count(data->texture_paths));
				return (0);
			}
		}
		else
		{
			if (chekagem_map_concluido(map_data, data) == 0)
			{
				//liberar_mapa(data->texture_paths, ft_count(data->texture_paths));
				return (0);
			}
		}
		(map_data->config_count)++;
	}
	//liberar_mapa(data->texture_paths, ft_count(data->texture_paths));
	return (1);
}
