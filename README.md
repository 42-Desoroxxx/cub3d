*This project has been created as part of the 42 curriculum by [llage](https://profile.intra.42.fr/users/llage), [rvitiell](https://profile.intra.42.fr/users/rvitiell).*

# cub3d

A minimal raycasting engine inspired by Wolfenstein 3D, written in C using the MiniLibX graphics library.

---

## Description

cub3d is a 42 school project whose goal is to implement a first-person 3D perspective using raycasting.

Having already worked with raycasting and 3D rendering before, the design philosophy here was deliberate minimalism: implement exactly what the subject requires, nothing more. The result is intentionally stripped down — grid-locked movement, four cardinal directions, one tile at a time — inspired by classic dungeon crawlers like *Dungeon Master* (1987), *Eye of the Beholder* (1991), and the modern throwback *Aeon of Sands*.

The renderer is a **1D raycaster**: rather than casting a ray per pixel like a full 3D engine, it casts one ray per column of pixels. Each ray returns a single distance value that determines the height of the wall slice drawn in that column. The projection plane is a 1D horizontal line, and the world is assumed to be flat — there is no vertical angle, just height derived from distance.

**What the engine does:**

- Parses a `.cub` scene file defining wall textures (N/S/E/W), floor color, and ceiling color
- Validates the map for correctness: legal characters, a single player spawn, and a fully closed playable area (verified via flood fill)
- Renders the scene using the DDA (Digital Differential Analysis) raycasting algorithm
- Applies distance-based fog and side-shading to walls
- Applies a flat fog gradient to floor and ceiling pixels
- Double-buffers the framebuffer to avoid tearing

---

## Instructions

### Dependencies

- A C compiler (`cc`)
- [MacroLibX](https://github.com/seekrs/MacroLibX)
- [libft](https://github.com/42School/libft) (or your own implementation)
- SDL2 (required by MacroLibX)

### Compilation

```sh
make
```

This produces the `cub3d` binary.

### Running

```sh
./cub3d path/to/map.cub
```

The `.cub` file must define the six scene identifiers before the map:

```
NO ./textures/north.xpm
SO ./textures/south.xpm
WE ./textures/west.xpm
EA ./textures/east.xpm
F 50,50,50
C 135,206,235
```

Followed by the map itself, made up of `0` (empty), `1` (wall), spaces, and one player spawn character (`N`, `S`, `E`, or `W`).

### Controls

| Key   | Action        |
|-------|---------------|
| `W`   | Move forward  |
| `S`   | Move backward |
| `A`   | Strafe left   |
| `D`   | Strafe right  |
| `←`   | Rotate left   |
| `→`   | Rotate right  |
| `ESC` | Quit          |

---

## Resources

### Raycasting & DDA

- [Lode's Raycasting Tutorial](https://lodev.org/cgtutor/raycasting.html) — the canonical reference for DDA-based raycasting in the style of this project
- [3DSage YouTube series on raycasting](https://www.youtube.com/watch?v=gYRrGTC7GtA) — a beginner-friendly video walkthrough

### MacroLibX

- [MacroLibX repo](https://github.com/seekrs/MacroLibX) — a modern, maintained alternative to MiniLibX used in this project
- [MacroLibX documentation](https://macrolibx.kbz8.me/)

### AI Usage

Claude (Anthropic) was used during this project for the following tasks:

- **Exploring concepts** — using AI as a learning aid to understand raycasting theory, DDA, and related math before and during implementation.
- **Code cleanup** — reviewing and refining code for clarity, consistency, and style.
- **Generating this README** — the structure, wording, and formatting of this file were produced with AI assistance.

No AI was used to directly write core engine logic.
