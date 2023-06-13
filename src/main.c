/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   main.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: rhorbach <rhorbach@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/05/21 16:05:09 by rhorbach      #+#    #+#                 */
/*   Updated: 2023/05/24 17:09:33 by rhorbach      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "so_long.h"
#include <stdlib.h>

int32_t	ft_rgba(int32_t r, int32_t g, int32_t b, int32_t a)
{
	return (r << 24 | g << 16 | b << 8 | a);
}

void	ft_color(mlx_image_t *image, int32_t color)
{
	uint32_t	x;
	uint32_t	y;

	x = 0;
	while (x < image->width)
	{
		y = 0;
		while (y < image->height)
		{
			mlx_put_pixel(image, x, y, color);
			y++;
		}
		x++;
	}
}

void	ft_hook(void *param)
{
	t_data *const	data = param;

	if (mlx_is_key_down(data->mlx, MLX_KEY_ESCAPE))
		mlx_close_window(data->mlx);
	if (mlx_is_key_down(data->mlx, MLX_KEY_UP)
		|| mlx_is_key_down(data->mlx, MLX_KEY_W))
		data->image->instances[data->instance_id].y -= 1;
	if (mlx_is_key_down(data->mlx, MLX_KEY_DOWN)
		|| mlx_is_key_down(data->mlx, MLX_KEY_S))
		data->image->instances[data->instance_id].y += 1;
	if (mlx_is_key_down(data->mlx, MLX_KEY_LEFT)
		|| mlx_is_key_down(data->mlx, MLX_KEY_A))
		data->image->instances[data->instance_id].x -= 1;
	if (mlx_is_key_down(data->mlx, MLX_KEY_RIGHT)
		|| mlx_is_key_down(data->mlx, MLX_KEY_D))
		data->image->instances[data->instance_id].x += 1;
}

t_error	window_init(t_data *data)
{
	//TODO: set dynamic window heigths and widths
	data->mlx = mlx_init(500, 500, "so_long", false);
	if (data->mlx == NULL)
		return (set_error(E_MLX));
	data->image = mlx_new_image(data->mlx, 500, 500);
	if (data->image == NULL)
		return (set_error(E_MLX));
	ft_color(data->image, ft_rgba(55, 110, 165, 255));
	data->instance_id = mlx_image_to_window(data->mlx, data->image, 0, 0);
	if (data->instance_id == -1)
		return (set_error(E_MLX));
	return (OK);
}

t_error map_function(t_data *data, const char *map_path) // betere naam
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
	if (map_function(&data, map_path) != OK)
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
	//build_mlx_map

	mlx_loop_hook(data.mlx, &ft_hook, &data);
	mlx_loop(data.mlx);
	ft_free_ptr_array((void **)data.map_grid);
	mlx_terminate(data.mlx); // does not free textures
	return (OK);
}

int	main(int argc, char **argv)
{
	atexit(&leak_check);
	if (argc != 2)
	{
		// print_error (); //make sensible
		return (EXIT_FAILURE);
	}
	if (so_long(argv[1]) != OK)
	{
		print_error(get_error());
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}
