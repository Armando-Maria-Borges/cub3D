/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ler_mapa.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lnzila <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 14:53:54 by lnzila            #+#    #+#             */
/*   Updated: 2025/03/24 14:54:56 by lnzila           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

int	validar_config(char *linha, t_data *data, int *config_count)
{
	int	resultado;

	resultado = validar_texturas(linha, data, config_count);
	if (resultado)
		return (1);
	resultado = validar_cores(linha, data, config_count);
	if (resultado)
		return (1);
	if (linha[0] != '\0')
		return (0);
	return (1);
}

static int	validar_e_contar(char *arquivo, t_data *data, int *config_count)
{
	int fd1;

	*config_count = 0;
	fd1 = open(arquivo, O_RDONLY);
	if (fd1 < 0)
	{
		write(2, "Erro ao abrir o mapa\n", 22);
		return (0);
	}
	if (!contar_linhas_mapa(fd1, data, config_count) || *config_count < 6)
	{
		write(2, "Erro: Configurações incompletas ou duplicadas\n", 46);
		close(fd1);
		return (0);
	}
	close(fd1);
	return (1);
}

static char	**carregar_mapa(char *arquivo, t_data *data)
{
	int		fd2;
	int		j;
	char	**mapa;

	mapa = alocar_mapa(data->map_height);
	if (!mapa)
		return (NULL);
	fd2 = open(arquivo, O_RDONLY);
	if (fd2 < 0)
	{
		free(mapa);
		return (NULL);
	}
	if (!carregar_linhas_mapa(fd2, mapa, data->map_height))
	{
		close(fd2);
		j = 0;
		while (mapa[j])
			free(mapa[j++]);
		free(mapa);
		return (NULL);
	}
	close(fd2);
	if (data->map_height > 0)
		data->map_width = ft_strlen(mapa[0]);
	return (mapa);
}

char	**ler_mapa(char *arquivo, t_data *data)
{
	int		config_count;
	char	**mapa;

	if (!validar_e_contar(arquivo, data, &config_count))
		return (NULL);
	mapa = carregar_mapa(arquivo, data);
	return (mapa);
}
