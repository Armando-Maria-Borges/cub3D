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

// DEFINIÇÕES DE CONSTANTES
#define TILE_SIZE 10             // Tamanho do tile para desenhar o mapa miniatura
#define NOVA_LARGURA 1920        // Largura da janela
#define NOVA_ALTURA 1080         // Altura da janela
#define FOV 1.309                // Campo de visão (FOV) em radianos (~75°)
#define NUM_RAYS NOVA_LARGURA    // Número de raios lançados (um por coluna)
#define MAX_DEPTH 20             // Distância máxima que um raio pode percorrer

// Definições de teclas (códigos para MLX)
#define KEY_W 119
#define KEY_A 97
#define KEY_S 115
#define KEY_D 100
#define KEY_ESC 65307
#define KEY_LEFT 65361
#define KEY_RIGHT 65363
#define TAB_SIZE 4  // Número de espaços que um tab equivale

// ESTRUTURAS DE DADOS

// Estrutura para armazenar as informações do jogador
typedef struct s_player
{
    double x;     // Posição X no mapa
    double y;     // Posição Y no mapa
    double angle; // Ângulo (direção) que o jogador está olhando, em radianos
} t_player;

// Estrutura para armazenar o estado das teclas
typedef struct s_key_state
{
    int w;
    int a;
    int s;
    int d;
    int left;
    int right;
} t_key_state;

// Estrutura para armazenar informações sobre uma textura
typedef struct s_texture
{
    void *img;              // Ponteiro para a imagem carregada
    char *addr;             // Endereço dos pixels da imagem
    int bpp;                // Bits por pixel
    int width;              // Largura da textura
    int height;             // Altura da textura
    int bits_per_pixel;     // Bits por pixel (redundante com bpp, mas pode ter outra finalidade)
    int line_length;        // Número de bytes por linha
    int endian;             // Ordem de bytes da imagem
} t_texture;

// Estrutura principal que agrupa todos os dados da aplicação
typedef struct s_data
{
    void *mlx;                  // Conexão com o MLX
    void *win;                  // Janela criada pelo MLX
    char **mapa;                // Mapa lido do arquivo (array de strings)
    t_player player;            // Dados do jogador
    t_key_state keys;           // Estado das teclas pressionadas
    void *img;                  // Imagem para renderização off-screen
    char *addr;                 // Endereço dos pixels da imagem
    int bits_per_pixel;         // Bits por pixel da imagem
    int line_length;            // Número de bytes por linha na imagem
    int endian;                 // Ordem de bytes da imagem
    int map_width;              // Largura do mapa (em número de células)
    int map_height;             // Altura do mapa (número de linhas)
    t_texture textures[4];      // Texturas para as paredes (Norte, Sul, Oeste, Leste)
    char *texture_paths[4];     // Caminhos dos arquivos de textura lidos do arquivo de configuração
    unsigned int floor_color;   // Cor do chão (armazenada em formato 0xRRGGBB)
    unsigned int ceiling_color; // Cor do teto (formato 0xRRGGBB)
} t_data;

// Declaração das funções auxiliares
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
void pintar_janela(t_data *data);
unsigned int cria_trgb(int t, int r, int g, int b);
void rotacionar_jogador(t_data *data);
void carregar_textura(void *mlx, t_texture *texture, const char *diretorio, const char *nome_textura);
void carregar_cor(char *linha, int *r, int *g, int *b);
char *substituir_tabs(const char *linha);

// FUNÇÃO PARA FECHAR A JANELA E ENCERRAR O PROGRAMA
int fechar_janela(void *param)
{
    t_data *data = (t_data *)param;

    if (data->win)
        mlx_destroy_window(data->mlx, data->win);
    exit(0);
    return (0);
}

// FUNÇÃO PARA PINTAR A JANELA (EXEMPLO, NÃO USADA NA RENDERIZAÇÃO PRINCIPAL)
void pintar_janela(t_data *data)
{
    if (!data->mlx || !data->win)
        return;

    int x, y;
    for (y = 0; y < NOVA_ALTURA; y++)
    {
        for (x = 0; x < NOVA_LARGURA; x++)
        {
            // Define a cor do teto e do chão manualmente
            int cor = (y < NOVA_ALTURA / 2) ? 0xFFD700 : 0x00008B;
            mlx_pixel_put(data->mlx, data->win, x, y, cor);
        }
    }
}

// FUNÇÃO PARA PINTAR O CHÃO E O TETO USANDO AS CORES DEFINIDAS
void pintar_chao_teto(t_data *data)
{
    int x, y;
    int floor_color = data->floor_color;   // Cor do chão (formato 0xRRGGBB)
    int ceiling_color = data->ceiling_color; // Cor do teto (formato 0xRRGGBB)

    // Pinta a metade superior (teto)
    for (y = 0; y < NOVA_ALTURA / 2; y++)
    {
        for (x = 0; x < NOVA_LARGURA; x++)
        {
            my_mlx_pixel_put(data, x, y, ceiling_color);
        }
    }
    // Pinta a metade inferior (chão)
    for (y = NOVA_ALTURA / 2; y < NOVA_ALTURA; y++)
    {
        for (x = 0; x < NOVA_LARGURA; x++)
        {
            my_mlx_pixel_put(data, x, y, floor_color);
        }
    }
}

// =====================================================================
// FUNÇÃO: carregar_texturas
// Descrição: Percorre os 4 caminhos de texturas e chama carregar_textura para cada uma.
// Parâmetros: 
//   - mlx: Contexto do MLX
//   - textures: Array de texturas onde as imagens serão armazenadas
//   - cub_file_path: Diretório base do arquivo .cub (pode ser usado para montar o caminho completo)
//   - paths: Array de strings com os caminhos relativos das texturas
// =====================================================================
void carregar_texturas(void *mlx, t_texture *textures, char *cub_file_path, char **paths)
{
    for (int i = 0; i < 4; i++)
    {
        carregar_textura(mlx, &textures[i], cub_file_path, paths[i]);
    }
}

// =====================================================================
// FUNÇÃO: carregar_textura
// Descrição: Monta o caminho completo da textura e carrega a imagem usando MLX.
// Parâmetros:
//   - mlx: Contexto do MLX
//   - texture: Ponteiro para a textura que será carregada
//   - base_dir: Diretório base (se não vazio, concatena com o nome da textura)
//   - nome_textura: Nome ou caminho relativo do arquivo de textura
// =====================================================================
void carregar_textura(void *mlx, t_texture *texture, const char *base_dir, const char *nome_textura)
{
    char caminho_completo[1024];

    /* Se base_dir estiver vazio, usamos nome_textura diretamente.
       Caso contrário, monta o caminho completo concatenando base_dir e nome_textura. */
    if (base_dir == NULL || base_dir[0] == '\0')
        snprintf(caminho_completo, sizeof(caminho_completo), "%s", nome_textura);
    else
        snprintf(caminho_completo, sizeof(caminho_completo), "%s/%s", base_dir, nome_textura);

    // Carrega a imagem XPM para a textura e atualiza as dimensões
    texture->img = mlx_xpm_file_to_image(mlx, caminho_completo, &texture->width, &texture->height);
    if (!texture->img)
    {
        fprintf(stderr, "Erro ao carregar a textura: %s\n", caminho_completo);
    }
    else
    {
        // Obtém o endereço dos pixels e outras informações da imagem
        texture->addr = mlx_get_data_addr(texture->img, &texture->bpp, &texture->line_length, &texture->endian);
    }
}

// =====================================================================
// FUNÇÃO: carregar_cor
// Descrição: Lê uma linha com os valores RGB separados por vírgula e armazena os inteiros.
// =====================================================================
void carregar_cor(char *linha, int *r, int *g, int *b)
{
    sscanf(linha, "%d,%d,%d", r, g, b);
}

// =====================================================================
// FUNÇÃO: my_mlx_pixel_put
// Descrição: Coloca um pixel na posição (x, y) da imagem armazenada em data->addr.
// Cuidado com os limites da imagem.
// =====================================================================
void my_mlx_pixel_put(t_data *data, int x, int y, int color)
{
    if (x >= 0 && x < NOVA_LARGURA && y >= 0 && y < NOVA_ALTURA)
    {
        // Calcula o endereço do pixel usando line_length e bits_per_pixel
        char *dst = data->addr + (y * data->line_length + x * (data->bits_per_pixel / 8));
        *(unsigned int *)dst = color;
    }
}

// =====================================================================
// FUNÇÃO: encontrar_jogador
// Descrição: Varre o mapa procurando os caracteres 'N', 'S', 'E', 'W' que indicam a posição e orientação do jogador.
// Ao encontrar, centraliza o jogador na célula (adiciona 0.5) e define o ângulo inicial.
// =====================================================================
void encontrar_jogador(t_data *data)
{
    int y, x;
    for (y = 0; data->mapa[y] != NULL; y++)
    {
        for (x = 0; data->mapa[y][x] != '\0'; x++)
        {
            // Se encontrar um dos caracteres que representam a posição inicial do jogador
            if (data->mapa[y][x] == 'N' || data->mapa[y][x] == 'W' || 
                data->mapa[y][x] == 'S' || data->mapa[y][x] == 'E')
            {
                data->player.x = x + 0.5; // Centraliza o jogador horizontalmente na célula
                data->player.y = y + 0.5; // Centraliza o jogador verticalmente na célula

                // Define o ângulo de acordo com a direção encontrada
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

// =====================================================================
// FUNÇÃO: rotacionar_jogador
// Descrição: Ajusta o ângulo do jogador com base na entrada das teclas de rotação.
// =====================================================================
void rotacionar_jogador(t_data *data)
{
    double rotation_speed = 0.05;
    
    if (data->keys.left)
        data->player.angle -= rotation_speed;
    if (data->keys.right)
        data->player.angle += rotation_speed;
        
    // Garante que o ângulo permaneça entre 0 e 2π
    if (data->player.angle < 0)
        data->player.angle += 2 * M_PI;
    if (data->player.angle > 2 * M_PI)
        data->player.angle -= 2 * M_PI;
}

// =====================================================================
// FUNÇÃO: substituir_tabs
// Descrição: Substitui os caracteres de tabulação ('\t') por espaços.
// =====================================================================
char *substituir_tabs(const char *linha)
{
    int j, tab_count = 0;
    size_t i, len = strlen(linha);

    // Conta quantos tabs existem na linha
    for (i = 0; i < len; i++)
        if (linha[i] == '\t')
            tab_count++;

    // Aloca memória para a nova string considerando os espaços extras
    char *nova_linha = malloc(len + (tab_count * (TAB_SIZE - 1)) + 1);
    if (!nova_linha)
        return NULL;

    // Copia a linha, substituindo cada tab por TAB_SIZE espaços
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

// =====================================================================
// FUNÇÃO: ler_mapa
// Descrição: Lê um arquivo de mapa (.cub) e processa as configurações e o layout do mapa.
// Processamento em duas passagens:
//   1. Primeira passagem: Lê as configurações (texturas, cores) e conta as linhas do mapa.
//   2. Segunda passagem: Lê novamente o arquivo e armazena as linhas do mapa (processando tabs se necessário).
// Parâmetros:
//   - arquivo: Nome do arquivo a ser lido
//   - data: Estrutura que armazenará os dados lidos (incluindo cores, caminhos e dimensões do mapa)
// =====================================================================
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

        // Processa as linhas de configuração para as texturas e cores
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
            // Lê a cor do chão e converte para o formato 0xRRGGBB
            sscanf(linha + 2, "%d,%d,%d", &r, &g, &b);
            data->floor_color = (r << 16) | (g << 8) | b;
            config_count++;
            continue;
        }
        else if (strncmp(linha, "C ", 2) == 0)
        {
            int r, g, b;
            // Lê a cor do teto e converte para 0xRRGGBB
            sscanf(linha + 2, "%d,%d,%d", &r, &g, &b);
            data->ceiling_color = (r << 16) | (g << 8) | b;
            config_count++;
            continue;
        }
        // Se a linha não for vazia, considera como parte do layout do mapa
        if (linha[0] != '\0')
            data->map_height++;
    }
    
    // Verifica se todas as configurações necessárias foram lidas
    if (config_count < 6)
    {
        fprintf(stderr, "Erro: Configurações incompletas no mapa.\n");
        fclose(f);
        return NULL;
    }
    
    // Aloca o array para as linhas do mapa (mais um espaço para o terminador NULL)
    mapa = malloc(sizeof(char *) * (data->map_height + 1));
    if (!mapa)
    {
        write(2, "Erro ao alocar memória para o mapa\n", 34);
        fclose(f);
        return NULL;
    }
    
    // Reinicia o arquivo para a segunda passagem
    fseek(f, 0, SEEK_SET);
    i = 0;
    while (fgets(linha, sizeof(linha), f))
    {
        linha[strcspn(linha, "\r\n")] = '\0';
        // Pula as linhas de configuração já processadas
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
            // Substitui os tabs por espaços para manter o alinhamento correto
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
    }
    mapa[i] = NULL;
    
    // Define a largura do mapa baseado na primeira linha (poderia ser ajustado para maior largura)
    if (i > 0)
        data->map_width = strlen(mapa[0]);
    
    printf("Mapa alocado com %d linhas e largura %d\n", data->map_height, data->map_width);
    fclose(f);
    return mapa;
}

// =====================================================================
// FUNÇÃO: get_pixel
// Descrição: Obtém a cor de um pixel na textura na posição (x, y)
// =====================================================================
unsigned int get_pixel(t_texture *texture, int x, int y)
{
    char *dst;

    // Calcula o endereço do pixel na textura
    dst = texture->addr + (y * texture->line_length + x * (texture->bpp / 8));
    return *(unsigned int *)dst;
}

// =====================================================================
// FUNÇÃO: cria_trgb
// Descrição: Cria um inteiro que representa uma cor no formato TRGB a partir dos componentes
// =====================================================================
unsigned int cria_trgb(int t, int r, int g, int b)
{
    return (t << 24) | (r << 16) | (g << 8) | b;
}

// =====================================================================
// FUNÇÃO: raycast
// Descrição: Implementa o algoritmo de Raycasting para renderizar a cena 3D a partir do mapa 2D.
// Para cada coluna de pixels da tela, lança um raio e calcula:
//   - A direção do raio com base no ângulo do jogador e FOV.
//   - As distâncias incrementais (delta_dist_x, delta_dist_y) para avançar célula a célula.
//   - Utiliza o algoritmo DDA (Digital Differential Analysis) para encontrar a colisão com a parede.
//   - Calcula a distância perpendicular à parede para evitar a distorção (efeito "fisheye").
//   - Determina a altura da parede a ser desenhada e mapeia a textura apropriada.
// =====================================================================
void raycast(t_data *data)
{
    int x;
    // Delta de ângulo entre cada raio (divide o FOV pelo número total de raios)
    double delta_angle = FOV / (double)NUM_RAYS;

    // Percorre cada coluna de pixels na tela
    for (x = 0; x < NUM_RAYS; x++)
    {
        // Calcula o ângulo do raio atual relativo ao ângulo do jogador
        double ray_angle = data->player.angle - (FOV / 2) + x * delta_angle;
        // Componentes da direção do raio usando funções trigonométricas
        double dir_x = cos(ray_angle);
        double dir_y = sin(ray_angle);

        // Posição atual do jogador convertida para índice do mapa
        int map_x = (int)data->player.x;
        int map_y = (int)data->player.y;

        // Calcula a distância que o raio deve percorrer para atravessar uma célula inteira
        // Se a direção for 0, evita divisão por zero usando um valor muito alto (1e30)
        double delta_dist_x = (dir_x == 0) ? 1e30 : fabs(1 / dir_x);
        double delta_dist_y = (dir_y == 0) ? 1e30 : fabs(1 / dir_y);
        double side_dist_x, side_dist_y;

        // Define o passo (step) para avançar no mapa: -1 se a direção é negativa, 1 se positiva
        int step_x = (dir_x < 0) ? -1 : 1;
        int step_y = (dir_y < 0) ? -1 : 1;

        // Calcula a distância inicial do jogador até a primeira borda do quadrado
        side_dist_x = (dir_x < 0) ? (data->player.x - map_x) * delta_dist_x : (map_x + 1.0 - data->player.x) * delta_dist_x;
        side_dist_y = (dir_y < 0) ? (data->player.y - map_y) * delta_dist_y : (map_y + 1.0 - data->player.y) * delta_dist_y;

        int hit = 0, side;
        double perp_wall_dist; // Distância perpendicular à parede

        // Loop do algoritmo DDA: avança célula a célula até encontrar uma parede
        while (!hit)
        {
            if (side_dist_x < side_dist_y)
            {
                side_dist_x += delta_dist_x;
                map_x += step_x;
                side = 0; // Indica que a colisão ocorreu em uma parede vertical
            }
            else
            {
                side_dist_y += delta_dist_y;
                map_y += step_y;
                side = 1; // Colisão em parede horizontal
            }

            // Verifica se o novo quadrado do mapa é uma parede (caractere '1')
            if (map_y >= 0 && map_y < data->map_height && map_x >= 0 && map_x < data->map_width &&
                data->mapa[map_y][map_x] == '1')
            {
                hit = 1;
                // Calcula a distância perpendicular à parede para evitar o efeito de distorção
                if (side == 0)
                    perp_wall_dist = (map_x - data->player.x + (1 - step_x) / 2.0) / dir_x;
                else
                    perp_wall_dist = (map_y - data->player.y + (1 - step_y) / 2.0) / dir_y;
                // Garante que a distância mínima não seja zero (evita divisão por zero em renderizações futuras)
                if (perp_wall_dist < 0.1)
                    perp_wall_dist = 0.1;
            }
        }

        // Cálculo da altura da parede com base na distância: quanto maior a distância, menor a altura
        int wall_height = (int)(NOVA_ALTURA / perp_wall_dist);

        // Determina os pontos de início e fim para desenhar a linha da parede
        int start = (NOVA_ALTURA / 2) - (wall_height / 2);
        int end = (NOVA_ALTURA / 2) + (wall_height / 2);
        if (start < 0) start = 0;
        if (end >= NOVA_ALTURA) end = NOVA_ALTURA - 1;

        // Calcula a posição exata onde o raio atingiu a parede para mapeamento da textura
        double wall_x;
        if (side == 0)
            wall_x = data->player.y + perp_wall_dist * dir_y;
        else
            wall_x = data->player.x + perp_wall_dist * dir_x;
        wall_x -= floor(wall_x); // Mantém wall_x entre 0 e 1

        // Mapeamento da coordenada x da textura
        int tex_x = (int)(wall_x * (double)(data->textures[0].width));
        if (tex_x < 0)
            tex_x = 0;
        if (tex_x >= data->textures[0].width)
            tex_x = data->textures[0].width - 1;

        // Ajusta a coordenada da textura de acordo com a orientação da parede
        if (side == 0 && dir_x > 0)
            tex_x = data->textures[0].width - tex_x - 1;
        if (side == 1 && dir_y < 0)
            tex_x = data->textures[0].width - tex_x - 1;

        // Seleciona a textura correta com base na direção da parede atingida
        t_texture *texture;
        if (side == 0 && step_x < 0)
            texture = &data->textures[0]; // Parede Norte
        else if (side == 0 && step_x > 0)
            texture = &data->textures[1]; // Parede Sul
        else if (side == 1 && step_y < 0)
            texture = &data->textures[2]; // Parede Oeste
        else
            texture = &data->textures[3]; // Parede Leste

        // Loop para desenhar a coluna de pixels da parede, mapeando a textura
        for (int y = start; y < end; y++)
        {
            // Calcula a posição relativa dentro da parede para o mapeamento vertical da textura
            int d = y * 256 - NOVA_ALTURA * 128 + wall_height * 128;
            int tex_y = ((d * texture->height) / wall_height) / 256;
            if (tex_y < 0)
                tex_y = 0;
            if (tex_y >= texture->height)
                tex_y = texture->height - 1;

            // Obtém a cor do pixel na textura
            unsigned int color = get_pixel(texture, tex_x, tex_y);
            // Desenha o pixel na imagem
            my_mlx_pixel_put(data, x, y, color);
        }

        // Desenha o teto (acima da parede) com a cor do teto
        for (int y = 0; y < start; y++)
        {
            unsigned int ceiling_color = data->ceiling_color;
            my_mlx_pixel_put(data, x, y, ceiling_color);
        }

        // Desenha o chão (abaixo da parede) com a cor do chão
        for (int y = end; y < NOVA_ALTURA; y++)
        {
            // Separa os componentes RGB da cor do chão e reconstrói a cor
            unsigned int r = (data->floor_color >> 16) & 0xFF;
            unsigned int g = (data->floor_color >> 8) & 0xFF;
            unsigned int b = data->floor_color & 0xFF;
            unsigned int floor_color = cria_trgb(0, r, g, b);
            my_mlx_pixel_put(data, x, y, floor_color);
        }
    }
}

// =====================================================================
// FUNÇÃO AUXILIAR: clamp
// Descrição: Limita o valor passado para estar entre min e max.
// =====================================================================
static double clamp(double value, double min, double max)
{
    if (value < min)
        return min;
    if (value > max)
        return max;
    return value;
}

// =====================================================================
// FUNÇÃO: check_collision
// Descrição: Verifica se a nova posição (new_x, new_y) do jogador colide com uma parede.
// Utiliza um “raio de colisão” e verifica células vizinhas.
// =====================================================================
int check_collision(t_data *data, double new_x, double new_y)
{
    double collision_radius = 0.2; // Define o raio de colisão (pode ser ajustado)
    int cell_min_x = (int)floor(new_x) - 1;
    int cell_max_x = (int)floor(new_x) + 1;
    int cell_min_y = (int)floor(new_y) - 1;
    int cell_max_y = (int)floor(new_y) + 1;
    int i, j;
    
    // Verifica as células próximas à nova posição
    for (i = cell_min_y; i <= cell_max_y; i++)
    {
        for (j = cell_min_x; j <= cell_max_x; j++)
        {
            // Garante que as células estão dentro dos limites do mapa
            if (i < 0 || j < 0 || i >= data->map_height || j >= data->map_width)
                continue;
            // Se a célula for uma parede ('1'), verifica a colisão
            if (data->mapa[i][j] == '1')
            {
                // Calcula o ponto mais próximo dentro da célula (entre j e j+1, i e i+1)
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

// =====================================================================
// FUNÇÃO: mover_jogador
// Descrição: Atualiza a posição do jogador de acordo com as teclas pressionadas,
// realizando a movimentação (frente, trás, lateral) e verificação de colisão.
// =====================================================================
void mover_jogador(t_data *data)
{
    double move_speed = 0.05;      // Velocidade de movimento
    double rotation_speed = 0.04;  // Velocidade de rotação
    double new_x, new_y;

    // Rotaciona o jogador se as teclas de rotação estiverem pressionadas
    if (data->keys.left)
        data->player.angle -= rotation_speed;
    if (data->keys.right)
        data->player.angle += rotation_speed;

    // Garante que o ângulo do jogador esteja sempre entre 0 e 2π
    if (data->player.angle < 0)
        data->player.angle += 2 * M_PI;
    if (data->player.angle > 2 * M_PI)
        data->player.angle -= 2 * M_PI;

    // Movimento para frente (tecla W)
    if (data->keys.w)
    {
        new_x = data->player.x + cos(data->player.angle) * move_speed;
        new_y = data->player.y + sin(data->player.angle) * move_speed;
        // Só move se não houver colisão na nova posição
        if (!check_collision(data, new_x, new_y))
        {
            data->player.x = new_x;
            data->player.y = new_y;
        }
    }
    
    // Movimento para trás (tecla S)
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
    
    // Movimento para a direita (deslocamento lateral - tecla D)
    if (data->keys.d)
    {
        // Usa o ângulo do jogador mais 90° para calcular o deslocamento lateral
        new_x = data->player.x + cos(data->player.angle + M_PI_2) * move_speed;
        new_y = data->player.y + sin(data->player.angle + M_PI_2) * move_speed;
        if (!check_collision(data, new_x, new_y))
        {
            data->player.x = new_x;
            data->player.y = new_y;
        }
    }
    
    // Movimento para a esquerda (deslocamento lateral - tecla A)
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

// =====================================================================
// FUNÇÃO: key_press
// Descrição: Callback para pressionamento de tecla. Define o estado de cada tecla conforme for pressionada.
// =====================================================================
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

// =====================================================================
// FUNÇÃO: key_release
// Descrição: Callback para liberação de tecla. Atualiza o estado das teclas quando soltas.
// =====================================================================
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

// =====================================================================
// FUNÇÃO: desenhar_mapa
// Descrição: Desenha uma representação 2D do mapa na tela (mini mapa) e posiciona o jogador.
// =====================================================================
void desenhar_mapa(t_data *data)
{
    int x, y, i, j;

    // Percorre cada célula do mapa
    for (y = 0; data->mapa[y] != NULL; y++)
    {
        for (x = 0; data->mapa[y][x] != '\0'; x++)
        {
            // Desenha cada tile (célula) com tamanho definido em TILE_SIZE
            for (i = 0; i < TILE_SIZE; i++)
            {
                for (j = 0; j < TILE_SIZE; j++)
                {
                    if (x * TILE_SIZE + i >= 0 && x * TILE_SIZE + i < NOVA_LARGURA &&
                        y * TILE_SIZE + j >= 0 && y * TILE_SIZE + j < NOVA_ALTURA)
                    {
                        // Se a célula for parede ('1'), pinta de branco
                        if (data->mapa[y][x] == '1')
                            my_mlx_pixel_put(data, x * TILE_SIZE + i, y * TILE_SIZE + j, 0xFFFFFF);
                        // Se for espaço vazio ('0'), pinta de cinza
                        else if (data->mapa[y][x] == '0')
                            my_mlx_pixel_put(data, x * TILE_SIZE + i, y * TILE_SIZE + j, 0x808080);
                    }
                }
            }
        }
    }
    // Desenha o jogador no mini mapa com uma cor distinta (laranja)
    for (i = 0; i < TILE_SIZE / 2; i++)
    {
        for (j = 0; j < TILE_SIZE / 2; j++)
        {
            my_mlx_pixel_put(data, data->player.x * TILE_SIZE + i, data->player.y * TILE_SIZE + j, 0xFFA500);
        }
    }
}

// =====================================================================
// FUNÇÃO: render_scene
// Descrição: Função principal de renderização chamada pelo loop do MLX.
// Atualiza o fundo (chão e teto), executa o raycasting para renderizar as paredes, 
// desenha a imagem na janela, movimenta o jogador e exibe o mini mapa.
// =====================================================================
int render_scene(void *param)
{
    t_data *data = (t_data *)param;

    pintar_chao_teto(data);                           // Atualiza o fundo
    raycast(data);                                    // Realiza o raycasting e renderiza as paredes
    mlx_put_image_to_window(data->mlx, data->win, data->img, 0, 0); // Exibe a imagem renderizada na janela
    mover_jogador(data);                              // Atualiza a posição do jogador

    desenhar_mapa(data);                              // Desenha o mini mapa na tela

    return 0;
}

// =====================================================================
// FUNÇÃO: debug_print_map
// Descrição: Imprime o mapa no console para fins de depuração.
// =====================================================================
void debug_print_map(t_data *data)
{
    printf("Imprimindo mapa:\n");
    for (int y = 0; y < data->map_height; y++)
    {
        printf("[%d] %s\n", y, data->mapa[y]);
    }
}

// =====================================================================
// FUNÇÃO: main
// Descrição: Função principal que inicializa os recursos, carrega o mapa, texturas,
// configura callbacks e inicia o loop principal do MLX.
// =====================================================================
int main(int argc, char **argv)
{
    t_data data;
    memset(&data, 0, sizeof(data));
    
    if (argc != 2)
    {
        fprintf(stderr, "Usage: ./cub3d <mapa>\n");
        return 1;
    }

    // Inicializa o MLX
    data.mlx = mlx_init();
    if (!data.mlx)
    {
        fprintf(stderr, "Erro ao iniciar o MLX\n");
        return 1;
    }
    
    // Cria a janela com as dimensões definidas
    data.win = mlx_new_window(data.mlx, NOVA_LARGURA, NOVA_ALTURA, "Cub3D");
    if (!data.win)
    {
        fprintf(stderr, "Erro ao criar a janela\n");
        return 1;
    }
    
    // Cria uma imagem para renderização off-screen
    data.img = mlx_new_image(data.mlx, NOVA_LARGURA, NOVA_ALTURA);
    if (!data.img)
    {
        fprintf(stderr, "Erro ao criar a imagem\n");
        return 1;
    }
    
    // Obtém o endereço dos pixels da imagem para manipulação direta
    data.addr = mlx_get_data_addr(data.img, &data.bits_per_pixel,
                                  &data.line_length, &data.endian);
    if (!data.addr)
    {
        fprintf(stderr, "Erro ao obter endereço da imagem\n");
        return 1;
    }
    
    printf("Image created: bpp=%d, line_length=%d, endian=%d\n",
           data.bits_per_pixel, data.line_length, data.endian);
    
    // Carrega o mapa a partir do arquivo fornecido
    data.mapa = ler_mapa(argv[1], &data);
    if (!data.mapa)
    {
        fprintf(stderr, "Erro ao carregar o mapa\n");
        return 1;
    }

    // Verifica se os caminhos das texturas foram lidos corretamente
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

    // Carrega cada textura utilizando os caminhos lidos do mapa
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

    // Imprime o mapa carregado no console para depuração
    printf("Mapa carregado:\n");
    for (int y = 0; y < data.map_height; y++)
    {
        printf("%s\n", data.mapa[y]);
    }
    
    // Encontra a posição e orientação inicial do jogador no mapa
    encontrar_jogador(&data);
    printf("Jogador encontrado em: %.2f, %.2f, ângulo: %.2f\n",
           data.player.x, data.player.y, data.player.angle);
    
    // Configura os hooks para eventos do teclado e fechamento da janelaFunções trigonométricas como cos (cosseno) e sin (seno) são fundamentais para trabalhar com ângulos e direções em um espaço 2D (e por extensão, em projeções 3D via raycasting). Além disso, a constante π (pi) é a base para medir ângulos em radianos. Veja os pontos principais:

Conversão de ângulos em vetores:
Em um círculo unitário, dado um ângulo θ, o cosseno de θ fornece a componente horizontal (x) e o seno fornece a componente vertical (y). Assim, para calcular a direção de um raio ou o deslocamento do jogador, usamos:

dir_x = cos(θ)
dir_y = sin(θ)
Essa conversão é essencial para transformar um ângulo (por exemplo, a direção em que o jogador está olhando) em um vetor que indica direção e sentido.
Uso do π (pi):
O valor de π (aproximadamente 3.14159) é utilizado porque a matemática dos círculos, das rotações e das funções trigonométricas está baseada nele. Em nosso código, π é usado para definir ângulos padrão (por exemplo, para orientar o jogador em direções cardeais) e para garantir que os cálculos com funções trigonométricas sejam feitos em radianos, que é a unidade padrão na maioria das bibliotecas matemáticas.

Aplicação no Raycasting:
No raycasting, para cada coluna de pixels da tela, o código calcula o ângulo do raio relativo ao ângulo do jogador. Com esse ângulo, as funções cos e sin geram o vetor de direção do raio, permitindo que o algoritmo avance célula a célula no mapa e determine onde o raio encontra uma parede. Isso permite calcular distâncias e, consequentemente, a altura das paredes renderizadas, criando a sensação de profundidade.

Movimentação do Jogador:
Funções trigonométricas também são usadas para atualizar a posição do jogador. Ao pressionar a tecla para mover para frente, por exemplo, o código calcula a nova posição somando cos(ângulo) e sin(ângulo) multiplicados pela velocidade de movimento, traduzindo a direção em deslocamento real no mapa.

Em resumo, cos e sin convertem ângulos em vetores de movimento, enquanto π é usado para definir corretamente os ângulos (em radianos) para esses cálculos. Essas operações são essenciais para simular a perspectiva e a movimentação em jogos baseados em raycasting.
    mlx_hook(data.win, 2, 1L << 0, key_press, &data);
    mlx_hook(data.win, 3, 1L << 1, key_release, &data);
    mlx_hook(data.win, 17, 0, fechar_janela, &data);
    // Define a função de renderização que será chamada a cada frame
    mlx_loop_hook(data.mlx, render_scene, &data);
    // Inicia o loop principal do MLX
    mlx_loop(data.mlx);
    
    return 0;
}

Cálculos e fórmulas: Por exemplo, no raycast explicamos como se calcula as distâncias incrementais, a distância perpendicular para evitar distorção, e o mapeamento da textura.
Lógica de movimentação: Na função mover_jogador, detalhamos como as posições são atualizadas usando funções trigonométricas e como se evita colisões.
Leitura do mapa: Na função ler_mapa, comentamos a dupla passagem para ler as configurações e depois as linhas do mapa, além do tratamento de tabs.
Essa versão comentada deve ajudar a entender cada parte e a lógica por trás das fórmulas utilizadas. Se precisar de mais detalhes ou esclarecimentos sobre alguma parte específica, sinta-se à vontade para perguntar!

Funções trigonométricas como cos (cosseno) e sin (seno) são fundamentais para trabalhar com ângulos e direções em um espaço 2D (e por extensão, em projeções 3D via raycasting). Além disso, a constante π (pi) é a base para medir ângulos em radianos. Veja os pontos principais:

Conversão de ângulos em vetores:
Em um círculo unitário, dado um ângulo θ, o cosseno de θ fornece a componente horizontal (x) e o seno fornece a componente vertical (y). Assim, para calcular a direção de um raio ou o deslocamento do jogador, usamos:

dir_x = cos(θ)
dir_y = sin(θ)
Essa conversão é essencial para transformar um ângulo (por exemplo, a direção em que o jogador está olhando) em um vetor que indica direção e sentido.
Uso do π (pi):
O valor de π (aproximadamente 3.14159) é utilizado porque a matemática dos círculos, das rotações e das funções trigonométricas está baseada nele. Em nosso código, π é usado para definir ângulos padrão (por exemplo, para orientar o jogador em direções cardeais) e para garantir que os cálculos com funções trigonométricas sejam feitos em radianos, que é a unidade padrão na maioria das bibliotecas matemáticas.

Aplicação no Raycasting:
No raycasting, para cada coluna de pixels da tela, o código calcula o ângulo do raio relativo ao ângulo do jogador. Com esse ângulo, as funções cos e sin geram o vetor de direção do raio, permitindo que o algoritmo avance célula a célula no mapa e determine onde o raio encontra uma parede. Isso permite calcular distâncias e, consequentemente, a altura das paredes renderizadas, criando a sensação de profundidade.

Movimentação do Jogador:
Funções trigonométricas também são usadas para atualizar a posição do jogador. Ao pressionar a tecla para mover para frente, por exemplo, o código calcula a nova posição somando cos(ângulo) e sin(ângulo) multiplicados pela velocidade de movimento, traduzindo a direção em deslocamento real no mapa.

Em resumo, cos e sin convertem ângulos em vetores de movimento, enquanto π é usado para definir corretamente os ângulos (em radianos) para esses cálculos. Essas operações são essenciais para simular a perspectiva e a movimentação em jogos baseados em raycasting.

