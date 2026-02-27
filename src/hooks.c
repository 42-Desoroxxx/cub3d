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

void	draw(t_context *context)
{
	const mlx_image	old = context->framebuffers.front;
	float			dir[4];
	int				x;

	get_dir(context->player.direction, dir);
	x = -1;
	while (++x < WINDOW_WIDTH)
		draw_column(context, x, dir);
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
