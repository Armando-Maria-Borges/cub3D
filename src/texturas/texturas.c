/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   texturas.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aborges <aborges@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 12:09:39 by lnzila            #+#    #+#             */
/*   Updated: 2025/04/28 12:31:20 by aborges          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	select_texture(t_data *data, t_ray *ray)
{
	if (ray->side == 0 && ray->step_x < 0)
		ray->texture = &data->textures[0];
	else if (ray->side == 0 && ray->step_x > 0)
		ray->texture = &data->textures[1];
	else if (ray->side == 1 && ray->step_y < 0)
		ray->texture = &data->textures[2];
	else
		ray->texture = &data->textures[3];
}

void	carregar_textura(void *mlx, t_texture *texture, const char *base_dir,
		const char *nome_textura)
{
	char	caminho_completo[1024];

	caminho_completo[0] = '\0';
	if (base_dir == NULL || base_dir[0] == '\0')
		ft_strlcpy(caminho_completo, nome_textura, sizeof(caminho_completo));
	else
	{
		ft_strlcpy(caminho_completo, base_dir, sizeof(caminho_completo));
		ft_strlcat(caminho_completo, "/", sizeof(caminho_completo));
		ft_strlcat(caminho_completo, nome_textura, sizeof(caminho_completo));
	}
	texture->img = mlx_xpm_file_to_image(mlx, caminho_completo, &texture->width,
			&texture->height);
	if (!texture->img)
		printf("Error: ao carregar a textura: %s\n", caminho_completo);
	else
		texture->addr = mlx_get_data_addr(texture->img, &texture->bpp,
				&texture->line_length, &texture->endian);
}

void	carregar_cor(char *linha, int *r, int *g, int *b)
{
	char	*ptr;

	ptr = linha;
	*r = ft_atoi(ptr);
	while (*ptr && *ptr != ',')
		ptr++;
	if (*ptr == ',')
		ptr++;
	*g = ft_atoi(ptr);
	while (*ptr && *ptr != ',')
		ptr++;
	if (*ptr == ',')
		ptr++;
	*b = ft_atoi(ptr);
}

int	load_textures(t_data *data)
{
	int	i;

	i = 0;
	while (i < 4)
	{
		carregar_textura(data->mlx, &data->textures[i], "",
			data->texture_paths[i]);
		if (!data->textures[i].img || !data->textures[i].addr)
		{
			printf("Error!\n ao carregar textura %d\n", i);
			return (1);
		}
		printf("Textura %d carregada: %dx%d\n", i, data->textures[i].width,
			data->textures[i].height);
		i++;
	}
	return (0);
}
