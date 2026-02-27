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
			if (c != '0' && c != '1' && c != ' ' && ft_strchr("NESW", c) == NULL)
				fatal_error("Invalid character in map");
			count_char(c);
			j++;
		}
		i++;
	}
	count_char('F');
}
