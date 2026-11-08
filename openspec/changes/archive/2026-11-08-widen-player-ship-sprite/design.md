# Design

## Context

The player sprite is currently a 16-by-20 character matrix rendered at four world units per pixel, for a 64-by-80 footprint. `Player.width` also determines horizontal clamping, initial centering, projectile centering, and the rectangular collision bounds. Existing game tests assert the current dimensions and edge behavior.

## Goals / Non-Goals

**Goals:**
- Make the pixel-art silhouette visibly wider at the wings while retaining a centered nose and bilateral symmetry.
- Keep rendered width, movement bounds, projectile origin, and collision width consistent.

**Non-Goals:**
- Change ship height, movement speed, firing cadence, colors, or enemy behavior.
- Introduce polygonal or per-pixel collision detection.

## Decisions

- Expand the sprite matrix horizontally at the wing rows, using `img/nave.jpg` as the visual guide. Keep the 20-row height, four-world-unit pixel scale, and existing color palette; derive the new world-space player width from the matrix width.
- Use that same expanded width for the player's rectangular hitbox. The existing game logic already uses `Player.width` for screen bounds, spawn centering, projectile centering, and collision checks, so updating the player width keeps these behaviors aligned without adding a separate width value.
- Treat ship contact and a lower-edge breach as separate loss conditions: ship overlap costs a life immediately, while an invader that passes the ship without contact costs a life when its bottom reaches the screen's lower edge. This replaces the current threshold at the player's top edge, which masks ship-contact behavior for any enemy at that height.
- Update game tests to assert the new width, preserve the 80-unit height and bottom margin, verify both horizontal bounds, and cover collision within the newly added wing span.
- Keep the pixel matrix and gameplay dimensions as their current owners: sprite geometry in `src/renderer.c`, player dimensions in `src/game.c`.

Alternatives considered: keeping the old gameplay width would require visual overhang and separate movement/collision dimensions; non-uniformly scaling the existing sprite would distort its pixels. Neither matches the intended wider-wing silhouette as cleanly as expanding the pixel matrix and using one shared width.

## Risks / Trade-offs

- A wider rectangular hitbox includes empty corners above and below the wings, so the player can be hit in a slightly larger region than the visible pixels. This follows the current rectangle-based collision model; a shape-specific hitbox is out of scope.
- The larger width reduces the player's horizontal travel range and makes wing-area collisions more likely. This is intentional and should be checked in a playable build after implementation.
- Lower-edge breaches happen later than the current player-height threshold, giving off-axis invaders more time to descend before costing a life. Verify both a missed ship pass and the eventual edge breach in tests.