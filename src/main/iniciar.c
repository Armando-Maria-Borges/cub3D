/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   iniciar.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aborges <aborges@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/13 20:50:08 by aborges           #+#    #+#             */
/*   Updated: 2025/04/14 07:31:27 by aborges          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

int	init_mlx(t_data *data)
{
	data->mlx = mlx_init();
	if (!data->mlx)
	{
		printf("Error!\nAo iniciar o MLX\n");
		return (1);
	}
	return (0);
}

size_t	print_original_map(t_data *data)
{
	int		y;
	size_t	maior_linha;

	y = 0;
	maior_linha = 0;
	printf("Mapa carregado:\n");
	while (y < data->map_height)
	{
		printf("%s\n", data->mapa[y]);
		if (maior_linha < ft_strlen(data->mapa[y]))
			maior_linha = ft_strlen(data->mapa[y]);
		y++;
	}
	printf ("\n\n->Maior linha: %ld\n", maior_linha);
	return (maior_linha);
}

int	print_map(t_data *data)
{
	size_t	maior_linha;
	char	**nova;

	maior_linha = print_original_map(data);
	nova = criar_nova_matriz(data, maior_linha);
	substituir_espacos_por_x(nova);
	completar_linhas_com_x(nova, maior_linha);
	print_nova_matriz(nova);
	if (!validar_conexoes_mapa(nova))
		return (0);
	return (1);
}

/*
int	print_map(t_data *data)
{
	size_t	maior_linha = 0;

	int	y;

	printf("Mapa carregado:\n");
	y = 0;
	while (y < data->map_height)
	{
		if (maior_linha < ft_strlen(data->mapa[y]))
			maior_linha = ft_strlen(data->mapa[y]);

		printf("%s\n", data->mapa[y]);
		y++;
	}


	printf ("\n\n->Maior linha: %ld\n", maior_linha);
	char **nova_matriz = malloc(sizeof(char *) * (data->map_height + 1));
	y = 0;
	while (data->mapa[y])
	{
		nova_matriz[y] = malloc(sizeof(char) * (maior_linha + 1));
		ft_strlcpy(nova_matriz[y], data->mapa[y], maior_linha + 1);
		y++;
	}
	nova_matriz[y] = NULL;
	



	int i;
	y = 0;
	while (nova_matriz[y])
	{
		i = 0;
		while (nova_matriz[y][i])
		{
			if (nova_matriz[y][i] == 32)
			{
				nova_matriz[y][i] = 'x';
			}
			i++;
		}
		y++;
	}

	y = 0;
	while (nova_matriz[y])
	{
		size_t k = ft_strlen(nova_matriz[y]);
		while (k < maior_linha)
		{
			nova_matriz[y][k] = 'x';
			k++;
		}
		nova_matriz[y][k] = '\0';
		y++;
	}
	
	
	printf("\nNOVA MATRIZ\n");
	y = 0;
	while (nova_matriz[y])
	{
		printf("%s\n", nova_matriz[y]);
		y++;
	}

	y = 0;
	while (nova_matriz[y + 1])
	{
		i = 0;
		while (nova_matriz[y][i])
		{
			if ((nova_matriz[y][i] == 'x' && nova_matriz[y + 1][i] == '0') 
					|| (nova_matriz[y][i] == '0' && nova_matriz[y + 1][i] == 'x')
					|| (nova_matriz[y][i] == '0' && nova_matriz[y][i + 1] == 'x')
					|| (nova_matriz[y][i] == 'x' && nova_matriz[y][i + 1] == '0'))
			{
				printf("\n\nError\n--------->Deve existir alguma ligacao entre 0 e x\n");
				return (0);
			}
			i++;
		}
		y++;
	}
	return (1);
}
*/

void	start_loop(t_data *data)
{
	mlx_loop(data->mlx);
}

void	init_data(t_data *data)
{
	ft_memset(data, 0, sizeof(t_data));
}
