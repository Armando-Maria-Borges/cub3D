/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   iniciar.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lnzila <lnzila@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/13 20:50:08 by aborges           #+#    #+#             */
/*   Updated: 2025/04/30 00:04:23 by lnzila           ###   ########.fr       */
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
	return (maior_linha);
}

/*void	liberar_matriz(char **mat)
{
	int	i;

	if (!mat)
		return ;
	i = 0;
	while (mat[i])
	{
		free(mat[i]);
		i++;
	}
	free(mat);
}*/

int	print_map(t_data *data)
{
	size_t	maior_linha;
	char	**nova;

	maior_linha = print_original_map(data);
	nova = criar_nova_matriz(data, maior_linha);
	substituir_espacos_por_x(nova);
	completar_linhas_com_x(nova, maior_linha);
	if (!validar_conexoes_mapa(nova))
		return (0);
	liberar_matriz(nova);
	return (1);
}

void	start_loop(t_data *data)
{
	mlx_loop(data->mlx);
}

void	init_data(t_data *data)
{
	ft_memset(data, 0, sizeof(t_data));
}
