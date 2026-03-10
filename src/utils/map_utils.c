/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llage <llage@student.42angouleme.fr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/19 20:56:53 by llage             #+#    #+#             */
/*   Updated: 2026/01/19 20:57:30 by llage            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cub3d.h>

int	get_width(char **map)
{
	int	width;

	width = 0;
	while (map[0][width])
		width++;
	return (width);
}

int	get_length(char **map)
{
	int	length;

	length = 0;
	while (map[length])
		length++;
	return (length);
}

char	**copy_map(char **map)
{
	char	**new_map;
	int		i;

	new_map = ft_calloc(get_length(map) + 1, sizeof(char *));
	if (new_map == NULL)
		fatal_error("Unable to allocate memory for map");
	i = 0;
	while (map[i])
	{
		new_map[i] = ft_calloc(ft_strlen(map[i]) + 1, sizeof(char));
		if (new_map[i] == NULL)
			fatal_error_map("Unable to allocate memory for map line", new_map);
		ft_strncpy(new_map[i], map[i], ft_strlen(map[i]));
		i++;
	}
	return (new_map);
}
