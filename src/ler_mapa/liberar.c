/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   liberar.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lnzila <lnzila@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 23:56:10 by lnzila            #+#    #+#             */
/*   Updated: 2025/05/07 13:15:22 by lnzila           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	free_linha(char *str)
{
	if (!str)
		return ;
	free(str);
}

void	liberar_memoria(t_map_data *map_data)
{
	int	i;

	if (!map_data || !map_data->mapa)
		return ;
	i = 0;
	while (map_data->mapa[i])
	{
		free(map_data->mapa[i]);
		map_data->mapa[i] = NULL;
		i++;
	}
	free(map_data->mapa);
	map_data->mapa = NULL;
}
