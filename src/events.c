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

static void	move_player_sideways(t_context *context, bool left)
{
	const t_direction	direction = context->player.direction;
	int					x;
	int					z;

	x = context->player.x;
	z = context->player.z;
	if (direction == NORTH && left)
		x--;
	else if (direction == NORTH)
		x++;
	else if (direction == EAST && left)
		z--;
	else if (direction == EAST)
		z++;
	else if (direction == SOUTH && left)
		x++;
	else if (direction == SOUTH)
		x--;
	else if (direction == WEST && left)
		z++;
	else if (direction == WEST)
		z--;
	if (context->map[z][x] == '1')
		return ;
	context->player.x = x;
	context->player.z = z;
}

static void	move_player(t_context *context, bool forward)
{
	const t_direction	direction = context->player.direction;
	int					x;
	int					z;

	x = context->player.x;
	z = context->player.z;
	if (direction == NORTH && forward)
		z--;
	else if (direction == NORTH)
		z++;
	else if (direction == EAST && forward)
		x++;
	else if (direction == EAST)
		x--;
	else if (direction == SOUTH && forward)
		z++;
	else if (direction == SOUTH)
		z--;
	else if (direction == WEST && forward)
		x--;
	else if (direction == WEST)
		x++;
	if (context->map[z][x] == '1')
		return ;
	context->player.x = x;
	context->player.z = z;
}

static void	on_window_event(const int event, void *param)
{
	if (event == 0)
		mlx_loop_end(*((mlx_context *) param));
}

// TODO: Only draw if something really changed instead of every input
static void	on_key_down_event(const int key, void *param)
{
	t_context	*context;

	context = param;
	if (key == SDL_SCANCODE_ESCAPE)
		mlx_loop_end(context->context);
	else if (key == SDL_SCANCODE_W)
		move_player(context, true);
	else if (key == SDL_SCANCODE_A)
		move_player_sideways(context, true);
	else if (key == SDL_SCANCODE_S)
		move_player(context, false);
	else if (key == SDL_SCANCODE_D)
		move_player_sideways(context, false);
	else if (key == SDL_SCANCODE_LEFT)
	{
		if (context->player.direction == NORTH)
			context->player.direction = WEST;
		else if (context->player.direction == EAST)
			context->player.direction = NORTH;
		else if (context->player.direction == SOUTH)
			context->player.direction = EAST;
		else if (context->player.direction == WEST)
			context->player.direction = SOUTH;
	}
	else if (key == SDL_SCANCODE_RIGHT)
	{
		if (context->player.direction == NORTH)
			context->player.direction = EAST;
		else if (context->player.direction == EAST)
			context->player.direction = SOUTH;
		else if (context->player.direction == SOUTH)
			context->player.direction = WEST;
		else if (context->player.direction == WEST)
			context->player.direction = NORTH;
	}
	draw(context);
}

void	setup_events(t_context *context)
{
	mlx_on_event(context->context, context->window,
		MLX_WINDOW_EVENT, on_window_event, &context->context);
	mlx_on_event(context->context, context->window,
		MLX_KEYDOWN, on_key_down_event, context);
}
