/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   types.h                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: rhorbach <rhorbach@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/05/24 13:48:19 by rhorbach      #+#    #+#                 */
/*   Updated: 2023/05/24 17:00:00 by rhorbach      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef TYPES_H
# define TYPES_H

# include "MLX42.h"

typedef enum e_error
{
	OK,
	E_MLX,
}	t_error;

typedef struct s_data
{
	mlx_t		*mlx;
	mlx_image_t *image;
	int32_t		instance_id;
}	t_data;
#endif
