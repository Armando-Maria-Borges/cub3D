/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tratar_tabs.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aborges <aborges@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 10:07:06 by lnzila            #+#    #+#             */
/*   Updated: 2025/05/06 09:06:22 by aborges          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

static int	contar_tabs(const char *linha)
{
	int		tab_count;
	size_t	i;

	tab_count = 0;
	i = 0;
	while (i < ft_strlen(linha))
	{
		if (linha[i] == '\t')
			tab_count++;
		i++;
	}
	return (tab_count);
}

char	*substituir_tabs_aux(const char *linha, char *nova_linha)
{
	int		k;
	size_t	i;
	size_t	j;

	i = 0;
	j = 0;
	while (linha[i])
	{
		if (linha[i] == '\t')
		{
			k = 0;
			while (k < TAB_SIZE)
			{
				nova_linha[j++] = ' ';
				k++;
			}
		}
		else
			nova_linha[j++] = linha[i];
		i++;
	}
	nova_linha[j] = '\0';
	return (nova_linha);
}

char	*substituir_tabs(const char *linha)
{
	size_t	len;
	char	*nova_linha;

	len = ft_strlen(linha);
	nova_linha = malloc(sizeof(char) * (len + (TAB_SIZE - 1)
				* contar_tabs(linha) + 1));
	if (!nova_linha)
		return (NULL);
	return (substituir_tabs_aux(linha, nova_linha));
}


int	ft_strcount(char **mat)
{
	int	i;

	if (!mat)
		return (0);
	i = 0;
	while (mat[i])
		i++;
	return (i);
}

void	liberar_tudo(t_data *data)
{
	//mlx_destroy_window(data->mlx, data->win);
	//mlx_destroy_display(data->mlx);
	free(data->mlx);
	liberar_mapa(data->mapa, ft_strcount(data->mapa));
	free_texturas(data);
	exit(0);
}