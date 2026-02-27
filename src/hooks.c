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

static void	get_dir(t_direction d, float dir[4])
{
	dir[0] = 0;
	dir[1] = 0;
	dir[2] = 0;
	dir[3] = 0;
	if (d == NORTH)
	{
		dir[1] = -1;
		dir[2] = 0.66F;
	}
	else if (d == SOUTH)
	{
		dir[1] = 1;
		dir[2] = -0.66F;
	}
	else if (d == EAST)
	{
		dir[0] = 1;
		dir[3] = 0.66F;
	}
	else
	{
		dir[0] = -1;
		dir[3] = -0.66F;
	}
}

static t_wall_texture	*get_tex(t_context *ctx, int side, float rx, float rz)
{
	if (side == 0 && rx > 0)
		return (&ctx->textures.wall_east);
	if (side == 0)
		return (&ctx->textures.wall_west);
	if (rz > 0)
		return (&ctx->textures.wall_south);
	return (&ctx->textures.wall_north);
}

static void	cast_ray(t_context *context, float rx, float rz, float out[3])
{
	float	p[2];
	int		m[2];
	float	dd[2];
	float	sd[2];
	int		s[2];

	p[0] = context->player.x + 0.5f;
	p[1] = context->player.z + 0.5f;
	m[0] = (int)p[0];
	m[1] = (int)p[1];
	dd[0] = rx == 0 ? 1e30f : fabsf(1.0f / rx);
	dd[1] = rz == 0 ? 1e30f : fabsf(1.0f / rz);
	s[0] = rx < 0 ? -1 : 1;
	s[1] = rz < 0 ? -1 : 1;
	sd[0] = rx < 0 ? (p[0] - m[0]) * dd[0] : (m[0] + 1.0f - p[0]) * dd[0];
	sd[1] = rz < 0 ? (p[1] - m[1]) * dd[1] : (m[1] + 1.0f - p[1]) * dd[1];
	out[2] = 0;
	while (context->map[m[1]][m[0]] != '1')
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

static mlx_color	shade(mlx_color color, float f)
{
	unsigned int	r;
	unsigned int	g;
	unsigned int	b;

	r = (unsigned int)fminf(((color.rgba >> 24) & 0xFF) * f * 1.1f, 255);
	g = (unsigned int)fminf(((color.rgba >> 16) & 0xFF) * f * 0.9f, 255);
	b = (unsigned int)fminf(((color.rgba >> 8) & 0xFF) * f * 0.7f, 255);
	return ((mlx_color){.rgba = (r << 24) | (g << 16) | (b << 8) | 0xFF});
}

static float	torch(int x, int y)
{
	float	dx;
	float	dy;

	dx = (x - WINDOW_WIDTH / 2) / (WINDOW_WIDTH / 2);
	dy = (y - WINDOW_HEIGHT / 2) / (WINDOW_HEIGHT / 2);
	return (1 / (1 + (dx * dx + dy * dy) * 4));
}

static float	torch_flat(int x, int y)
{
	float	dx;
	float	dy;

	dx = (x - WINDOW_WIDTH / 2) / (WINDOW_WIDTH / 2);
	dy = (y - WINDOW_HEIGHT) / WINDOW_HEIGHT;
	return (1 / (1 + (dx * dx + dy * dy) * 4));
}

static float	fog(float dist)
{
	return (1 / (1 + dist * dist * 0.2));
}

static float	flat_fog(int y)
{
	float	d;

	d = fabsf((float) y - WINDOW_HEIGHT / 2) / (WINDOW_HEIGHT / 2);
	return (0.05f + 0.95f * d);
}

static void	draw_col(t_context *context, int x, float dir[4])
{
	float			out[3];
	float			ray[2];
	t_wall_texture	*tex;
	int				col[3];
	int				y;
	float			w;

	ray[0] = dir[0] + dir[2] * (2.0f * x / WINDOW_WIDTH - 1.0f);
	ray[1] = dir[1] + dir[3] * (2.0f * x / WINDOW_WIDTH - 1.0f);
	cast_ray(context, ray[0], ray[1], out);
	col[0] = (int)(WINDOW_HEIGHT / out[0]);
	col[1] = (WINDOW_HEIGHT - col[0]) / 2;
	tex = get_tex(context, (int)out[2], ray[0], ray[1]);
	col[2] = (int)(out[1] * tex->width) % tex->width;
	w = fog(out[0]) * (1 - 0.3f * (int)out[2]);
	y = -1;
	while (++y < WINDOW_HEIGHT)
	{
		if (y >= col[1] && y < col[1] + col[0])
			mlx_set_image_pixel(context->context, context->framebuffers.back,
				x, y, shade(mlx_get_image_pixel(context->context, tex->image,
						col[2], ((y - col[1]) * tex->height) / col[0]),
					torch(x, y) * w));
		else if (y < WINDOW_HEIGHT / 2)
			mlx_set_image_pixel(context->context, context->framebuffers.back,
				x, y, shade((mlx_color){.rgba = context->scene.ceiling_color},
					torch_flat(x, y) * flat_fog(y)));
		else
			mlx_set_image_pixel(context->context, context->framebuffers.back,
				x, y, shade((mlx_color){.rgba = context->scene.floor_color},
					torch_flat(x, y) * flat_fog(y)));
	}
}

void	draw(t_context *context)
{
	const mlx_image	old = context->framebuffers.front;
	float			dir[4];
	int				x;

	get_dir(context->player.direction, dir);
	x = -1;
	while (++x < WINDOW_WIDTH)
		draw_col(context, x, dir);
	context->framebuffers.front = context->framebuffers.back;
	context->framebuffers.back = old;
}

void	loop_hook(void *param)
{
	t_context	*context;

	context = param;
	mlx_clear_window(context->context, context->window,
		(mlx_color){.rgba = 0x000000FF});
	mlx_put_image_to_window(context->context, context->window,
		context->framebuffers.front, 0, 0);
}
