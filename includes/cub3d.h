/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aborges <aborges@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 08:56:15 by lnzila            #+#    #+#             */
/*   Updated: 2025/04/13 20:56:32 by aborges          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUB3D_H
# define CUB3D_H

# include "libft.h"
# include "mlx.h"
# include <errno.h>
# include <fcntl.h>
# include <libgen.h>
# include <limits.h>
# include <math.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>

//# define TILE_SIZE 10
# define NOVA_LARGURA 1400
# define NOVA_ALTURA 1060
//# define FOV 1.309
# define TILE_SIZE 7
# define FOV 1.0
# define NUM_RAYS NOVA_LARGURA
# define MAX_DEPTH 20

# define KEY_W 119
# define KEY_A 97
# define KEY_S 115
# define KEY_D 100
# define KEY_ESC 65307
# define KEY_LEFT 65361
# define KEY_RIGHT 65363
# define KEY_CIMA 38
# define KEY_BAIXO 40
# define TAB_SIZE 4

typedef struct s_player
{
	double			x;
	double			y;
	double			angle;
}					t_player;

typedef struct s_key_state
{
	int				w;
	int				a;
	int				s;
	int				d;
	int				left;
	int				right;
}					t_key_state;

typedef struct s_texture
{
	void			*img;
	char			*addr;
	int				bpp;
	int				width;
	int				height;
	int				bits_per_pixel;
	int				line_length;
	int				endian;
}					t_texture;

typedef struct s_data
{
	void			*mlx;
	void			*win;
	char			**mapa;
	char			*texture_paths[4];
	void			*img;
	char			*addr;
	unsigned int	floor_color;
	unsigned int	ceiling_color;
	int				bits_per_pixel;
	int				line_length;
	int				endian;
	int				map_width;
	int				map_height;
	t_texture		textures[4];
	t_player		player;
	t_key_state		keys;
}					t_data;

typedef struct s_map_data
{
	int				f;
	char			**mapa;
	int				flags[6];
	int				config_count;
	int				map_iniciado;
}					t_map_data;

typedef struct s_ray
{
	int				map_x;
	int				map_y;
	double			ray_angle;
	double			dir_x;
	double			dir_y;
	double			delta_dist_x;
	double			delta_dist_y;
	int				step_x;
	int				step_y;
	double			side_dist_x;
	double			side_dist_y;
	int				hit;
	int				side;
	double			perp_wall_dist;
	int				wall_height;
	int				start;
	int				end;
	int				tex_x;
	double			wall_x;
	t_texture		*texture;
}					t_ray;

// Main
int					init_mlx(t_data *data);
void				print_map(t_data *data);
void				start_loop(t_data *data);
void				init_data(t_data *data);

// Player
int					check_collision(t_data *data, double new_x, double new_y);
double				clamp(double value, double min, double max);
void				calcular_minimos(double new_x, double new_y, int *min_x,
						int *min_y);
void				calcular_maximos(double new_x, double new_y, int *max_x,
						int *max_y);
double				calcular_distancia(double new_x, double new_y, int i,
						int j);
int					verificar_colisao_celula(t_data *data, double new_x,
						double new_y, int *var);

// Ler mapa
void				liberar_mapa(char **mapa, int altura);
char				**validar_e_alocar(t_data *data, t_map_data *map_data);
int					processar_segunda_passagem(t_data *data,
						t_map_data *map_data, char *arquivo);
char				**ler_mapa(char *arquivo, t_data *data,
						t_map_data *map_data);
int					ler_mapa1(char *arquivo, t_data *data,
						t_map_data *map_data);
int					verifica_linha_cor(const char *str, int *r, int *g, int *b);
size_t				ft_strcspn(const char *s, const char *reject);

int					processar_segunda_passagem1(char *linha, int *var);
int					processar_segunda(char *linha, int *var,
						t_map_data *map_data);
int					abrir_arquivo(t_map_data *map_data, char *arquivo);
int					segunda_passagem_ciclo(t_map_data *map_data, int *var,
						char *linha, char *linha_corrigida);
int					processar_segunda_passagem(t_data *data,
						t_map_data *map_data, char *arquivo);
int					processar_segunda_passagem2(char *linha, int *var,
						t_map_data *map_data);
int					nova_linha_corrigida(char *linha_corrigida, char *linha,
						t_map_data *map_data, int *var);
int					processar_segunda_passagem3(char *linha_corrigida,
						t_map_data *map_data, int *var);
int					processar_segunda_passagem4(t_data *data,
						char *linha_corrigida, t_map_data *map_data, int *var);

int					read_line(int fd, char *linha, int max_len);
int					ft_chao(t_data *data, t_map_data *map_data, char *linha);
int					ft_teto(t_data *data, t_map_data *map_data, char *linha);
int					chekagem_map_concluido(t_map_data *map_data, t_data *data);
int					processar_primeira_passagem(t_data *data,
						t_map_data *map_data);

int					we_ea(t_data *data, t_map_data *map_data, char *linha);
int					no_so(t_data *data, t_map_data *map_data, char *linha);
int					ft_processar_cordenadas(int *valor_return);
int					ft_return_cordenadas(t_data *data, t_map_data *map_data,
						char *linha);

void				my_mlx_pixel_put(t_data *data, int x, int y, int color);
unsigned int		get_pixel(t_texture *tex, int x, int y);
void				desenhar_mapa(t_data *data);
int					render_scene(void *param);
void				carregar_cor(char *linha, int *r, int *g, int *b);
void				raycast(t_data *data);
void				mover_jogador(t_data *data);

// Funções para as teclas
int					key_press(int keycode, void *param);
int					key_release(int keycode, void *param);
int					encontrar_jogador(t_data *data);
void				setup_hooks(t_data *data);
int					encontrar_jogador(t_data *data);

// Funções para janela
int					create_window(t_data *data);
int					fechar_janela(void *param);
void				pintar_janela(t_data *data);
void				pintar_chao_teto(t_data *data);

// Funções para imagem
int					create_image(t_data *data);
int					get_image_addr(t_data *data);

// Funções para validações
int					check_args(int ac, char **av);
int					load_map(char *arquivo, t_data *data, t_map_data *map_data);

int					validate_positions(t_data *data);
int					validate_characters(t_data *data);
int					validate_textures(t_data *data);

unsigned int		cria_trgb(int t, int r, int g, int b);
void				rotacionar_jogador(t_data *data);
void				carregar_cor(char *linha, int *r, int *g, int *b);
char				*substituir_tabs(const char *linha);

int					check_number_position(t_data *data);
int					check_other_cracter(t_data *data);

// Funções de raycasting
void				calc_side_dist(t_data *data, t_ray *ray);
void				calc_distancias_iniciais(t_data *data, t_ray *ray);
void				calc_wall_height(t_data *data, t_ray *ray);
void				calc_wall_x(t_data *data, t_ray *ray);
void				calc_perp_dist(t_data *data, t_ray *ray);
void				calc_tex_x(t_data *data, t_ray *ray);
void				setup_ray_2(t_data *data, t_ray *ray);
void				exec_dda(t_data *data, t_ray *ray);
void				init_delta_dist(t_ray *ray);

void				draw_wall(t_data *data, int x, t_ray *ray);
void				draw_ceiling(t_data *data, int x, int start);
void				draw_floor(t_data *data, int x, int end);
void				draw_ray(t_data *data, int x, t_ray *ray);

void				setup_ray_1(t_data *data, int x, t_ray *ray);
void				setup_ray_3(t_data *data, t_ray *ray);
void				calc_raio(t_data *data, int x, t_ray *ray);
void				process_ray(t_data *data, int x);
void				select_texture(t_data *data, t_ray *ray);
void				init_passos(t_ray *ray);
void				init_mapa(t_data *data, t_ray *ray);

//	Funções de Texturas
int					load_textures(t_data *data);
void				carregar_textura(void *mlx, t_texture *texture,
						const char *diretorio, const char *nome_textura);
void				carregar_texturas(void *mlx, t_texture *textures,
						char *cub_file_path, char **paths);

#endif
