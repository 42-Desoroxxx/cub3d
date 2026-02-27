/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llage <llage@student.42angouleme.fr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/27 05:12:46 by llage             #+#    #+#             */
/*   Updated: 2026/02/27 05:12:46 by llage            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cub3d.h>

void	create_window(t_context *context)
{
	mlx_window_create_info	window_info;

	ft_bzero(&window_info, sizeof(mlx_window_create_info));
	window_info.title = "cub3d";
	window_info.width = WINDOW_WIDTH;
	window_info.height = WINDOW_HEIGHT;
	window_info.is_resizable = false;
	context->window = mlx_new_window(context->context, &window_info);
	if (context->window == NULL)
		fatal_error("Unable to create window");
}

static t_texture	create_texture(const mlx_context *context, char *path)
{
	t_texture	texture;

	texture.image = mlx_new_image_from_file(*context, path, &texture.width,
			&texture.height);
	if (texture.image == NULL)
		fatal_error("Unable to create texture");
	return (texture);
}

static mlx_image	create_framebuffer(const mlx_context *context)
{
	const mlx_image	framebuffer = mlx_new_image(*context, WINDOW_WIDTH,
			WINDOW_HEIGHT);

	if (framebuffer == NULL)
		fatal_error("Unable to create framebuffer");
	return (framebuffer);
}

void	create_textures(t_context *context)
{
	context->textures.wall_north = create_texture(&context->context,
			context->scene.path_north);
	context->textures.wall_east = create_texture(&context->context,
			context->scene.path_east);
	context->textures.wall_south = create_texture(&context->context,
			context->scene.path_south);
	context->textures.wall_west = create_texture(&context->context,
			context->scene.path_west);
	context->framebuffers.back = create_framebuffer(&context->context);
	context->framebuffers.front = create_framebuffer(&context->context);
}
