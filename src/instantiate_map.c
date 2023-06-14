/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   instantiate_map.c                                  :+:    :+:            */
/*                                                     +:+                    */
/*   By: rhorbach <rhorbach@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/06/14 19:13:30 by rhorbach      #+#    #+#                 */
/*   Updated: 2023/06/14 19:26:20 by rhorbach      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "so_long.h"

static t_error	create_instance(t_data *data, t_texture_index tile_type,
						size_t x, size_t y)
{
	int32_t			id;

	id = mlx_image_to_window(data->mlx, data->images[tile_type], \
								x * TILE_SIZE, y * TILE_SIZE);
	if (id == -1)
		return (set_error(E_MLX));
	if (tile_type == FLOOR)
		mlx_set_instance_depth(&data->images[tile_type]->instances[id], 0);
	if (tile_type == WALL || tile_type == SHINY)
		mlx_set_instance_depth(&data->images[tile_type]->instances[id], 1);
	if (tile_type == EXIT)
	{
		mlx_set_instance_depth(&data->images[tile_type]->instances[id], 1);
		create_instance(data, HATCH, x, y);
	}
	if (tile_type == HATCH)
		mlx_set_instance_depth(&data->images[HATCH]->instances[id], 2);
	if (tile_type == PLAYER)
		mlx_set_instance_depth(&data->images[tile_type]->instances[id], 3);
	return (OK);
}

static t_texture_index	get_tile_type(char c)
{
	if (c == 'E')
		return (EXIT);
	else if (c == 'C')
		return (SHINY);
	else if (c == 'P')
		return (PLAYER);
	else if (c == '1')
		return (WALL);
	return (FLOOR);
}

t_error	instantiate_map(t_data *data)
{
	size_t			x;
	size_t			y;
	t_texture_index	tile_type;

	y = 0;
	while (y < data->height)
	{
		x = 0;
		while (x < data->width)
		{
			if (create_instance(data, FLOOR, x, y) != OK)
				return (get_error());
			tile_type = get_tile_type(data->map_grid[y][x]);
			if (tile_type != FLOOR)
			{
				if (create_instance(data, tile_type, x, y) != OK)
					return (get_error());
			}
			x++;
		}
		y++;
	}
	return (OK);
}
