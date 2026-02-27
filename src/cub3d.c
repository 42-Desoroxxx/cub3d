/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llage <llage@student.42angouleme.fr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/19 20:50:22 by llage             #+#    #+#             */
/*   Updated: 2026/01/19 20:50:22 by llage            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cub3d.h>

static t_context	g_context = {0};

int	main(int argc, const char *argv[])
{
	validate_args(argc, argv);
	parse_map(open_map(argv), &g_context);
	g_context.context = mlx_init();
	if (g_context.context == NULL)
		fatal_error("Unable to initialize mlx");
	create_textures(&g_context);
	create_window(&g_context);
	setup_events(&g_context);
	draw(&g_context);
	mlx_add_loop_hook(g_context.context, loop_hook, &g_context);
	mlx_loop(g_context.context);
	cleanup();
}

static void	destroy_image(void *image)
{
	mlx_destroy_image(g_context.context, image);
}

static void	destroy_window(void *window)
{
	mlx_destroy_window(g_context.context, window);
}

void	cleanup(void)
{
	free(g_context.scene.path_north);
	free(g_context.scene.path_south);
	free(g_context.scene.path_east);
	free(g_context.scene.path_west);
	free_resource((void **)&g_context.textures.wall_north.image, destroy_image);
	free_resource((void **)&g_context.textures.wall_east.image, destroy_image);
	free_resource((void **)&g_context.textures.wall_south.image, destroy_image);
	free_resource((void **)&g_context.textures.wall_west.image, destroy_image);
	free_resource((void **)&g_context.map, (void (*)(void *))free_map);
	free_resource((void **)&g_context.framebuffers.back, destroy_image);
	free_resource((void **)&g_context.framebuffers.front, destroy_image);
	free_resource((void **)&g_context.window, destroy_window);
	free_resource((void **)&g_context.context,
		(void (*)(void *))mlx_destroy_context);
}
