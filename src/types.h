/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   types.h                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: rhorbach <rhorbach@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/05/24 13:48:19 by rhorbach      #+#    #+#                 */
/*   Updated: 2023/06/13 16:51:20 by rhorbach      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef TYPES_H
# define TYPES_H

# include "MLX42.h"

typedef enum e_error
{
	OK,
	E_MLX,
	E_SYS,
	E_NO_MAP,
	E_MAP_NOT_CLOSED,
	E_MAP_NOT_RECT,
	E_INCORRECT_ELEMENT,
	E_WRONG_QUANTITY,
	E_EXIT_UNREACHABLE,
	E_COLLECTIBLE_UNREACHABLE
}	t_error;

typedef enum e_texture_index {
	EXIT,
	FLOOR,
	PLAYER,
	SHINY,
	WALL,
	TEXTURE_COUNT
}	t_texture_index;

typedef struct s_data
{
	mlx_t		*mlx;
	mlx_image_t	*images[TEXTURE_COUNT];
	char		**map_grid;
	size_t		width;
	size_t		height;
}	t_data;

#endif
