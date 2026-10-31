#ifndef SPACE_INVADERS_GAME_H
#define SPACE_INVADERS_GAME_H

#include <stdbool.h>
#include <stddef.h>

#define GAME_WORLD_WIDTH 1024.0f
#define GAME_WORLD_HEIGHT 768.0f
#define GAME_STARTING_LIVES 3

typedef enum {
    GAME_STATE_MENU,
    GAME_STATE_PLAYING,
    GAME_STATE_PAUSED,
    GAME_STATE_GAME_OVER,
    GAME_STATE_VICTORY
} GameState;

typedef struct {
    float x;
    float y;
    float width;
    float height;
    float speed;
} Player;

typedef struct {
    float x;
    float y;
    float width;
    float height;
    float speed;
    int points;
    bool active;
} Enemy;

typedef struct {
    float x;
    float y;
    float width;
    float height;
    float speed;
    bool active;
} Projectile;

typedef struct {
    bool move_left;
    bool move_right;
    bool fire;
    bool pause;
    bool confirm;
} GameInput;

typedef enum {
    GAME_KEY_FIRE,
    GAME_KEY_PAUSE,
    GAME_KEY_CONFIRM
} GameKey;

typedef struct {
    GameState state;
    Player player;
    Enemy *enemies;
    size_t enemy_count;
    size_t enemy_capacity;
    Projectile *projectiles;
    size_t projectile_count;
    size_t projectile_capacity;
    int score;
    int lives;
    int wave;
    int final_wave;
    int wave_enemy_columns;
    float wave_enemy_speed;
    float enemy_direction;
    float enemy_drop_distance;
} Game;

bool game_init(Game *game, int final_wave);
void game_destroy(Game *game);
bool game_start(Game *game);
bool game_add_enemy(Game *game, Enemy enemy);
bool game_fire(Game *game);
void game_input_press(GameInput *input, GameKey key);
bool game_update(Game *game, const GameInput *input, float delta_time);

#endif
