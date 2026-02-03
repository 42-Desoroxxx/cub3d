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
	int		i;
	int		j;

	i = 0;
	while (map[i])
	{
		j = 0;
		while (map[i][j])
		{
			c = map[i][j];
			if (c != '0' && c != '1' && ft_strchr("NESW", c) == NULL)
				fatal_error("Invalid character in map");
			count_char(c);
			j++;
		}
		i++;
	}
	count_char('F');
}

void	validate_rectangle(char **map)
{
	const size_t	length = ft_strlen(map[0]);
	int				i;
	int				j;

	i = 0;
	while (map[i])
	{
		if (ft_strlen(map[i]) != length)
			fatal_error("Map is not a rectangle");
		j = 0;
		if (i == 0 || map[i + 1] == NULL)
		{
			while (map[i][j])
			{
				if (map[i][j] != '1')
					fatal_error("Map outer layer is not fully walls");
				j++;
			}
		}
		else if (map[i][get_width(map) - 1] != '1' || map[i][0] != '1')
			fatal_error("Map outer layer is not fully walls");
		i++;
	}
}
