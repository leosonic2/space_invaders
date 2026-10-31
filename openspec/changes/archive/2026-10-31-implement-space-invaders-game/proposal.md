# Proposal

## Why

The repository currently contains the Space Invaders specification but no game implementation. This change turns the specification into a playable, testable desktop game with the documented controls, wave progression, scoring, lives, and end states.

## What Changes

- Add a modular C game with OpenGL rendering, GLFW input/window management, GLAD loading, and a CMake build for Windows and Linux.
- Implement the menu and game lifecycle, player movement and shooting, enemy formations, collisions, score and lives, pause/resume, difficulty progression, victory, Game Over, and restart.
- Use delta-time updates and separate game logic from rendering so core behavior can be tested without initializing OpenGL.
- Add unit, integration, and functional verification for the requirements in `docs/specification.md`.
- Resolve unspecified gameplay defaults by making campaign length configurable with a default of five waves; lose a life when invaders reach the lower region or collide with the player.

## Capabilities

### New Capabilities
- `space-invaders-game`: Play and control the game, progress through waves, and receive score, life, pause, victory, and Game Over feedback.

### Modified Capabilities
- None.

## Impact

- Add the C source and public headers, CMake configuration, shaders, and tests described by `docs/specification.md`; these implementation files are not currently present in the repository.
- Add or configure the GLFW, GLAD, and OpenGL build dependencies, with supported Windows and Linux builds.
- No existing code or OpenSpec capabilities need migration; the current project contains documentation only and no existing capability specs.
