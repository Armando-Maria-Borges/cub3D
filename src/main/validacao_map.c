/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validacao_map.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aborges <aborges@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 07:29:35 by aborges           #+#    #+#             */
/*   Updated: 2025/04/25 23:10:40 by aborges          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

char	**criar_nova_matriz(t_data *data, size_t maior_linha)
{
	int		y;
	char	**nova;

	nova = malloc(sizeof(char *) * (data->map_height + 1));
	if (!nova)
		return (NULL);
	y = 0;
	while (data->mapa[y])
	{
		nova[y] = malloc(sizeof(char) * (maior_linha + 1));
		ft_strlcpy(nova[y], data->mapa[y], maior_linha + 1);
		y++;
	}
	nova[y] = NULL;
	return (nova);
}

void	substituir_espacos_por_x(char **nova)
{
	int	y;
	int	i;

	y = 0;
	while (nova[y])
	{
		i = 0;
		while (nova[y][i])
		{
			if (nova[y][i] == ' ' || nova[y][i] == 9)
				nova[y][i] = 'x';
			i++;
		}
		y++;
	}
}

void	completar_linhas_com_x(char **nova, size_t maior_linha)
{
	int		y;
	size_t	k;

	y = 0;
	while (nova[y])
	{
		k = ft_strlen(nova[y]);
		while (k < maior_linha)
		{
			nova[y][k] = 'x';
			k++;
		}
		nova[y][k] = '\0';
		y++;
	}
}

int	validar_conexoes_mapa(char **nova)
{
	int	y;
	int	i;

	y = 0;
	while (nova[y + 1])
	{
		i = 0;
		while (nova[y][i])
		{
			if ((nova[y][i] == 'x' && nova[y + 1][i] == '0')
				|| (nova[y][i] == '0' && nova[y + 1][i] == 'x')
				|| (nova[y][i] == '0' && nova[y][i + 1] == 'x')
				|| (nova[y][i] == 'x' && nova[y][i + 1] == '0'))
			{
				printf("\n\nError\n-->Deve existir alguma ligacao entre 0 e x\n");
				return (0);
			}
			i++;
		}
		y++;
	}
	return (1);
}

void	print_nova_matriz(char **nova)
{
	int	y;

	y = 0;
	printf("\nNOVA MATRIZ\n");
	while (nova[y])
	{
		printf("%s\n", nova[y]);
		y++;
	}
}
