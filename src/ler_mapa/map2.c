/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map2.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lnzila <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 10:09:00 by lnzila            #+#    #+#             */
/*   Updated: 2025/03/18 10:09:03 by lnzila           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

int	processar_segunda_passagem1(char *linha, int *var)
{
	if ((var[3]) != 6 && (strncmp(linha, "NO ", 3) == 0 || strncmp(linha, "SO ",
				3) == 0 || strncmp(linha, "WE ", 3) == 0 || strncmp(linha,
				"EA ", 3) == 0 || strncmp(linha, "F ", 2) == 0 || strncmp(linha,
				"C ", 2) == 0 || strlen(linha) == strspn(linha, " ")))
	{
		if (strlen(linha) == strspn(linha, " "))
			return (-1);
		(var[3])++;
		if ((var[3]) > 6)
		{
			printf("\nError! EXISTE elementos a mais\n");
			return (0);
		}
		return (-1);
	}
	else if ((var[3]) != 6)
	{
		printf("\nError! EXISTE elemento invalido, %s\n", linha);
		return (0);
	}
	return (1);
}

int	processar_segunda(char *linha, int *var, t_map_data *map_data)
{
	int	result;
	int	result2;

	result = processar_segunda_passagem1(linha, var);
	if (result)
	{
		if (result == -1)
			return (-1);
		else if (result == 0)
			return (0);
	}
	result2 = processar_segunda_passagem2(linha, var, map_data);
	if (result2)
	{
		if (result2 == 2)
			return (2);
		else if (result2 == -1)
			return (-1);
		else if (result2 == 0)
			return (0);
	}
	return (1);
}

int	abrir_arquivo(t_map_data *map_data, char *arquivo)
{
	if (!map_data->f)
	{
		printf("Error\nArquivo ja tinha que estar aberto.\n");
		return (1);
	}
	close(map_data->f);
	map_data->f = open(arquivo, O_RDONLY);
	return (0);
}

int	segunda_passagem_ciclo(t_map_data *map_data, int *var, char *linha,
		char *linha_corrigida)
{
	int	result;
	int	result1;

	if (linha[strlen(linha) - 1] == '\n')
		linha[strlen(linha) - 1] = '\0';
	result = processar_segunda(linha, var, map_data);
	if (result)
	{
		if (result == 2)
			return (2);
		else if (result == -1)
			return (-1);
		else if (result == 0)
			return (0);
	}
	linha_corrigida = ft_strdup(linha);
	result1 = nova_linha_corrigida(linha_corrigida, linha, map_data, var);
	if (result1)
	{
		if (result1 == 0)
			return (0);
	}
	return (1);
}

int	processar_segunda_passagem(t_data *data, t_map_data *map_data,
		char *arquivo)
{
	char	linha[1024];
	char	*linha_corrigida;
	int		var[4];
	int		result;

	ft_memset(var, 0, sizeof(var));
	if (abrir_arquivo(map_data, arquivo))
		return (1);
	while (read_line(map_data->f, linha, sizeof(linha)))
	{
		linha_corrigida = ft_strdup(linha);
		result = segunda_passagem_ciclo(map_data, var, linha, linha_corrigida);
		if (result == 2)
			break ;
		else if (result == -1)
			continue ;
		else if (result == 0)
			return (0);
	}
	result = processar_segunda_passagem4(data, linha_corrigida, map_data, var);
	if (result == 0)
		return (0);
	free(linha_corrigida);
	return (1);
}
