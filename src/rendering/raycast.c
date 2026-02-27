/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycast.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llage <llage@student.42angouleme.fr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/27 06:07:45 by llage             #+#    #+#             */
/*   Updated: 2026/02/27 06:07:45 by llage            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cub3d.h>
#include <math.h>

static void	init_delta_dist(float ray_x, float ray_z, t_ray *ray)
{
	if (ray_x == 0)
		ray->delta_dist[0] = 1e30f;
	else
		ray->delta_dist[0] = fabsf(1.0f / ray_x);
	if (ray_z == 0)
		ray->delta_dist[1] = 1e30f;
	else
		ray->delta_dist[1] = fabsf(1.0f / ray_z);
}

static void	init_step(float ray_x, float ray_z, t_ray *ray)
{
	if (ray_x < 0)
	{
		ray->step[0] = -1;
		ray->side_dist[0] = (ray->player_pos[0] - ray->map_pos[0]);
	}
	else
	{
		ray->step[0] = 1;
		ray->side_dist[0] = (ray->map_pos[0] + 1 - ray->player_pos[0]);
	}
	if (ray_z < 0)
	{
		ray->step[1] = -1;
		ray->side_dist[1] = (ray->player_pos[1] - ray->map_pos[1]);
	}
	else
	{
		ray->step[1] = 1;
		ray->side_dist[1] = (ray->map_pos[1] + 1 - ray->player_pos[1]);
	}
	ray->side_dist[0] *= ray->delta_dist[0];
	ray->side_dist[1] *= ray->delta_dist[1];
}

static void	perform_dda(t_context *context, t_ray *ray, float out[3])
{
	out[2] = 0;
	while (context->map[ray->map_pos[1]][ray->map_pos[0]] != '1')
	{
		if (ray->side_dist[0] < ray->side_dist[1])
		{
			ray->side_dist[0] += ray->delta_dist[0];
			ray->map_pos[0] += ray->step[0];
			out[2] = 0;
		}
		else
		{
			ray->side_dist[1] += ray->delta_dist[1];
			ray->map_pos[1] += ray->step[1];
			out[2] = 1;
		}
	}
}

static void	compute_wall_hit(float ray_x, float ray_z, t_ray *ray, float out[3])
{
	if ((int)out[2] == 0)
		out[0] = (ray->map_pos[0] - ray->player_pos[0]
				+ (1 - ray->step[0]) / 2.0f) / ray_x;
	else
		out[0] = (ray->map_pos[1] - ray->player_pos[1]
				+ (1 - ray->step[1]) / 2.0f) / ray_z;
	if ((int)out[2] == 0)
		out[1] = ray->player_pos[1] + out[0] * ray_z;
	else
		out[1] = ray->player_pos[0] + out[0] * ray_x;
	out[1] -= floorf(out[1]);
}

void	cast_ray(t_context *context, float ray_x, float ray_z, float out[3])
{
	t_ray	ray;

	ray.player_pos[0] = context->player.x + 0.5f;
	ray.player_pos[1] = context->player.z + 0.5f;
	ray.map_pos[0] = (int)ray.player_pos[0];
	ray.map_pos[1] = (int)ray.player_pos[1];
	init_delta_dist(ray_x, ray_z, &ray);
	init_step(ray_x, ray_z, &ray);
	perform_dda(context, &ray, out);
	compute_wall_hit(ray_x, ray_z, &ray, out);
}
