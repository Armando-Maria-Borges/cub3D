/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lnzila <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 09:38:18 by lnzila            #+#    #+#             */
/*   Updated: 2025/03/18 09:38:23 by lnzila           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

char **ler_mapa(char *arquivo, t_data *data)
{
    t_map_data map_data_struct;
    t_map_data *map_data;
    
    map_data = &map_data_struct;  // Agora map_data aponta para uma estrutura válida
    
    map_data->f = fopen(arquivo, "r");  // Agora é seguro acessar seus membros
    map_data->mapa = NULL;
    ft_memset(map_data->flags, 0, sizeof(map_data->flags));

    if (!map_data->f) {
        write(2, "Erro ao abrir o mapa\n", 22);
        return NULL;
    }
    if (!processar_primeira_passagem(data, map_data))
    {
        fclose(map_data->f);
        return NULL;
    }
    if (!(map_data->mapa = validar_e_alocar(data, map_data)))
    {
        fclose(map_data->f);
        return NULL;
    }
    if (!processar_segunda_passagem(data, map_data))
    {
        liberar_mapa(map_data->mapa, data->map_height);
        fclose(map_data->f);
        return NULL;
    }
    if (data->map_height > 0)
    {
        data->map_width = strlen(map_data->mapa[0]);
    }
    printf("Mapa carregado com %d linhas e largura %d\n", data->map_height, data->map_width);
    fclose(map_data->f);
    return (map_data->mapa);
}

char **validar_e_alocar(t_data *data, t_map_data *map_data)
{
    if (map_data->config_count < 6)
    {
        fprintf(stderr, "Configurações faltando\n");
        fclose(map_data->f);
        return NULL;
    }
    if (!map_data->map_iniciado || data->map_height <= 0)
    {
        fprintf(stderr, "Mapa inválido\n");
        fclose(map_data->f);
        return NULL;
    }
    
    char **mapa = malloc(sizeof(char *) * (data->map_height + 1));
    if (!mapa)
    {
        write(2, "Erro de alocação\n", 17);
        fclose(map_data->f);
        return NULL;
    }
    return (mapa);
}
void liberar_mapa(char **mapa, int altura)
{
    int i;

    i = 0;
    while (i < altura)
    {
        free(mapa[i]);
        i++;
    }
    free(mapa);
}
/*
void liberar_mapa(char **mapa, int altura)
{
    for (int i = 0; i < altura; i++) 
        free(mapa[i]);
    free(mapa);
}
*/
