# Testing

## Test strategy

The project uses CTest to run five C test executables. The suite focuses on game rules and deterministic renderer helpers; it does not create a GLFW window or OpenGL context. This makes the current automated tests suitable for headless environments such as CI runners.

## Configure, build, and run

From the repository root, install the prerequisites described in [README.md](README.md), then run:

```sh
cmake -S . -B build -G Ninja
cmake --build build --parallel
ctest --test-dir build --output-on-failure
```

To fail if CTest has not discovered any tests, use:

```sh
ctest --test-dir build --output-on-failure --no-tests=error
```

To verify a non-default campaign configuration, configure a build with an override:

```sh
cmake -S . -B build-wave-8 -G Ninja -DSPACE_INVADERS_FINAL_WAVE=8
cmake --build build-wave-8 --parallel
ctest --test-dir build-wave-8 --output-on-failure
```

`BUILD_TESTING` is controlled by CTest and is enabled by default unless explicitly disabled. The campaign configuration test receives the configured wave value as its expected value at compile time.

## Automated test targets

| CTest name | Executable | Current coverage |
| --- | --- | --- |
| `campaign_configuration` | `test_configuration` | Confirms the generated `SPACE_INVADERS_FINAL_WAVE` equals the value CMake supplied to the test. |
| `game_logic` | `test_game` | Checks left/right movement and world bounds; movement equivalence at 30 and 60 updates per second; enemy edge reversal/descent; projectile movement and removal off-screen; enemy downward movement; hit/miss behavior and scoring; one-life loss for an enemy contact; wave difficulty and final-wave victory; menu, pause/resume, Game Over, and restart. |
| `renderer_viewport` | `test_renderer` | Checks the 800 x 600 and 1024 x 768 reference viewports, 4:3 letterboxing at 1280 x 720, and a zero-width invalid input. |
| `renderer_text` | `test_renderer_text` | Checks menu title/prompt, gameplay and paused HUD, Game Over and victory text, final score/restart prompt, and null/invalid-state arguments. |
| `game_integration` | `test_integration` | Exercises a session through start, movement, firing, hit/scoring, wave progression, pause/resume, life loss, Game Over, and restart; also keeps 100 projectiles active during an update. |

The test programs use a small local `CHECK` macro. Failed conditions are printed to standard error and cause a nonzero process exit.

## Coverage limits

- The renderer tests call viewport calculation and text formatting only; they do not compile shaders, create an OpenGL context, or compare rendered pixels.
- The integration test covers the game model, not end-to-end keyboard/window interaction.
- Allocation-failure behavior, entity-array capacity overflow, invalid or non-finite frame deltas, and many malformed externally constructed `Game` values are not currently covered.
- The configuration test checks the generated macro for the build configuration used; it does not independently test CMake's rejection of invalid cache values.
- The reported 60 FPS and long-running stability observations in [`docs/performance-validation.md`](docs/performance-validation.md) are manual validation records, not automated CTest assertions.

## Recommended additional tests

These are recommendations, not current test guarantees:

1. Add explicit `game_update` tests for null arguments, negative/non-finite delta times, zero-delta firing, and the 0.25-second clamp.
2. Exercise entity growth across multiple capacity doublings and inject allocation failures to verify error propagation and cleanup.
3. Add collision edge cases for exact rectangle boundaries, multiple enemies at the lower threshold, terminal states, and a projectile overlapping more than one enemy.
4. Add a graphics smoke test that creates a context where a display server is available, then verifies shader initialization, resize handling, and clean renderer teardown. Keep it separate from headless tests.
5. Add a repeatable interactive test or manual checklist for GLFW key mapping, window closure, minimum size, and visible output at multiple aspect ratios.
6. Configure separate test builds with the default campaign, a one-wave campaign, and a custom multi-wave campaign to verify configuration boundaries.
