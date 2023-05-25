/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   so_long.h                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: rhorbach <rhorbach@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/05/21 16:29:48 by rhorbach      #+#    #+#                 */
/*   Updated: 2023/05/24 14:37:02 by rhorbach      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef SO_LONG_H
# define SO_LONG_H

# include "libft.h"
# include "types.h"
# include "MLX42.h"

// void	generate_map(char **map_file);


t_error	set_error(t_error err);
t_error	get_error(void);
void	print_error(t_error err);

#endif
