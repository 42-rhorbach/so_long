# **************************************************************************** #
#                                                                              #
#                                                         ::::::::             #
#    Makefile                                           :+:    :+:             #
#                                                      +:+                     #
#    By: rhorbach <rhorbach@student.codam.nl>         +#+                      #
#                                                    +#+                       #
#    Created: 2022/10/14 12:49:02 by rhorbach      #+#    #+#                  #
#    Updated: 2023/06/20 15:36:29 by rhorbach      ########   odam.nl          #
#                                                                              #
# **************************************************************************** #

NAME = so_long
HEADERFILES = src/so_long.h src/types.h
NORMFLAGS = -Wall -Wextra -Werror $(if $(DEBUG),-g -fsanitize=address) -D SO_LONG_DIR='"$(dir $(realpath $(lastword $(MAKEFILE_LIST))))/"' -D SL_TEX='"$(dir $(realpath $(lastword $(MAKEFILE_LIST))))/textures/"'
OBJDIR = obj
FILES = \
	src/main.c						\
	src/map/init.c					\
	src/map/validate_player_path.c	\
	src/map/validate.c				\
	src/error.c						\
	src/instantiate_map.c			\
	src/mlx_window.c				\
	src/player_map_interaction.c

#########################

MLX42_DIR = ./MLX42
MLX42_BUILD_DIR = build
MLX42 = $(MLX42_DIR)/$(MLX42_BUILD_DIR)/libmlx42.a

#########################

LIBFT = ./libft/libft.a

#########################

HEADERFILES += ./libft/libft.h ./MLX42/include/MLX42/MLX42.h
LIBFLAGS =	-L$(dir $(LIBFT)) -lft		\
			-L$(dir $(MLX42)) -lmlx42

ifeq ($(shell uname),Linux)
LIBFLAGS += -lglfw
else
LIBFLAGS += -lglfw3 -framework Cocoa -framework OpenGL -framework IOKit
endif

#########################

INCLUDES = $(addprefix -I, $(sort $(dir $(HEADERFILES))))
OBJFILES = $(addprefix $(OBJDIR)/,$(FILES:c=o))

#########################

clear_line = \e[K
move_up = \e[A
define prettycomp
printf "$(1)$(clear_line)\n"
$(1)
printf "$(move_up)"
endef

#########################

all: $(NAME)

$(NAME): $(OBJFILES) $(LIBFT) $(MLX42)
	$(CC) $(NORMFLAGS) $(INCLUDES) $(OBJFILES) $(LIBFLAGS) -o $(NAME)

$(LIBFT):
	@$(MAKE) -C $(dir $(LIBFT))

$(MLX42):
	cmake -S $(MLX42_DIR) -B $(MLX42_DIR)/$(MLX42_BUILD_DIR)
	$(MAKE) -C $(MLX42_DIR)/$(MLX42_BUILD_DIR) -j4

$(OBJDIR)/%.o: %.c $(HEADERFILES)
	@mkdir -p $(dir $@)
	@$(call prettycomp,$(CC) -c $(NORMFLAGS) $(INCLUDES) $< -o $@)

clean:
	rm -rf $(OBJDIR)
	@$(MAKE) -C $(dir $(LIBFT)) clean

fclean: clean
	rm -f $(NAME)
	@$(MAKE) -C $(dir $(LIBFT)) fclean
	rm -rf $(MLX42_DIR)/$(MLX42_BUILD_DIR)

re: fclean all

#########################

ifdef DEBUG
test: $(NAME)
	$(CC) $(NORMFLAGS) main.c -L. -l ftprintf -o DEBUG_ft_printf
else
test:
	$(MAKE) test DEBUG=1
endif

.PHONY: all clean fcleam re test bonus
