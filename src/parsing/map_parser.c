/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_parser.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llage <llage@student.42angouleme.fr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/19 20:56:53 by llage             #+#    #+#             */
/*   Updated: 2026/01/19 20:57:19 by llage            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cub3d.h>

static char	**add_to_map(char **map, char *line, int fd)
{
	const int	current_map_length = get_length(map);
	char		**new_map;
	char		*new_line;

	new_map = ft_calloc(current_map_length + 2, sizeof(char *));
	if (new_map == NULL)
		fatal_error_fd("Unable to allocate memory for map", fd);
	ft_memcpy(new_map, map, (current_map_length) * sizeof(char *));
	new_line = ft_strtrim(line, "\n\r");
	new_map[current_map_length] = ft_calloc(ft_strlen(new_line) + 1,
			sizeof(char));
	if (new_map[current_map_length] == NULL)
	{
		free(new_map);
		free(new_line);
		fatal_error_fd("Unable to allocate memory for map line", fd);
	}
	ft_strncpy(new_map[current_map_length], new_line, ft_strlen(new_line));
	free(new_line);
	new_map[current_map_length + 1] = NULL;
	return (new_map);
}

static char	**read_map(const int fd)
{
	char	**new_map;
	char	**map;
	char	*line;

	line = get_next_line(fd);
	if (line == NULL)
		fatal_error_fd("Unable to read map file (Is it empty?)", fd);
	while (line && line[0] == '\n')
	{
		free(line);
		line = get_next_line(fd);
	}
	map = ft_calloc(1, sizeof(char *));
	if (map == NULL)
		fatal_error_fd("Unable to allocate memory for map", fd);
	while (line && line[0] != '\n')
	{
		new_map = add_to_map(map, line, fd);
		free(map);
		free(line);
		map = new_map;
		line = get_next_line(fd);
	}
	free(line);
	return (map);
}

static int	get_direction(char c)
{
	if (c == 'N')
		return (NORTH);
	if (c == 'E')
		return (EAST);
	if (c == 'S')
		return (SOUTH);
	return (WEST);
}

static t_player	get_player(char **map)
{
	t_player	player;
	int			z;
	int			x;

	player.x = -1;
	player.z = -1;
	z = -1;
	while (map[++z])
	{
		x = -1;
		while (map[z][++x])
		{
			if (ft_strchr("NESW", map[z][x]))
			{
				player.x = x;
				player.z = z;
				player.direction = get_direction(map[z][x]);
				return (player);
			}
		}
	}
	return (player);
}

void	parse_map(const int fd, t_context *context)
{
	parse_scene(fd, &context->scene);
	context->map = read_map(fd);
	close(fd);
	validate_chars(context->map);
	context->player = get_player(context->map);
	validate_via_flood(context);
}
