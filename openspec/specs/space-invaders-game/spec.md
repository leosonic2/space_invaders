# space-invaders-game Specification

## Purpose

Defines the player-facing behavior and runtime expectations of the Space Invaders desktop game, from its initial menu through gameplay and terminal states.

## Requirements

### Requirement: Game menu and session start
The system MUST present the game title before play and start a fresh session when the player presses ENTER from the menu.

#### Scenario: Display initial menu
- **WHEN** the application starts successfully
- **THEN** the system displays the game title and an option to start

#### Scenario: Start a fresh session
- **WHEN** the player presses ENTER on the initial menu
- **THEN** the system initializes the player, sets the score to zero and lives to three, creates wave one, and enters the PLAYING state

### Requirement: Horizontal player movement
The system MUST move the player's ship horizontally while play is active and keep the ship within the visible screen bounds.

#### Scenario: Move left
- **WHEN** the player holds A or the left arrow key during play
- **THEN** the ship moves left

#### Scenario: Move right
- **WHEN** the player holds D or the right arrow key during play
- **THEN** the ship moves right

#### Scenario: Enforce horizontal bounds
- **WHEN** movement would place any part of the ship beyond a horizontal screen edge
- **THEN** the ship remains within the screen

### Requirement: Player projectiles
The system MUST let the player fire upward-moving projectiles during play and remove projectiles that leave the top of the screen.

#### Scenario: Fire a projectile
- **WHEN** the player presses SPACE during play
- **THEN** the system creates a projectile at the player's ship position and moves it toward the enemies

#### Scenario: Remove an off-screen projectile
- **WHEN** a projectile passes the top screen boundary
- **THEN** the system removes that projectile from active play

### Requirement: Enemy formation movement
The system MUST create enemies in a formation and move the formation horizontally, reversing direction and descending when it reaches either horizontal edge.

#### Scenario: Move formation horizontally
- **WHEN** a wave is active and the formation has not reached a screen edge
- **THEN** the formation continues moving horizontally in its current direction

#### Scenario: Reverse and descend at an edge
- **WHEN** any active part of the formation reaches a horizontal screen edge
- **THEN** the formation reverses horizontal direction and descends one position

### Requirement: Invader advance and life loss
The system MUST move invaders toward the player's region and deduct exactly one life for each distinct event in which an invader reaches the lower region or collides with the player's ship.

#### Scenario: Advance toward the player
- **WHEN** a wave remains active over time
- **THEN** the invaders gradually move down toward the player's region

#### Scenario: Invaders reach the lower region
- **WHEN** an invader reaches the lower region of the screen
- **THEN** the player loses one life for that breach event

#### Scenario: Invader collides with the ship
- **WHEN** an invader collides with the player's ship
- **THEN** the player loses one life for that collision event

### Requirement: Projectile collision
The system MUST detect projectile-to-enemy collisions and remove both the projectile and the hit enemy from active play.

#### Scenario: Projectile hits an enemy
- **WHEN** an active projectile overlaps an active enemy
- **THEN** the system removes that projectile and enemy from active play

#### Scenario: Projectile misses
- **WHEN** an active projectile does not overlap any enemy
- **THEN** the projectile continues moving toward the top of the screen

### Requirement: Enemy scoring
The system MUST increase the player's score when an enemy is eliminated, using the enemy's configured point value; a common enemy is worth 10 points.

#### Scenario: Score a common enemy
- **WHEN** the player eliminates a common enemy
- **THEN** the score increases by 10 points and the updated score is displayed

### Requirement: Game status display
The system MUST display the current score, remaining lives, and wave during play.

#### Scenario: View current status
- **WHEN** a wave is active
- **THEN** the display shows the current score, remaining lives, and wave number

### Requirement: Wave progression and difficulty
The system MUST start a new wave after all enemies in the current wave are eliminated, increasing difficulty as waves progress.

#### Scenario: Advance after clearing a wave
- **WHEN** no active enemies remain and the campaign has not reached its configured final wave
- **THEN** the system creates the next wave and displays its wave number

#### Scenario: Increase early-wave speed
- **WHEN** the system creates wave two or wave three
- **THEN** enemy speed is 15 percent greater than the preceding wave's speed

#### Scenario: Increase later-wave difficulty
- **WHEN** the system creates wave four or a later wave
- **THEN** the system increases enemy quantity, speed, or both compared with the preceding wave

### Requirement: Campaign victory
The system MUST enter VICTORY when the player clears the configured final wave; the default campaign length MUST be five waves.

#### Scenario: Complete the final wave
- **WHEN** the player eliminates all enemies in the configured final wave
- **THEN** the system enters VICTORY, stops gameplay updates, and displays the final score

### Requirement: Lives and Game Over
The system MUST enter GAME OVER when the player's lives reach zero.

#### Scenario: Lose the last life
- **WHEN** the player's lives reach zero
- **THEN** the system enters GAME OVER, stops gameplay updates, and displays the final score and restart option

### Requirement: Pause and resume
The system MUST let the player pause an active game with ESC and resume it with ENTER without changing the game state while paused.

#### Scenario: Pause active gameplay
- **WHEN** the player presses ESC during play
- **THEN** the system enters PAUSED, stops gameplay movement, and displays a PAUSED message

#### Scenario: Resume paused gameplay
- **WHEN** the player presses ENTER while paused
- **THEN** the system returns to PLAYING and continues from the preserved game state

### Requirement: Restart after Game Over
The system MUST allow the player to start a fresh session from GAME OVER by pressing ENTER.

#### Scenario: Restart a completed session
- **WHEN** the player presses ENTER on the GAME OVER screen
- **THEN** the system clears active projectiles, resets the score and lives, creates wave one, and enters PLAYING

### Requirement: Game rendering and display size
The system MUST render the game with OpenGL and support a window of at least 800 by 600 pixels.

#### Scenario: Render the game scene
- **WHEN** the application is running
- **THEN** the display includes the player ship, enemy formation, projectiles, space background, and the applicable menu or game status

#### Scenario: Use the minimum supported resolution
- **WHEN** the game window is displayed at 800 by 600 pixels or larger
- **THEN** gameplay elements and status information remain visible

### Requirement: Frame-independent gameplay timing
The system MUST update movement using elapsed frame time so gameplay speed does not depend on frame rate.

#### Scenario: Update movement at different frame rates
- **WHEN** the same duration of gameplay is simulated at different frame rates
- **THEN** player and enemy movement cover equivalent distances within normal timing precision

### Requirement: Rendering performance target
The system MUST target approximately 60 frames per second in the reference environment during a standard wave.

#### Scenario: Render a standard wave
- **WHEN** a standard wave is played at the 1024 by 768 reference resolution on the reference environment
- **THEN** rendering maintains approximately 60 frames per second

### Requirement: Supported platform lifecycle
The system MUST run on supported Windows and Linux environments and release its application and graphics resources when the player closes the window.

#### Scenario: Close the application
- **WHEN** the player requests window closure
- **THEN** the system exits and releases allocated game and graphics resources

#### Scenario: Graphics initialization fails
- **WHEN** required window or graphics initialization fails
- **THEN** the system reports the initialization failure and exits without continuing into gameplay
