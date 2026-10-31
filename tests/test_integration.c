#include "game.h"

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

static void test_complete_session(void)
{
    Game game;
    GameInput input = {0};

    CHECK(game_init(&game, 2));
    input.confirm = true;
    CHECK(game_update(&game, &input, 0.01f));
    CHECK(game.state == GAME_STATE_PLAYING);
    CHECK(game.wave == 1);
    CHECK(game.score == 0);
    CHECK(game.lives == GAME_STARTING_LIVES);

    input.confirm = false;
    input.move_right = true;
    float starting_x = game.player.x;
    CHECK(game_update(&game, &input, 0.1f));
    CHECK(game.player.x > starting_x);

    input.move_right = false;
    input.fire = true;
    CHECK(game_update(&game, &input, 0.0f));
    CHECK(game.projectile_count == 1);
    input.fire = false;

    game.enemy_count = 0;
    Enemy target = {
        .x = game.player.x + 16.0f,
        .y = game.player.y - 24.0f,
        .width = 20.0f,
        .height = 20.0f,
        .speed = 0.0f,
        .points = 10
    };
    CHECK(game_add_enemy(&game, target));
    CHECK(game_update(&game, &input, 0.01f));
    CHECK(game.score == 10);
    CHECK(game.wave == 2);
    CHECK(game.state == GAME_STATE_PLAYING);
    CHECK(game.projectile_count == 0);

    input.pause = true;
    float position_before_pause = game.player.x;
    float enemy_x_before_pause = game.enemies[0].x;
    CHECK(game_update(&game, &input, 0.1f));
    CHECK(game.state == GAME_STATE_PAUSED);
    input.pause = false;
    input.move_left = true;
    input.fire = true;
    CHECK(game_update(&game, &input, 0.1f));
    CHECK(game.player.x == position_before_pause);
    CHECK(game.enemies[0].x == enemy_x_before_pause);
    CHECK(game.projectile_count == 0);

    input.confirm = true;
    CHECK(game_update(&game, &input, 0.1f));
    CHECK(game.state == GAME_STATE_PLAYING);
    CHECK(game.player.x == position_before_pause);
    input.confirm = false;
    input.move_left = false;
    input.fire = false;

    game.lives = 1;
    game.enemy_count = 0;
    Enemy attacker = {
        .x = game.player.x,
        .y = game.player.y,
        .width = 20.0f,
        .height = 20.0f,
        .speed = 0.0f,
        .points = 10
    };
    CHECK(game_add_enemy(&game, attacker));
    CHECK(game_update(&game, &input, 0.01f));
    CHECK(game.lives == 0);
    CHECK(game.state == GAME_STATE_GAME_OVER);
    CHECK(game.score == 10);

    input.confirm = true;
    CHECK(game_update(&game, &input, 0.01f));
    CHECK(game.state == GAME_STATE_PLAYING);
    CHECK(game.wave == 1);
    CHECK(game.score == 0);
    CHECK(game.lives == GAME_STARTING_LIVES);
    CHECK(game.projectile_count == 0);
    CHECK(game.enemy_count == 32);
    game_destroy(&game);
}

static void test_one_hundred_active_projectiles(void)
{
    Game game;

    CHECK(game_init(&game, 5));
    CHECK(game_start(&game));
    for (int i = 0; i < 100; i++) {
        CHECK(game_fire(&game));
    }
    CHECK(game.projectile_count == 100);
    CHECK(game_update(&game, &(GameInput){0}, 1.0f / 60.0f));
    CHECK(game.projectile_count == 100);
    for (size_t i = 0; i < game.projectile_count; i++) {
        CHECK(game.projectiles[i].active);
    }
    game_destroy(&game);
}

int main(void)
{
    test_complete_session();
    test_one_hundred_active_projectiles();
    return failures == 0 ? 0 : 1;
}
