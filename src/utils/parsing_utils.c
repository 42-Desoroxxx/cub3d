/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llage <llage@student.42angouleme.fr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/19 20:56:53 by llage             #+#    #+#             */
/*   Updated: 2026/01/19 20:57:34 by llage            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cub3d.h>

static void	validate_extension(const char *path)
{
	const char	*extension = ft_strrchr(path, '.');

	if (extension == NULL)
		fatal_error("Missing file extension, must be `.cub`");
	if (ft_strncmp(extension, ".cub", 4) != 0 || extension[4] != '\0')
		fatal_error("Invalid file extension, must be `.cub`");
}

void	validate_args(const int argc, const char *argv[])
{
	if (argc == 1)
		fatal_error("Must provide a path to a map file (*.cub)");
	if (argc > 2)
		fatal_error("Too many arguments");
	validate_extension(argv[1]);
}

int	open_map(const char *argv[])
{
	const int	fd = open(argv[1], O_RDONLY);

	if (fd < 0)
		fatal_error("Unable to open map file (Are the permissions correct?) "
			"(Does it exist?)");
	return (fd);
}
