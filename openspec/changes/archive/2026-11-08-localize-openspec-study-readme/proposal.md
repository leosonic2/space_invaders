# Proposal

## Why

The current README explains how to build and play the game but does not state that the repository is a study project centered on learning OpenSpec. Clear educational framing and complete English, Brazilian Portuguese, and Spanish versions will make that purpose accessible without changing the game itself.

## What Changes

- Make `README.md` the default English version and explain that the game is a practical example for studying OpenSpec workflows.
- Add `README.pt-BR.md` and `README.es.md` with equivalent, localized project and setup information.
- Add links between all three language versions and keep commands, paths, environment variables, and technical identifiers consistent across them.

## Capabilities

### New Capabilities

None.

### Modified Capabilities

None. This is a documentation-only change with no spec-level behavior changes; `.openspec.yaml` declares `skip_specs: true`.

## Impact

Affected files are `README.md` and the new `README.pt-BR.md` and `README.es.md`. No source code, APIs, dependencies, or game behavior change.