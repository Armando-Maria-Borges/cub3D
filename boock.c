
O método de raycasting é uma técnica de renderização que projeta raios em um ambiente 2D para simular uma visão tridimensional em tempo real. É amplamente utilizado em jogos 2.5D, como Wolfenstein 3D, e também serve como base para técnicas mais avançadas em gráficos 3D.


****************


1. Simular a Perspectiva 3D (Raycasting)
A principal técnica para criar uma perspectiva 3D em jogos como o seu é o raycasting. Essencialmente, o raycasting lança raios a partir da posição do jogador e determina onde o raio atinge o ambiente, e isso é usado para desenhar as paredes e o chão.

O raycasting envolve os seguintes passos básicos:

O jogador tem uma posição e uma direção para onde está olhando.
São lançados raios a partir da posição do jogador em ângulos variados.
Quando um raio colide com uma parede ou outro objeto, a distância até o objeto é calculada, o que é usado para determinar o quão longe a parede está.
O tamanho da parede é ajustado de acordo com a distância para criar uma ilusão de profundidade.
Aqui está um exemplo básico de como fazer isso em C com a MiniLibX:

2. Implementação Básica do Raycasting
Vou dar um exemplo simples de raycasting, onde a cena é desenhada em 2D, mas com a perspectiva 3D. Esse código vai ser uma base para você melhorar com o movimento.

Estrutura inicial do raycasting:
c
Copiar


// Estrutura do jogador
typedef struct s_player {
    float x;
    float y;
    float angle;
    float move_speed;
    float rot_speed;
} t_player;

typedef struct s_ray {
    float angle;
    float dist;
    int hit;
} t_ray;

// Função para desenhar o mapa e a perspectiva 3D
void desenhar_mapa(void *mlx, void *win, t_player *player, char **mapa) {
    int x, y;
    float ray_angle;
    t_ray ray;

    // Limpar a tela
    mlx_clear_window(mlx, win);

    // Raycasting
    for (x = 0; x < WIN_WIDTH; x++) {
        // Calcular o ângulo do raio
        ray_angle = player->angle - (FOV / 2) + ((float)x / WIN_WIDTH) * FOV;

        // Normalizar o ângulo
        ray_angle = fmod(ray_angle, 360.0);
        if (ray_angle < 0) ray_angle += 360.0;

        // Lançar o raio
        ray.dist = 0;
        ray.hit = 0;
        while (!ray.hit && ray.dist < 1000) {
            ray.dist += 0.1;
            // Aqui você deve checar se o raio colidiu com uma parede no mapa
            // Exemplo simples: se o raio estiver a uma distância x, consideramos uma colisão
            if (mapa[(int)(player->y + ray.dist * sin(ray_angle * PI / 180))][(int)(player->x + ray.dist * cos(ray_angle * PI / 180))] == '1') {
                ray.hit = 1;
            }
        }

        // Calcular a altura da parede
        int wall_height = (int)(WIN_HEIGHT / (ray.dist * cos(ray_angle - player->angle)));
        // Desenhar a parede (pode ser um quadrado simples ou você pode melhorar isso)
        for (y = (WIN_HEIGHT - wall_height) / 2; y < (WIN_HEIGHT + wall_height) / 2; y++) {
            mlx_pixel_put(mlx, win, x, y, 0xFFFFFF);  // Desenha o pixel da parede
        }
    }
}

// Função para movimentar o jogador
void mover_jogador(t_player *player, char **mapa) {
    if (key_is_pressed(KEY_W)) {
        player->x += player->move_speed * cos(player->angle * PI / 180);
        player->y += player->move_speed * sin(player->angle * PI / 180);
    }
    if (key_is_pressed(KEY_S)) {
        player->x -= player->move_speed * cos(player->angle * PI / 180);
        player->y -= player->move_speed * sin(player->angle * PI / 180);
    }
    if (key_is_pressed(KEY_A)) {
        player->x += player->move_speed * cos((player->angle - 90) * PI / 180);
        player->y += player->move_speed * sin((player->angle - 90) * PI / 180);
    }
    if (key_is_pressed(KEY_D)) {
        player->x -= player->move_speed * cos((player->angle - 90) * PI / 180);
        player->y -= player->move_speed * sin((player->angle - 90) * PI / 180);
    }
}

// Função para lidar com a rotação
void rotacionar_jogador(t_player *player) {
    if (key_is_pressed(KEY_LEFT)) {
        player->angle -= player->rot_speed;
    }
    if (key_is_pressed(KEY_RIGHT)) {
        player->angle += player->rot_speed;
    }
    player->angle = fmod(player->angle, 360.0);
    if (player->angle < 0) player->angle += 360.0;
}

int main(void) {
    void *mlx;
    void *win;
    t_player player;
    char *mapa[] = {
        "111111",
        "100001",
        "101101",
        "100001",
        "111111",
        NULL
    };

    mlx = mlx_init();
    if (!mlx) {
        write(2, "Erro ao inicializar MiniLibX\n", 29);
        return (1);
    }

    win = mlx_new_window(mlx, WIN_WIDTH, WIN_HEIGHT, "cub3D");
    if (!win) {
        write(2, "Erro ao criar janela\n", 21);
        return (1);
    }

    // Inicializar jogador
    player.x = 2.5;
    player.y = 2.5;
    player.angle = 0;  // Olhando para a direita
    player.move_speed = 0.1;
    player.rot_speed = 5.0;

    // Loop principal
    while (1) {
        mover_jogador(&player, mapa);
        rotacionar_jogador(&player);
        desenhar_mapa(mlx, win, &player, mapa);
        mlx_loop(mlx);
    }

    return 0;
}
Explicação:
Estrutura do Jogador (t_player): Esta estrutura mantém a posição (x, y) e a direção do jogador (angle), além da velocidade de movimento (move_speed) e a velocidade de rotação (rot_speed).

Raycasting (desenhar_mapa): A função desenhar_mapa simula o lançamento de raios a partir da posição do jogador, calculando a distância até a parede e desenhando as paredes na janela com base na distância.

Movimento (mover_jogador): A função mover_jogador altera a posição do jogador com base nas teclas pressionadas. O movimento é feito em relação à direção em que o jogador está olhando.

Rotação (rotacionar_jogador): A função rotacionar_jogador permite que o jogador rotacione sua visão, alterando o ângulo da direção.

3. Proximos Passos:
Aprimorar o Raycasting: O código acima é um exemplo básico. Você pode adicionar coisas como texturas de parede, sombras para melhorar a perspectiva e até mesmo objetos no mundo.
Interação: O jogador pode interagir com o mapa (abrindo portas, coletando itens, etc.).
Aprimorar a Física: Adicionar colisões e limites para o movimento do jogador.
Esse é apenas um ponto de partida, mas com isso você pode começar a adicionar mais detalhes e funcionalidades ao seu jogo!



Branco: 0xFFFFFF
Preto: 0x000000
Vermelho: 0xFF0000
Verde: 0x00FF00
Azul: 0x0000FF
Amarelo: 0xFFFF00
Ciano: 0x00FFFF
Magenta: 0xFF00FF
Cinza: 0x808080
Cinza Claro: 0xD3D3D3
Marrom: 0xA52A2A
Bege: 0xF5F5DC
Roxo: 0x800080
Laranja: 0xFFA500
Rosa: 0xFFC0CB
Verde Claro: 0x90EE90
Azul Claro: 0xADD8E6
Azul Marinho: 0x000080
Azul Turquesa: 0x40E0D0
Ouro: 0xFFD700
Prata: 0xC0C0C0
Turquesa: 0x48D1CC
Vermelho Escuro: 0x8B0000
Verde Escuro: 0x006400
Azul Escuro: 0x00008B
Pêssego: 0xFFDAB9
Lavanda: 0xE6E6FA
Coral: 0xFF7F50
Chocolate: 0xD2691E
Lima: 0x00FF00

Aqui estão as cores no formato x,x,x:

Branco: 255,255,255
Preto: 0,0,0
Vermelho: 255,0,0
Verde: 0,255,0
Azul: 0,0,255
Amarelo: 255,255,0
Ciano: 0,255,255
Magenta: 255,0,255
Cinza: 128,128,128
Cinza Claro: 211,211,211
Marrom: 165,42,42
Bege: 245,245,220
Roxo: 128,0,128
Laranja: 255,165,0
Rosa: 255,192,203
Verde Claro: 144,238,144
Azul Claro: 173,216,230
Azul Marinho: 0,0,128
Azul Turquesa: 64,224,208
Ouro: 255,215,0
Prata: 192,192,192
Turquesa: 72,209,204
Vermelho Escuro: 139,0,0
Verde Escuro: 0,100,0
Azul Escuro: 0,0,139
Pêssego: 255,218,185
Lavanda: 230,230,250
Coral: 255,127,80
Chocolate: 210,105,30
Lima: 0,255,0



https://imageresizer.com/pt
https://convertio.co/
//verificar se o jogador nao esta colidindo com a parede em mover jogador, atribuir uma //distancia.
//aumentar o tamanho das paredes (largura e comprimento).
//Aumentar a tela da janela.
//aplicar texturas.
//desenhar o mapa de acordo os parametros recomendados.



Passos para depuração
Antes de corrigir, tente rodar esses testes para entender onde está o erro:

Print do mapa carregado
Adicione este código após ler o mapa:

c
Copiar
Editar
for (int j = 0; j < data->map_height; j++)
    printf("Linha %d: [%s]\n", j, mapa[j]);
Isso garante que o mapa foi carregado corretamente.

Print das posições do jogador
No loop principal (onde você atualiza o movimento do jogador), adicione:

c
Copiar
Editar
printf("Player está em: x=%f, y=%f\n", data->player.x, data->player.y);
Se os valores saírem muito grandes ou negativos, o jogador pode estar saindo dos limites.

Print do que o jogador está pisando
No código que lida com o movimento, antes de processar a posição nova, adicione:

c
Copiar
Editar
printf("Tentando pisar em: mapa[%d][%d] = %c\n", (int)data->player.y, (int)data->player.x, data->mapa[(int)data->player.y][(int)data->player.x]);



void raycast(t_data *data)
{
    int x;
    double delta_angle = FOV / (double)NUM_RAYS;
    double ray_angle = data->player.angle - (FOV / 2);

    for (x = 0; x < NUM_RAYS; x++)
    {
        double dir_x = cos(ray_angle);
        double dir_y = sin(ray_angle);

        int map_x = (int)data->player.x;
        int map_y = (int)data->player.y;

        double delta_dist_x = (dir_x != 0) ? fabs(1 / dir_x) : 1e30;
        double delta_dist_y = (dir_y != 0) ? fabs(1 / dir_y) : 1e30;
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
                side = 0;
            }
            else
            {
                side_dist_y += delta_dist_y;
                map_y += step_y;
                side = 1;
            }

            if (map_y >= 0 && map_y < data->map_height && map_x >= 0 && map_x < data->map_width &&
                data->mapa[map_y][map_x] == '1')
            {
                hit = 1;
                perp_wall_dist = (side == 0) ? (map_x - data->player.x + (1 - step_x) / 2) / dir_x
                                             : (map_y - data->player.y + (1 - step_y) / 2) / dir_y;

                if (perp_wall_dist < 0.1) // Evita divisão por valores muito pequenos
                    perp_wall_dist = 0.1;
            }
        }

        // Aumentar a altura das paredes (multiplicador ajustável)
        int wall_height = (int)(NOVA_ALTURA / perp_wall_dist * 1.5);  // Ajuste fino do fator de altura

        int start = (NOVA_ALTURA / 2) - (wall_height / 2);
        int end = (NOVA_ALTURA / 2) + (wall_height / 2);

        if (start < 0) start = 0;
        if (end >= NOVA_ALTURA) end = NOVA_ALTURA - 1;

        int color = (side == 1) ? 0x7F7F7F : 0xFFFFFF;

        for (int y = start; y < end; y++)
        {
            my_mlx_pixel_put(data, x, y, color);
        }

        ray_angle += delta_angle;
    }
}


Para o projeto cub3D, as texturas das paredes (norte, sul, leste e oeste) 
devem ser imagens quadradas com dimensões que sejam potências de 2, 
como 64x64, 128x128, 256x256, 512x512 ou 1024x1024 pixels.

2048x2048