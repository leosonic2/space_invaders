# Proposal

## Why

The project currently documents its Space Invaders requirements in Portuguese, while an English-language specification has been requested for `docs/`. Providing an English version makes the existing requirements accessible to English-speaking contributors without replacing the Portuguese reference.

## What Changes

- Add `docs/specification.md` as a faithful English translation of `docs/Especificacao_Space_Invaders.md`.
- Preserve the source's scope, section structure, requirement and test identifiers, gameplay rules, numerical values, code examples, and stated assumptions.
- Format the translated content as readable Markdown without introducing new requirements or changing the game's behavior.
- Leave the Portuguese source document unchanged.

## Capabilities

### New Capabilities

None. This change adds documentation only and does not introduce system behavior.

### Modified Capabilities

None.

## Impact

- Adds `docs/specification.md`.
- Uses `docs/Especificacao_Space_Invaders.md` as the translation source; that file remains unchanged.
- No source code, APIs, build configuration, dependencies, or runtime behavior are affected.
