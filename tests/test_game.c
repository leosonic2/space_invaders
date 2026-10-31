#include "game.h"

#include <math.h>
#include <stdio.h>

static int failures;

#define CHECK(condition)                                                        \
    do {                                                                        \
        if (!(condition)) {                                                     \
            fprintf(stderr, "%s:%d: check failed: %s\n", __FILE__, __LINE__,    \
                    #condition);                                                \
            failures++;                                                         \
        }                                                                       \
    } while (0)

static void test_player_movement_and_bounds(void)
{
    Game game;
    GameInput input = {0};

    CHECK(game_init(&game, 5));
    CHECK(game_start(&game));
    float starting_x = game.player.x;
    input.move_left = true;
    game_update(&game, &input, 0.1f);
    CHECK(game.player.x < starting_x);

    float left_x = game.player.x;
    input.move_left = false;
    input.move_right = true;
    game_update(&game, &input, 0.1f);
    CHECK(game.player.x > left_x);

    game.player.x = 0.0f;
    input.move_left = true;
    input.move_right = false;
    game_update(&game, &input, 0.25f);
    CHECK(game.player.x == 0.0f);

    game.player.x = GAME_WORLD_WIDTH - game.player.width;
    input.move_left = false;
    input.move_right = true;
    game_update(&game, &input, 0.25f);
    CHECK(game.player.x == GAME_WORLD_WIDTH - game.player.width);
    game_destroy(&game);
}

static void test_movement_is_frame_rate_independent(void)
{
    Game sixty_fps;
    Game thirty_fps;
    GameInput input = {.move_right = true};

    CHECK(game_init(&sixty_fps, 5));
    CHECK(game_init(&thirty_fps, 5));
    CHECK(game_start(&sixty_fps));
    CHECK(game_start(&thirty_fps));

    for (int frame = 0; frame < 60; frame++) {
        game_update(&sixty_fps, &input, 1.0f / 60.0f);
    }
    for (int frame = 0; frame < 30; frame++) {
        game_update(&thirty_fps, &input, 1.0f / 30.0f);
    }

    CHECK(fabsf(sixty_fps.player.x - thirty_fps.player.x) < 0.01f);
    game_destroy(&sixty_fps);
    game_destroy(&thirty_fps);
}

static void test_enemy_and_projectile_lifecycle(void)
{
    Game game;
    GameInput input = {0};
    Enemy enemy = {
        .x = 1000.0f,
        .y = 120.0f,
        .width = 20.0f,
        .height = 18.0f,
        .speed = 200.0f,
        .points = 10
    };

    CHECK(game_init(&game, 5));
    CHECK(game_start(&game));
    game.enemy_count = 0;
    CHECK(game_add_enemy(&game, enemy));
    float initial_enemy_y = game.enemies[0].y;
    game_update(&game, &input, 0.1f);
    CHECK(game.enemies[0].x == GAME_WORLD_WIDTH - game.enemies[0].width);
    CHECK(game.enemies[0].y > initial_enemy_y);
    CHECK(game.enemy_direction == -1.0f);

    CHECK(game_fire(&game));
    CHECK(game.projectile_count == 1);
    float initial_projectile_y = game.projectiles[0].y;
    game_update(&game, &input, 0.1f);
    CHECK(game.projectiles[0].y < initial_projectile_y);

    game.projectiles[0].y = 0.0f;
    game_update(&game, &input, 0.1f);
    CHECK(game.projectile_count == 0);
    game_destroy(&game);
}

static void test_enemies_approach_player(void)
{
    Game game;
    GameInput input = {0};
    Enemy enemy = {
        .x = 100.0f,
        .y = 100.0f,
        .width = 20.0f,
        .height = 18.0f,
        .speed = 80.0f,
        .points = 10
    };

    CHECK(game_init(&game, 5));
    CHECK(game_start(&game));
    game.enemy_count = 0;
    CHECK(game_add_enemy(&game, enemy));
    game_update(&game, &input, 0.1f);
    CHECK(game.enemies[0].y > enemy.y);
    game_destroy(&game);
}

static void test_projectile_collision_and_score(void)
{
    Game game;
    GameInput input = {0};
    Enemy enemy = {
        .x = 0.0f,
        .y = 650.0f,
        .width = 20.0f,
        .height = 20.0f,
        .speed = 0.0f,
        .points = 10
    };

    CHECK(game_init(&game, 1));
    CHECK(game_start(&game));
    game.enemy_count = 0;
    enemy.x = game.player.x + (game.player.width - enemy.width) * 0.5f;
    CHECK(game_add_enemy(&game, enemy));
    CHECK(game_fire(&game));
    CHECK(game_update(&game, &input, 0.1f));
    CHECK(game.enemy_count == 0);
    CHECK(game.projectile_count == 0);
    CHECK(game.score == 10);
    game_destroy(&game);
}

static void test_missed_projectile_remains_active(void)
{
    Game game;
    GameInput input = {0};
    Enemy enemy = {
        .x = 40.0f,
        .y = 100.0f,
        .width = 20.0f,
        .height = 20.0f,
        .speed = 0.0f,
        .points = 10
    };

    CHECK(game_init(&game, 1));
    CHECK(game_start(&game));
    game.enemy_count = 0;
    CHECK(game_add_enemy(&game, enemy));
    CHECK(game_fire(&game));
    float projectile_y = game.projectiles[0].y;
    CHECK(game_update(&game, &input, 0.1f));
    CHECK(game.enemy_count == 1);
    CHECK(game.projectile_count == 1);
    CHECK(game.projectiles[0].y < projectile_y);
    CHECK(game.score == 0);
    game_destroy(&game);
}

static void test_invader_contact_costs_one_life_once(void)
{
    Game game;
    GameInput input = {0};
    Enemy enemy = {
        .x = 0.0f,
        .y = GAME_WORLD_HEIGHT - 40.0f,
        .width = 20.0f,
        .height = 20.0f,
        .speed = 0.0f,
        .points = 10
    };

    CHECK(game_init(&game, 1));
    CHECK(game_start(&game));
    game.enemy_count = 0;
    enemy.x = game.player.x;
    enemy.y = game.player.y;
    CHECK(game_add_enemy(&game, enemy));
    CHECK(game_update(&game, &input, 0.01f));
    CHECK(game.lives == GAME_STARTING_LIVES - 1);
    CHECK(game.enemy_count == 0);
    CHECK(game_update(&game, &input, 0.01f));
    CHECK(game.lives == GAME_STARTING_LIVES - 1);
    game_destroy(&game);
}

static void test_wave_difficulty_and_victory(void)
{
    Game game;
    GameInput input = {0};
    CHECK(game_init(&game, 5));
    CHECK(game_start(&game));
    float wave_one_speed = game.wave_enemy_speed;
    size_t wave_one_count = game.enemy_count;

    game.enemy_count = 0;
    CHECK(game_update(&game, &input, 0.01f));
    CHECK(game.wave == 2);
    CHECK(fabsf(game.wave_enemy_speed - wave_one_speed * 1.15f) < 0.001f);
    float wave_two_speed = game.wave_enemy_speed;

    game.enemy_count = 0;
    CHECK(game_update(&game, &input, 0.01f));
    CHECK(game.wave == 3);
    CHECK(fabsf(game.wave_enemy_speed - wave_two_speed * 1.15f) < 0.001f);

    size_t wave_three_count = game.enemy_count;
    float wave_three_speed = game.wave_enemy_speed;
    game.enemy_count = 0;
    CHECK(game_update(&game, &input, 0.01f));
    CHECK(game.wave == 4);
    CHECK(game.enemy_count > wave_three_count);
    CHECK(game.enemy_count > wave_one_count);
    CHECK(game.wave_enemy_speed > wave_three_speed);
    game_destroy(&game);

    CHECK(game_init(&game, 2));
    CHECK(game_start(&game));
    game.enemy_count = 0;
    CHECK(game_update(&game, &input, 0.01f));
    CHECK(game.state == GAME_STATE_PLAYING);
    CHECK(game.wave == 2);
    game.enemy_count = 0;
    CHECK(game_update(&game, &input, 0.01f));
    CHECK(game.state == GAME_STATE_VICTORY);
    game_destroy(&game);
}

static void test_menu_pause_game_over_and_restart(void)
{
    Game game;
    GameInput input = {0};
    Enemy enemy = {
        .x = 0.0f,
        .y = 0.0f,
        .width = 20.0f,
        .height = 20.0f,
        .speed = 0.0f,
        .points = 10
    };

    CHECK(game_init(&game, 5));
    CHECK(game.state == GAME_STATE_MENU);
    CHECK(game.lives == GAME_STARTING_LIVES);
    game_input_press(&input, GAME_KEY_CONFIRM);
    CHECK(game_update(&game, &input, 0.01f));
    CHECK(game.state == GAME_STATE_PLAYING);
    CHECK(game.wave == 1);
    CHECK(game.score == 0);

    input.confirm = false;
    game_input_press(&input, GAME_KEY_FIRE);
    CHECK(game_update(&game, &input, 0.01f));
    CHECK(game.projectile_count == 1);
    input.fire = false;
    game.projectile_count = 0;
    input.move_right = true;
    CHECK(game_update(&game, &input, 0.1f));
    float position_before_pause = game.player.x;
    game_input_press(&input, GAME_KEY_PAUSE);
    CHECK(game_update(&game, &input, 0.1f));
    CHECK(game.state == GAME_STATE_PAUSED);

    input.pause = false;
    CHECK(game_update(&game, &input, 0.1f));
    CHECK(game.player.x == position_before_pause);
    game_input_press(&input, GAME_KEY_CONFIRM);
    CHECK(game_update(&game, &input, 0.1f));
    CHECK(game.state == GAME_STATE_PLAYING);
    CHECK(game.player.x == position_before_pause);

    input.confirm = false;
    input.move_right = false;
    game.enemy_count = 0;
    game.lives = 1;
    enemy.x = 0.0f;
    enemy.y = GAME_WORLD_HEIGHT - enemy.height;
    CHECK(game_add_enemy(&game, enemy));
    CHECK(game_fire(&game));
    game.score = 123;
    CHECK(game_update(&game, &input, 0.01f));
    CHECK(game.state == GAME_STATE_GAME_OVER);
    CHECK(game.lives == 0);
    CHECK(game.score == 123);
    CHECK(game.projectile_count == 1);

    game_input_press(&input, GAME_KEY_CONFIRM);
    CHECK(game_update(&game, &input, 0.01f));
    CHECK(game.state == GAME_STATE_PLAYING);
    CHECK(game.wave == 1);
    CHECK(game.score == 0);
    CHECK(game.lives == GAME_STARTING_LIVES);
    CHECK(game.projectile_count == 0);
    CHECK(game.enemy_count == 32);
    game_destroy(&game);
}

int main(void)
{
    test_player_movement_and_bounds();
    test_movement_is_frame_rate_independent();
    test_enemy_and_projectile_lifecycle();
    test_enemies_approach_player();
    test_projectile_collision_and_score();
    test_missed_projectile_remains_active();
    test_invader_contact_costs_one_life_once();
    test_wave_difficulty_and_victory();
    test_menu_pause_game_over_and_restart();
    return failures == 0 ? 0 : 1;
}
