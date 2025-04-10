/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cordenadas.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aborges <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 08:43:10 by aborges           #+#    #+#             */
/*   Updated: 2025/04/10 08:43:13 by aborges          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

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
