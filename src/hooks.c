/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hooks.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llage <llage@student.42angouleme.fr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/19 20:55:35 by llage             #+#    #+#             */
/*   Updated: 2026/02/03 19:43:40 by llage            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* hooks.c */
#include <cub3d.h>
#include <math.h>

#define W 1280
#define H 720
#define TEX 64

static void	draw_col(t_context *c, mlx_image img, int x, int draw[3],
            mlx_image tex, float wall_x)
{
    int			y;
    int			tex_x;
    int			tex_y;
    mlx_color	col;

    tex_x = (int)(wall_x * TEX) % TEX;
    y = -1;
    while (++y < H)
    {
        if (y < draw[0])
            col = (mlx_color){.rgba = 0x000000FF};
        else if (y >= draw[1])
            col = (mlx_color){.rgba = 0x999999FF};
        else
        {
            tex_y = ((y - draw[0]) * TEX) / draw[2];
            col = mlx_get_image_pixel(c->context, tex, tex_x, tex_y);
        }
        mlx_set_image_pixel(c->context, img, x, y, col);
    }
}

static mlx_image	get_tex(t_context *c, int side, float rx, float rz)
{
    if (side == 0)
    {
        if (rx > 0)
            return (c->textures.wall_east);
        return (c->textures.wall_west);
    }
    if (rz > 0)
        return (c->textures.wall_south);
    return (c->textures.wall_north);
}

static void	cast(t_context *c, float rx, float rz, float out[3])
{
    int		m[2];
    float	dd[2];
    float	sd[2];
    int		s[2];
    float	p[2];

    p[0] = c->player.x + 0.5f;
    p[1] = c->player.z + 0.5f;
    m[0] = (int)p[0];
    m[1] = (int)p[1];
    if (rx == 0)
        dd[0] = 1e30f;
    else
        dd[0] = fabsf(1.0f / rx);
    if (rz == 0)
        dd[1] = 1e30f;
    else
        dd[1] = fabsf(1.0f / rz);
    s[0] = 1;
    s[1] = 1;
    if (rx < 0)
        s[0] = -1;
    if (rz < 0)
        s[1] = -1;
    if (rx < 0)
        sd[0] = (p[0] - m[0]) * dd[0];
    else
        sd[0] = (m[0] + 1.0f - p[0]) * dd[0];
    if (rz < 0)
        sd[1] = (p[1] - m[1]) * dd[1];
    else
        sd[1] = (m[1] + 1.0f - p[1]) * dd[1];
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
    else if (d == WEST)
    {
        dir[0] = -1;
        dir[3] = -0.66f;
    }
}

void	loop_hook(void *param)
{
    t_context	*c;
    mlx_image	img;
    float		dir[4];
    float		out[3];
    int			draw[3];
    int			x;
    float		cam;
    float		ray[2];

    c = param;
    img = mlx_new_image(c->context, W, H);
    get_dir(c->player.direction, dir);
    x = -1;
    while (++x < W)
    {
        cam = 2.0f * x / W - 1.0f;
        ray[0] = dir[0] + dir[2] * cam;
        ray[1] = dir[1] + dir[3] * cam;
        cast(c, ray[0], ray[1], out);
        draw[2] = (int)(H / out[0]);
        draw[0] = (H - draw[2]) / 2;
        draw[1] = (H + draw[2]) / 2;
        if (draw[0] < 0)
            draw[0] = 0;
        if (draw[1] > H)
            draw[1] = H;
        draw_col(c, img, x, draw, get_tex(c, (int)out[2], ray[0], ray[1]),
            out[1]);
    }
    mlx_put_image_to_window(c->context, c->window, img, 0, 0);
    mlx_destroy_image(c->context, img);
}
