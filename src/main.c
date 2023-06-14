/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   main.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: rhorbach <rhorbach@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/05/21 16:05:09 by rhorbach      #+#    #+#                 */
/*   Updated: 2023/06/13 18:14:27 by rhorbach      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "so_long.h"
#include <stdlib.h>

// int32_t	ft_rgba(int32_t r, int32_t g, int32_t b, int32_t a) //temp?
// {
// 	return (r << 24 | g << 16 | b << 8 | a);
// }

// void	ft_color(mlx_image_t *image, int32_t color) //temp?
// {
// 	uint32_t	x;
// 	uint32_t	y;

// 	x = 0;
// 	while (x < image->width)
// 	{
// 		y = 0;
// 		while (y < image->height)
// 		{
// 			mlx_put_pixel(image, x, y, color);
// 			y++;
// 		}
// 		x++;
// 	}
// }

void	move_player(t_data *data, int dx, int dy)
{
	const int	tx = data->px + dx;
	const int	ty = data->py + dy;
	int			i;

	if (data->map_grid[ty][tx] == '1')
		return ;
	if (data->map_grid[ty][tx] == 'C')
	{
		i = 0;
		while (i < data->images[SHINY]->count)
		{
			if (data->images[SHINY]->instances[i].x / TILE_SIZE == tx \
			&& data->images[SHINY]->instances[i].y / TILE_SIZE == ty)
				data->images[SHINY]->instances[i].enabled = false;
			i++;
		}
		data->map_grid[ty][tx] = '0';
		i = 0;
		while (i < data->images[SHINY]->count \
		&& data->images[SHINY]->instances[i].enabled == false)
			i++;
		if (i == data->images[SHINY]->count)
			data->images[HATCH]->enabled = false;
	}
	if (data->map_grid[ty][tx] == 'E')
	{
		i = 0;
		while (i < data->images[SHINY]->count \
		&& data->images[SHINY]->instances[i].enabled == false)
			i++;
		if (i == data->images[SHINY]->count)
			mlx_close_window(data->mlx);
	}
	data->px = tx;
	data->py = ty;
	data->images[PLAYER]->instances[0].x = data->px * TILE_SIZE;
	data->images[PLAYER]->instances[0].y = data->py * TILE_SIZE;
	ft_putnbr_fd(++data->move_count, STDOUT_FILENO);
	write(STDOUT_FILENO, "\n", 1);
}

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

t_error	load_texture(t_data *data, const char *texture_path, mlx_image_t **img)
{
	mlx_texture_t	*texture;

	texture = mlx_load_png(texture_path);
	if (texture == NULL)
		return (set_error(E_MLX));
	*img = mlx_texture_to_image(data->mlx, texture);
	mlx_delete_texture(texture);
	if (*img == NULL)
		return (set_error(E_MLX));
	return (OK);
}

t_error	create_instance(t_data *data, t_texture_index tile_type,
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

t_texture_index	get_tile_type(char c)
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

t_error	generate_map(t_data *data) // TODO: rename / too long
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
				if (create_instance(data, tile_type, x, y) != OK)
					return (get_error());
			x++;
		}
		y++;
	}
	return (OK);
}

t_error	window_init(t_data *data)
{
	data->mlx = mlx_init(data->width * TILE_SIZE, \
						data->height * TILE_SIZE, "so_long", false);
	if (data->mlx == NULL)
		return (set_error(E_MLX));
	if (load_texture(data, SL_TEX "exit.png", &data->images[EXIT]) != OK \
	|| load_texture(data, SL_TEX "floor.png", &data->images[FLOOR]) != OK \
	|| load_texture(data, SL_TEX "hatch.png", &data->images[HATCH]) != OK \
	|| load_texture(data, SL_TEX "player.png", &data->images[PLAYER]) != OK \
	|| load_texture(data, SL_TEX "shiny.png", &data->images[SHINY]) != OK \
	|| load_texture(data, SL_TEX "wall.png", &data->images[WALL]) != OK)
		return (get_error());
	if (generate_map(data) != OK)
		return (get_error());
	return (OK);
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
