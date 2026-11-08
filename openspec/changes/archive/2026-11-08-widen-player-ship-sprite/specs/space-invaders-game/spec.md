# Spec Delta

## ADDED Requirements

### Requirement: Player fighter ship appearance
The system MUST render the player ship as a symmetric top-down combat aircraft with wider wings, following the visual reference in `img/nave.jpg`.

#### Scenario: Display the widened fighter ship
- **WHEN** the player ship is rendered during gameplay
- **THEN** it has a central nose and visibly broad, symmetric wings matching the reference's combat-aircraft silhouette

## MODIFIED Requirements

### Requirement: Invader advance and life loss
The system MUST move invaders toward the player's region and deduct exactly one life when an invader overlaps the player's expanded ship collision bounds or, without colliding with the ship, reaches the bottom edge of the screen.

#### Scenario: Advance toward the player
- **WHEN** a wave remains active over time
- **THEN** the invaders gradually move down toward the player's region

#### Scenario: Invaders reach the lower region
- **WHEN** an invader that has not collided with the player reaches the bottom edge of the screen
- **THEN** the player loses one life for that breach event

#### Scenario: Invader collides with the ship
- **WHEN** an invader overlaps the player's ship collision bounds, including its widened wings
- **THEN** the player loses one life for that collision event