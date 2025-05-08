/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   checar_passagens.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aborges <aborges@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 08:58:46 by aborges           #+#    #+#             */
/*   Updated: 2025/05/07 20:20:10 by aborges          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

size_t	ft_strspn(const char *s, const char *accept)
{
	size_t	i;
	size_t	j;
	int		found;

	i = 0;
	while (s[i])
	{
		j = 0;
		found = 0;
		while (accept[j])
		{
			if (s[i] == accept[j])
			{
				found = 1;
				break ;
			}
			j++;
		}
		if (!found)
			break ;
		i++;
	}
	return (i);
}

int	processar_segunda_passagem2(char *linha, int *var, t_map_data *map_data)
{
	if (ft_strlen(linha) == ft_strspn(linha, " "))
	{
		if ((var[2]) == 1)
		{
			while (read_line(map_data->f, linha, sizeof(linha)) > 0)
			{
				if (ft_strlen(linha) != ft_strspn(linha, " "))
				{
					write(2, "\nError! EXISTE linha vazia dentro do mapa\n",
						43);
					map_data->map_iniciado = 0;
					return (0);
				}
			}
		}
		return (-1);
	}
	else
		(var[2]) = 1;
	if (ft_strlen(linha) == ft_strspn(linha, " "))
		return (2);
	return (1);
}

int	nova_linha_corrigida(char *linha_corrigida, char *linha,
		t_map_data *map_data, int *var)
{
	int	result;

	linha_corrigida = ft_strdup(linha);
	if ((var[1]) == 0)
	{
		while (linha_corrigida[(var[1])])
		{
			if (linha_corrigida[(var[1])] == '0')
			{
				printf("Error\nPRIMEIRA LINHA COM ERRO\n");
				map_data->map_iniciado = 0;
				free_linha(linha_corrigida);
				return (0);
			}
			(var[1])++;
		}
	}
	result = processar_segunda_passagem3(linha_corrigida, map_data, var);
	if (result)
	{
		if (result == 0)
			return (0);
	}
	return (1);
}

int	processar_segunda_passagem3(char *linha_corrigida, t_map_data *map_data,
		int *var)
{
	int	k;

	k = 0;
	if (!linha_corrigida)
		return (write(2, "Error\nErro de processamento\n", 29),
			map_data->map_iniciado = 0, liberar_memoria(map_data), 0);
	while (linha_corrigida[k] == 32 || linha_corrigida[k] == 39)
		k++;
	if (linha_corrigida[k] == '0' || linha_corrigida[ft_strlen(linha_corrigida)
			- 1] == '0')
		return (map_data->map_iniciado = 0, free_linha(linha_corrigida),
			write(2, "Error\nINICIO OU FIM SEM PAREDE\n", 31), 0);
	map_data->mapa[var[0]++] = linha_corrigida;
	map_data->mapa[var[0]] = NULL;
	return (1);
}

int	processar_segunda_passagem4(t_data *data, char *linha_corrigida,
		t_map_data *map_data, int *var)
{
	int	j;

	j = 0;
	while (linha_corrigida[j])
	{
		if (linha_corrigida[j] == '0')
		{
			write(2, "Error\nExiste caminho aberto na ultima linha\n", 44);
			map_data->map_iniciado = 0;
			return (0);
		}
		j++;
	}
	map_data->mapa[(var[0])] = NULL;
	if ((var[0]) != data->map_height)
	{
		write(2, "Error\nInconsistência no mapa\n", 29);
		map_data->map_iniciado = 0;
		return (0);
	}
	return (1);
}
