# Proposal

## Why

The current 16-by-20 player sprite reads as a generic ship and has narrower wings than the combat-aircraft reference in `img/nave.jpg`. A wider, more recognizable fighter silhouette will make the player craft match the intended visual direction.

## What Changes

- Redraw the player sprite with wider, symmetric wings inspired by `img/nave.jpg`.
- Increase the player's world-space width and hitbox to match the expanded sprite, keeping the ship within screen bounds and firing from its center.
- Update the relevant game tests for the new player dimensions and bounds.

## Capabilities

### New Capabilities

None.

### Modified Capabilities

- `space-invaders-game`: Specify the player ship's combat-aircraft silhouette and require its collision bounds to reflect the wider ship.

## Impact

Affected code includes `src/renderer.c` for the pixel-art sprite and `src/game.c` for player dimensions and behavior derived from them. Tests in `tests/test_game.c` will need to cover the updated size and screen-edge bounds. No new dependencies or public APIs are expected.