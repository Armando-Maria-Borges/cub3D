/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lnzila <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 08:56:15 by lnzila            #+#    #+#             */
/*   Updated: 2025/03/25 08:10:46 by lnzila           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUB3D_H
# define CUB3D_H

# include "mlx.h"
# include <unistd.h>
# include <fcntl.h>
# include <stdlib.h>
# include <string.h>
# include <math.h>
# include <stdio.h>
# include "libft.h" 

# include <errno.h>
# include <libgen.h>
# include <limits.h>

# define TILE_SIZE 10
# define NOVA_LARGURA 1920
# define NOVA_ALTURA 1080
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
	double	x;
	double	y;
	double	angle;
}	t_player;

typedef struct s_key_state
{
	int	w;
	int	a;
	int	s;
	int	d;
	int	left;
	int	right;
}	t_key_state;

typedef struct s_texture
{
	void	*img;
	char	*addr;
	int		bpp;
	int		width;
	int		height;
	int		bits_per_pixel;
	int		line_length;
	int		endian;
}	t_texture;

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
}	t_data;

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
char				**ler_mapa(char *arquivo, t_data *data);
void				encontrar_jogador(t_data *data);
int					fechar_janela(void *param);
void				pintar_chao_teto(t_data *data);
void				pintar_janela(t_data *data);
unsigned int		cria_trgb(int t, int r, int g, int b);
void				rotacionar_jogador(t_data *data);
void				carregar_textura(void *mlx, t_texture *texture,
						const char *diretorio, const char *nome_textura);
void				carregar_cor(char *linha, int *r, int *g, int *b);
char				*substituir_tabs(const char *linha);
int					check_collision(t_data *data, double new_x, double new_y);
int					ft_read_line(int fd, char *buffer, int size);
void				ft_trim_newline(char *str);

int					contar_linhas_mapa(int fd, t_data *data, int *config_count);
char				**alocar_mapa(int height);
int     				eh_config_ou_vazio(char *linha);
int     				carregar_linhas_mapa(int fd, char **mapa, int height);
int					validar_config(char *linha, t_data *data, int *config_count);
int					validar_cores(char *linha, t_data *data, int *config_count);
int					validar_texturas(char *linha, t_data *data,
						int 	*config_count);
int					parse_color(const char *str, int *r, int *g, int *b);



#endif
