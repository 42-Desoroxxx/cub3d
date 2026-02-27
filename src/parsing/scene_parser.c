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

static uint32_t	parse_color(char *str, int fd)
{
	int		rgb[3];
	char	*p;

	while (*str == ' ' || *str == '\t')
		str++;
	rgb[0] = ft_atoi(str);
	p = ft_strchr(str, ',');
	if (!p)
		fatal_error_fd("Invalid color format", fd);
	rgb[1] = ft_atoi(++p);
	p = ft_strchr(p, ',');
	if (!p)
		fatal_error_fd("Invalid color format", fd);
	rgb[2] = ft_atoi(++p);
	if (rgb[0] < 0 || rgb[0] > 255 || rgb[1] < 0 || rgb[1] > 255
		|| rgb[2] < 0 || rgb[2] > 255)
		fatal_error_fd("Color values must be in [0,255]", fd);
	return (((uint32_t)rgb[0] << 24) | ((uint32_t)rgb[1] << 16)
		| ((uint32_t)rgb[2] << 8) | 0xFF);
}

static void	set_path(char **dst, char *src, int fd)
{
	if (*dst != NULL)
		fatal_error_fd("Duplicate texture identifier", fd);
	*dst = ft_strtrim(src, " \t");
	if (*dst == NULL)
		fatal_error_fd("Memory allocation failed", fd);
}

static int	parse_header_line(char *line, t_scene *scene, int fd)
{
	if (ft_strncmp(line, "NO ", 3) == 0)
		set_path(&scene->path_north, line + 3, fd);
	else if (ft_strncmp(line, "SO ", 3) == 0)
		set_path(&scene->path_south, line + 3, fd);
	else if (ft_strncmp(line, "WE ", 3) == 0)
		set_path(&scene->path_west, line + 3, fd);
	else if (ft_strncmp(line, "EA ", 3) == 0)
		set_path(&scene->path_east, line + 3, fd);
	else if (ft_strncmp(line, "F ", 2) == 0)
		scene->floor_color = parse_color(line + 2, fd);
	else if (ft_strncmp(line, "C ", 2) == 0)
		scene->ceiling_color = parse_color(line + 2, fd);
	else
		return (0);
	return (1);
}

void	parse_scene(int fd, t_scene *scene)
{
	char	*line;
	char	*trimmed;
	int		count;

	count = 0;
	while (count < 6)
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
			count++;
		}
		free(trimmed);
	}
}
