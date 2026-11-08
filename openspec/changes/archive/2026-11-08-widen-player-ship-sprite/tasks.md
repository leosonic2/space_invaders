# Tasks

## 1. Sprite and Player Dimensions

- [x] 1.1 Redraw the 20-row player sprite with wider, symmetric fighter wings based on `img/nave.jpg`, update `PLAYER_SPRITE_WIDTH`, and set the game player width to the sprite width times the existing pixel scale; verify the project builds and each sprite row matches the declared width.
- [x] 1.2 Update game tests for the new player width while preserving height and bottom margin; verify left/right clamping, centered projectile placement, immediate collision in the added wing span, and that an off-axis invader only costs a life at the bottom edge with the `game_logic` test.

## 2. Runtime Verification

- [x] 2.1 Run the full CTest suite and launch the game at the reference resolution; verify the rendered silhouette matches the reference direction and that the wider ship remains within screen bounds and behaves as expected when firing and colliding.