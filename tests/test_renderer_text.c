#include "renderer.h"

#include <stdio.h>
#include <string.h>

static int failures;

#define CHECK(condition)                                                        \
    do {                                                                        \
        if (!(condition)) {                                                     \
            fprintf(stderr, "%s:%d: check failed: %s\n", __FILE__, __LINE__,    \
                    #condition);                                                \
            failures++;                                                         \
        }                                                                       \
    } while (0)

static void test_menu_text(void)
{
    Game game = {.state = GAME_STATE_MENU};
    RendererText text;

    CHECK(renderer_format_text(&game, &text));
    CHECK(text.show_title);
    CHECK(text.show_prompt);
    CHECK(!text.show_hud);
    CHECK(strcmp(text.title, "SPACE INVADERS") == 0);
    CHECK(strcmp(text.prompt, "PRESS ENTER TO START") == 0);
}

static void test_gameplay_and_pause_hud(void)
{
    Game game = {
        .state = GAME_STATE_PLAYING,
        .score = 120,
        .lives = 2,
        .wave = 3
    };
    RendererText playing;
    RendererText paused;

    CHECK(renderer_format_text(&game, &playing));
    CHECK(playing.show_hud);
    CHECK(!playing.show_message);
    CHECK(strcmp(playing.score, "SCORE: 120") == 0);
    CHECK(strcmp(playing.lives, "LIVES: 2") == 0);
    CHECK(strcmp(playing.wave, "WAVE: 3") == 0);

    game.state = GAME_STATE_PAUSED;
    CHECK(renderer_format_text(&game, &paused));
    CHECK(paused.show_hud);
    CHECK(paused.show_message);
    CHECK(strcmp(paused.message, "PAUSED") == 0);
    CHECK(strcmp(paused.score, playing.score) == 0);
    CHECK(strcmp(paused.lives, playing.lives) == 0);
    CHECK(strcmp(paused.wave, playing.wave) == 0);
}

static void test_terminal_state_text(void)
{
    Game game = {
        .state = GAME_STATE_GAME_OVER,
        .score = 250,
        .lives = 0,
        .wave = 4
    };
    RendererText text;

    CHECK(renderer_format_text(&game, &text));
    CHECK(text.show_hud);
    CHECK(text.show_message);
    CHECK(text.show_final_score);
    CHECK(text.show_restart_prompt);
    CHECK(strcmp(text.message, "GAME OVER") == 0);
    CHECK(strcmp(text.final_score, "FINAL SCORE: 250") == 0);
    CHECK(strcmp(text.restart_prompt, "PRESS ENTER TO RESTART") == 0);

    game.state = GAME_STATE_VICTORY;
    CHECK(renderer_format_text(&game, &text));
    CHECK(strcmp(text.message, "VICTORY") == 0);
    CHECK(strcmp(text.final_score, "FINAL SCORE: 250") == 0);
    CHECK(text.show_restart_prompt);
}

static void test_invalid_arguments(void)
{
    Game game = {.state = (GameState)99};
    RendererText text;

    CHECK(!renderer_format_text(NULL, &text));
    CHECK(!renderer_format_text(&game, NULL));
    CHECK(!renderer_format_text(&game, &text));
}

int main(void)
{
    test_menu_text();
    test_gameplay_and_pause_hud();
    test_terminal_state_text();
    test_invalid_arguments();
    return failures == 0 ? 0 : 1;
}
