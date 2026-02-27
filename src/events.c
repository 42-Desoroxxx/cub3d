/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   events.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llage <llage@student.42angouleme.fr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/21 19:33:41 by llage             #+#    #+#             */
/*   Updated: 2026/01/21 19:33:41 by llage            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cub3d.h>

static void	move_player(t_context *context, bool sideways, bool positive)
{
	static const int	dx[2][4] = {{0, 1, 0, -1}, {-1, 0, 1, 0}};
	static const int	dz[2][4] = {{-1, 0, 1, 0}, {0, -1, 0, 1}};
	int					sign;
	int					x;
	int					z;

	sign = -1;
	if (positive)
		sign = 1;
	x = context->player.x + sign * dx[sideways][context->player.direction];
	z = context->player.z + sign * dz[sideways][context->player.direction];
	if (context->map[z][x] == '1')
		return ;
	context->player.x = x;
	context->player.z = z;
}

static void	rotate_player(t_context *context, bool right)
{
	if (right)
		context->player.direction = (context->player.direction + 1) % 4;
	else
		context->player.direction = (context->player.direction + 3) % 4;
}

static void	on_key_down_event(const int key, void *param)
{
	t_context	*context;
	t_player	before;
	bool		changed;

	context = param;
	before = context->player;
	if (key == SDL_SCANCODE_ESCAPE)
		mlx_loop_end(context->context);
	else if (key == SDL_SCANCODE_W)
		move_player(context, false, true);
	else if (key == SDL_SCANCODE_A)
		move_player(context, true, true);
	else if (key == SDL_SCANCODE_S)
		move_player(context, false, false);
	else if (key == SDL_SCANCODE_D)
		move_player(context, true, false);
	else if (key == SDL_SCANCODE_LEFT)
		rotate_player(context, false);
	else if (key == SDL_SCANCODE_RIGHT)
		rotate_player(context, true);
	changed = context->player.x != before.x || context->player.z != before.z
		|| context->player.direction != before.direction;
	if (changed)
		draw(context);
}

static void	on_window_event(const int event, void *param)
{
	if (event == 0)
		mlx_loop_end(*((mlx_context *) param));
}

void	setup_events(t_context *context)
{
	mlx_on_event(context->context, context->window,
		MLX_KEYDOWN, on_key_down_event, context);
	mlx_on_event(context->context, context->window,
		MLX_WINDOW_EVENT, on_window_event, &context->context);
}
