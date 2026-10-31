# Design

## Context

See `proposal.md` for motivation and `specs/space-invaders-game/spec.md` for the behavior contract. The repository currently has no game source, build configuration, assets, or tests, so the design establishes a small modular baseline rather than extending existing code.

## Goals / Non-Goals

**Goals:**
- Keep game rules and state updates testable without creating a window or OpenGL context.
- Deliver a reproducible CMake build for Windows and Linux with explicit dependency and resource lifecycles.
- Support the required HUD and game-state messages with the specified graphics stack.

**Non-Goals:**
- Add high scores, save files, audio, alternate weapons, bosses, or controller support.
- Add a settings UI or runtime campaign editor; campaign length is a build-time setting.
- Introduce a general-purpose entity engine or graphics framework.

## Decisions

1. **Separate simulation from platform and rendering code.** Use a game/session module that owns state, score, lives, wave configuration, player, enemies, and projectiles. Input is translated into a small game command structure; update functions consume commands and elapsed time. GLFW/window setup and event polling stay in the application layer, while rendering receives read-only game state. This keeps collision, movement, scoring, and wave progression unit-testable without OpenGL. A single main-loop module that mixes input, rules, and drawing was considered but rejected because it couples tests and gameplay to graphics initialization.

2. **Use small C modules and growable entity arrays.** Organize public interfaces under `include/` and implementations under `src/` for game/state, player, enemies, projectiles, collision, renderer, and the application loop. Store enemies and active projectiles in dynamically grown arrays, remove expired or hit entries during simulation, and propagate allocation failures instead of silently dropping entities. This avoids an arbitrary gameplay cap while supporting the documented 100-projectile performance check. Fixed-capacity arrays were considered but would impose an undocumented firing limit.

3. **Use a fixed 1024 x 768 logical playfield with a resizable, bounded window.** Keep game coordinates independent of window pixels and scale the renderer viewport to the logical 4:3 playfield, preserving aspect ratio when the window is resized. Set 800 x 600 as the minimum window size. This matches the reference layout and prevents resizing from changing movement or collision rules; recalculating game physics from physical window dimensions was rejected.

4. **Use OpenGL 3.3 Core with GLFW and GLAD through CMake.** Build as C17, matching the existing VS Code GCC configuration, link the platform OpenGL implementation and GLFW, and use GLAD to load OpenGL functions. Use shaders and VAO/VBO geometry for simple 2D shapes; draw the starfield and game objects as primitives. Render menu, HUD, and state labels with a small bundled bitmap glyph set rather than adding a separate font-rendering dependency. GLFW and GLAD are preferred to platform-specific window/input APIs, as required by the specification.

5. **Keep campaign length configurable at build time.** Define a CMake cache setting for the final wave, defaulting to five, and pass the resolved value into game configuration. Increase wave-two and wave-three enemy speeds by 15 percent relative to the preceding wave; from wave four onward, increase speed, enemy count, or both. This provides a concrete victory condition without adding a settings screen. A permanently endless mode was rejected because it cannot reach the specified VICTORY state.

6. **Use CTest with headless logic tests.** Compile game-logic tests without GLFW or an OpenGL context; cover movement bounds, projectile lifecycle, collisions, scoring, lives, pause/resume, wave difficulty, and terminal states. Keep graphics smoke checks separate so environments without a display can still run the core suite. A third-party test framework was not selected because the repository has no existing framework and a small C test harness is sufficient.

7. **Make initialization and shutdown explicit.** Initialize GLFW, create the window and context, load GLAD, then load shaders and game resources; on each failure, report the failing stage, release already-created resources, and return a non-zero process status. On normal closure, destroy game allocations and renderer resources before destroying the window and terminating GLFW. This avoids success-shaped startup failures and supports the documented lifecycle requirement.

## Risks / Trade-offs

- [Dependency availability differs across developer machines] → Pin/document dependency versions and provide CMake configuration that clearly reports missing GLFW, OpenGL, or GLAD components.
- [A bitmap glyph set limits typography and localization] → Keep glyph drawing isolated behind the renderer text interface so a font library can replace it later without changing game logic.
- [Frame stalls can produce large movement steps] → Use elapsed time for simulation, clamp unusually large frame deltas after stalls, and verify movement at multiple update rates.
- [Difficulty increases can make later waves impractical] → Keep wave parameters in one configuration path and test progression through the configured final wave.
- [No source or build setup exists to confirm the proposed module layout] → Establish the build and headless logic tests before adding rendering integration.

## Migration Plan

No existing executable or source code requires migration. Add the build configuration, dependency setup, modules, and tests; validate headless tests and supported platform builds; then validate rendering on a machine with a graphical environment. Rollback consists of removing this newly introduced game implementation and its build configuration.
