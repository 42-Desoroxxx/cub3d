/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hooks.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llage <llage@student.42angouleme.fr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/19 20:55:35 by llage             #+#    #+#             */
/*   Updated: 2026/02/12 02:01:15 by llage            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cub3d.h>
#include <math.h>

#define TEX 64

static void	get_dir(t_direction d, float dir[4])
{
	dir[0] = 0;
	dir[1] = 0;
	dir[2] = 0;
	dir[3] = 0;
	if (d == NORTH)
	{
		dir[1] = -1;
		dir[2] = 0.66f;
	}
	else if (d == SOUTH)
	{
		dir[1] = 1;
		dir[2] = -0.66f;
	}
	else if (d == EAST)
	{
		dir[0] = 1;
		dir[3] = 0.66f;
	}
	else
	{
		dir[0] = -1;
		dir[3] = -0.66f;
	}
}

static mlx_image	get_tex(t_context *c, int side, float rx, float rz)
{
	if (side == 0 && rx > 0)
		return (c->textures.wall_east);
	if (side == 0)
		return (c->textures.wall_west);
	if (rz > 0)
		return (c->textures.wall_south);
	return (c->textures.wall_north);
}

static void	cast_ray(t_context *c, float rx, float rz, float out[3])
{
	float	p[2];
	int		m[2];
	float	dd[2];
	float	sd[2];
	int		s[2];

	p[0] = c->player.x + 0.5f;
	p[1] = c->player.z + 0.5f;
	m[0] = (int)p[0];
	m[1] = (int)p[1];
	dd[0] = rx == 0 ? 1e30f : fabsf(1.0f / rx);
	dd[1] = rz == 0 ? 1e30f : fabsf(1.0f / rz);
	s[0] = rx < 0 ? -1 : 1;
	s[1] = rz < 0 ? -1 : 1;
	sd[0] = rx < 0 ? (p[0] - m[0]) * dd[0] : (m[0] + 1.0f - p[0]) * dd[0];
	sd[1] = rz < 0 ? (p[1] - m[1]) * dd[1] : (m[1] + 1.0f - p[1]) * dd[1];
	out[2] = 0;
	while (c->map[m[1]][m[0]] != '1')
	{
		if (sd[0] < sd[1])
		{
			sd[0] += dd[0];
			m[0] += s[0];
			out[2] = 0;
		}
		else
		{
			sd[1] += dd[1];
			m[1] += s[1];
			out[2] = 1;
		}
	}
	if ((int)out[2] == 0)
		out[0] = (m[0] - p[0] + (1 - s[0]) / 2.0f) / rx;
	else
		out[0] = (m[1] - p[1] + (1 - s[1]) / 2.0f) / rz;
	if ((int)out[2] == 0)
		out[1] = p[1] + out[0] * rz;
	else
		out[1] = p[0] + out[0] * rx;
	out[1] -= floorf(out[1]);
}

/*
** Tints RGB with warm orange (torch color) and dims by factor f.
*/
static mlx_color	shade(mlx_color col, float f)
{
	unsigned int	r;
	unsigned int	g;
	unsigned int	b;

	r = (unsigned int)fminf(((col.rgba >> 24) & 0xFF) * f * 1.1f, 255);
	g = (unsigned int)fminf(((col.rgba >> 16) & 0xFF) * f * 0.9f, 255);
	b = (unsigned int)fminf(((col.rgba >> 8) & 0xFF) * f * 0.7f, 255);
	return ((mlx_color){.rgba = (r << 24) | (g << 16) | (b << 8) | 0xFF});
}

/*
** Radial vignette: 1.0 at screen center, fades to ~0.3 in corners.
** Simulates a cone of light from the player's torch.
*/
static float	torch(int x, int y)
{
	float	dx;
	float	dy;

	dx = (x - WINDOW_WIDTH / 2.0f) / (WINDOW_WIDTH / 2.0f);
	dy = (y - WINDOW_HEIGHT / 2.0f) / (WINDOW_HEIGHT / 2.0f);
	return (1.0f / (1.0f + (dx * dx + dy * dy) * 4.0f));
}

/*
** Same as torch but light originates from bottom-center (player's position).
** Creates a semicircle on the floor, ceiling stays mostly dark.
*/
static float	torch_flat(int x, int y)
{
	float	dx;
	float	dy;

	dx = (x - WINDOW_WIDTH / 2.0f) / (WINDOW_WIDTH / 2.0f);
	dy = (y - (float)WINDOW_HEIGHT) / (float)WINDOW_HEIGHT;
	return (1.0f / (1.0f + (dx * dx + dy * dy) * 4.0f));
}


/*
** Walls darken with distance squared.
*/
static float	fog(float dist)
{
	return (1.0f / (1.0f + dist * dist * 0.2f));
}

/*
** Ceiling/floor: horizon is far (dark), top/bottom edges are near (bright).
*/
static float	flat_fog(int y)
{
	float	d;

	d = fabsf(y - WINDOW_HEIGHT / 2.0f) / (WINDOW_HEIGHT / 2.0f);
	return (0.05f + 0.95f * d);
}

static void	draw_col(t_context *c, int x, float dir[4])
{
	float		out[3];
	float		ray[2];
	mlx_image	tex;
	int			col[3];
	int			y;
	float		w;

	ray[0] = dir[0] + dir[2] * (2.0f * x / WINDOW_WIDTH - 1.0f);
	ray[1] = dir[1] + dir[3] * (2.0f * x / WINDOW_WIDTH - 1.0f);
	cast_ray(c, ray[0], ray[1], out);
	col[0] = (int)(WINDOW_HEIGHT / out[0]);
	col[1] = (WINDOW_HEIGHT - col[0]) / 2;
	col[2] = (int)(out[1] * TEX) % TEX;
	tex = get_tex(c, (int)out[2], ray[0], ray[1]);
	w = fog(out[0]) * (1.0f - 0.3f * (int)out[2]);
	y = -1;
	while (++y < WINDOW_HEIGHT)
	{
		if (y >= col[1] && y < col[1] + col[0])
			mlx_set_image_pixel(c->context, c->textures.framebuffer, x, y,
				shade(mlx_get_image_pixel(c->context, tex, col[2],
					((y - col[1]) * TEX) / col[0]), torch(x, y) * w));
		else if (y < WINDOW_HEIGHT / 2)
			mlx_set_image_pixel(c->context, c->textures.framebuffer, x, y,
				shade((mlx_color){.rgba = 0x6699CCFF},
					torch_flat(x, y) * flat_fog(y)));
		else
			mlx_set_image_pixel(c->context, c->textures.framebuffer, x, y,
				shade((mlx_color){.rgba = 0x996633FF},
					torch_flat(x, y) * flat_fog(y)));
	}
}

static const unsigned char	g_arrows[4][8] = {
	{0x18, 0x3C, 0x7E, 0x18, 0x18, 0x18, 0x18, 0x00},
	{0x00, 0x04, 0x06, 0x7F, 0x7F, 0x06, 0x04, 0x00},
	{0x00, 0x18, 0x18, 0x18, 0x18, 0x7E, 0x3C, 0x18},
	{0x00, 0x20, 0x60, 0xFE, 0xFE, 0x60, 0x20, 0x00},
};

#define MM_RADIUS 8
#define MM_SIZE 8
#define MM_SZ ((MM_RADIUS * 2 + 1) * MM_SIZE)

static int	in_map(char **map, int x, int y)
{
	int	i;

	if (y < 0 || x < 0)
		return (0);
	i = -1;
	while (++i <= y)
		if (!map[i])
			return (0);
	return (x < (int)ft_strlen(map[y]));
}

static mlx_color	mm_pixel(t_context *c, int x, int y)
{
	const int tx = x / MM_SIZE - MM_RADIUS;
	const int ty = y / MM_SIZE - MM_RADIUS;

	if (tx == 0 && ty == 0 && (g_arrows[c->player.direction][(y % MM_SIZE) * 8 / MM_SIZE] & (0x80 >> ((x % MM_SIZE) * 8 / MM_SIZE))))
		return ((mlx_color){.rgba = 0xFF0000FF});
	if (tx == 0 && ty == 0)
		return ((mlx_color){.rgba = 0x333333FF});
	if (c->map[c->player.z + ty][c->player.x + tx] == '1')
		return ((mlx_color){.rgba = 0xFFFFFFFF});
	return ((mlx_color){.rgba = 0x333333FF});
}

static void	draw_minimap(t_context *c)
{
	int	x;
	int	y;

	y = -1;
	while (++y < MM_SZ)
	{
		x = -1;
		while (++x < MM_SZ)
		{
			if (!in_map(c->map, c->player.x + (x / MM_SIZE - MM_RADIUS), c->player.z + (y / MM_SIZE - MM_RADIUS)))
				continue;
			mlx_set_image_pixel(c->context, c->textures.framebuffer,x, y, mm_pixel(c, x, y));
		}
	}
}

void	loop_hook(void *param)
{
	t_context	*c;
	float		dir[4];
	int			x;

	c = param;
	mlx_clear_window(c->context, c->window, (mlx_color){.rgba = 0x000000FF});
	get_dir(c->player.direction, dir);
	x = -1;
	while (++x < WINDOW_WIDTH)
		draw_col(c, x, dir);
	draw_minimap(c);
	mlx_put_image_to_window(c->context, c->window,
		c->textures.framebuffer, 0, 0);
}
