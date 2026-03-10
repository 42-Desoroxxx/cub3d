/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scene_parser.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llage <llage@student.42angouleme.fr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/27 01:20:34 by llage             #+#    #+#             */
/*   Updated: 2026/02/27 01:20:34 by llage            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cub3d.h>

static uint32_t	parse_color(char *str, int fd, char *line)
{
	int		rgb[3];
	char	*comma;

	while (*str == ' ' || *str == '\t')
		str++;
	rgb[0] = ft_atoi(str);
	comma = ft_strchr(str, ',');
	if (!comma)
		fatal_error_fd_line("Invalid color format", fd, line);
	rgb[1] = ft_atoi(++comma);
	comma = ft_strchr(comma, ',');
	if (!comma)
		fatal_error_fd_line("Invalid color format", fd, line);
	rgb[2] = ft_atoi(++comma);
	if (ft_strchr(comma, ','))
		fatal_error_fd_line("Invalid color format", fd, line);
	return (((uint32_t)rgb[0] << 24) | ((uint32_t)rgb[1] << 16)
		| ((uint32_t)rgb[2] << 8) | 0xFF);
}

static void	set_color(uint32_t *destination, char *source, int fd, char *line)
{
	if (*destination != 0)
		fatal_error_fd_line("Duplicate color identifier", fd, line);
	*destination = parse_color(source, fd, line);
}

static void	set_path(char **destination, char *source, int fd, char *line)
{
	if (*destination != NULL)
		fatal_error_fd_line("Duplicate texture identifier", fd, line);
	*destination = ft_strtrim(source, " \t");
	if (*destination == NULL)
		fatal_error_fd_line("Memory allocation failed", fd, line);
}

static int	parse_header_line(char *line, t_scene *scene, int fd)
{
	if (ft_strncmp(line, "NO ", 3) == 0)
		set_path(&scene->path_north, line + 3, fd, line);
	else if (ft_strncmp(line, "SO ", 3) == 0)
		set_path(&scene->path_south, line + 3, fd, line);
	else if (ft_strncmp(line, "WE ", 3) == 0)
		set_path(&scene->path_west, line + 3, fd, line);
	else if (ft_strncmp(line, "EA ", 3) == 0)
		set_path(&scene->path_east, line + 3, fd, line);
	else if (ft_strncmp(line, "F ", 2) == 0)
		set_color(&scene->floor_color, line + 2, fd, line);
	else if (ft_strncmp(line, "C ", 2) == 0)
		set_color(&scene->ceiling_color, line + 2, fd, line);
	else
		return (0);
	return (1);
}

void	parse_scene(int fd, t_scene *scene)
{
	char	*line;
	char	*trimmed;
	int		headers_found;

	headers_found = 0;
	while (headers_found < 6)
	{
		line = get_next_line(fd);
		if (!line)
			fatal_error_fd("Missing scene identifiers (NO,SO,WE,EA,F,C)", fd);
		trimmed = ft_strtrim(line, " \t\n\r");
		free(line);
		if (trimmed[0] != '\0')
		{
			if (!parse_header_line(trimmed, scene, fd))
			{
				free(trimmed);
				fatal_error_fd("Invalid scene identifier", fd);
			}
			headers_found++;
		}
		free(trimmed);
	}
}
