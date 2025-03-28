/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aborges <aborges@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 12:17:15 by aborges           #+#    #+#             */
/*   Updated: 2025/03/17 18:14:31 by lnzila           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

unsigned int get_pixel(t_texture *texture, int x, int y)
{
    char *dst;

    dst = texture->addr + (y * texture->line_length + x * (texture->bpp / 8));
    return *(unsigned int *)dst;
}

unsigned int cria_trgb(int t, int r, int g, int b)
{
    return (t << 24) | (r << 16) | (g << 8) | b;
}

int main(int argc, char **argv)
{
    t_data data;
    memset(&data, 0, sizeof(data));

    if (argc != 2)
    {
        fprintf(stderr, "Usage: ./cub3d <mapa>\n");
        return 1;
    }

    // Carrega e valida o mapa antes de inicializar o MLX
    data.mapa = ler_mapa(argv[1], &data);
    if (!data.mapa)
    {
        fprintf(stderr, "Erro ao carregar o mapa\n");
        return 1;
    }

    // Verifica se todas as texturas foram definidas
    for (int i = 0; i < 4; i++)
    {
        if (!data.texture_paths[i])
        {
            fprintf(stderr, "Erro: caminho da textura %d não definido!\n", i);
            return 1;
        }
        printf("Textura %d lida do mapa: %s\n", i, data.texture_paths[i]);
    }
    printf("Cor do Chão (F): #%06X\n", data.floor_color);
    printf("Cor do Teto (C): #%06X\n", data.ceiling_color);

    // Inicializa o MLX e a janela somente após a validação
    data.mlx = mlx_init();
    if (!data.mlx)
    {
        fprintf(stderr, "Erro ao iniciar o MLX\n");
        return 1;
    }

    data.win = mlx_new_window(data.mlx, NOVA_LARGURA, NOVA_ALTURA, "Cub3D");
    if (!data.win)
    {
        fprintf(stderr, "Erro ao criar a janela\n");
        return 1;
    }

    data.img = mlx_new_image(data.mlx, NOVA_LARGURA, NOVA_ALTURA);
    if (!data.img)
    {
        fprintf(stderr, "Erro ao criar a imagem\n");
        return 1;
    }

    data.addr = mlx_get_data_addr(data.img, &data.bits_per_pixel,
                                  &data.line_length, &data.endian);
    if (!data.addr)
    {
        fprintf(stderr, "Erro ao obter endereço da imagem\n");
        return 1;
    }

    printf("Image created: bpp=%d, line_length=%d, endian=%d\n",
           data.bits_per_pixel, data.line_length, data.endian);

    // Carrega as texturas
    for (int i = 0; i < 4; i++)
    {
        carregar_textura(data.mlx, &data.textures[i], "", data.texture_paths[i]);
        if (!data.textures[i].img || !data.textures[i].addr)
        {
            fprintf(stderr, "Erro ao carregar textura %d\n", i);
            return 1;
        }
        printf("Textura %d carregada: %dx%d\n", i,
               data.textures[i].width, data.textures[i].height);
    }

    printf("Mapa carregado:\n");
    for (int y = 0; y < data.map_height; y++)
    {
        printf("%s\n", data.mapa[y]);
    }

    encontrar_jogador(&data);
    printf("Jogador encontrado em: %.2f, %.2f, ângulo: %.2f\n",
           data.player.x, data.player.y, data.player.angle);

    // Configura hooks e inicia o loop
    mlx_hook(data.win, 2, 1L << 0, key_press, &data);
    mlx_hook(data.win, 3, 1L << 1, key_release, &data);
    mlx_hook(data.win, 17, 0, fechar_janela, &data);
    mlx_loop_hook(data.mlx, render_scene, &data);
    mlx_loop(data.mlx);

    return 0;
}





/*
main.c: Ponto de entrada, gerencia o fluxo principal.
render.c: Lida com o motor gráfico e renderização (raycasting).
parse_map.c: Faz o parsing do mapa e validação.
utils.c: Contém funções auxiliares reutilizáveis.
*/
