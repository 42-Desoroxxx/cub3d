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

static const short	g_window_center_x = WINDOW_WIDTH / 2;
static const short	g_window_center_y = WINDOW_HEIGHT / 2;

mlx_color	torch_tint(mlx_color color, float intensity)
{
	return ((mlx_color){
		.r = (unsigned int) fminf(color.r * intensity * TORCH_RED, 255),
		.g = (unsigned int) fminf(color.g * intensity * TORCH_GREEN, 255),
		.b = (unsigned int) fminf(color.b * intensity * TORCH_BLUE, 255),
		.a = 255
	});
}

float	torch(const int x, const int y)
{
	const float	dx = (x - g_window_center_x) / g_window_center_x;
	const float	dy = (y - g_window_center_y) / g_window_center_y;

	return (1 / (1 + (((dx * dx) + (dy * dy)) * TORCH_LIGHT_FALLOFF)));
}

float	torch_flat(const int x, const int y)
{
	const float	dx = (x - g_window_center_x) / g_window_center_x;
	const float	dy = (y - WINDOW_HEIGHT) / WINDOW_HEIGHT;

	return (1 / (1 + (((dx * dx) + (dy * dy)) * TORCH_LIGHT_FALLOFF)));
}

float	fog(const float distance)
{
	return (1 / (1 + ((distance * distance) * FOG_DENSITY)));
}

float	flat_fog(const int y)
{
	const float	d = fabsf(y - g_window_center_y) / g_window_center_y;

	return (FLAT_FOG_MINIMUM + (FLAT_FOG_GRADIENT * d));
}
