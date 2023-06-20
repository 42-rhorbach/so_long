/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   so_long.h                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: rhorbach <rhorbach@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/05/21 16:29:48 by rhorbach      #+#    #+#                 */
/*   Updated: 2023/06/20 16:05:56 by rhorbach      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef SO_LONG_H
# define SO_LONG_H

# include "libft.h"
# include "types.h"
# include "MLX42.h"

# ifndef SL_TEX
#  warning SL_TEX must be defined (dir to so_long textures)
#  define SL_TEX ""
# endif

# ifndef TILE_SIZE
#  define TILE_SIZE 64
# endif

char	**load_map(const char *map_path);
t_error	is_valid_map(t_data *data);
t_error	set_error(t_error err);
t_error	get_error(void);
void	print_error(t_error err);
t_error	has_valid_path(t_data *data);
void	move_player(t_data *data, int dx, int dy);
t_error	instantiate_map(t_data *data);
t_error	window_init(t_data *data);
t_error	flood_fill(size_t index, bool *seen, t_data *data);

#endif
