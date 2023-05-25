/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   error.c                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: rhorbach <rhorbach@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/05/24 13:52:36 by rhorbach      #+#    #+#                 */
/*   Updated: 2023/05/24 14:38:55 by rhorbach      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "so_long.h"

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
	// [E_SAMPLE_ERROR] = "Failed to parse integer.",
	};

	if (err == E_MLX)
	{
		ft_putstr_fd("so_long: ", STDERR_FILENO);
		ft_putendl_fd((char *)mlx_strerror(mlx_errno), STDERR_FILENO);
	}
	else
	{
		ft_putstr_fd("so_long: ", STDERR_FILENO);
		ft_putendl_fd((char *)error_table[err], STDERR_FILENO);
	}
}
