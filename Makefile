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
CFLAGS = -Wall -Wextra -Werror

# Diretórios
SRCDIR = src
OBJDIR = obj
INCDIR = includes
LIBFTDIR = libs/libft
MLXDIR = libs/minilibx-linux

# Arquivos
SRCS = $(wildcard $(SRCDIR)/*.c)
OBJS = $(SRCS:$(SRCDIR)/%.c=$(OBJDIR)/%.o)

# Bibliotecas
LIBFT = $(LIBFTDIR)/libft.a
MLX = $(MLXDIR)/libmlx.a

# Flags para as bibliotecas (no Linux usa X11, OpenGL e outras dependências)
MLX_FLAGS = -L$(MLXDIR) -lmlx -lX11 -lXext -lm -lGL
LIBFT_FLAGS = -L$(LIBFTDIR) -lft

# Regras
all: $(NAME)

$(NAME): $(LIBFT) $(MLX) $(OBJS)
	@$(CC) $(CFLAGS) $(OBJS) $(LIBFT_FLAGS) $(MLX_FLAGS) -o $(NAME)
	@echo "✅ Projeto compilado com sucesso!"

$(LIBFT):
	@$(MAKE) -C $(LIBFTDIR)

$(MLX):
	@$(MAKE) -C $(MLXDIR)

# Compilar os objetos no diretório obj
$(OBJDIR)/%.o: $(SRCDIR)/%.c | $(OBJDIR)
	@$(CC) $(CFLAGS) -I$(INCDIR) -I$(LIBFTDIR) -I$(MLXDIR) -c $< -o $@

$(OBJDIR):
	@mkdir -p $(OBJDIR)

# Limpeza
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

# Para evitar conflitos de nomes de arquivos
.PHONY: all clean fclean re
