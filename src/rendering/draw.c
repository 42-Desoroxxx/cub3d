/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llage <llage@student.42angouleme.fr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/27 06:10:57 by llage             #+#    #+#             */
/*   Updated: 2026/02/27 06:10:57 by llage            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cub3d.h>
#include <math.h>

static t_texture	*get_tex(t_context *ctx, int side, float rx, float rz)
{
	if (side == 0 && rx > 0)
		return (&ctx->textures.wall_east);
	if (side == 0)
		return (&ctx->textures.wall_west);
	if (rz > 0)
		return (&ctx->textures.wall_south);
	return (&ctx->textures.wall_north);
}

static void	compute_column(t_context *ctx, float ray_dir[2], float hit[3],
		t_column *column)
{
	t_texture	*texture;

	column->wall_height = (int)(WINDOW_HEIGHT / hit[0]);
	column->wall_start = (WINDOW_HEIGHT - column->wall_height) / 2;
	texture = get_tex(ctx, (int)hit[2], ray_dir[0], ray_dir[1]);
	column->tex_col_x = (int)(hit[1] * texture->width) % texture->width;
	column->brightness = fog(hit[0]) * (1 - 0.3f * (int)hit[2]);
	column->texture = texture;
}

static void	draw_wall_pixel(t_context *ctx, t_column *column, int x, int y)
{
	int			tex_y;
	mlx_color	color;

	tex_y = ((y - column->wall_start) * column->texture->height)
		/ column->wall_height;
	color = mlx_get_image_pixel(ctx->context, column->texture->image,
			column->tex_col_x, tex_y);
	mlx_set_image_pixel(ctx->context, ctx->framebuffers.back,
		x, y, shade(color, torch(x, y) * column->brightness));
}

static void	draw_column_pixels(t_context *ctx, t_column *column, int x)
{
	mlx_color	flat_color;
	int			wall_end;
	int			y;

	wall_end = column->wall_start + column->wall_height;
	y = -1;
	while (++y < WINDOW_HEIGHT)
	{
		if (y >= column->wall_start && y < wall_end)
			draw_wall_pixel(ctx, column, x, y);
		else
		{
			if (y < WINDOW_HEIGHT / 2)
				flat_color.rgba = ctx->scene.ceiling_color;
			else
				flat_color.rgba = ctx->scene.floor_color;
			mlx_set_image_pixel(ctx->context, ctx->framebuffers.back,
				x, y, shade(flat_color, torch_flat(x, y) * flat_fog(y)));
		}
	}
}

void	draw_column(t_context *ctx, int x, float direction[4])
{
	float		hit[3];
	float		ray_direction[2];
	t_column	column;
	float		offset;

	offset = (float) 2 * x / WINDOW_WIDTH - 1;
	ray_direction[0] = direction[0] + direction[2] * offset;
	ray_direction[1] = direction[1] + direction[3] * offset;
	cast_ray(ctx, ray_direction[0], ray_direction[1], hit);
	compute_column(ctx, ray_direction, hit, &column);
	draw_column_pixels(ctx, &column, x);
}
