/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cordenadas.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lnzila <lnzila@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 08:43:10 by aborges           #+#    #+#             */
/*   Updated: 2025/04/28 14:41:04 by lnzila           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

int	we_ea(t_data *data, t_map_data *map_data, char *linha)
{
	if (ft_strncmp(linha, "WE ", 3) == 0)
	{
		if (map_data->flags[2]++)
		{
			write(2, "WE duplicado\n", 13);
			return (-1);
		}
		data->texture_paths[2] = ft_strdup(linha + 3);
	}
	else if (ft_strncmp(linha, "EA ", 3) == 0)
	{
		if (map_data->flags[3]++)
		{
			write(2, "EA duplicado\n", 13);
			return (-1);
		}
		data->texture_paths[3] = ft_strdup(linha + 3);
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
			write(2, "NO duplicado\n", 13);
			return (-1);
		}
		data->texture_paths[0] = ft_strdup(linha + 3);
	}
	else if (ft_strncmp(linha, "SO ", 3) == 0)
	{
		if (map_data->flags[1]++)
		{
			write(2, "SO duplicado\n", 13);
			return (-1);
		}
		data->texture_paths[1] = ft_strdup(linha + 3);
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
