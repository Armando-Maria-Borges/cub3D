/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   checar_passagens.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aborges <aborges@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 08:58:46 by aborges           #+#    #+#             */
/*   Updated: 2025/04/28 12:05:59 by aborges          ###   ########.fr       */
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
					printf("\nError! EXISTE linha vazia dentro do mapa\n");
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
				printf("Error\n EXISTE CAMINHO ABERTO NA PRIMEIRA LINHA\n");
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
	int	j;

	k = 0;
	while (linha_corrigida[k] == 32 || linha_corrigida[k] == 39)
		k++;
	if (linha_corrigida[k] == '0' || linha_corrigida[ft_strlen(linha_corrigida)
			- 1] == '0')
		return (printf("Error\n INICIO OU FIM SEM PAREDE\n"), 0);
	if (!linha_corrigida)
	{
		printf("Error\n Erro de processamento\n");
		j = 0;
		while (j < (var[0]))
		{
			free(map_data->mapa[j]);
			j++;
		}
		return (0);
	}
	map_data->mapa[(var[0])++] = linha_corrigida;
	map_data->mapa[(var[0])] = '\0';
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
			printf("Error\nEXISTE CAMINHO ABERTO NA ULTIMA LINHA\n");
			return (0);
		}
		j++;
	}
	map_data->mapa[(var[0])] = NULL;
	if ((var[0]) != data->map_height)
	{
		printf("Error\nInconsistência no mapa\n");
		return (0);
	}
	return (1);
}
