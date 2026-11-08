# Design

## Context

The repository currently has one English `README.md` covering the C17 game, build prerequisites, platform setup, run commands, controls, tests, and related documentation. See proposal.md for the motivation and requested language set.

## Goals / Non-Goals

**Goals:**
- Keep English as the default GitHub landing page while making Brazilian Portuguese and Spanish versions easy to discover.
- Keep the learning purpose and practical OpenSpec focus prominent in all languages.
- Preserve equivalent section coverage and working technical instructions across versions.

**Non-Goals:**
- Translate source code, in-game text, or documentation beyond the three README files.
- Change game behavior, build configuration, dependencies, or OpenSpec specs.

## Decisions

- Keep `README.md` as the English source and add sibling files `README.pt-BR.md` and `README.es.md`. Place links to all versions near the top of each file; this preserves GitHub's default landing page and makes each translation directly addressable.
- Use the same section order and coverage in all versions: project purpose and features, requirements, platform setup, configure/build, campaign configuration, run, controls, tests, frame-rate logging, and related docs. This makes the translations comparable and easier to keep in sync.
- Lead with the educational purpose: OpenSpec is the main subject of study, while the game is the practical example. Translate explanatory prose, but keep commands, paths, filenames, environment variables, keys, and API/tool identifiers unchanged so they remain executable and searchable.
- Use Brazilian Portuguese for `README.pt-BR.md` and broadly understandable Spanish for `README.es.md`.

Alternative considered: putting all three translations in one README would keep the file count lower, but make the default landing page long and force readers to scroll past unrelated languages. Separate files with reciprocal links are clearer for readers and maintainers.

## Risks / Trade-offs

- **Translations can drift as setup steps change.** Mitigation: retain matching section order and verify command/code blocks and referenced paths across all versions during documentation updates.
- **Localized names for technical concepts can obscure OpenSpec commands.** Mitigation: keep CLI names, artifact identifiers, and literal commands unchanged; explain them in localized prose where useful.