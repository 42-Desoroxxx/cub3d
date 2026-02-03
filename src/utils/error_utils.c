/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llage <llage@student.42angouleme.fr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/19 20:56:53 by llage             #+#    #+#             */
/*   Updated: 2026/01/19 20:57:23 by llage            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cub3d.h>

_Noreturn void	fatal_error(char *message)
{
	ft_fprintf(STDERR_FILENO, "Error\n%s\n", message);
	exit(EXIT_FAILURE);
}

_Noreturn void	fatal_error_fd(char *message, int fd)
{
	close(fd);
	ft_fprintf(STDERR_FILENO, "Error\n%s\n", message);
	exit(EXIT_FAILURE);
}

_Noreturn void	fatal_error_map(char *message, char **map)
{
	free_map(map);
	ft_fprintf(STDERR_FILENO, "Error\n%s\n", message);
	exit(EXIT_FAILURE);
}
