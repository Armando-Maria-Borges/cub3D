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

int processar_segunda_passagem(t_data *data, t_map_data *map_data, char *arquivo)
{
    char linha[1024];
    char *linha_corrigida;
    int i = 0;
    int j = 0; //um valor de checagem para me ajudar a verificar a primeira e ultima linha
    int z = 0;
    int y = 0;

    if (!map_data->f)
    {
        printf("Erro: Arquivo não pôde ser aberto na segunda passagem.\n");
        return 1;  // Ou outro código de erro
    }
    //PARA REINICIAR A LEITURA
    close(map_data->f);
    map_data->f = open(arquivo, O_RDONLY);
    // Loop para ler cada linha do arquivo
    while (read_line(map_data->f, linha, sizeof(linha)))
    {
        // Remover caracteres de nova linha (\r\n), substituindo por null
        if (linha[strlen(linha) - 1] == '\n')
            linha[strlen(linha) - 1] = '\0';
        // Ignorar linhas de configuração ou vazias
        if (z != 6 && (strncmp(linha, "NO ", 3) == 0 || strncmp(linha, "SO ", 3) == 0 ||
            strncmp(linha, "WE ", 3) == 0 || strncmp(linha, "EA ", 3) == 0 ||
            strncmp(linha, "F ", 2) == 0 || strncmp(linha, "C ", 2) == 0 ||
            strlen(linha) == strspn(linha, " ")))
        {
            if (strlen(linha) == strspn(linha, " "))
                continue;
            z++;
            if (z > 6)
            {
                printf("\nError! EXISTE elementos a mais\n");
                return (0);
            }
            continue;
        }
        else if (z != 6)
        {
            printf("\nError! EXISTE elemento invalido, %s\n", linha);
            return (0);
        }
        if (strlen(linha) == strspn(linha, " "))
        {
            if (y == 1)
            {
                while (read_line(map_data->f, linha, sizeof(linha)) > 0)
                {
                    if (strlen(linha) != strspn(linha, " "))
                    {
                        printf("\nError! EXISTE linha vazia dentro do mapa\n");
                        return (0);
                    }
                }
            }
            continue;
        }
        else
            y = 1;
        if (strlen(linha) == strspn(linha, " "))
            break;

        linha_corrigida = strdup(linha);
        //VERIFICAR ABERTURA NA PRIMEIRA LINHA
        if (j == 0)
        {
            while (linha_corrigida[j])
            {
                if (linha_corrigida[j] == '0')
                {
                    printf("\nError! EXISTE CAMINHO ABERTO NA PRIMEIRA LINHA\n");
                    return (0);
                }
                j++;
            }
        }
        int k = 0;
        while (linha_corrigida[k] == 32 || linha_corrigida[k] == 39) 
            k++;
        if (linha_corrigida[k] == '0' || linha_corrigida[strlen(linha_corrigida) - 1] == '0')
        {    
            printf("\nError! INICIO OU FIM SEM PAREDE\n");
            return (0);
        }
        if (!linha_corrigida) {
            write(2, "Erro de processamento\n", 22);

            //int j = 0;

            for (int j = 0; j < i; j++)
                free(map_data->mapa[j]);
            return 0;
        }
        map_data->mapa[i++] = linha_corrigida;
    }
    //VERIFICAR ABERTURA NA ULTIMA LINHA
    j = 0;
    while (linha_corrigida[j])
    {
        if (linha_corrigida[j] == '0')
        {
            printf("\nError! EXISTE CAMINHO ABERTO NA ULTIMA LINHA\n");
            return (0);
        }
        j++;
    }
    // Marcar o final do mapa
    map_data->mapa[i] = NULL;
    // Verificar consistência do mapa
    if (i != data->map_height) {
        printf("Inconsistência no mapa\n");
        return 0;
    }
    // Retorno obrigatório para evitar warnings
    return (1);
}
// Função auxiliar 3 (24 linhas)
/*
int processar_segunda_passagem(t_data *data, t_map_data *map_data)
{
    char linha[1024];
    char *linha_corrigida;
    int i = 0;
    int j = 0; //um valor de checagem para me ajudar a verificar a primeira e ultima linha
    //int check_space = 0;

    // Voltar ao início do arquivo
    //fseek(f, 0, SEEK_SET);
    if (!map_data->f)
    {
        printf("Erro: Arquivo não pôde ser aberto na segunda passagem.\n");
        return 1;  // Ou outro código de erro
    }
    fseek(map_data->f, 0, SEEK_SET);

    int z = 0;
    int y = 0;
    // Loop para ler cada linha do arquivo
    while (fgets(linha, sizeof(linha), map_data->f))
    {
        // Remover caracteres de nova linha (\r\n)
        if (linha[strlen(linha) - 1] == '\n')
            linha[strlen(linha) - 1] = '\0';
        // linha[strcspn(linha, "\r\n")] = '\0';

        // Ignorar linhas de configuração ou vazias
        if (z != 6 && (strncmp(linha, "NO ", 3) == 0 || strncmp(linha, "SO ", 3) == 0 ||
            strncmp(linha, "WE ", 3) == 0 || strncmp(linha, "EA ", 3) == 0 ||
            strncmp(linha, "F ", 2) == 0 || strncmp(linha, "C ", 2) == 0 ||
            strlen(linha) == strspn(linha, " ")))
        {
            if (strlen(linha) == strspn(linha, " "))
                continue;
            z++;
            if (z > 6)
            {
                printf("\nError! EXISTE elementos a mais\n");
                return (0);
            }
            //check_space += 1;
            continue;
        }
        else if (z != 6)
        {
            printf("\nError! EXISTE elemento invalido, %s\n", linha);
            return (0);
        }

        if (strlen(linha) == strspn(linha, " "))
        {
            if (y == 1)
            {
                while (fgets(linha, sizeof(linha), map_data->f))
                {
                    if (strlen(linha) != strspn(linha, " "))
                    {
                        printf("\nError! EXISTE linha vazia dentro do mapa\n");
                        return (0);
                    }
                }
            }
            continue;
        }
        else
            y = 1;
        if (strlen(linha) == strspn(linha, " "))
            break;

        // Substituir tabs (exemplo)
        linha_corrigida = strdup(linha);


        //VERIFICAR ABERTURA NA PRIMEIRA LINHA
        if (j == 0)
        {
            while (linha_corrigida[j])
            {
                if (linha_corrigida[j] == '0')
                {
                    printf("\nError! EXISTE CAMINHO ABERTO NA PRIMEIRA LINHA\n");
                    return (0);
                }
                j++;
            }
        }
        int k = 0;
        while (linha_corrigida[k] == 32 || linha_corrigida[k] == 39) 
            k++;
        if (linha_corrigida[k] == '0' || linha_corrigida[strlen(linha_corrigida) - 1] == '0')
        {    
            printf("\nError! INICIO OU FIM SEM PAREDE\n");
            return (0);
        }
        if (!linha_corrigida) {
            write(2, "Erro de processamento\n", 22);
            for (int j = 0; j < i; j++)
                free(map_data->mapa[j]);
            return 0;
        }
        map_data->mapa[i++] = linha_corrigida;
    }
    //VERIFICAR ABERTURA NA ULTIMA LINHA
    j = 0;
    while (linha_corrigida[j])
    {
        if (linha_corrigida[j] == '0')
        {
            printf("\nError! EXISTE CAMINHO ABERTO NA ULTIMA LINHA\n");
            return (0);
        }
        j++;
    }
    // Marcar o final do mapa
    map_data->mapa[i] = NULL;

    // Verificar consistência do mapa
    if (i != data->map_height) {
        fprintf(stderr, "Inconsistência no mapa\n");
        return 0;
    }
    // Retorno obrigatório para evitar warnings
    return (1);
}

*/