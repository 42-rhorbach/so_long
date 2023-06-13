/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   checker.c                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: rhorbach <rhorbach@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/06/07 15:28:08 by rhorbach      #+#    #+#                 */
/*   Updated: 2023/06/12 17:56:08 by rhorbach      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "so_long.h"
#include <stdlib.h>
//TODO: SPLIT
t_error	is_rectangular(t_data *data)
{
	size_t	y;

	y = 0;
	while (y < data->height && ft_strlen(data->map_grid[y]) == data->width)
		y++;
	if (y != data->height)
		return (set_error(E_MAP_NOT_RECT));
	return (OK);
}

t_error	is_enclosed(t_data *data)
{
	size_t	x;
	size_t	y;

	y = 0;
	while (y < data->height && data->map_grid[y][0] == '1' \
	&& data->map_grid[y][data->width - 1] == '1')
		y++;
	if (y != data->height)
		return (set_error(E_MAP_NOT_CLOSED));
	x = 0;
	while (data->map_grid[0][x] == '1' && data->map_grid[y - 1][x] == '1')
		x++;
	if (data->map_grid[0][x] != '\0' || data->map_grid[y - 1][x] != '\0')
		return (set_error(E_MAP_NOT_CLOSED));
	return (OK);
}

t_error	contains_elements(t_data *data)
{
	size_t	exit;
	size_t	player;
	size_t	collectibles;
	size_t	y;
	size_t	x;

	exit = 0;
	player = 0;
	collectibles = 0;
	y = 0;
	while (y < data->height)
	{
		x = 0;
		while (x < data->width)
		{
			if (data->map_grid[y][x] == 'P')
				player++;
			else if (data->map_grid[y][x] == 'E')
				exit++;
			else if (data->map_grid[y][x] == 'C')
				collectibles++;
			else if (data->map_grid[y][x] != '1' && data->map_grid[y][x] != '0')
				return (set_error(E_INCORRECT_ELEMENT));
			x++;
		}
		y++;
	}
	if (exit != 1 || player != 1 || collectibles <= 0)
		return (set_error(E_WRONG_QUANTITY));
	return (OK);
}

size_t	find_player(t_data *data)
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

void	flood_fill(size_t index, bool *been_here, t_data *data)
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

t_error	elements_reachable(bool *been_here, t_data *data)
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

t_error	is_valid_path(t_data *data)
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

t_error	is_valid_map(t_data *data)
{
	if (data->height == 0 || data->width == 0)
		return (set_error(E_NO_MAP));
	if (is_rectangular(data) != OK \
	|| is_enclosed(data) != OK \
	|| contains_elements(data) != OK \
	|| is_valid_path(data) != OK)
		return (get_error());
	return (OK);
}
