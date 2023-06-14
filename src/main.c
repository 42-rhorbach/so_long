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

void	leak_check(void)
{
	system("leaks so_long");
}

#  include <stdio.h>
void DEBUG_print_map(char **map_grid)
{
	printf("MAP ---\n");
	(void)map_grid;
	for (int y = 0; map_grid[y] != NULL; y++) {
		printf("%s\n", map_grid[y]);
		// printf("a\n");
	}
}

t_error parse_map_file(t_data *data, const char *map_path) // betere naam
{
	data->map_grid = load_map(map_path); //TODO: free dobby the house elf
	if (data->map_grid == NULL)
		return (set_error(E_SYS));
	//DEBUG_print_map(data->map_grid);
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

	//TODO: check if all `t_error` functions return set_error
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
			mlx_terminate(data.mlx); // does not free textures
		}
		ft_free_ptr_array((void **)data.map_grid);
		return (get_error());
	}
	data.px = data.images[PLAYER]->instances[0].x / TILE_SIZE;
	data.py = data.images[PLAYER]->instances[0].y / TILE_SIZE;
	mlx_key_hook(data.mlx, &ft_hook, &data);
	mlx_loop(data.mlx);
	ft_free_ptr_array((void **)data.map_grid);
	mlx_terminate(data.mlx);
	return (OK);
}

int	main(int argc, char **argv)
{
	atexit(&leak_check);
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
