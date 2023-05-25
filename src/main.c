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

int	main(void)
{
	t_data	data;

	// generate_map(argv);
	data.mlx = NULL;
	if (window_init(&data) != OK)
	{
		if (data.mlx != NULL)
		{
			mlx_close_window(data.mlx);
			mlx_terminate(data.mlx); // does not free textures
		}
		print_error(get_error());
		return (EXIT_FAILURE);
	}
	mlx_loop_hook(data.mlx, &ft_hook, &data);

	mlx_loop(data.mlx);
	mlx_terminate(data.mlx); // does not free textures
	return (EXIT_SUCCESS);
}
