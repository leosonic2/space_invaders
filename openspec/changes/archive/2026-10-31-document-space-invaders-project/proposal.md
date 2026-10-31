# Proposal

## Why

The project has a playable C17 implementation and a build-and-controls README, but it lacks a cohesive English reference for its architecture, public C API, and test coverage. Documentation grounded in the current implementation will make the project easier to build, maintain, review, and extend without implying behavior that the code does not provide.

## What Changes

- Replace the Portuguese project README with an English overview, prerequisites, dependency installation, build, run, configuration, controls, and test instructions.
- Add `ARCHITECTURE.md` describing the current modules, dependencies, execution flow, algorithms, resource ownership, and Mermaid diagrams.
- Add `API_REFERENCE.md` documenting the public declarations in `include/game.h` and `include/renderer.h`, including their types, constants, parameters, return behavior, and relevant lifecycle constraints.
- Add `TESTING.md` describing the existing CTest suite, its scenarios and edge cases, execution commands, and clearly labeled recommendations for future coverage.
- Document error handling and security-relevant implementation considerations based on source inspection; do not present this as a separate security audit or change runtime behavior.
- Keep all four documents in English and distinguish verified implementation details from assumptions or open questions.

## Capabilities

### New Capabilities

None.

### Modified Capabilities

None. This documentation-only change does not alter game behavior or any existing requirements; `skip_specs: true` is set in this change's `.openspec.yaml`.

## Impact

- Update `README.md`; add `ARCHITECTURE.md`, `API_REFERENCE.md`, and `TESTING.md`.
- Documentation will cover `src/main.c`, `src/game.c`, `src/renderer.c`, `include/game.h`, `include/renderer.h`, the generated game configuration, shaders, CMake targets, CI setup, and the five existing CTest executables.
- No C source, public API, build behavior, dependencies, or runtime behavior is intended to change.
