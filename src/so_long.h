/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   so_long.h                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: rhorbach <rhorbach@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/05/21 16:29:48 by rhorbach      #+#    #+#                 */
/*   Updated: 2023/06/13 16:55:49 by rhorbach      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef SO_LONG_H
# define SO_LONG_H

# include "libft.h"
# include "types.h"
# include "MLX42.h"

# ifndef SO_LONG_DIR
#  warning SO_LONG_DIR must be defined
#  define SO_LONG_DIR ""
# endif

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

#endif
