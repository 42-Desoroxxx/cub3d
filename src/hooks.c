/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hooks.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llage <llage@student.42angouleme.fr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/19 20:55:35 by llage             #+#    #+#             */
/*   Updated: 2026/01/19 20:55:35 by llage            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cub3d.h>

void	loop_hook(void *param)
{
	const t_context	*c = param;
	int				x;
	int				y;

	c = param;
	mlx_clear_window(c->context, c->window, (mlx_color){.rgba = 0x996633FF});
	y = -1;
	while (++y < WINDOW_HEIGHT)
	{
		x = -1;
		while (++x < WINDOW_WIDTH)
		{
			if (y < WINDOW_HEIGHT / 2)
				mlx_set_image_pixel(c->context, c->textures.framebuffer, x, y, (mlx_color){.rgba = 0x6699CCFF});
		}
	}
	mlx_put_image_to_window(c->context, c->window, c->textures.framebuffer, 0, 0);
}
