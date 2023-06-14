/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   validate.c                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: rhorbach <rhorbach@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/06/07 15:28:08 by rhorbach      #+#    #+#                 */
/*   Updated: 2023/06/14 18:39:56 by rhorbach      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "so_long.h"

static t_error	is_rectangular(t_data *data)
{
	size_t	y;

	y = 0;
	while (y < data->height && ft_strlen(data->map_grid[y]) == data->width)
		y++;
	if (y != data->height)
		return (set_error(E_MAP_NOT_RECT));
	return (OK);
}

static t_error	is_enclosed(t_data *data)
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

static t_error	count_element(char c,
						size_t *player, size_t *exit, size_t *collectibles)
{
	if (c == 'P')
		(*player)++;
	else if (c == 'E')
		(*exit)++;
	else if (c == 'C')
		(*collectibles)++;
	else if (c != '1' && c != '0')
		return (set_error(E_INCORRECT_ELEMENT));
	return (OK);
}

static t_error	contains_elements(t_data *data)
{
	size_t	player;
	size_t	exit;
	size_t	collectibles;
	size_t	y;
	size_t	x;

	player = 0;
	exit = 0;
	collectibles = 0;
	y = 0;
	while (y < data->height)
	{
		x = 0;
		while (x < data->width)
		{
			if (count_element(data->map_grid[y][x], \
								&player, &exit, &collectibles) != OK)
				return (get_error());
			x++;
		}
		y++;
	}
	if (exit != 1 || player != 1 || collectibles <= 0)
		return (set_error(E_WRONG_QUANTITY));
	return (OK);
}

t_error	is_valid_map(t_data *data)
{
	if (data->height == 0 || data->width == 0)
		return (set_error(E_NO_MAP));
	if (is_rectangular(data) != OK \
	|| is_enclosed(data) != OK \
	|| contains_elements(data) != OK \
	|| has_valid_path(data) != OK)
		return (get_error());
	return (OK);
}
