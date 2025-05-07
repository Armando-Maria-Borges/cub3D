/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validar_args.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lnzila <lnzila@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 18:20:05 by lnzila            #+#    #+#             */
/*   Updated: 2025/05/07 18:22:44 by lnzila           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

static int	ft_strcmp(const char *s1, const char *s2)
{
	int	i;

	i = 0;
	while (s1[i] != '\0' && s1[i] == s2[i])
		i++;
	if (s1[i] == s2[i])
		return (0);
	else if (s1[i] > s2[i])
		return (1);
	else
		return (-1);
	
}

int	verificar_extensao_cub(const char *nome_arquivo)
{
	const char	*extensao;
	size_t		len_nome;
	size_t		len_extensao;

	extensao = ".cub";
	len_nome = ft_strlen(nome_arquivo);
	len_extensao = ft_strlen(extensao);
	if (len_nome < len_extensao)
		return (1);
	if (ft_strcmp(nome_arquivo + len_nome - len_extensao, extensao) == 0)
		return (0);
	else
		printf("Error\nMapa '%s' não tem a extensão '.cub'.\n", nome_arquivo);
	return (1);
}

int	check_args(int ac, char **av)
{
	(void)av;
	if (ac != 2)
	{
		printf("Error\nUsage: ./cub3d <mapa>\n");
		return (1);
	}
	if (verificar_extensao_cub(av[1]) != 0)
		return (1);
	return (0);
}