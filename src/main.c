/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aborges <aborges@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 12:17:15 by aborges           #+#    #+#             */
/*   Updated: 2025/02/12 12:56:04 by aborges          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

#include "mlx.h"
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdio.h>

#include <errno.h>
#include <libgen.h>
#include <limits.h>  // Para PATH_MAX

#define TILE_SIZE 10
#define NOVA_LARGURA 1920
#define NOVA_ALTURA 1080
#define FOV 1.309
#define NUM_RAYS NOVA_LARGURA
#define MAX_DEPTH 20

#define KEY_W 119
#define KEY_A 97
#define KEY_S 115
#define KEY_D 100
#define KEY_ESC 65307
#define KEY_LEFT 65361
#define KEY_RIGHT 65363
#define TAB_SIZE 4  // Define quantos espaços um tab equivale

typedef struct s_player
{
    double x;
    double y;
    double angle;
} t_player;

typedef struct s_key_state
{
    int w;
    int a;
    int s;
    int d;
    int left;
    int right;
} t_key_state;

typedef struct s_texture
{
    void *img;
    char *addr;
    int bpp;
    int width;
    int height;
    int bits_per_pixel;
    int line_length;
    int endian;
} t_texture;

typedef struct s_data
{
    void *mlx;
    void *win;
    char **mapa;
    t_player player;
    t_key_state keys;
    void *img;
    char *addr;
    int bits_per_pixel;
    int line_length;
    int endian;
    int map_width;
    int map_height;
    t_texture textures[4]; // Norte, Sul, Oeste, Leste
    char    *texture_paths[4]; // Armazena os caminhos lidos do arquivo
    unsigned int floor_color;  // Cor do chão (armazenada como um único inteiro)
    unsigned int ceiling_color;// Cor do teto
} t_data;

// Funções auxiliares
void my_mlx_pixel_put(t_data *data, int x, int y, int color);
unsigned int get_pixel(t_texture *tex, int x, int y);
void desenhar_mapa(t_data *data);
int render_scene(void *param);
void carregar_texturas(void *mlx, t_texture *textures, char *cub_file_path, char **paths);
void carregar_cor(char *linha, int *r, int *g, int *b);
void raycast(t_data *data);
void mover_jogador(t_data *data);
int key_press(int keycode, void *param);
int key_release(int keycode, void *param);
char **ler_mapa(char *arquivo, t_data *data);
void encontrar_jogador(t_data *data);
int fechar_janela(void *param);
void pintar_chao_teto(t_data *data);


unsigned int cria_trgb(int t, int r, int g, int b);

//void rotacionar_jogador(int keycode, t_player *player);
void rotacionar_jogador(t_data *data);

// Prototipação das funções utilizadas
void carregar_textura(void *mlx, t_texture *texture, const char *diretorio, const char *nome_textura);
void carregar_cor(char *linha, int *r, int *g, int *b);
char *substituir_tabs(const char *linha);

int fechar_janela(void *param)
{
    t_data *data = (t_data *)param;

    if (data->win)
        mlx_destroy_window(data->mlx, data->win);
    exit(0);
    return (0);
}

void pintar_janela(t_data *data)
{
    if (!data->mlx || !data->win)
        return;

    int x, y;
    for (y = 0; y < NOVA_ALTURA; y++)
    {
        for (x = 0; x < NOVA_LARGURA; x++)
        {
            int cor = (y < NOVA_ALTURA / 2) ? 0xFFD700 : 0x00008B;
            mlx_pixel_put(data->mlx, data->win, x, y, cor);
        }
    }
}

void pintar_chao_teto(t_data *data)
{
    int x, y;
    int floor_color = data->floor_color;   // Cor já no formato 0xRRGGBB
    int ceiling_color = data->ceiling_color; // Cor já no formato 0xRRGGBB

    for (y = 0; y < NOVA_ALTURA / 2; y++)
    {
        for (x = 0; x < NOVA_LARGURA; x++)
        {
            my_mlx_pixel_put(data, x, y, ceiling_color);
        }
    }
    for (y = NOVA_ALTURA / 2; y < NOVA_ALTURA; y++)
    {
        for (x = 0; x < NOVA_LARGURA; x++)
        {
            my_mlx_pixel_put(data, x, y, floor_color);
        }
    }
}


// Implementação da função (no arquivo .c)
void carregar_texturas(void *mlx, t_texture *textures, char *cub_file_path, char **paths)
{
    for (int i = 0; i < 4; i++)
    {
        carregar_textura(mlx, &textures[i], cub_file_path, paths[i]);
    }
}

// Função para carregar texturas
void carregar_textura(void *mlx, t_texture *texture, const char *base_dir, const char *nome_textura)
{
    char caminho_completo[1024];

    /* Se base_dir estiver vazio, usamos nome_textura diretamente */
    if (base_dir == NULL || base_dir[0] == '\0')
        snprintf(caminho_completo, sizeof(caminho_completo), "%s", nome_textura);
    else
        snprintf(caminho_completo, sizeof(caminho_completo), "%s/%s", base_dir, nome_textura);

    texture->img = mlx_xpm_file_to_image(mlx, caminho_completo, &texture->width, &texture->height);
    if (!texture->img)
    {
        fprintf(stderr, "Erro ao carregar a textura: %s\n", caminho_completo);
    }
    else
    {
        texture->addr = mlx_get_data_addr(texture->img, &texture->bpp, &texture->line_length, &texture->endian);
    }
}


void carregar_cor(char *linha, int *r, int *g, int *b)
{
    sscanf(linha, "%d,%d,%d", r, g, b);
}

void my_mlx_pixel_put(t_data *data, int x, int y, int color)
{
    if (x >= 0 && x < NOVA_LARGURA && y >= 0 && y < NOVA_ALTURA)
    {
        char *dst = data->addr + (y * data->line_length + x * (data->bits_per_pixel / 8));
        *(unsigned int *)dst = color;
    }
}

void encontrar_jogador(t_data *data)
{
    int y, x;
    for (y = 0; data->mapa[y] != NULL; y++)
    {
        for (x = 0; data->mapa[y][x] != '\0'; x++)
        {
            // Apenas considerar caracteres válidos para o jogador
            if (data->mapa[y][x] == 'N' || data->mapa[y][x] == 'W' || 
                data->mapa[y][x] == 'S' || data->mapa[y][x] == 'E')
            {
                data->player.x = x + 0.5; // Centraliza no bloco
                data->player.y = y + 0.5; // Centraliza no bloco

                if (data->mapa[y][x] == 'N')
                    data->player.angle = 3 * M_PI / 2;
                else if (data->mapa[y][x] == 'S')
                    data->player.angle = M_PI / 2;
                else if (data->mapa[y][x] == 'E')
                    data->player.angle = 0;
                else if (data->mapa[y][x] == 'W')
                    data->player.angle = M_PI;
                return;
            }
        }
    }
    printf("Jogador não encontrado no mapa!\n");
}

void rotacionar_jogador(t_data *data)
{
    double rotation_speed = 0.05;
    
    if (data->keys.left)
        data->player.angle -= rotation_speed;
    if (data->keys.right)
        data->player.angle += rotation_speed;
        
    // Manter o ângulo entre 0 e 2*PI
    if (data->player.angle < 0)
        data->player.angle += 2 * M_PI;
    if (data->player.angle > 2 * M_PI)
        data->player.angle -= 2 * M_PI;
}

char *substituir_tabs(const char *linha)
{
    int j, tab_count = 0;
    size_t i, len = strlen(linha);

    // Conta quantos tabs existem na linha
    for (i = 0; i < len; i++)
        if (linha[i] == '\t')
            tab_count++;

    // Aloca memória para nova string com os espaços extras
    char *nova_linha = malloc(len + (tab_count * (TAB_SIZE - 1)) + 1);
    if (!nova_linha)
        return NULL;

    // Copia a linha substituindo tabs por espaços
    for (i = 0, j = 0; linha[i] != '\0'; i++)
    {
        if (linha[i] == '\t')
        {
            for (int k = 0; k < TAB_SIZE; k++)
                nova_linha[j++] = ' ';
        }
        else
            nova_linha[j++] = linha[i];
    }
    nova_linha[j] = '\0';

    return nova_linha;
}

char **ler_mapa(char *arquivo, t_data *data)
{
    FILE *f = fopen(arquivo, "r");
    char **mapa = NULL;
    char linha[1024];
    int i = 0;
    int config_count = 0;
    
    data->map_height = 0; // Inicializa a altura do mapa

    if (!f)
    {
        write(2, "Erro ao abrir o mapa\n", 22);
        return NULL;
    }
    printf("Abrindo arquivo: %s\n", arquivo);

    /* Primeira passagem: processa as configurações e conta as linhas do mapa */
    while (fgets(linha, sizeof(linha), f))
    {
        linha[strcspn(linha, "\r\n")] = '\0';  // Remove \n e \r

        if (strncmp(linha, "NO ", 3) == 0)
        {
            data->texture_paths[0] = strdup(linha + 3);
            config_count++;
            continue;
        }
        else if (strncmp(linha, "SO ", 3) == 0)
        {
            data->texture_paths[1] = strdup(linha + 3);
            config_count++;
            continue;
        }
        else if (strncmp(linha, "WE ", 3) == 0)
        {
            data->texture_paths[2] = strdup(linha + 3);
            config_count++;
            continue;
        }
        else if (strncmp(linha, "EA ", 3) == 0)
        {
            data->texture_paths[3] = strdup(linha + 3);
            config_count++;
            continue;
        }
        else if (strncmp(linha, "F ", 2) == 0)
        {
            int r, g, b;
            sscanf(linha + 2, "%d,%d,%d", &r, &g, &b);
            data->floor_color = (r << 16) | (g << 8) | b;
            config_count++;
            continue;
        }
        else if (strncmp(linha, "C ", 2) == 0)
        {
            int r, g, b;
            sscanf(linha + 2, "%d,%d,%d", &r, &g, &b);
            data->ceiling_color = (r << 16) | (g << 8) | b;
            config_count++;
            continue;
        }
        // Se a linha não for vazia, ela faz parte do mapa
        if (linha[0] != '\0')
            data->map_height++;
    }
    
    if (config_count < 6)
    {
        fprintf(stderr, "Erro: Configurações incompletas no mapa.\n");
        fclose(f);
        return NULL;
    }
    
    // Aloca o array para as linhas do mapa (+1 para o terminador NULL)
    mapa = malloc(sizeof(char *) * (data->map_height + 1));
    if (!mapa)
    {
        write(2, "Erro ao alocar memória para o mapa\n", 34);
        fclose(f);
        return NULL;
    }
    
    fseek(f, 0, SEEK_SET);
    i = 0;
    while (fgets(linha, sizeof(linha), f))
    {
        linha[strcspn(linha, "\r\n")] = '\0';
        if (strncmp(linha, "NO ", 3) == 0 ||
            strncmp(linha, "SO ", 3) == 0 ||
            strncmp(linha, "WE ", 3) == 0 ||
            strncmp(linha, "EA ", 3) == 0 ||
            strncmp(linha, "F ", 2) == 0  ||
            strncmp(linha, "C ", 2) == 0)
        {
            continue;
        }
        if (linha[0] != '\0')
        {
            char *linha_corrigida = substituir_tabs(linha); // Sua função para tratar tabs
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
    }
    mapa[i] = NULL;
    
    if (i > 0)
        data->map_width = strlen(mapa[0]);
    
    printf("Mapa alocado com %d linhas e largura %d\n", data->map_height, data->map_width);
    fclose(f);
    return mapa;
}

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

void raycast(t_data *data)
{
    int x;
    double delta_angle = FOV / (double)NUM_RAYS;

    for (x = 0; x < NUM_RAYS; x++)
    {
        double ray_angle = data->player.angle - (FOV / 2) + x * delta_angle;
        double dir_x = cos(ray_angle);
        double dir_y = sin(ray_angle);

        int map_x = (int)data->player.x;
        int map_y = (int)data->player.y;

        double delta_dist_x = (dir_x == 0) ? 1e30 : fabs(1 / dir_x);
        double delta_dist_y = (dir_y == 0) ? 1e30 : fabs(1 / dir_y);
        double side_dist_x, side_dist_y;

        int step_x = (dir_x < 0) ? -1 : 1;
        int step_y = (dir_y < 0) ? -1 : 1;

        side_dist_x = (dir_x < 0) ? (data->player.x - map_x) * delta_dist_x : (map_x + 1.0 - data->player.x) * delta_dist_x;
        side_dist_y = (dir_y < 0) ? (data->player.y - map_y) * delta_dist_y : (map_y + 1.0 - data->player.y) * delta_dist_y;

        int hit = 0, side;
        double perp_wall_dist;

        while (!hit)
        {
            if (side_dist_x < side_dist_y)
            {
                side_dist_x += delta_dist_x;
                map_x += step_x;
                side = 0; // Parede vertical
            }
            else
            {
                side_dist_y += delta_dist_y;
                map_y += step_y;
                side = 1; // Parede horizontal
            }

            if (map_y >= 0 && map_y < data->map_height && map_x >= 0 && map_x < data->map_width &&
                data->mapa[map_y][map_x] == '1')
            {
                hit = 1;
                if (side == 0)
                    perp_wall_dist = (map_x - data->player.x + (1 - step_x) / 2.0) / dir_x;
                else
                    perp_wall_dist = (map_y - data->player.y + (1 - step_y) / 2.0) / dir_y;
                if (perp_wall_dist < 0.1)
                    perp_wall_dist = 0.1;
            }
        }

        // Cálculo da altura da parede a ser desenhada
        int wall_height = (int)(NOVA_ALTURA / perp_wall_dist);

        // Determina os pontos de início e fim da linha da parede
        int start = (NOVA_ALTURA / 2) - (wall_height / 2);
        int end = (NOVA_ALTURA / 2) + (wall_height / 2);
        if (start < 0) start = 0;
        if (end >= NOVA_ALTURA) end = NOVA_ALTURA - 1;

        // Calcula a posição onde o raio atingiu a parede
        double wall_x;
        if (side == 0)
            wall_x = data->player.y + perp_wall_dist * dir_y;
        else
            wall_x = data->player.x + perp_wall_dist * dir_x;
        wall_x -= floor(wall_x);

        // Determina a coordenada x da textura
        int tex_x = (int)(wall_x * (double)(data->textures[0].width));
        if (tex_x < 0)
            tex_x = 0;
        if (tex_x >= data->textures[0].width)
            tex_x = data->textures[0].width - 1;

        // Ajusta tex_x para a orientação correta da textura
        if (side == 0 && dir_x > 0)
            tex_x = data->textures[0].width - tex_x - 1;
        if (side == 1 && dir_y < 0)
            tex_x = data->textures[0].width - tex_x - 1;

        // Seleciona a textura correta com base na direção
        t_texture *texture;
        if (side == 0 && step_x < 0)
            texture = &data->textures[0]; // Parede Norte
        else if (side == 0 && step_x > 0)
            texture = &data->textures[1]; // Parede Sul
        else if (side == 1 && step_y < 0)
            texture = &data->textures[2]; // Parede Oeste
        else
            texture = &data->textures[3]; // Parede Leste

        // Desenha os pixels da parede com a textura
        for (int y = start; y < end; y++)
        {
            int d = y * 256 - NOVA_ALTURA * 128 + wall_height * 128;
            int tex_y = ((d * texture->height) / wall_height) / 256;
            if (tex_y < 0)
                tex_y = 0;
            if (tex_y >= texture->height)
                tex_y = texture->height - 1;

            unsigned int color = get_pixel(texture, tex_x, tex_y);
            my_mlx_pixel_put(data, x, y, color);
        }

        // Desenha o teto acima da parede usando a cor já armazenada
        for (int y = 0; y < start; y++)
        {
            unsigned int ceiling_color = data->ceiling_color;
            my_mlx_pixel_put(data, x, y, ceiling_color);
        }

        // Desenha o chão abaixo da parede
        for (int y = end; y < NOVA_ALTURA; y++)
        {
            // Extração dos componentes RGB a partir do inteiro floor_color
            unsigned int r = (data->floor_color >> 16) & 0xFF;
            unsigned int g = (data->floor_color >> 8) & 0xFF;
            unsigned int b = data->floor_color & 0xFF;
            unsigned int floor_color = cria_trgb(0, r, g, b);
            my_mlx_pixel_put(data, x, y, floor_color);
        }
    }
}

//##########################################################

// Função auxiliar para limitar um valor entre um mínimo e máximo.
static double clamp(double value, double min, double max)
{
    if (value < min)
        return min;
    if (value > max)
        return max;
    return value;
}

// Função que verifica colisão: retorna 1 se houver colisão, 0 caso contrário.
int check_collision(t_data *data, double new_x, double new_y)
{
    double collision_radius = 0.2; // Ajuste conforme necessário.
    int cell_min_x = (int)floor(new_x) - 1;
    int cell_max_x = (int)floor(new_x) + 1;
    int cell_min_y = (int)floor(new_y) - 1;
    int cell_max_y = (int)floor(new_y) + 1;
    int i, j;
    
    for (i = cell_min_y; i <= cell_max_y; i++)
    {
        for (j = cell_min_x; j <= cell_max_x; j++)
        {
            // Verifica se a célula está dentro dos limites do mapa
            if (i < 0 || j < 0 || i >= data->map_height || j >= data->map_width)
                continue;
            // Se a célula for uma parede
            if (data->mapa[i][j] == '1')
            {
                // Calcula o ponto mais próximo na célula retangular (de j a j+1 e de i a i+1)
                double nearest_x = clamp(new_x, j, j + 1);
                double nearest_y = clamp(new_y, i, i + 1);
                double dx = new_x - nearest_x;
                double dy = new_y - nearest_y;
                double distance = sqrt(dx * dx + dy * dy);
                if (distance < collision_radius)
                    return 1; // Colisão detectada
            }
        }
    }
    return 0;
}

void mover_jogador(t_data *data)
{
    double move_speed = 0.1;
    double rotation_speed = 0.05;
    double new_x, new_y;

    // Rotação do player
    if (data->keys.left)
        data->player.angle -= rotation_speed;
    if (data->keys.right)
        data->player.angle += rotation_speed;

    // Manter o ângulo entre 0 e 2*PI
    if (data->player.angle < 0)
        data->player.angle += 2 * M_PI;
    if (data->player.angle > 2 * M_PI)
        data->player.angle -= 2 * M_PI;

    // Movimento para frente
    if (data->keys.w)
    {
        new_x = data->player.x + cos(data->player.angle) * move_speed;
        new_y = data->player.y + sin(data->player.angle) * move_speed;
        if (!check_collision(data, new_x, new_y))
        {
            data->player.x = new_x;
            data->player.y = new_y;
        }
    }
    
    // Movimento para trás
    if (data->keys.s)
    {
        new_x = data->player.x - cos(data->player.angle) * move_speed;
        new_y = data->player.y - sin(data->player.angle) * move_speed;
        if (!check_collision(data, new_x, new_y))
        {
            data->player.x = new_x;
            data->player.y = new_y;
        }
    }
    
    // Movimento para direita (strafe)
    if (data->keys.d)
    {
        new_x = data->player.x + cos(data->player.angle + M_PI_2) * move_speed;
        new_y = data->player.y + sin(data->player.angle + M_PI_2) * move_speed;
        if (!check_collision(data, new_x, new_y))
        {
            data->player.x = new_x;
            data->player.y = new_y;
        }
    }
    
    // Movimento para esquerda (strafe)
    if (data->keys.a)
    {
        new_x = data->player.x - cos(data->player.angle + M_PI_2) * move_speed;
        new_y = data->player.y - sin(data->player.angle + M_PI_2) * move_speed;
        if (!check_collision(data, new_x, new_y))
        {
            data->player.x = new_x;
            data->player.y = new_y;
        }
    }
}


//##########################################################3333

int key_press(int keycode, void *param)
{
    t_data *data = (t_data *)param;
    if (keycode == KEY_ESC)
    {
        mlx_destroy_window(data->mlx, data->win);
        exit(0);
    }
    else if (keycode == KEY_W)
        data->keys.w = 1;
    else if (keycode == KEY_A)
        data->keys.a = 1;
    else if (keycode == KEY_S)
        data->keys.s = 1;
    else if (keycode == KEY_D)
        data->keys.d = 1;
    else if (keycode == KEY_LEFT)
        data->keys.left = 1;
    else if (keycode == KEY_RIGHT)
        data->keys.right = 1;
    return (0);
}

int key_release(int keycode, void *param)
{
    t_data *data = (t_data *)param;
    if (keycode == KEY_W)
        data->keys.w = 0;
    else if (keycode == KEY_A)
        data->keys.a = 0;
    else if (keycode == KEY_S)
        data->keys.s = 0;
    else if (keycode == KEY_D)
        data->keys.d = 0;
    else if (keycode == KEY_LEFT)
        data->keys.left = 0;
    else if (keycode == KEY_RIGHT)
        data->keys.right = 0;
    return (0);
}

void desenhar_mapa(t_data *data)
{
    int x, y, i, j;

    for (y = 0; data->mapa[y] != NULL; y++)
    {
        for (x = 0; data->mapa[y][x] != '\0'; x++)
        {
            for (i = 0; i < TILE_SIZE; i++)
            {
                for (j = 0; j < TILE_SIZE; j++)
                {
                    if (x * TILE_SIZE + i >= 0 && x * TILE_SIZE + i < NOVA_LARGURA &&
                        y * TILE_SIZE + j >= 0 && y * TILE_SIZE + j < NOVA_ALTURA)
                    {
                        if (data->mapa[y][x] == '1')
                            my_mlx_pixel_put(data, x * TILE_SIZE + i, y * TILE_SIZE + j, 0xFFFFFF);
                        else if (data->mapa[y][x] == '0')
                            my_mlx_pixel_put(data, x * TILE_SIZE + i, y * TILE_SIZE + j, 0x808080);
                    }
                }
            }
        }
    }

    // Desenhar jogador no minimapa
    for (i = 0; i < TILE_SIZE / 2; i++)
    {
        for (j = 0; j < TILE_SIZE / 2; j++)
        {
            my_mlx_pixel_put(data, data->player.x * TILE_SIZE + i, data->player.y * TILE_SIZE + j, 0xFFA500);
        }
    }
}

int render_scene(void *param)
{
    t_data *data = (t_data *)param;

    pintar_chao_teto(data);
    raycast(data); // Chamada para a função raycast
    mlx_put_image_to_window(data->mlx, data->win, data->img, 0, 0);

    mover_jogador(data); // Atualiza a posição do jogador

    desenhar_mapa(data); //tem o mini mapa

    return 0;
}



void debug_print_map(t_data *data)
{
    printf("Imprimindo mapa:\n");
    for (int y = 0; y < data->map_height; y++)
    {
        printf("[%d] %s\n", y, data->mapa[y]);
    }
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
    
    // Inicializar a MLX
    data.mlx = mlx_init();
    if (!data.mlx)
    {
        fprintf(stderr, "Erro ao iniciar o MLX\n");
        return 1;
    }
    
    // Criar a janela
    data.win = mlx_new_window(data.mlx, NOVA_LARGURA, NOVA_ALTURA, "Cub3D");
    if (!data.win)
    {
        fprintf(stderr, "Erro ao criar a janela\n");
        return 1;
    }
    
    // Criar a imagem principal
    data.img = mlx_new_image(data.mlx, NOVA_LARGURA, NOVA_ALTURA);
    if (!data.img)
    {
        fprintf(stderr, "Erro ao criar a imagem\n");
        return 1;
    }
    
    // Obter endereço da imagem
    data.addr = mlx_get_data_addr(data.img, &data.bits_per_pixel,
                                  &data.line_length, &data.endian);
    if (!data.addr)
    {
        fprintf(stderr, "Erro ao obter endereço da imagem\n");
        return 1;
    }
    
    printf("Image created: bpp=%d, line_length=%d, endian=%d\n",
           data.bits_per_pixel, data.line_length, data.endian);
    
    /* Ler o arquivo de mapa – esta função também processa as configurações (texturas e cores) */
    data.mapa = ler_mapa(argv[1], &data);
    if (!data.mapa)
    {
        fprintf(stderr, "Erro ao carregar o mapa\n");
        return 1;
    }
    
    /* Verificar as configurações lidas */
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
    
    /* Carregar as texturas usando os caminhos lidos */
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
    
    /* Debug: imprimir o mapa carregado */
    printf("Mapa carregado:\n");
    for (int y = 0; y < data.map_height; y++)
    {
        printf("%s\n", data.mapa[y]);
    }
    
    encontrar_jogador(&data);
    printf("Jogador encontrado em: %.2f, %.2f, ângulo: %.2f\n",
           data.player.x, data.player.y, data.player.angle);
    
    // Hooks para eventos de teclado e renderização
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
