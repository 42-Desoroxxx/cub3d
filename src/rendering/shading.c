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

void	scale_rgb(mlx_color *color, float scale)
{
	color->r = fmaxf(fminf((color->r * scale), 255), 0);
	color->g = fmaxf(fminf((color->g * scale), 255), 0);
	color->b = fmaxf(fminf((color->b * scale), 255), 0);
}

float	fog(const float distance)
{
	return (1 / (1 + ((distance * distance) * FOG_DENSITY)));
}

float	flat_fog(const int y)
{
	const short	window_center_y = WINDOW_HEIGHT / 2;
	const float	dist = fabsf((float) y - window_center_y) / window_center_y;

	return (FLAT_FOG_MINIMUM + (FLAT_FOG_GRADIENT * dist));
}
