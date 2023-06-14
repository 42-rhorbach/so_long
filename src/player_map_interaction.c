/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   player_map_interaction.c                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: rhorbach <rhorbach@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/06/14 19:03:29 by rhorbach      #+#    #+#                 */
/*   Updated: 2023/06/14 19:05:12 by rhorbach      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "so_long.h"

static bool	has_collected_all_shinies(t_data *data)
{
	int			i;

	i = 0;
	while (i < data->images[SHINY]->count)
	{
		if (data->images[SHINY]->instances[i].enabled)
			return (false);
		i++;
	}
	return (true);
}

static void	collect_shiny(t_data *data, int x, int y)
{
	int			i;

	i = 0;
	while (i < data->images[SHINY]->count)
	{
		if (data->images[SHINY]->instances[i].x / TILE_SIZE == x \
		&& data->images[SHINY]->instances[i].y / TILE_SIZE == y)
			data->images[SHINY]->instances[i].enabled = false;
		i++;
	}
	data->map_grid[y][x] = '0'; // TODO: think about whether you want this or not
	if (has_collected_all_shinies(data))
		data->images[HATCH]->enabled = false;
}

void	move_player(t_data *data, int dx, int dy)
{
	const int	tx = data->px + dx;
	const int	ty = data->py + dy;

	if (data->map_grid[ty][tx] == '1')
		return ;
	else if (data->map_grid[ty][tx] == 'C')
		collect_shiny(data, tx, ty);
	else if (data->map_grid[ty][tx] == 'E' && has_collected_all_shinies(data))
		mlx_close_window(data->mlx);
	data->px = tx;
	data->py = ty;
	data->images[PLAYER]->instances[0].x = data->px * TILE_SIZE;
	data->images[PLAYER]->instances[0].y = data->py * TILE_SIZE;
	ft_putnbr_fd(++data->move_count, STDOUT_FILENO);
	write(STDOUT_FILENO, "\n", 1);
}
