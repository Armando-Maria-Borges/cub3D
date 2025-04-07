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

/*
static size_t	ft_strlcpy(char *dst, const char *src, size_t size)
{
	size_t	i;

	i = 0;
	if (size > 0)
	{
		while (src[i] && i < size - 1)
		{
			dst[i] = src[i];
			i++;
		}
		dst[i] = '\0';
	}
	while (src[i])
		i++;
	return (i);
}

static size_t	ft_strlcat(char *dst, const char *src, size_t size)
{
	size_t	dst_len;
	size_t	src_len;
	size_t	i;

	dst_len = 0;
	while (dst[dst_len] && dst_len < size)
		dst_len++;
	src_len = 0;
	while (src[src_len])
		src_len++;
	if (dst_len >= size)
		return (dst_len + src_len);
	i = 0;
	while (src[i] && (dst_len + i) < (size - 1))
	{
		dst[dst_len + i] = src[i];
		i++;
	}
	dst[dst_len + i] = '\0';
	return (dst_len + src_len);
}

static int	ft_atoi(const char *str)
{
	int	result;
	int	sign;

	result = 0;
	sign = 1;
	while (*str == ' ' || (*str >= 9 && *str <= 13))
		str++;
	if (*str == '-')
	{
		sign = -1;
		str++;
	}
	else if (*str == '+')
		str++;
	while (*str >= '0' && *str <= '9')
	{
		result = result * 10 + (*str - '0');
		str++;
	}
	return (sign * result);
}
*/
// Carrega textura
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

// Carrega cor
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
void	carregar_texturas(void *mlx, t_texture *textures, char *cub_file_path,
		char **paths)
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
