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

#include "../includes/cub3d.h"

// Função auxiliar 1 (23 linhas)
int processar_primeira_passagem(FILE *f, t_data *data, int *config_count, int *map_iniciado, int flags[6])
{
    char linha[1024];

    // Loop para ler cada linha do arquivo
    while (fgets(linha, sizeof(linha), f)) // Corrigido: adicionado o parêntese final
    {
        // Remover caracteres de nova linha (\r\n)
        linha[strcspn(linha, "\r\n")] = '\0';

        // Ignorar linhas vazias
        if (linha[0] == '\0') continue;

        // Processar configurações
        if (strncmp(linha, "NO ", 3) == 0) {
            if (flags[0]++) { fprintf(stderr, "NO duplicado\n"); return 0; }
            data->texture_paths[0] = strdup(linha + 3);
        }
        else if (strncmp(linha, "SO ", 3) == 0) {
            if (flags[1]++) { fprintf(stderr, "SO duplicado\n"); return 0; }
            data->texture_paths[1] = strdup(linha + 3);
        }
        else if (strncmp(linha, "WE ", 3) == 0) {
            if (flags[2]++) { fprintf(stderr, "WE duplicado\n"); return 0; }
            data->texture_paths[2] = strdup(linha + 3);
        }
        else if (strncmp(linha, "EA ", 3) == 0) {
            if (flags[3]++) { fprintf(stderr, "EA duplicado\n"); return 0; }
            data->texture_paths[3] = strdup(linha + 3);
        }
        else if (strncmp(linha, "F ", 2) == 0) {
            if (flags[4]++) { fprintf(stderr, "F duplicado\n"); return 0; }
            int r, g, b;
            if (sscanf(linha + 2, "%d,%d,%d", &r, &g, &b) != 3 || r < 0 || r > 255 || g < 0 || g > 255 || b < 0 || b > 255) {
                fprintf(stderr, "Cor F inválida\n"); return 0;
            }
            data->floor_color = (r << 14) | (g << 6) | b;
        }
        else if (strncmp(linha, "C ", 2) == 0) {
            if (flags[5]++) { fprintf(stderr, "C duplicado\n"); return 0; }
            int r, g, b;
            if (sscanf(linha + 2, "%d,%d,%d", &r, &g, &b) != 3 || r < 0 || r > 255 || g < 0 || g > 255 || b < 0 || b > 255) {
                fprintf(stderr, "Cor C inválida\n"); return 0;
            }
            data->ceiling_color = (r << 16) | (g << 8) | b;
        }
        else {
            // Verificar se todas as configurações foram processadas antes de iniciar o mapa
            if (*config_count < 6) {
                fprintf(stderr, "Mapa incompleto\n"); return 0;
            }
            (*map_iniciado) = 1;
            data->map_height++;
        }
        (*config_count)++;
    }

    // Retorno obrigatório para evitar warnings
    return 1;
}



