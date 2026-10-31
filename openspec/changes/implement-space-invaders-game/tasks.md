# Tasks

## 1. Build and platform foundation

- [x] 1.1 Add the C17 CMake project, GLFW/GLAD/OpenGL dependency configuration, CTest setup, and a build-time campaign-wave setting defaulting to five; verify `cmake -S . -B build` configures and reports the expected default and an overridden value.
- [x] 1.2 Update the VS Code build and launch configurations to build and debug the CMake game target instead of compiling only the active file; verify a clean VS Code build and launch use the configured target.
- [x] 1.3 Implement GLFW window/context startup, GLAD loading, failure reporting, and ordered shutdown; verify the game opens at the minimum supported size, closes cleanly, and exits with an explicit error when graphics initialization is unavailable.

## 2. Headless game simulation

- [x] 2.1 Implement the game model, player movement commands, horizontal bounds, and elapsed-time updates; add unit tests verifying both movement directions, boundary enforcement, and equivalent movement at different frame rates.
- [x] 2.2 Implement enemy formations, projectiles, and their movement and lifecycle; add unit tests verifying projectile spawn/update/removal, formation reversal and descent at edges, and invader approach toward the player.
- [x] 2.3 Implement projectile-enemy collision handling, enemy removal, score updates, and one-life-per-distinct-breach-or-collision behavior; add unit tests for hit/miss, common-enemy scoring, and life deduction.
- [x] 2.4 Implement wave creation, difficulty progression, and configured campaign victory; add unit tests verifying the 15 percent speed increases for waves two and three, later-wave difficulty increases, and victory on the configured final wave.

## 3. Session states and controls

- [x] 3.1 Implement menu start, pause/resume, Game Over, and restart transitions with the documented keys; add tests verifying initial score/lives, preserved state while paused, terminal behavior at zero lives, and reset values on restart.
- [x] 3.2 Integrate GLFW keyboard events for A/left, D/right, SPACE, ESC, and ENTER with the simulation commands; add an input integration test or repeatable manual check verifying each key has the specified effect only in its applicable state.

## 4. Rendering and user documentation

- [x] 4.1 Implement the OpenGL 3.3 renderer, shaders, logical 1024 by 768 playfield, scalable viewport, game shapes, and space background; verify a graphics smoke test at 800 by 600 and 1024 by 768 keeps required elements visible.
- [x] 4.2 Render title/menu, score, lives, wave, pause, Game Over, and Victory using the bundled glyph set; verify functional checks show the correct text and status for each game state.
- [x] 4.3 Update the project `README.md` with build/run instructions, controls, campaign configuration, and test commands; verify every documented command works from a clean build directory.

## 5. Cross-platform integration and acceptance

- [x] 5.1 Add integration coverage for start, movement, firing, hit/scoring, wave progression, pause/resume, life loss, Game Over, and restart; verify the complete CTest suite passes.
- [x] 5.2 Measure the standard-wave frame rate at 1024 by 768, exercise 100 active projectiles, and run a 30-minute stability session; record whether the approximate 60 FPS target is met and confirm no freezes or unexpected shutdowns.
- [ ] 5.3 Add Windows and Linux CI jobs that configure, build, and run headless tests; verify both platform jobs pass on the change.
