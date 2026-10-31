# Tasks

## 1. Project entry documentation

- [x] 1.1 Rewrite `README.md` in English with the current project purpose, features, prerequisites, Linux and Windows dependency setup, CMake build/run commands, campaign configuration, controls, and a test entry point; verify each command and dependency against `CMakeLists.txt`, `requirements-build.txt`, and `.github/workflows/ci.yml`.

## 2. Architecture and API references

- [x] 2.1 Create `ARCHITECTURE.md` describing the current modules, dependencies, startup/main-loop/shutdown flow, game-state and entity algorithms, resource ownership, error handling, and evidence-based security considerations; include Mermaid module/dependency and execution-flow diagrams, and verify each relationship against the source.
- [x] 2.2 Create `API_REFERENCE.md` covering every public type, enum value, constant, struct field, and function declared in `include/game.h` and `include/renderer.h`; verify completeness against both headers and document parameter expectations, return behavior, failure cases, side effects, and lifecycle requirements.

## 3. Testing and documentation verification

- [x] 3.1 Create `TESTING.md` documenting the five CTest targets, test scenarios and edge cases, exact test commands, current coverage limits, and clearly labeled future test recommendations; verify test names and claims against `CMakeLists.txt` and each test source.
- [x] 3.2 Review `README.md`, `ARCHITECTURE.md`, `API_REFERENCE.md`, and `TESTING.md` together for English-only content, accurate source-derived claims, consistent terminology, working relative links, and valid Mermaid diagrams; run `openspec validate --change document-space-invaders-project` and confirm no project code or behavior changed.
