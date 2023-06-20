/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   main.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: rhorbach <rhorbach@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/05/21 16:05:09 by rhorbach      #+#    #+#                 */
/*   Updated: 2023/06/14 19:22:54 by rhorbach      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "so_long.h"
#include <stdlib.h>

void	ft_hook(mlx_key_data_t keydata, void *param)
{
	t_data *const	data = param;

	if (keydata.action == MLX_PRESS)
	{
		if (keydata.key == MLX_KEY_ESCAPE)
			mlx_close_window(data->mlx);
		if (keydata.key == MLX_KEY_UP || keydata.key == MLX_KEY_W)
			move_player(data, 0, -1);
		if (keydata.key == MLX_KEY_DOWN || keydata.key == MLX_KEY_S)
			move_player(data, 0, 1);
		if (keydata.key == MLX_KEY_LEFT || keydata.key == MLX_KEY_A)
			move_player(data, -1, 0);
		if (keydata.key == MLX_KEY_RIGHT || keydata.key == MLX_KEY_D)
			move_player(data, 1, 0);
	}
}

t_error	parse_map_file(t_data *data, const char *map_path)
{
	data->map_grid = load_map(map_path);
	if (data->map_grid == NULL)
		return (set_error(E_SYS));
	data->height = ft_ptr_array_length((void **)data->map_grid);
	data->width = 0;
	if (data->height != 0)
		data->width = ft_strlen(data->map_grid[0]);
	if (is_valid_map(data) != OK)
	{
		ft_free_ptr_array((void **)data->map_grid);
		return (get_error());
	}
	return (OK);
}

/**
 * Free everything in data (does not free data itself)
 * @param data address of bzero'd t_data struct
*/
void	free_data_content(t_data *data)
{
	if (data->map_grid != NULL)
		ft_free_ptr_array((void **)data->map_grid);
}

t_error	so_long(const char *map_path)
{
	t_data	data;

	ft_bzero(&data, sizeof(data));
	if (parse_map_file(&data, map_path) != OK)
		return (get_error());
	data.mlx = NULL;
	if (window_init(&data) != OK)
	{
		if (data.mlx != NULL)
		{
			mlx_close_window(data.mlx);
			mlx_terminate(data.mlx);
		}
		free_data_content(&data);
		return (get_error());
	}
	data.px = data.images[PLAYER]->instances[0].x / TILE_SIZE;
	data.py = data.images[PLAYER]->instances[0].y / TILE_SIZE;
	mlx_key_hook(data.mlx, &ft_hook, &data);
	mlx_loop(data.mlx);
	free_data_content(&data);
	mlx_terminate(data.mlx);
	return (OK);
}

// void	leak_check(void)
// {
// 	system("leaks -q so_long");
// }
	// atexit(&leak_check);

int	main(int argc, char **argv)
{
	if (argc != 2)
	{
		print_error(E_ARGC);
		return (EXIT_FAILURE);
	}
	if (so_long(argv[1]) != OK)
	{
		print_error(get_error());
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}
