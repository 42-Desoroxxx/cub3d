/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_checker.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llage <llage@student.42angouleme.fr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/19 20:56:53 by llage             #+#    #+#             */
/*   Updated: 2026/01/19 20:57:06 by llage            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cub3d.h>

static void	count_char(const char c)
{
	static bool	has_player;

	if (ft_strchr("NESW", c))
	{
		if (has_player)
			fatal_error("Map contains multiple players");
		has_player = true;
	}
	if (c == 'F' && !has_player)
		fatal_error("Map does not contain a player");
}

void	validate_chars(char **map)
{
	char	c;
	int		z;
	int		x;

	z = 0;
	while (map[z])
	{
		x = 0;
		while (map[z][x])
		{
			c = map[z][x];
			if (c != '0' && c != '1' && c != ' '
				&& ft_strchr("NESW", c) == NULL)
				fatal_error("Invalid character in map");
			count_char(c);
			x++;
		}
		z++;
	}
	count_char('F');
}
