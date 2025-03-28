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

// Protótipos das funções auxiliares
int processar_primeira_passagem(FILE *f, t_data *data, int *config_count, int *map_iniciado, int flags[6]);
static char **validar_e_alocar(FILE *f, t_data *data, int config_count, int map_iniciado);
static int processar_segunda_passagem(FILE *f, t_data *data, char **mapa);
static void liberar_mapa(char **mapa, int altura);

char **ler_mapa(char *arquivo, t_data *data) {
    FILE *f = fopen(arquivo, "r");
    char **mapa = NULL;
    int flags[6] = {0};
    int config_count = 0;
    int map_iniciado = 0;

    if (!f) {
        write(2, "Erro ao abrir o mapa\n", 22);
        return NULL;
    }

    // Primeira passagem: processar configurações
    if (!processar_primeira_passagem(f, data, &config_count, &map_iniciado, flags)) {
        fclose(f);
        return NULL;
    }

    // Validação e alocação de memória
    if (!(mapa = validar_e_alocar(f, data, config_count, map_iniciado))) {
        fclose(f);
        return NULL;
    }

    // Segunda passagem: carregar mapa
    if (!processar_segunda_passagem(f, data, mapa)) {
        liberar_mapa(mapa, data->map_height);
        fclose(f);
        return NULL;
    }

    // Definir largura do mapa
    if (data->map_height > 0) {
        data->map_width = strlen(mapa[0]);
    }

    printf("Mapa carregado com %d linhas e largura %d\n", data->map_height, data->map_width);
    fclose(f);
    return mapa;
}

// Função auxiliar 2 (22 linhas)
static char **validar_e_alocar(FILE *f, t_data *data, int config_count, int map_iniciado) {
    if (config_count < 6) {
        fprintf(stderr, "Configurações faltando\n");
        fclose(f);
        return NULL;
    }
    if (!map_iniciado || data->map_height <= 0) {
        fprintf(stderr, "Mapa inválido\n");
        fclose(f);
        return NULL;
    }
    
    char **mapa = malloc(sizeof(char *) * (data->map_height + 1));
    if (!mapa) {
        write(2, "Erro de alocação\n", 17);
        fclose(f);
        return NULL;
    }
    return mapa;
}

// Função auxiliar 3 (24 linhas)
static int processar_segunda_passagem(FILE *f, t_data *data, char **mapa)
{
    char linha[1024];
    int i = 0;

    // Voltar ao início do arquivo
    fseek(f, 0, SEEK_SET);

    // Loop para ler cada linha do arquivo
    while (fgets(linha, sizeof(linha), f)) // Corrigido: adicionado o parêntese final
    {
        // Remover caracteres de nova linha (\r\n)
        linha[strcspn(linha, "\r\n")] = '\0';

        // Ignorar linhas de configuração ou vazias
        if (strncmp(linha, "NO ", 3) == 0 || strncmp(linha, "SO ", 3) == 0 ||
            strncmp(linha, "WE ", 3) == 0 || strncmp(linha, "EA ", 3) == 0 ||
            strncmp(linha, "F ", 2) == 0 || strncmp(linha, "C ", 2) == 0 ||
            linha[0] == '\0') continue;

        // Substituir tabs (exemplo)
        char *linha_corrigida = strdup(linha);
        if (!linha_corrigida) {
            write(2, "Erro de processamento\n", 22);
            for (int j = 0; j < i; j++) free(mapa[j]);
            return 0;
        }
        mapa[i++] = linha_corrigida;
    }

    // Marcar o final do mapa
    mapa[i] = NULL;

    // Verificar consistência do mapa
    if (i != data->map_height) {
        fprintf(stderr, "Inconsistência no mapa\n");
        return 0;
    }

    // Retorno obrigatório para evitar warnings
    return 1;
}

// Função auxiliar para limpeza
static void liberar_mapa(char **mapa, int altura) {
    for (int i = 0; i < altura; i++) free(mapa[i]);
    free(mapa);
}
