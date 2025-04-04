/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   texturas.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lnzila <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 12:09:39 by lnzila            #+#    #+#             */
/*   Updated: 2025/03/24 12:10:25 by lnzila           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"


void	carregar_textura(void *mlx, t_texture *texture,
		const char *base_dir, const char *nome_textura)
{
	char	caminho_completo[1024];

	if (base_dir == NULL || base_dir[0] == '\0')
		snprintf(caminho_completo, sizeof(caminho_completo),
			"%s", nome_textura);
	else
		snprintf(caminho_completo,
			sizeof(caminho_completo), "%s/%s", base_dir, nome_textura);
	texture->img = mlx_xpm_file_to_image(mlx,
			caminho_completo, &texture->width, &texture->height);
	if (!texture->img)
		printf("Error: ao carregar a textura: %s\n", caminho_completo);
	else
		texture->addr = mlx_get_data_addr(texture->img, &texture->bpp,
				&texture->line_length, &texture->endian);
}

void	carregar_texturas(void *mlx, t_texture *textures,
		char *cub_file_path, char **paths)
{
	int	i;

	i = 0;
	while (i < 4)
	{
		carregar_textura(mlx, &textures[i], cub_file_path, paths[i]);
		i++;
	}
}

// Carrega as texturas
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


void	carregar_cor(char *linha, int *r, int *g, int *b)
{
	sscanf(linha, "%d,%d,%d", r, g, b);
}
