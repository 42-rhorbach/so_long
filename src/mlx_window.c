/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   mlx_window.c                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: rhorbach <rhorbach@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/06/14 19:32:40 by rhorbach      #+#    #+#                 */
/*   Updated: 2023/06/14 19:33:15 by rhorbach      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "so_long.h"
#include <stddef.h>

static t_error	load_texture(t_data *data, const char *texture_path,
								mlx_image_t **img)
{
	mlx_texture_t	*texture;

	texture = mlx_load_png(texture_path);
	if (texture == NULL)
		return (set_error(E_MLX));
	*img = mlx_texture_to_image(data->mlx, texture);
	mlx_delete_texture(texture);
	if (*img == NULL)
		return (set_error(E_MLX));
	return (OK);
}

t_error	window_init(t_data *data)
{
	data->mlx = mlx_init(data->width * TILE_SIZE, \
						data->height * TILE_SIZE, "so_long", false);
	if (data->mlx == NULL)
		return (set_error(E_MLX));
	if (load_texture(data, SL_TEX "exit.png", &data->images[EXIT]) != OK \
	|| load_texture(data, SL_TEX "floor.png", &data->images[FLOOR]) != OK \
	|| load_texture(data, SL_TEX "hatch.png", &data->images[HATCH]) != OK \
	|| load_texture(data, SL_TEX "player.png", &data->images[PLAYER]) != OK \
	|| load_texture(data, SL_TEX "shiny.png", &data->images[SHINY]) != OK \
	|| load_texture(data, SL_TEX "wall.png", &data->images[WALL]) != OK)
		return (get_error());
	if (instantiate_map(data) != OK)
		return (get_error());
	return (OK);
}
