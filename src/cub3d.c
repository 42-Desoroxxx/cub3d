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

static void	*create_texture(char *path)
{
	void	*handle;

	handle = mlx_new_image_from_file(g_context.context, path, NULL, NULL);
	if (handle == NULL)
		fatal_error("Unable to create texture");
	return (handle);
}

static void	create_textures(void)
{
	g_context.textures.wall_north = create_texture("assets/wall_north.png");
	g_context.textures.wall_east = create_texture("assets/wall_east.png");
	g_context.textures.wall_south = create_texture("assets/wall_south.png");
	g_context.textures.wall_west = create_texture("assets/wall_west.png");
	g_context.textures.player = create_texture("assets/player.png");
	g_context.textures.framebuffer = mlx_new_image(g_context.context, WINDOW_WIDTH, WINDOW_HEIGHT);
	if (g_context.textures.framebuffer == NULL)
		fatal_error("Unable to create screen texture");
}

static void	create_window(void)
{
	mlx_window_create_info	window_info;

	ft_bzero(&window_info, sizeof(mlx_window_create_info));
	window_info.title = "cub3d";
	window_info.width = WINDOW_WIDTH;
	window_info.height = WINDOW_HEIGHT;
	window_info.is_resizable = false;
	g_context.window = mlx_new_window(g_context.context, &window_info);
	if (g_context.window == NULL)
		fatal_error("Unable to create window");
}

int	main(int argc, const char *argv[])
{
	validate_args(argc, argv);
	parse_map(open_map(argv), &g_context);
	g_context.context = mlx_init();
	if (g_context.context == NULL)
		fatal_error("Unable to initialize mlx");
	create_textures();
	create_window();
	setup_events(&g_context);
	mlx_set_fps_goal(g_context.context, 24);
	mlx_add_loop_hook(g_context.context, loop_hook, &g_context);
	mlx_loop(g_context.context);
}

__attribute__((destructor))
static void	cleanup(void)
{
	if (g_context.map != NULL)
		free_map(g_context.map);
	if (g_context.textures.wall_north != NULL)
		mlx_destroy_image(g_context.context, g_context.textures.wall_north);
	if (g_context.textures.wall_east != NULL)
		mlx_destroy_image(g_context.context, g_context.textures.wall_east);
	if (g_context.textures.wall_south != NULL)
		mlx_destroy_image(g_context.context, g_context.textures.wall_south);
	if (g_context.textures.wall_west != NULL)
		mlx_destroy_image(g_context.context, g_context.textures.wall_west);
	if (g_context.textures.framebuffer != NULL)
		mlx_destroy_image(g_context.context, g_context.textures.framebuffer);
	if (g_context.window != NULL)
		mlx_destroy_window(g_context.context, g_context.window);
	if (g_context.context != NULL)
		mlx_destroy_context(g_context.context);
}
