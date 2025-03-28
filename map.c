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

char	**ler_mapa(char *arquivo, t_data *data)
{
	FILE *f = fopen(arquivo, "r");
	char **mapa = NULL;
	char linha[1024];
    int i = 0;
    int config_count = 0;
    int map_started = 0;
    // Flags para evitar duplicatas
    int flags[6] = {0}; // 0: NO, 1: SO, 2: WE, 3: EA, 4: F, 5: C

    data->map_height = 0;

    if (!f)
    {
        write(2, "Erro ao abrir o mapa\n", 22);
        return NULL;
    }
    printf("Abrindo arquivo: %s\n", arquivo);

    /* Primeira passagem: processa configurações e valida */
    while (fgets(linha, sizeof(linha), f))
    {
        linha[strcspn(linha, "\r\n")] = '\0'; // Remove \n e \r

        if (linha[0] == '\0') // Ignora linhas vazias
            continue;

        if (strncmp(linha, "NO ", 3) == 0)
        {
            if (flags[0]) { fprintf(stderr, "Erro: NO duplicado\n"); fclose(f); return NULL; }
            flags[0] = 1;
            data->texture_paths[0] = strdup(linha + 3);
            config_count++;
            continue;
        }
        else if (strncmp(linha, "SO ", 3) == 0)
        {
            if (flags[1]) { fprintf(stderr, "Erro: SO duplicado\n"); fclose(f); return NULL; }
            flags[1] = 1;
            data->texture_paths[1] = strdup(linha + 3);
            config_count++;
            continue;
        }
        else if (strncmp(linha, "WE ", 3) == 0)
        {
            if (flags[2]) { fprintf(stderr, "Erro: WE duplicado\n"); fclose(f); return NULL; }
            flags[2] = 1;
            data->texture_paths[2] = strdup(linha + 3);
            config_count++;
            continue;
        }
        else if (strncmp(linha, "EA ", 3) == 0)
        {
            if (flags[3]) { fprintf(stderr, "Erro: EA duplicado\n"); fclose(f); return NULL; }
            flags[3] = 1;
            data->texture_paths[3] = strdup(linha + 3);
            config_count++;
            continue;
        }
        else if (strncmp(linha, "F ", 2) == 0)
        {
            if (flags[4]) { fprintf(stderr, "Erro: F duplicado\n"); fclose(f); return NULL; }
            flags[4] = 1;
            int r, g, b;
            sscanf(linha + 2, "%d,%d,%d", &r, &g, &b);
            if (r < 0 || r > 255 || g < 0 || g > 255 || b < 0 || b > 255)
            {
                fprintf(stderr, "Erro: Cor F fora do intervalo (0-255)\n");
                fclose(f);
                return NULL;
            }
            data->floor_color = (r << 16) | (g << 8) | b;
            config_count++;
            continue;
        }
        else if (strncmp(linha, "C ", 2) == 0)
        {
            if (flags[5]) { fprintf(stderr, "Erro: C duplicado\n"); fclose(f); return NULL; }
            flags[5] = 1;
            int r, g, b;
            sscanf(linha + 2, "%d,%d,%d", &r, &g, &b);
            if (r < 0 || r > 255 || g < 0 || g > 255 || b < 0 || b > 255)
            {
                fprintf(stderr, "Erro: Cor C fora do intervalo (0-255)\n");
                fclose(f);
                return NULL;
            }
            data->ceiling_color = (r << 16) | (g << 8) | b;
            config_count++;
            continue;
        }

        // Se ainda não temos todas as configurações, não podemos começar o mapa
        if (config_count < 6)
        {
            fprintf(stderr, "Erro: Mapa encontrado antes de todas as configurações\n");
            fclose(f);
            return NULL;
        }

        // Linha é parte do mapa
        map_started = 1;
        data->map_height++;
    }

    if (config_count < 6)
    {
        fprintf(stderr, "Erro: Configurações incompletas no mapa\n");
        fclose(f);
        return NULL;
    }
    if (!map_started)
    {
        fprintf(stderr, "Erro: Mapa não encontrado\n");
        fclose(f);
        return NULL;
    }

    // Aloca o array para o mapa
    mapa = malloc(sizeof(char *) * (data->map_height + 1));
    if (!mapa)
    {
        write(2, "Erro ao alocar memória para o mapa\n", 34);
        fclose(f);
        return NULL;
    }

    // Segunda passagem: carrega o mapa
    fseek(f, 0, SEEK_SET);
    i = 0;
    while (fgets(linha, sizeof(linha), f))
    {
        linha[strcspn(linha, "\r\n")] = '\0';
        if (strncmp(linha, "NO ", 3) == 0 || strncmp(linha, "SO ", 3) == 0 ||
            strncmp(linha, "WE ", 3) == 0 || strncmp(linha, "EA ", 3) == 0 ||
            strncmp(linha, "F ", 2) == 0 || strncmp(linha, "C ", 2) == 0 ||
            linha[0] == '\0')
            continue;

        char *linha_corrigida = substituir_tabs(linha);
        if (!linha_corrigida)
        {
            write(2, "Erro ao processar linha do mapa\n", 32);
            fclose(f);
            for (int j = 0; j < i; j++)
                free(mapa[j]);
            free(mapa);
            return NULL;
        }
        printf("Linha carregada: %s\n", linha_corrigida);
        mapa[i++] = linha_corrigida;
    }
    mapa[i] = NULL;

    if (i > 0)
        data->map_width = strlen(mapa[0]);

    printf("Mapa alocado com %d linhas e largura %d\n", data->map_height, data->map_width);
    fclose(f);
    return mapa;
}
