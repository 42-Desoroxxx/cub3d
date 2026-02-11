/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llage <llage@student.42angouleme.fr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/19 20:53:25 by llage             #+#    #+#             */
/*   Updated: 2026/01/19 20:53:29 by llage            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUB3D_H
# define CUB3D_H

# include <libft.h>
# include <ft_memory.h>
# include <ft_printf.h>
# include <get_next_line.h>
# include <mlx.h>
# include <SDL_scancode.h>
# include <fcntl.h>

#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720

typedef enum e_direction : uint8_t
{
	NORTH,
	EAST,
	SOUTH,
	WEST,
}	t_direction;

typedef struct s_player
{
	unsigned short	x;
	unsigned short	z;
	t_direction		direction;
}	t_player;

typedef struct s_textures
{
	void	*wall_north;
	void	*wall_east;
	void	*wall_south;
	void	*wall_west;
	void	*framebuffer;
}	t_textures;

typedef struct s_context
{
	mlx_context				context;
	mlx_window				window;
	t_player				player;
	t_textures				textures;
	char					**map;
}	t_context;

// Utils

_Noreturn void
fatal_error(char *message);

_Noreturn void
fatal_error_fd(char *message, int fd);

_Noreturn void
fatal_error_map(char *message, char **map);

void
validate_args(int argc, const char *argv[]);

int
open_map(const char *argv[]);

void
free_map(char **map);

int
get_width(char **map);

int
get_length(char **map);

char
**copy_map(char **map);

void
validate_via_flood(const t_context *context);

void
validate_chars(char **map);

void
validate_rectangle(char **map);

// Parsing

void
parse_map(int fd, t_context *context);

// events.c

void
setup_events(t_context *context);

// hooks.c

void
loop_hook(void *param);

#endif
