/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   flood_fill.c                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: rhorbach <rhorbach@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/06/14 18:36:03 by rhorbach      #+#    #+#                 */
/*   Updated: 2023/06/14 18:39:33 by rhorbach      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "so_long.h"
#include <stdlib.h>

static size_t	find_player(t_data *data)
{
	size_t	x;
	size_t	y;

	y = 0;
	while (y < data->height)
	{
		x = 0;
		while (x < data->width)
		{
			if (data->map_grid[y][x] == 'P')
				return (y * data->width + x);
			x++;
		}
		y++;
	}
	return (0);
}

static void	flood_fill(size_t index, bool *been_here, t_data *data)
{
	size_t	x;
	size_t	y;

	x = index % data->width;
	y = index / data->width;
	been_here[index] = true;
	if (data->map_grid[y + 1][x] != '1' \
	&& been_here[(y + 1) * data->width + x] == false)
		flood_fill((y + 1) * data->width + x, been_here, data);
	if (data->map_grid[y][x + 1] != '1' \
	&& been_here[y * data->width + (x + 1)] == false)
		flood_fill(y * data->width + (x + 1), been_here, data);
	if (data->map_grid[y - 1][x] != '1' \
	&& been_here[(y - 1) * data->width + x] == false)
		flood_fill((y - 1) * data->width + x, been_here, data);
	if (data->map_grid[y][x - 1] != '1' \
	&& been_here[y * data->width + (x - 1)] == false)
		flood_fill(y * data->width + (x - 1), been_here, data);
}

static t_error	elements_reachable(bool *been_here, t_data *data)
{
	size_t	x;
	size_t	y;

	y = 0;
	while (y < data->height)
	{
		x = 0;
		while (x < data->width)
		{
			if (data->map_grid[y][x] == 'E' \
			&& been_here[y * data->width + x] == false)
				return (set_error(E_EXIT_UNREACHABLE));
			if (data->map_grid[y][x] == 'C' \
			&& been_here[y * data->width + x] == false)
				return (set_error(E_COLLECTIBLE_UNREACHABLE));
			x++;
		}
		y++;
	}
	return (OK);
}

t_error	has_valid_path(t_data *data)
{
	size_t	player_pos;
	bool	*been_here;

	been_here = ft_calloc(data->width * data->height, sizeof(bool));
	if (been_here == NULL)
		return (set_error(E_SYS));
	player_pos = find_player(data);
	flood_fill(player_pos, been_here, data);
	if (elements_reachable(been_here, data) != OK)
	{
		free(been_here);
		return (get_error());
	}
	free(been_here);
	return (OK);
}
