/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lnzila <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 08:56:15 by lnzila            #+#    #+#             */
/*   Updated: 2025/03/18 08:56:19 by lnzila           ###   ########.fr       */
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

# define TILE_SIZE 10
# define NOVA_LARGURA 1400
# define NOVA_ALTURA 1060
# define FOV 1.309
# define NUM_RAYS NOVA_LARGURA
# define MAX_DEPTH 20

# define KEY_W 119
# define KEY_A 97
# define KEY_S 115
# define KEY_D 100
# define KEY_ESC 65307
# define KEY_LEFT 65361
# define KEY_RIGHT 65363
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

void				liberar_mapa(char **mapa, int altura);
int					processar_primeira_passagem(t_data *data,
						t_map_data *map_data);
char				**validar_e_alocar(t_data *data, t_map_data *map_data);
int					processar_segunda_passagem(t_data *data,
						t_map_data *map_data, char *arquivo);

char				**ler_mapa(char *arquivo, t_data *data,
						t_map_data *map_data);
int					ler_mapa1(char *arquivo, t_data *data,
						t_map_data *map_data);

int					verifica_linha_cor(const char *str, int *r, int *g, int *b);
size_t				ft_strcspn(const char *s, const char *reject);

int					read_line(int fd, char *linha, int max_len);

void				my_mlx_pixel_put(t_data *data, int x, int y, int color);
unsigned int		get_pixel(t_texture *tex, int x, int y);
void				desenhar_mapa(t_data *data);
int					render_scene(void *param);
void				carregar_texturas(void *mlx, t_texture *textures,
						char *cub_file_path, char **paths);
void				carregar_cor(char *linha, int *r, int *g, int *b);
void				raycast(t_data *data);
void				mover_jogador(t_data *data);
int					key_press(int keycode, void *param);
int					key_release(int keycode, void *param);
int					check_collision(t_data *data, double new_x, double new_y);
int					encontrar_jogador(t_data *data);
int					fechar_janela(void *param);
void				pintar_chao_teto(t_data *data);
void				pintar_janela(t_data *data);
unsigned int		cria_trgb(int t, int r, int g, int b);
void				rotacionar_jogador(t_data *data);
void				carregar_textura(void *mlx, t_texture *texture,
						const char *diretorio, const char *nome_textura);
void				carregar_cor(char *linha, int *r, int *g, int *b);
char				*substituir_tabs(const char *linha);

int					check_number_position(t_data *data);
int					check_other_cracter(t_data *data);

#endif
