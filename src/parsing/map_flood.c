/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_flood.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llage <llage@student.42angouleme.fr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/19 20:56:53 by llage             #+#    #+#             */
/*   Updated: 2026/01/19 20:57:15 by llage            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cub3d.h>

static void	check_and_fill(char **flood_map, int x, int y, int *changes)
{
	const int	direction_x[4] = {0, 1, 0, -1};
	const int	direction_y[4] = {-1, 0, 1, 0};
	int			new_x;
	int			new_y;
	int			i;

	i = 0;
	while (i < 4)
	{
		new_x = x + direction_x[i];
		new_y = y + direction_y[i];
		if (new_x >= 0 && new_x < get_width(flood_map) && new_y >= 0
			&& new_y < get_length(flood_map) && flood_map[new_y][new_x] == '0')
		{
			flood_map[new_y][new_x] = 'P';
			(*changes)++;
		}
		i++;
	}
}

static char	**flood_fill(char **map, int x, int z)
{
	char	**flood_map;
	int		changes;

	flood_map = copy_map(map);
	if (flood_map == NULL)
		fatal_error("Unable to allocate memory for flooded map");
	while (true)
	{
		changes = 0;
		z = 0;
		while (z < get_length(flood_map))
		{
			x = 0;
			while (x < ((int) ft_strlen(flood_map[0])))
			{
				if (flood_map[z][x] == 'P')
					check_and_fill(flood_map, x, z, &changes);
				x++;
			}
			z++;
		}
		if (changes == 0)
			break ;
	}
	return (flood_map);
}

static void	validate_everything(char **map, char **flooded_map)
{
	int	x;
	int	z;
	int	len;

	z = -1;
	while (++z < get_length(map))
	{
		len = (int)ft_strlen(map[z]);
		x = -1;
		while (++x < len)
		{
			if (map[z][x] == '1' || map[z][x] == ' ')
				continue ;
			if (z == 0 || z == get_length(map) - 1
				|| x == 0 || x == len - 1)
				fatal_error_map("Map is not closed", flooded_map);
			if (x >= (int)ft_strlen(map[z - 1]) || map[z - 1][x] == ' ')
				fatal_error_map("Map is not closed", flooded_map);
			if (x >= (int)ft_strlen(map[z + 1]) || map[z + 1][x] == ' ')
				fatal_error_map("Map is not closed", flooded_map);
			if (map[z][x - 1] == ' ' || map[z][x + 1] == ' ')
				fatal_error_map("Map is not closed", flooded_map);
		}
	}
}

void	validate_via_flood(const t_context *context)
{
	char	**flood_map;

	flood_map = flood_fill(context->map, context->player.x, context->player.z);
	validate_everything(context->map, flood_map);
	free_map(flood_map);
}
