/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tratar_tabs.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lnzila <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 10:07:06 by lnzila            #+#    #+#             */
/*   Updated: 2025/03/24 12:04:51 by lnzila           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

size_t	ft_strlen(const char *s)
{
	int	i;

	i = 0;
	while (s[i])
		i++;
	return (i);
}

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

char	*substituir_tabs(const char *linha)
{
	size_t	i;
	size_t	j;
	size_t	len;
	int		k;
	char	*nova_linha;

	len = ft_strlen(linha);
	i = 0;
	j = 0;
	nova_linha = malloc(sizeof(char) * (len + (TAB_SIZE - 1)
				* contar_tabs(linha) + 1));
	if (!nova_linha)
		return (NULL);
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
