/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map1.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aborges <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 15:15:59 by aborges           #+#    #+#             */
/*   Updated: 2025/04/01 15:16:00 by aborges          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

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

int	we_ea(t_data *data, t_map_data *map_data, char *linha)
{
	if (ft_strncmp(linha, "WE ", 3) == 0)
	{
		if (map_data->flags[2]++)
		{
			printf("WE duplicado\n");
			return (-1);
		}
		data->texture_paths[2] = strdup(linha + 3);
	}
	else if (ft_strncmp(linha, "EA ", 3) == 0)
	{
		if (map_data->flags[3]++)
		{
			printf("EA duplicado\n");
			return (-1);
		}
		data->texture_paths[3] = strdup(linha + 3);
	}
	else
		return (0);
	return (1);
}

int	no_so(t_data *data, t_map_data *map_data, char *linha)
{
	if (ft_strncmp(linha, "NO ", 3) == 0)
	{
		if (map_data->flags[0]++)
		{
			printf("NO duplicado\n");
			return (-1);
		}
		data->texture_paths[0] = strdup(linha + 3);
	}
	else if (ft_strncmp(linha, "SO ", 3) == 0)
	{
		if (map_data->flags[1]++)
		{
			printf("SO duplicado\n");
			return (-1);
		}
		data->texture_paths[1] = strdup(linha + 3);
	}
	else
		return (0);
	return (1);
}

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

int	ft_processar_cordenadas(int *valor_return)
{
	if (valor_return[0])
	{
		if (valor_return[0] == -1)
			return (-1);
	}
	else if (valor_return[1])
	{
		if (valor_return[1] == -1)
			return (-1);
	}
	else if (valor_return[2])
	{
		if (valor_return[2] == -1)
			return (-1);
	}
	else if (valor_return[3])
	{
		if (valor_return[3] == -1)
			return (-1);
	}
	else
		return (0);
	return (1);
}

int	ft_return_cordenadas(t_data *data, t_map_data *map_data, char *linha)
{
	int	valor_return[5];

	valor_return[0] = no_so(data, map_data, linha);
	valor_return[1] = we_ea(data, map_data, linha);
	valor_return[2] = ft_chao(data, map_data, linha);
	valor_return[3] = ft_teto(data, map_data, linha);
	return (ft_processar_cordenadas(valor_return));
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

int	processar_primeira_passagem(t_data *data, t_map_data *map_data)
{
	char	linha[1024];
	int		return_cordenadas;

	while (read_line(map_data->f, linha, 1024))
	{
		linha[ft_strcspn(linha, "\r\n")] = '\0';
		if (linha[0] == '\0')
			continue ;
		return_cordenadas = ft_return_cordenadas(data, map_data, linha);
		if (return_cordenadas)
		{
			if (return_cordenadas == -1)
				return (0);
		}
		else
		{
			if (chekagem_map_concluido(map_data, data) == 0)
				return (0);
		}
		(map_data->config_count)++;
	}
	return (1);
}
