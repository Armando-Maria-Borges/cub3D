/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map1.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aborges <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 15:15:59 by aborges           #+#    #+#             */
/*   Updated: 2025/04/01 15:16:00 by aborges          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

int	read_line(int fd, char *linha, int max_len)
{
	int		i = 0;
	char	c;
	int		bytes_read;

	while ((bytes_read = read(fd, &c, 1)) > 0)
	{
		if (c == '\n' || i >= max_len - 1) // Se encontrar \n ou atingir o limite
			break;
		linha[i++] = c;
	}
	linha[i] = '\0'; // Finaliza a string

	return (bytes_read > 0 || i > 0); // Retorna 1 se leu algo, 0 se chegou ao fim
}

// Função auxiliar 1 (23 linhas)
int processar_primeira_passagem(t_data *data, t_map_data *map_data)
{
    char linha[1024];

    // Loop para ler cada linha do arquivo
    //while (fgets(linha, sizeof(linha), map_data->f)) // Corrigido: adicionado o parêntese final
    while (read_line(map_data->f, linha, 1024))
    {
        // Remover caracteres de nova linha (\r\n)
        linha[strcspn(linha, "\r\n")] = '\0';

        // Ignorar linhas vazias
        if (linha[0] == '\0') continue;

        // Processar configurações
        if (strncmp(linha, "NO ", 3) == 0)
        {
            if (map_data->flags[0]++)
            { 
                fprintf(stderr, "NO duplicado\n"); return 0;
            }
            data->texture_paths[0] = strdup(linha + 3);
        }
        else if (strncmp(linha, "SO ", 3) == 0)
        {
            if (map_data->flags[1]++)
            {
                fprintf(stderr, "SO duplicado\n"); return 0;
            }
            data->texture_paths[1] = strdup(linha + 3);
        }
        else if (strncmp(linha, "WE ", 3) == 0) {
            if (map_data->flags[2]++) { fprintf(stderr, "WE duplicado\n"); return 0; }
            data->texture_paths[2] = strdup(linha + 3);
        }
        else if (strncmp(linha, "EA ", 3) == 0) {
            if (map_data->flags[3]++) { fprintf(stderr, "EA duplicado\n"); return 0; }
            data->texture_paths[3] = strdup(linha + 3);
        }
        else if (strncmp(linha, "F ", 2) == 0) {
            if (map_data->flags[4]++) { fprintf(stderr, "F duplicado\n"); return 0; }
            int r, g, b;
            if (sscanf(linha + 2, "%d,%d,%d", &r, &g, &b) != 3 || r < 0 || r > 255 || g < 0 || g > 255 || b < 0 || b > 255) {
                fprintf(stderr, "Cor F inválida\n"); return 0;
            }
            data->floor_color = (r << 16) | (g << 8) | b;
        }
        else if (strncmp(linha, "C ", 2) == 0) {
            if (map_data->flags[5]++) { fprintf(stderr, "C duplicado\n"); return 0; }
            int r, g, b;
            if (sscanf(linha + 2, "%d,%d,%d", &r, &g, &b) != 3 || r < 0 || r > 255 || g < 0 || g > 255 || b < 0 || b > 255) {
                fprintf(stderr, "Cor C inválida\n"); return 0;
            }
            data->ceiling_color = (r << 16) | (g << 8) | b;
        }
        else {
            // Verificar se todas as configurações foram processadas antes de iniciar o mapa
            if (map_data->config_count < 6) {
                fprintf(stderr, "Mapa prematuro\n"); return 0;
            }
            (map_data->map_iniciado) = 1;
            data->map_height++;
        }
        (map_data->config_count)++;
        /*
        else {
            // Verificar se todas as configurações foram processadas antes de iniciar o mapa
            if (*config_count < 6) {
                fprintf(stderr, "Mapa prematuro\n"); return 0;
            }
            (*map_iniciado) = 1;
            data->map_height++;
        }
        (*config_count)++;
        */
    }

    // Retorno obrigatório para evitar warnings
    return 1;
}
