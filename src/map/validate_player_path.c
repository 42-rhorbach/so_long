/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   validate_player_path.c                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: rhorbach <rhorbach@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/06/14 18:36:03 by rhorbach      #+#    #+#                 */
/*   Updated: 2023/06/20 15:33:59 by rhorbach      ########   odam.nl         */
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

static t_error	elements_reachable(bool *seen, t_data *data)
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
			&& seen[y * data->width + x] == false)
				return (set_error(E_EXIT_UNREACHABLE));
			if (data->map_grid[y][x] == 'C' \
			&& seen[y * data->width + x] == false)
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
	bool	*seen;

	seen = ft_calloc(data->width * data->height, sizeof(bool));
	if (seen == NULL)
		return (set_error(E_SYS));
	player_pos = find_player(data);
	if (flood_fill(player_pos, seen, data) != OK \
	|| elements_reachable(seen, data) != OK)
	{
		free(seen);
		return (get_error());
	}
	free(seen);
	return (OK);
}
