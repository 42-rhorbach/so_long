/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   error.c                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: rhorbach <rhorbach@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/05/24 13:52:36 by rhorbach      #+#    #+#                 */
/*   Updated: 2023/06/14 16:03:30 by rhorbach      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "so_long.h"
#include <errno.h>
#include <stdio.h>

static t_error	*get_error_ptr(void)
{
	static t_error	err_ptr = OK;

	return (&err_ptr);
}

t_error	set_error(t_error err)
{
	t_error	*err_ptr;

	err_ptr = get_error_ptr();
	*err_ptr = err;
	return (*err_ptr);
}

t_error	get_error(void)
{
	t_error	*err_ptr;

	err_ptr = get_error_ptr();
	return (*err_ptr);
}

void	print_error(t_error err)
{
	static const char	*error_table[] = {
	[E_ARGC] = "Argument must be a path to map of type .ber",
	[E_NO_MAP] = "Received empty map.",
	[E_MAP_NOT_CLOSED] = "Map is not enclosed by walls.",
	[E_MAP_NOT_RECT] = "Map is not rectangular.",
	[E_INCORRECT_ELEMENT] = "Unknown symbol found in map.",
	[E_WRONG_QUANTITY] = "Too many/few exits/players/collectibles in map.",
	[E_EXIT_UNREACHABLE] = "Player cannot reach the exit.",
	[E_COLLECTIBLE_UNREACHABLE] = "Player cannot reach all collectables"
	};

	if (err == E_MLX)
	{
		ft_putstr_fd("so_long: ", STDERR_FILENO);
		ft_putendl_fd((char *)mlx_strerror(mlx_errno), STDERR_FILENO);
	}
	else if (err == E_SYS)
	{
		perror("so_long");
	}
	else
	{
		ft_putstr_fd("so_long: ", STDERR_FILENO);
		ft_putendl_fd((char *)error_table[err], STDERR_FILENO);
	}
}
