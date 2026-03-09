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

# include <fcntl.h>
# include <ft_memory.h>
# include <ft_printf.h>
# include <get_next_line.h>
# include <libft.h>
# include <mlx.h>
# include <SDL_scancode.h>

# define WINDOW_WIDTH 1280
# define WINDOW_HEIGHT 720

# define FOG_DENSITY 0.2F
# define FLAT_FOG_MINIMUM 0.05F
# define FLAT_FOG_GRADIENT 0.95F

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

typedef struct s_framebuffers
{
	mlx_image	back;
	mlx_image	front;
}	t_framebuffers;

typedef struct s_texture
{
	mlx_image	image;
	int			width;
	int			height;
}	t_texture;

typedef struct s_textures
{
	t_texture	wall_north;
	t_texture	wall_east;
	t_texture	wall_south;
	t_texture	wall_west;
}	t_textures;

typedef struct s_scene
{
	char		*path_north;
	char		*path_south;
	char		*path_east;
	char		*path_west;
	uint32_t	ceiling_color;
	uint32_t	floor_color;
}	t_scene;

typedef struct s_context
{
	mlx_context				context;
	mlx_window				window;
	t_player				player;
	t_framebuffers			framebuffers;
	t_textures				textures;
	t_scene					scene;
	char					**map;
}	t_context;

typedef struct s_ray
{
	float	player_pos[2];
	int		map_pos[2];
	float	delta_dist[2];
	float	side_dist[2];
	int		step[2];
}	t_ray;

typedef struct s_column
{
	int			wall_height;
	int			wall_start;
	int			tex_col_x;
	float		brightness;
	t_texture	*texture;
}	t_column;

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
parse_scene(int fd, t_scene *scene);

void
parse_map(int fd, t_context *context);

// ---–––———–––--- Rendering ---–––———–––--- \\

// draw.c

void
draw_column(t_context *ctx, int x, float direction[4]);

// raycast.c

void
cast_ray(t_context *context, float ray_x, float ray_z, float out[3]);

// shading.c

void
scale_rgb(mlx_color *color, float scale);

float
fog(float distance);

float
flat_fog(int y);

// ---–––———–––--- Utils ---–––———–––--- \\

// create_utils.c

void
create_window(t_context *context);

void
create_textures(t_context *context);

// free_utils.c

void
free_map(char **map);

void
free_resource(void **ptr, void (*destructor)(void *));

// ---–––———–––---–––———–––---–––———–––--- \\

// cub3d.c

void
cleanup(void);

// events.c

void
setup_events(t_context *context);

// hooks.c

void
draw(t_context *context);

void
loop_hook(void *param);

#endif
