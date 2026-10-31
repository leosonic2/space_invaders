# C API Reference

This reference covers the public declarations in [`include/game.h`](include/game.h) and [`include/renderer.h`](include/renderer.h). The game API is implemented in `src/game.c`; the rendering API is implemented in `src/renderer.c`.

## Game API (`game.h`)

### Constants

| Constant | Value | Meaning |
| --- | ---: | --- |
| `GAME_WORLD_WIDTH` | `1024.0f` | Logical horizontal playfield size in world units. |
| `GAME_WORLD_HEIGHT` | `768.0f` | Logical vertical playfield size in world units. |
| `GAME_STARTING_LIVES` | `3` | Lives assigned when a game is initialized or started. |

### Enumerations

#### `GameState`

| Value | Meaning |
| --- | --- |
| `GAME_STATE_MENU` | Session is waiting to start. |
| `GAME_STATE_PLAYING` | Simulation is active. |
| `GAME_STATE_PAUSED` | Simulation is paused. |
| `GAME_STATE_GAME_OVER` | Player has lost all lives. |
| `GAME_STATE_VICTORY` | Configured final wave has been cleared. |

#### `GameKey`

| Value | Meaning |
| --- | --- |
| `GAME_KEY_FIRE` | Sets the fire event in a `GameInput`. |
| `GAME_KEY_PAUSE` | Sets the pause event in a `GameInput`. |
| `GAME_KEY_CONFIRM` | Sets the confirm event in a `GameInput`. |

These are sequential C enum constants; the API does not assign stable external numeric values to them.

### Structures

#### `Player`

| Field | Type | Meaning |
| --- | --- | --- |
| `x`, `y` | `float` | Top-left position in logical world units. |
| `width`, `height` | `float` | Player rectangle dimensions. |
| `speed` | `float` | Horizontal movement speed in world units per second. |

#### `Enemy`

| Field | Type | Meaning |
| --- | --- | --- |
| `x`, `y` | `float` | Top-left position in logical world units. |
| `width`, `height` | `float` | Enemy rectangle dimensions. |
| `speed` | `float` | Horizontal formation movement speed. |
| `points` | `int` | Score awarded when this enemy is hit. |
| `active` | `bool` | Whether the simulation treats this enemy as active. `game_add_enemy` sets this to `true`. |

#### `Projectile`

| Field | Type | Meaning |
| --- | --- | --- |
| `x`, `y` | `float` | Top-left position in logical world units. |
| `width`, `height` | `float` | Projectile rectangle dimensions. |
| `speed` | `float` | Upward movement speed in world units per second. |
| `active` | `bool` | Whether the simulation treats this projectile as active. |

#### `GameInput`

| Field | Type | Meaning |
| --- | --- | --- |
| `move_left` | `bool` | Left movement is held. |
| `move_right` | `bool` | Right movement is held. |
| `fire` | `bool` | Fire event for this update. |
| `pause` | `bool` | Pause event for this update. |
| `confirm` | `bool` | Confirm/start/resume/restart event for this update. |

The application clears the one-shot `fire`, `pause`, and `confirm` flags after forwarding them to an update; movement is polled as a held input.

#### `Game`

| Field | Type | Meaning |
| --- | --- | --- |
| `state` | `GameState` | Current session state. |
| `player` | `Player` | Player state. |
| `enemies` | `Enemy *` | Dynamically allocated enemy array owned by this game. |
| `enemy_count` | `size_t` | Number of currently stored enemies. Inactive entries are removed during updates. |
| `enemy_capacity` | `size_t` | Allocated enemy-array capacity. |
| `projectiles` | `Projectile *` | Dynamically allocated projectile array owned by this game. |
| `projectile_count` | `size_t` | Number of currently stored projectiles. Inactive entries are removed during updates. |
| `projectile_capacity` | `size_t` | Allocated projectile-array capacity. |
| `score` | `int` | Current score. |
| `lives` | `int` | Remaining lives. |
| `wave` | `int` | Current campaign wave. |
| `final_wave` | `int` | Configured campaign final wave. |
| `wave_enemy_columns` | `int` | Formation columns used by the current wave; also used by renderer coloring. |
| `wave_enemy_speed` | `float` | Current configured formation speed. |
| `enemy_direction` | `float` | Horizontal formation direction, represented as `1.0f` or `-1.0f` by wave creation and edge handling. |
| `enemy_drop_distance` | `float` | Additional vertical distance applied to the formation when it reaches an edge. |

The entity pointers and their capacities are managed by the game implementation. Callers may inspect the public state, but should not free or replace the arrays. The interface does not make `Game` opaque; callers that construct or mutate it directly are responsible for preserving its invariants.

### Functions

#### `bool game_init(Game *game, int final_wave)`

Initializes a game in the menu state, sets up the player and starting lives, and stores the campaign length.

| Parameter | Expectations |
| --- | --- |
| `game` | Non-null pointer to storage for a new `Game`. |
| `final_wave` | Must be at least `1`. |

Returns `true` on success and `false` for a null game pointer or non-positive final wave. The function zeroes the supplied structure before setting initial values. Do not call it on a live `Game` with allocated entity arrays unless `game_destroy` has already released those arrays.

#### `void game_destroy(Game *game)`

Frees the enemy and projectile arrays and zeroes the structure. A null pointer is ignored. Call once for an initialized game when its session is finished; calling it on an already zeroed game is safe.

#### `bool game_start(Game *game)`

Starts a fresh session: resets score, lives, wave, player horizontal position, and active entity counts, then creates the first formation.

| Parameter | Expectations |
| --- | --- |
| `game` | Non-null, initialized `Game` whose `final_wave` is at least `1`. |

Returns `false` for an invalid pointer/final-wave value or if creating the initial formation fails (including entity allocation failure); otherwise returns `true`. On formation creation failure, the state is returned to the menu. Existing array allocations are retained for reuse when the active counts are reset.

#### `bool game_add_enemy(Game *game, Enemy enemy)`

Appends an enemy to the game-owned array and forces its `active` field to `true`.

| Parameter | Expectations |
| --- | --- |
| `game` | Non-null initialized game with internally consistent enemy storage. |
| `enemy` | Enemy values in logical world units; geometry and scores are not otherwise validated. |

Returns `false` for a null game or failed/overflowing capacity growth, and `true` when the enemy is appended. This is a utility exposed in the public header and is also used to build formations.

#### `bool game_fire(Game *game)`

Creates an upward-moving projectile centered horizontally over the player's current position.

| Parameter | Expectations |
| --- | --- |
| `game` | Non-null initialized game with internally consistent projectile storage and player dimensions/position. |

Returns `false` for a null game or failed/overflowing projectile-array growth; otherwise returns `true`. The function itself does not enforce a fixed projectile limit or check the current game state.

#### `void game_input_press(GameInput *input, GameKey key)`

Sets the corresponding `fire`, `pause`, or `confirm` boolean to `true`.

| Parameter | Expectations |
| --- | --- |
| `input` | Pointer to the input structure to update; null is ignored. |
| `key` | One of `GAME_KEY_FIRE`, `GAME_KEY_PAUSE`, or `GAME_KEY_CONFIRM`. |

The function does not clear other flags. An enum value outside the listed constants has no effect.

#### `bool game_update(Game *game, const GameInput *input, float delta_time)`

Advances the game state and, while playing, updates player/enemy/projectile positions, resolves collisions, compacts inactive entities, and advances the wave or terminal state.

| Parameter | Expectations |
| --- | --- |
| `game` | Non-null initialized `Game` with valid entity storage and state. |
| `input` | Non-null input snapshot for this update. |
| `delta_time` | Finite, non-negative elapsed seconds. Values greater than 0.25 seconds are clamped to 0.25 seconds. |

Returns `false` if either pointer is null, `delta_time` is negative or non-finite, or entity allocation fails during firing/wave creation. Otherwise returns `true`. A zero delta still permits firing but skips movement and collision work. Invalid externally constructed game state and enum values are not comprehensively validated.

## Renderer API (`renderer.h`)

Renderer functions that make OpenGL calls require a current, valid OpenGL context with GLAD initialized. The game model and the pure renderer helpers `renderer_calculate_viewport` and `renderer_format_text` do not require a live context.

### Structures

#### `RendererViewport`

| Field | Type | Meaning |
| --- | --- | --- |
| `x`, `y` | `int` | Origin of the viewport in framebuffer pixels. |
| `width`, `height` | `int` | Viewport size in framebuffer pixels. |

#### `Renderer`

| Field | Type | Meaning |
| --- | --- | --- |
| `program` | `unsigned int` | OpenGL shader program handle. |
| `vertex_array` | `unsigned int` | OpenGL vertex-array object handle. |
| `vertex_buffer` | `unsigned int` | OpenGL vertex-buffer object handle. |
| `color_location` | `int` | Location of the shader's `shape_color` uniform. |

Initialize this structure with zero values before calling `renderer_init`. Its OpenGL handles are owned by the renderer and should not be modified or deleted by callers.

#### `RendererText`

| Field | Type | Meaning |
| --- | --- | --- |
| `show_title` | `bool` | Show the menu title. |
| `show_prompt` | `bool` | Show the menu start prompt. |
| `show_hud` | `bool` | Show score, lives, and wave. |
| `show_message` | `bool` | Show a state message. |
| `show_final_score` | `bool` | Show the final score. |
| `show_restart_prompt` | `bool` | Show the restart prompt. |
| `title[32]` | `char` array | Menu title text. |
| `prompt[48]` | `char` array | Menu prompt text. |
| `score[48]` | `char` array | Formatted score. |
| `lives[48]` | `char` array | Formatted remaining lives. |
| `wave[48]` | `char` array | Formatted wave. |
| `message[32]` | `char` array | Pause or terminal-state message. |
| `final_score[48]` | `char` array | Formatted terminal score. |
| `restart_prompt[48]` | `char` array | Terminal-state restart prompt. |

The fixed buffers are filled by `renderer_format_text` using bounded `snprintf` calls.

### Functions

#### `bool renderer_init(Renderer *renderer)`

Zero-initializes the renderer and creates the shader program, vertex array, and vertex buffer.

| Parameter | Expectations |
| --- | --- |
| `renderer` | Non-null pointer to a zero-initialized or otherwise safely disposable `Renderer`. |

Returns `false` for a null pointer, shader file/compile/link failure, or missing `shape_color` uniform; returns `true` after the renderer resources are set up. Shader files are resolved relative to the process working directory under `shaders/`. A current OpenGL context and loaded GLAD functions are required. If reinitializing a live renderer, call `renderer_destroy` first to avoid leaking its existing handles.

#### `void renderer_destroy(Renderer *renderer)`

Deletes nonzero buffer, vertex-array, and program handles, then zeroes the structure. A null pointer is ignored. If handles are live, a current OpenGL context is required.

#### `RendererViewport renderer_calculate_viewport(int width, int height)`

Calculates a centered viewport that preserves the logical playfield aspect ratio.

| Parameter | Expectations |
| --- | --- |
| `width`, `height` | Framebuffer dimensions in pixels. |

Returns a zero-initialized viewport when either input dimension is non-positive. Otherwise returns the largest centered 4:3 viewport that fits the supplied dimensions; integer conversion may truncate a fractional pixel.

#### `bool renderer_format_text(const Game *game, RendererText *text)`

Creates the strings and visibility flags for the supplied game state.

| Parameter | Expectations |
| --- | --- |
| `game` | Non-null game state. |
| `text` | Non-null output structure. |

Returns `false` if either pointer is null or the state is not one of the defined `GameState` values. On valid pointers it clears the output before formatting. The menu shows the title and start prompt; playing/paused states show the HUD, and paused also shows a message; Game Over/victory show the HUD, terminal message, final score, and restart prompt. This helper does not call OpenGL.

#### `void renderer_render(Renderer *renderer, const Game *game, int width, int height)`

Draws the background, applicable game entities, and state-specific text for the supplied framebuffer size.

| Parameter | Expectations |
| --- | --- |
| `renderer` | Successfully initialized renderer. |
| `game` | Readable game state and entity arrays. |
| `width`, `height` | Current framebuffer dimensions in pixels. |

Returns no status. It silently does nothing when the renderer/game pointer is null or the program handle is zero. It requires a current OpenGL context and loaded GLAD functions; invalid game contents are not validated. Rendering includes the fixed starfield, player, entities, HUD, and relevant state messages.
