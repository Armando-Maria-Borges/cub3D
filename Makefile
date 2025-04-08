# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: aborges <aborges@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/01/27 12:16:34 by aborges           #+#    #+#              #
#    Updated: 2025/02/10 11:45:37 by aborges          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# Nome do executável
NAME = cub3D

# Compilador e flags
CC = cc
CFLAGS = -Wall -Wextra -Werror -Iincludes
MLX_FLAGS = -L$(MLXDIR) -lmlx -lX11 -lXext -lm -lGL
LIBFT_FLAGS = -L$(LIBFTDIR) -lft

# Diretórios
SRCS =	src/main/main.c					\
		src/main/imagens.c				\
		src/main/teclas.c				\
		src/main/validacao.c			\
		src/ler_mapa/map.c				\
		src/ler_mapa/map1.c				\
		src/ler_mapa/map2.c				\
		src/ler_mapa/map_aux1.c			\
		src/aux/tratar_tabs.c			\
		src/desenhar/desenhar_mapa.c	\
		src/desenhar/desenhar.c			\
		src/jogador/posicionamento.c	\
		src/jogador/mover_jogador.c		\
		src/jogador/checar_colisao.c	\
		src/raycast/algdda.c			\
		src/raycast/configuracoes.c		\
		src/raycast/ray2.c				\
		src/raycast/ray3.c				\
		src/raycast/raycast.c			\
		src/texturas/aux.c				\
		src/texturas/texturas.c

OBJDIR = obj
LIBFTDIR = libs/libft
MLXDIR = libs/minilibx-linux
INCDIR = includes

# Gerar a lista de objetos a partir dos arquivos SRCS
OBJS = $(SRCS:src/%.c=$(OBJDIR)/%.o)


# Bibliotecas
LIBFT = $(LIBFTDIR)/libft.a
MLX = $(MLXDIR)/libmlx.a

all: $(NAME)

$(NAME): $(LIBFT) $(MLX) $(OBJS)
	@$(CC) $(CFLAGS) $(OBJS) -I$(INCDIR) -I$(LIBFTDIR) -I$(MLXDIR) $(LIBFT_FLAGS) $(MLX_FLAGS) -o $(NAME)
	@echo "✅ Projeto compilado com sucesso!"

$(LIBFT):
	@$(MAKE) -C $(LIBFTDIR)

$(MLX):
	@$(MAKE) -C $(MLXDIR)

# Compilar os arquivos de origem para objetos, criando os subdiretórios necessários
$(OBJDIR)/%.o: src/%.c
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) -I$(INCDIR) -I$(LIBFTDIR) -I$(MLXDIR) -c $< -o $@

# Criar o diretório obj e seus subdiretórios
$(OBJDIR)/%:
	@mkdir -p $(dir $@)

clean:
	@$(MAKE) -C $(LIBFTDIR) clean
	@$(MAKE) -C $(MLXDIR) clean
	@rm -rf $(OBJDIR)
	@echo "🧹 Objetos limpos!"

fclean: clean
	@$(MAKE) -C $(LIBFTDIR) fclean
	@rm -f $(NAME)
	@echo "🗑️ Limpeza completa!"

re: fclean all

.PHONY: all clean fclean re
