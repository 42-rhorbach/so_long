/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   init.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: rhorbach <rhorbach@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/05/21 16:05:04 by rhorbach      #+#    #+#                 */
/*   Updated: 2023/06/12 16:57:34 by rhorbach      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "so_long.h"
#include <stdlib.h>
#include <fcntl.h>

static t_list	*get_map_lines(int map_fd)
{
	t_list	*head;
	char	*new_line;
	int		i;

	i = 0;
	while (true)
	{
		new_line = get_next_line(map_fd);
		if (new_line == NULL) // maybe improve get_next_line to differentiate between error and EOF
			break ;
		if (new_line[ft_strlen(new_line) - 1] == '\n')
			new_line[ft_strlen(new_line) - 1] = '\0';
		if (ft_lstnew_front(&head, new_line) == NULL)
		{
			free(new_line);
			ft_lstclear(&head, &free);
			return (NULL);
		}
	}
	return (head);
}

static char	**load_map_grid(int map_fd)
{
	t_list	*lst;
	char	**map_grid;

	lst = get_map_lines(map_fd);
	if (lst == NULL)
		return (NULL);
	map_grid = (char **)ft_lst_to_array_rev(lst);
	if (map_grid == NULL)
	{
		ft_lstclear(&lst, &free);
		return (NULL);
	}
	ft_lstclear(&lst, NULL);
	return (map_grid);
}

char	**load_map(const char *map_path)
{
	int		map_fd;
	char	**map_grid;

	map_fd = open(map_path, O_RDONLY);
	if (map_fd < 0)
		return (NULL);
	map_grid = load_map_grid(map_fd);
	close(map_fd);
	return (map_grid);
}

//TODO:
// - draw map using custom textures and a pixel grid


// //tile = 100x100
// t_error	load_map(char **map_file)
// {
// 	int	map_fd;
// 	char	map_line;


// 	get_next_line(map_fd);
// }
