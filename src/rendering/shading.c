/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shading.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llage <llage@student.42angouleme.fr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/27 05:44:03 by llage             #+#    #+#             */
/*   Updated: 2026/02/27 05:44:34 by llage            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cub3d.h>
#include <math.h>

mlx_color	shade(mlx_color color, float f)
{
	unsigned int	r;
	unsigned int	g;
	unsigned int	b;

	r = (unsigned int) fminf(((color.rgba >> 24) & 0xFF) * f * 1.1f, 255);
	g = (unsigned int) fminf(((color.rgba >> 16) & 0xFF) * f * 0.9f, 255);
	b = (unsigned int) fminf(((color.rgba >> 8) & 0xFF) * f * 0.7f, 255);
	return ((mlx_color){.rgba = (r << 24) | (g << 16) | (b << 8) | 0xFF});
}

float	torch(int x, int y)
{
	float	dx;
	float	dy;

	dx = (x - WINDOW_WIDTH / 2) / (WINDOW_WIDTH / 2);
	dy = (y - WINDOW_HEIGHT / 2) / (WINDOW_HEIGHT / 2);
	return (1 / (1 + (dx * dx + dy * dy) * 4));
}

float	torch_flat(int x, int y)
{
	float	dx;
	float	dy;

	dx = (x - WINDOW_WIDTH / 2) / (WINDOW_WIDTH / 2);
	dy = (y - WINDOW_HEIGHT) / WINDOW_HEIGHT;
	return (1 / (1 + (dx * dx + dy * dy) * 4));
}

float	fog(float dist)
{
	return (1 / (1 + dist * dist * 0.2));
}

float	flat_fog(int y)
{
	float	d;

	d = fabsf((float) y - WINDOW_HEIGHT / 2) / (WINDOW_HEIGHT / 2);
	return (0.05f + 0.95f * d);
}
