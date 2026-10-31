# Design

## Context

See `proposal.md` for the motivation and deliverables. The implementation is a C17 desktop game organized into a GLFW application loop (`src/main.c`), a graphics-independent game model (`src/game.c`), and an OpenGL renderer (`src/renderer.c`). Public declarations are in `include/game.h` and `include/renderer.h`; CMake builds the game and five CTest executables. The current README is in Portuguese, and there are no separate architecture, API, or testing references.

The documentation must describe observed implementation rather than treating `docs/specification.md` or the archived design as proof that a feature exists. In particular, rendering tests exercise viewport calculation and text formatting without opening a graphics context; the current automated suite does not verify rendered pixels or interactive window behavior.

## Goals / Non-Goals

**Goals:**
- Keep the four requested documents consistent with one another and the current source, build configuration, CI workflow, and tests.
- Explain the ownership boundary between application, simulation, and renderer, including allocations and cleanup.
- Make public API preconditions and failure behavior clear enough for callers to use the headers safely.
- Separate automated coverage, manual validation evidence, and recommended future tests.

**Non-Goals:**
- Change game behavior, source comments, public declarations, build configuration, dependencies, or CI.
- Claim that the documentation itself adds tests, completes a security audit, or proves platform/runtime behavior beyond the observed evidence.
- Create or modify a game-behavior OpenSpec capability; this is a documentation-only change.

## Decisions

1. **Treat current implementation as the source of truth.** Document behavior from `src/`, public headers, CMake, CI, and test code. Use existing prose specifications only as context, and call out any gap between specified intent and automated evidence rather than describing intended behavior as verified.

2. **Separate reader journeys across the requested files.** Keep `README.md` focused on project purpose, supported prerequisites, dependency installation, configuration/build/run, controls, and a concise test entry point. Put module responsibilities, dependency and execution diagrams, key algorithms, timing, allocation, and lifecycle details in `ARCHITECTURE.md`. Put declarations, types, constants, fields, parameters, return semantics, and failure/precondition notes from both public headers in `API_REFERENCE.md`. Put CTest commands, actual named test coverage, edge cases, validation limits, and labeled future recommendations in `TESTING.md`. This avoids duplicating the full API and test inventory in the README.

3. **Use Mermaid for relationships and flows, not for decorative repetition.** Include a module/dependency diagram and a startup/main-loop/shutdown flow in `ARCHITECTURE.md`; add a sequence diagram for an input-to-update/render operation if it makes the boundaries clearer. Ensure node names and edges match the actual call direction and do not imply that the renderer owns game state.

4. **Describe actual failure and resource semantics.** Explain that `game_init` and `game_update` report failure with `false`, entity arrays grow using `realloc`, and `game_destroy` frees both arrays. Describe startup error reporting and cleanup in `main`, shader file/compile/link errors in the renderer, and that renderer teardown requires a valid OpenGL context. Do not promise checks that are absent, such as a fixed projectile limit or comprehensive validation of every struct field.

5. **Record exact build requirements from the repository.** Use C17 and CMake 3.21 or newer, Ninja, a C compiler, Python 3 with the pinned Jinja2 build requirement, and the GLFW/OpenGL platform development dependencies shown by README and CI. Describe GLFW 3.4 and GLAD 2.0.8 as CMake FetchContent dependencies. Document the default configurable final wave and the actual configure/build/test/run commands for Windows and Linux.

6. **Keep security discussion proportional and evidence-based.** Describe the limited runtime input surface, file-based shader loading, allocation/error handling, and boundaries of the current test suite. Distinguish build-time dependency retrieval from runtime behavior. Record potential validation limitations as considerations, not as confirmed vulnerabilities.

## Risks / Trade-offs

- [The existing specification and implementation may differ in details] → Base statements on current source and explicitly identify intent-only claims or evidence gaps.
- [Detailed function-by-function API material can make references long] → Organize by public header and type/function, using concise parameter/return tables while covering every public declaration.
- [Build dependency instructions vary by platform] → Keep platform-specific installation snippets distinct and verify them against `.github/workflows/ci.yml` and the existing README.
- [Mermaid rendering may vary between Markdown viewers] → Use standard Mermaid flowchart and sequence syntax and keep adjacent text sufficient to understand each diagram.
- [Security discussion could be mistaken for an audit] → Label it as implementation considerations and state that no security audit or behavior change is in scope.

## Migration Plan

No runtime migration is required. Update `README.md` and add the three requested Markdown references. Review all documents against the source, headers, CMake, CI, and test names; verify commands and internal links where practical; run OpenSpec validation for the change. Rollback is limited to reverting these documentation-only files.
