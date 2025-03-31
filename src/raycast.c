/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycast.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lnzila <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 12:51:05 by lnzila            #+#    #+#             */
/*   Updated: 2025/03/29 18:02:28 by lnzila           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

#include <math.h>

// Calcula o ângulo e direção do raio
static void	calc_raio(t_data *data, int x, t_ray *ray)
{
	double	delta_angle;

	delta_angle = FOV / (double)NUM_RAYS;
	ray->ray_angle = data->player.angle - (FOV / 2) + x * delta_angle;
	ray->dir_x = cos(ray->ray_angle);
	ray->dir_y = sin(ray->ray_angle);
}

// Inicializa variáveis de distância e passo (parte 1)
static void	init_distancias_1(t_data *data, t_ray *ray)
{
	double	div_x;
	double	div_y;
	
	ray->map_x = (int)data->player.x;
	ray->map_y = (int)data->player.y;
	div_x = 1 / ray->dir_x;
	div_y = 1 / ray->dir_y;
	if (ray->dir_x == 0)
		ray->delta_dist_x = 1e30;
	else
		ray->delta_dist_x = fabs(div_x);
	if (ray->dir_y == 0)
		ray->delta_dist_y = 1e30;
	else
		ray->delta_dist_y = fabs(div_y);
}

// Inicializa variáveis de distância e passo (parte 2)
static void	init_distancias_2(t_ray *ray)
{
	ray->step_x = (ray->dir_x < 0) ? -1 : 1;
	ray->step_y = (ray->dir_y < 0) ? -1 : 1;
}

// Calcula as distâncias iniciais até as paredes
static void	calc_distancias_iniciais(t_data *data, t_ray *ray)
{
	double	px;
	double	py;

	px = data->player.x;
	py = data->player.y;
	ray->side_dist_x = (ray->dir_x < 0) ? (px - ray->map_x) * ray->delta_dist_x : (ray->map_x + 1.0 - px) * ray->delta_dist_x;
	ray->side_dist_y = (ray->dir_y < 0) ? (py - ray->map_y) * ray->delta_dist_y : (ray->map_y + 1.0 - py) * ray->delta_dist_y;
}

// Executa o DDA até encontrar uma parede
static void	dda(t_data *data, t_ray *ray)
{
	while (!ray->hit)
	{
		if (ray->side_dist_x < ray->side_dist_y)
		{
			ray->side_dist_x += ray->delta_dist_x;
			ray->map_x += ray->step_x;
			ray->side = 0;
		}
		else
		{
			ray->side_dist_y += ray->delta_dist_y;
			ray->map_y += ray->step_y;
			ray->side = 1;
		}
		if (ray->map_y >= 0 && ray->map_y < data->map_height && ray->map_x >= 0 && ray->map_x < data->map_width && data->mapa[ray->map_y][ray->map_x] == '1')
			ray->hit = 1;
	}
}

// Calcula a distância perpendicular à parede
static void	calc_dist_perp(t_data *data, t_ray *ray)
{
	double	px;
	double	py;

	px = data->player.x;
	py = data->player.y;
	if (ray->side == 0)
		ray->perp_wall_dist = (ray->map_x - px + (1 - ray->step_x) / 2.0) / ray->dir_x;
	else
		ray->perp_wall_dist = (ray->map_y - py + (1 - ray->step_y) / 2.0) / ray->dir_y;
	if (ray->perp_wall_dist < 0.1)
		ray->perp_wall_dist = 0.1;
}

// Calcula altura da parede e limites de desenho
static void	calc_altura_parede(t_ray *ray)
{
	ray->wall_height = (int)(NOVA_ALTURA / ray->perp_wall_dist);
	ray->start = (NOVA_ALTURA / 2) - (ray->wall_height / 2);
	ray->end = (NOVA_ALTURA / 2) + (ray->wall_height / 2);
	if (ray->start < 0)
		ray->start = 0;
	if (ray->end >= NOVA_ALTURA)
		ray->end = NOVA_ALTURA - 1;
}

// Calcula a coordenada x da textura
static void	calc_textura_x(t_data *data, t_ray *ray)
{
	double	wall_x;

	if (ray->side == 0)
		wall_x = data->player.y + ray->perp_wall_dist * ray->dir_y;
	else
		wall_x = data->player.x + ray->perp_wall_dist * ray->dir_x;
	wall_x -= floor(wall_x);
	ray->tex_x = (int)(wall_x * (double)(data->textures[0].width));
	if (ray->tex_x < 0)
		ray->tex_x = 0;
	if (ray->tex_x >= data->textures[0].width)
		ray->tex_x = data->textures[0].width - 1;
	if (ray->side == 0 && ray->dir_x > 0)
		ray->tex_x = data->textures[0].width - ray->tex_x - 1;
	if (ray->side == 1 && ray->dir_y < 0)
		ray->tex_x = data->textures[0].width - ray->tex_x - 1;
}

// Seleciona a textura com base na direção
static void	seleciona_textura(t_data *data, t_ray *ray)
{
	if (ray->side == 0 && ray->step_x < 0)
		ray->texture = &data->textures[0]; // Norte
	else if (ray->side == 0 && ray->step_x > 0)
		ray->texture = &data->textures[1]; // Sul
	else if (ray->side == 1 && ray->step_y < 0)
		ray->texture = &data->textures[2]; // Oeste
	else
		ray->texture = &data->textures[3]; // Leste
}

// Desenha a parede com textura
static void	desenha_parede(t_data *data, int x, t_ray *ray)
{
	int	y;
	int	tex_y;

	y = ray->start;
	while (y < ray->end)
	{
		int	d;

		d = y * 256 - NOVA_ALTURA * 128 + (ray->end - ray->start) * 128;
		tex_y = ((d * ray->texture->height) / (ray->end - ray->start)) / 256;
		if (tex_y < 0)
			tex_y = 0;
		if (tex_y >= ray->texture->height)
			tex_y = ray->texture->height - 1;
		my_mlx_pixel_put(data, x, y, get_pixel(ray->texture, ray->tex_x, tex_y));
		y++;
	}
}

// Desenha o teto
static void	desenha_teto(t_data *data, int x, int start)
{
	int	y;

	y = 0;
	while (y < start)
	{
		my_mlx_pixel_put(data, x, y, data->ceiling_color);
		y++;
	}
}

// Desenha o chão
static void	desenha_chao(t_data *data, int x, int end)
{
	int	y;
	unsigned int	r;
	unsigned int	g;
	unsigned int	b;

	r = (data->floor_color >> 16) & 0xFF;
	g = (data->floor_color >> 8) & 0xFF;
	b = data->floor_color & 0xFF;
	y = end;
	while (y < NOVA_ALTURA)
	{
		my_mlx_pixel_put(data, x, y, cria_trgb(0, r, g, b));
		y++;
	}
}

// Configura o raio (parte 1)
static void	config_raio_1(t_data *data, int x, t_ray *ray)
{
	calc_raio(data, x, ray);
	init_distancias_1(data, ray);
	init_distancias_2(ray);
	calc_distancias_iniciais(data, ray);
}

// Configura o raio (parte 2)
static void	config_raio_2(t_data *data, t_ray *ray)
{
	ray->hit = 0;
	dda(data, ray);
	calc_dist_perp(data, ray);
	calc_altura_parede(ray);
}

// Configura o raio (parte 3)
static void	config_raio_3(t_data *data, t_ray *ray)
{
	calc_textura_x(data, ray);
	seleciona_textura(data, ray);
}

// Desenha o raio completo
static void	desenha_raio(t_data *data, int x, t_ray *ray)
{
	desenha_parede(data, x, ray);
	desenha_teto(data, x, ray->start);
	desenha_chao(data, x, ray->end);
}

// Função principal de raycasting para um raio
static void	raycast_raio(t_data *data, int x)
{
	t_ray	ray;

	config_raio_1(data, x, &ray);
	config_raio_2(data, &ray);
	config_raio_3(data, &ray);
	desenha_raio(data, x, &ray);
}

// Função principal
void	raycast(t_data *data)
{
	int	x;

	x = 0;
	while (x < NUM_RAYS)
	{
		raycast_raio(data, x);
		x++;
	}
}
