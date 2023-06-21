/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   flood_fill.c                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: rhorbach <rhorbach@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/06/20 15:34:08 by rhorbach      #+#    #+#                 */
/*   Updated: 2023/06/21 14:57:39 by rhorbach      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "so_long.h"
#include <stdlib.h>

static t_flood_list	*sl_lstnew(size_t index)
{
	t_flood_list	*new_list;

	new_list = malloc(sizeof(t_flood_list));
	if (new_list == NULL)
		return (NULL);
	new_list->index = index;
	new_list->next = NULL;
	return (new_list);
}

static t_flood_list	*sl_lstnew_front(t_flood_list **lst, size_t index)
{
	t_flood_list	*new;

	new = sl_lstnew(index);
	if (new == NULL)
		return (NULL);
	new->next = *lst;
	*lst = new;
	return (new);
}

static void	sl_lst_advance(t_flood_list **lst)
{
	t_flood_list	*next;

	next = (*lst)->next;
	free(*lst);
	*lst = next;
}

static t_error	try_add_tile(t_data *data, bool *seen, size_t index,
								t_flood_list **lst)
{
	size_t	x;
	size_t	y;

	x = index % data->width;
	y = index / data->width;
	if (data->map_grid[y][x] != '1' && seen[index] == false)
	{
		if (sl_lstnew_front(lst, index) == NULL)
			return (set_error(E_SYS));
	}
	return (OK);
}

t_error	flood_fill(size_t index, bool *seen, t_data *data)
{
	t_flood_list	*lst;

	lst = NULL;
	if (sl_lstnew_front(&lst, index) == NULL)
		return (set_error(E_SYS));
	while (lst != NULL)
	{
		index = lst->index;
		sl_lst_advance(&lst);
		if (seen[index])
			continue ;
		seen[index] = true;
		if (try_add_tile(data, seen, index + 1, &lst) != OK \
		|| try_add_tile(data, seen, index - 1, &lst) != OK \
		|| try_add_tile(data, seen, index + data->width, &lst) != OK \
		|| try_add_tile(data, seen, index - data->width, &lst) != OK)
		{
			while (lst != NULL)
				sl_lst_advance(&lst);
			return (get_error());
		}
	}
	return (OK);
}
