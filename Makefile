# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: aborges <aborges@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/01/27 12:16:34 by aborges           #+#    #+#              #
#    Updated: 2025/04/14 07:32:03 by aborges          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = cub3D
CC = cc
CFLAGS = -Wall -Wextra -Werror -Iincludes
MLX_FLAGS = -L$(MLXDIR) -lmlx -lX11 -lXext -lm -lGL
LIBFT_FLAGS = -L$(LIBFTDIR) -lft

SRCS =	src/main/main.c					\
		src/main/iniciar.c				\
		src/main/imagens.c				\
		src/main/teclas.c				\
		src/main/validacao.c			\
		src/main/validacao_map.c		\
		src/ler_mapa/map.c				\
		src/ler_mapa/map1.c				\
		src/ler_mapa/map2.c				\
		src/ler_mapa/map_aux1.c			\
		src/ler_mapa/cordenadas.c		\
		src/ler_mapa/checar_passagens.c	\
		src/aux/tratar_tabs.c			\
		src/desenhar/desenhar_mapa.c	\
		src/desenhar/desenhar.c			\
		src/jogador/posicionamento.c	\
		src/jogador/mover_jogador.c		\
		src/jogador/checar_colisao.c	\
		src/jogador/verificar_colisao.c	\
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

OBJS = $(SRCS:src/%.c=$(OBJDIR)/%.o)

LIBFT = $(LIBFTDIR)/libft.a
MLX = $(MLXDIR)/libmlx.a

all: $(NAME)

$(LIBFT):
	@$(MAKE) -C $(LIBFTDIR)
$(MLX):
	@$(MAKE) -C $(MLXDIR)

$(NAME): $(LIBFT) $(MLX) $(OBJS)
	@$(CC) $(CFLAGS) $(OBJS) -I$(INCDIR) -I$(LIBFTDIR) -I$(MLXDIR) $(LIBFT_FLAGS) $(MLX_FLAGS) -o $(NAME)
	@echo "✅ Projeto compilado com sucesso!"

$(OBJDIR)/%.o: src/%.c
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) -I$(INCDIR) -I$(LIBFTDIR) -I$(MLXDIR) -c $< -o $@

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
