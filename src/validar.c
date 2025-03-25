/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validar.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lnzila <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 08:40:35 by lnzila            #+#    #+#             */
/*   Updated: 2025/03/25 08:41:37 by lnzila           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

int	parse_color(const char *str, int *r, int *g, int *b)
{
	int		values[3] = {0};
	int		i;
	char	*token;
	char	*copy;

	i = 0;
	copy = ft_strdup(str);
	if (!copy)
		return (0);
	token = strtok(copy, ",");
	while (token && i < 3)
	{
		values[i] = ft_atoi(token);
		if (values[i] < 0 || values[i] > 255)
		{
			free(copy);
			return (0);
		}
		i++;
		token = strtok(NULL, ",");
	}
	free(copy);
	if (i != 3)
		return (0);
	*r = values[0];
	*g = values[1];
	*b = values[2];
	return (1);
}

int	validar_texturas(char *linha, t_data *data, int *config_count)
{
	if (ft_strncmp(linha, "NO ", 3) == 0 && !data->texture_paths[0])
	{
		data->texture_paths[0] = ft_strdup(linha + 3);
		(*config_count)++;
		return (1);
	}
	if (ft_strncmp(linha, "SO ", 3) == 0 && !data->texture_paths[1])
	{
		data->texture_paths[1] = ft_strdup(linha + 3);
		(*config_count)++;
		return (1);
	}
	if (ft_strncmp(linha, "WE ", 3) == 0 && !data->texture_paths[2])
	{
		data->texture_paths[2] = ft_strdup(linha + 3);
		(*config_count)++;
		return (1);
	}
	if (ft_strncmp(linha, "EA ", 3) == 0 && !data->texture_paths[3])
	{
		data->texture_paths[3] = ft_strdup(linha + 3);
		(*config_count)++;
		return (1);
	}
	return (0);
}

int	validar_cores(char *linha, t_data *data, int *config_count)
{
	int	r;
	int	g;
	int	b;

	if (ft_strncmp(linha, "F ", 2) == 0 && !data->floor_color)
	{
		if (!parse_color(linha + 2, &r, &g, &b))
			return (0);
		data->floor_color = (r << 16) | (g << 8) | b;
		(*config_count)++;
		return (1);
	}
	if (ft_strncmp(linha, "C ", 2) == 0 && !data->ceiling_color)
	{
		if (!parse_color(linha + 2, &r, &g, &b))
			return (0);
		data->ceiling_color = (r << 16) | (g << 8) | b;
		(*config_count)++;
		return (1);
	}
	return (0);
}
