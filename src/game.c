#include "game.h"

#include <math.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define PLAYER_WIDTH 64.0f
#define PLAYER_HEIGHT 80.0f
#define PLAYER_SPEED 320.0f
#define PLAYER_Y (GAME_WORLD_HEIGHT - PLAYER_HEIGHT - 32.0f)
#define MAX_FRAME_DELTA 0.25f
#define ENEMY_DESCENT_SPEED 6.0f
#define ENEMY_WIDTH 36.0f
#define ENEMY_HEIGHT 24.0f
#define ENEMY_HORIZONTAL_GAP 16.0f
#define ENEMY_VERTICAL_GAP 18.0f
#define WAVE_ROWS 4
#define BASE_ENEMY_SPEED 60.0f
#define MAX_ENEMY_COLUMNS 18
#define PROJECTILE_WIDTH 6.0f
#define PROJECTILE_HEIGHT 16.0f
#define PROJECTILE_SPEED 520.0f
#define ENTITY_INITIAL_CAPACITY 32U

/**
 * EN: Initializes a game in the menu state with its player and match settings.
 * PT: Inicializa o jogo no menu com o jogador e as configurações da partida.
 */
bool game_init(Game *game, int final_wave)
{
    if (game == NULL || final_wave < 1) {
        return false;
    }

    memset(game, 0, sizeof(*game));
    game->state = GAME_STATE_MENU;
    game->final_wave = final_wave;
    game->player.width = PLAYER_WIDTH;
    game->player.height = PLAYER_HEIGHT;
    game->player.speed = PLAYER_SPEED;
    game->player.y = PLAYER_Y;
    game->player.x = (GAME_WORLD_WIDTH - PLAYER_WIDTH) * 0.5f;
    game->lives = GAME_STARTING_LIVES;
    game->enemy_direction = 1.0f;
    game->enemy_drop_distance = 24.0f;
    return true;
}

/**
 * EN: Releases entity storage owned by the game and clears its state.
 * PT: Libera o armazenamento de entidades do jogo e limpa seu estado.
 */
void game_destroy(Game *game)
{
    if (game == NULL) {
        return;
    }

    free(game->enemies);
    free(game->projectiles);
    memset(game, 0, sizeof(*game));
}

/**
 * EN: Calculates the enemy movement speed for the requested wave.
 * PT: Calcula a velocidade de movimento dos inimigos para a onda informada.
 */
static float speed_for_wave(int wave)
{
    float speed = BASE_ENEMY_SPEED;
    if (wave >= 2) {
        speed *= 1.15f;
    }
    if (wave >= 3) {
        speed *= 1.15f;
    }
    if (wave >= 4) {
        speed *= powf(1.10f, (float)(wave - 3));
    }
    return isfinite(speed) ? speed : 0.0f;
}

/**
 * EN: Creates the enemy formation for the current wave.
 * PT: Cria a formação de inimigos da onda atual.
 */
static bool create_wave(Game *game)
{
    int additional_columns = game->wave > 3 ? game->wave - 3 : 0;
    game->wave_enemy_columns = additional_columns >= MAX_ENEMY_COLUMNS - 8
                                   ? MAX_ENEMY_COLUMNS
                                   : 8 + additional_columns;
    game->wave_enemy_speed = speed_for_wave(game->wave);
    if (game->wave_enemy_speed <= 0.0f) {
        return false;
    }

    float formation_width =
        game->wave_enemy_columns * (ENEMY_WIDTH + ENEMY_HORIZONTAL_GAP) -
        ENEMY_HORIZONTAL_GAP;
    float start_x = (GAME_WORLD_WIDTH - formation_width) * 0.5f;
    for (int row = 0; row < WAVE_ROWS; row++) {
        for (int column = 0; column < game->wave_enemy_columns; column++) {
            Enemy enemy = {
                .x = start_x + column * (ENEMY_WIDTH + ENEMY_HORIZONTAL_GAP),
                .y = 88.0f + row * (ENEMY_HEIGHT + ENEMY_VERTICAL_GAP),
                .width = ENEMY_WIDTH,
                .height = ENEMY_HEIGHT,
                .speed = game->wave_enemy_speed,
                .points = 10,
                .active = true
            };
            if (!game_add_enemy(game, enemy)) {
                game->enemy_count = 0;
                return false;
            }
        }
    }
    game->enemy_direction = 1.0f;
    return true;
}

/**
 * EN: Resets match progress and starts the first wave.
 * PT: Reinicia o progresso da partida e inicia a primeira onda.
 */
bool game_start(Game *game)
{
    if (game == NULL || game->final_wave < 1) {
        return false;
    }

    game->score = 0;
    game->lives = GAME_STARTING_LIVES;
    game->wave = 1;
    game->player.x = (GAME_WORLD_WIDTH - game->player.width) * 0.5f;
    game->enemy_direction = 1.0f;
    game->enemy_count = 0;
    game->projectile_count = 0;
    game->state = GAME_STATE_PLAYING;
    if (!create_wave(game)) {
        game->state = GAME_STATE_MENU;
        return false;
    }
    return true;
}

/**
 * EN: Grows a dynamic entity array when it has no free slot.
 * PT: Aumenta um vetor dinâmico de entidades quando não há espaço disponível.
 */
static bool reserve_entities(void **entities, size_t *capacity, size_t count,
                             size_t element_size)
{
    if (count < *capacity) {
        return true;
    }

    size_t new_capacity = *capacity == 0 ? ENTITY_INITIAL_CAPACITY : *capacity * 2;
    if (new_capacity < *capacity || new_capacity > SIZE_MAX / element_size) {
        return false;
    }

    void *new_entities = realloc(*entities, new_capacity * element_size);
    if (new_entities == NULL) {
        return false;
    }

    *entities = new_entities;
    *capacity = new_capacity;
    return true;
}

/**
 * EN: Adds an active enemy to the game's dynamically sized collection.
 * PT: Adiciona um inimigo ativo à coleção dinâmica do jogo.
 */
bool game_add_enemy(Game *game, Enemy enemy)
{
    if (game == NULL ||
        !reserve_entities((void **)&game->enemies, &game->enemy_capacity,
                         game->enemy_count, sizeof(*game->enemies))) {
        return false;
    }

    enemy.active = true;
    game->enemies[game->enemy_count++] = enemy;
    return true;
}

/**
 * EN: Creates a projectile at the player's current horizontal position.
 * PT: Cria um projétil na posição horizontal atual do jogador.
 */
bool game_fire(Game *game)
{
    if (game == NULL ||
        !reserve_entities((void **)&game->projectiles, &game->projectile_capacity,
                         game->projectile_count, sizeof(*game->projectiles))) {
        return false;
    }

    Projectile projectile = {
        .x = game->player.x + (game->player.width - PROJECTILE_WIDTH) * 0.5f,
        .y = game->player.y,
        .width = PROJECTILE_WIDTH,
        .height = PROJECTILE_HEIGHT,
        .speed = PROJECTILE_SPEED,
        .active = true
    };
    game->projectiles[game->projectile_count++] = projectile;
    return true;
}

/**
 * EN: Records a one-shot action key in the supplied input state.
 * PT: Registra uma tecla de ação pontual no estado de entrada informado.
 */
void game_input_press(GameInput *input, GameKey key)
{
    if (input == NULL) {
        return;
    }

    switch (key) {
    case GAME_KEY_FIRE:
        input->fire = true;
        break;
    case GAME_KEY_PAUSE:
        input->pause = true;
        break;
    case GAME_KEY_CONFIRM:
        input->confirm = true;
        break;
    }
}

/**
 * EN: Moves the player according to input and clamps it to the world bounds.
 * PT: Move o jogador conforme a entrada e o mantém dentro dos limites do mundo.
 */
static void update_player(Game *game, const GameInput *input, float delta_time)
{
    float direction = 0.0f;
    float max_x = GAME_WORLD_WIDTH - game->player.width;

    if (input->move_left != input->move_right) {
        direction = input->move_left ? -1.0f : 1.0f;
    }

    game->player.x += direction * game->player.speed * delta_time;
    if (game->player.x < 0.0f) {
        game->player.x = 0.0f;
    } else if (game->player.x > max_x) {
        game->player.x = max_x;
    }
}

/**
 * EN: Moves the enemy formation and makes it descend when it reaches an edge.
 * PT: Move a formação de inimigos e a faz descer ao alcançar uma extremidade.
 */
static void update_enemies(Game *game, float delta_time)
{
    float left = GAME_WORLD_WIDTH;
    float right = 0.0f;
    float speed = 0.0f;
    bool has_active_enemy = false;

    for (size_t i = 0; i < game->enemy_count; i++) {
        Enemy *enemy = &game->enemies[i];
        if (!enemy->active) {
            continue;
        }
        has_active_enemy = true;
        if (enemy->x < left) {
            left = enemy->x;
        }
        if (enemy->x + enemy->width > right) {
            right = enemy->x + enemy->width;
        }
        if (enemy->speed > speed) {
            speed = enemy->speed;
        }
    }

    if (!has_active_enemy) {
        return;
    }

    float offset = game->enemy_direction * speed * delta_time;
    bool reached_edge = left + offset < 0.0f || right + offset > GAME_WORLD_WIDTH;
    if (left + offset < 0.0f) {
        offset = -left;
        game->enemy_direction = 1.0f;
    } else if (right + offset > GAME_WORLD_WIDTH) {
        offset = GAME_WORLD_WIDTH - right;
        game->enemy_direction = -1.0f;
    }

    float descent = ENEMY_DESCENT_SPEED * delta_time;
    if (reached_edge) {
        descent += game->enemy_drop_distance;
    }
    for (size_t i = 0; i < game->enemy_count; i++) {
        if (game->enemies[i].active) {
            game->enemies[i].x += offset;
            game->enemies[i].y += descent;
        }
    }
}

/**
 * EN: Moves projectiles upward and deactivates those that leave the world.
 * PT: Move os projéteis para cima e desativa os que saem do mundo.
 */
static void update_projectiles(Game *game, float delta_time)
{
    for (size_t i = 0; i < game->projectile_count; i++) {
        Projectile *projectile = &game->projectiles[i];
        projectile->y -= projectile->speed * delta_time;
        if (projectile->y + projectile->height < 0.0f) {
            projectile->active = false;
        }
    }
}

/**
 * EN: Returns whether two axis-aligned rectangles overlap.
 * PT: Informa se dois retângulos alinhados aos eixos se sobrepõem.
 */
static bool boxes_overlap(float ax, float ay, float aw, float ah,
                          float bx, float by, float bw, float bh)
{
    return ax < bx + bw && ax + aw > bx && ay < by + bh && ay + ah > by;
}

/**
 * EN: Deactivates colliding projectiles and enemies and awards enemy points.
 * PT: Desativa projéteis e inimigos em colisão e concede os pontos do inimigo.
 */
static void check_projectile_hits(Game *game)
{
    for (size_t i = 0; i < game->projectile_count; i++) {
        Projectile *projectile = &game->projectiles[i];
        if (!projectile->active) {
            continue;
        }

        for (size_t j = 0; j < game->enemy_count; j++) {
            Enemy *enemy = &game->enemies[j];
            if (!enemy->active ||
                !boxes_overlap(projectile->x, projectile->y,
                               projectile->width, projectile->height,
                               enemy->x, enemy->y, enemy->width, enemy->height)) {
                continue;
            }

            projectile->active = false;
            enemy->active = false;
            game->score += enemy->points;
            break;
        }
    }
}

/**
 * EN: Removes enemies that touch the player or reach the lower play area.
 * PT: Remove inimigos que tocam o jogador ou alcançam a parte inferior da área.
 */
static void check_enemy_player_contacts(Game *game)
{
    for (size_t i = 0; i < game->enemy_count; i++) {
        Enemy *enemy = &game->enemies[i];
        if (!enemy->active) {
            continue;
        }

        bool hit_player = boxes_overlap(enemy->x, enemy->y,
                                        enemy->width, enemy->height,
                                        game->player.x, game->player.y,
                                        game->player.width, game->player.height);
        bool reached_lower_region = enemy->y + enemy->height >= game->player.y;
        if (!hit_player && !reached_lower_region) {
            continue;
        }

        enemy->active = false;
        if (game->lives > 0) {
            game->lives--;
        }
        if (game->lives == 0) {
            game->state = GAME_STATE_GAME_OVER;
            return;
        }
    }
}

/**
 * EN: Compacts enemy and projectile arrays by removing inactive entities.
 * PT: Compacta os vetores de inimigos e projéteis removendo entidades inativas.
 */
static void remove_inactive_entities(Game *game)
{
    size_t enemy_write_index = 0;
    for (size_t i = 0; i < game->enemy_count; i++) {
        if (game->enemies[i].active) {
            game->enemies[enemy_write_index++] = game->enemies[i];
        }
    }
    game->enemy_count = enemy_write_index;

    size_t projectile_write_index = 0;
    for (size_t i = 0; i < game->projectile_count; i++) {
        if (game->projectiles[i].active) {
            game->projectiles[projectile_write_index++] = game->projectiles[i];
        }
    }
    game->projectile_count = projectile_write_index;
}

/**
 * EN: Starts the next wave, or switches to victory after the final wave.
 * PT: Inicia a próxima onda ou muda para vitória após a onda final.
 */
static bool advance_wave(Game *game)
{
    if (game->wave >= game->final_wave) {
        game->state = GAME_STATE_VICTORY;
        return true;
    }

    game->wave++;
    game->projectile_count = 0;
    if (!create_wave(game)) {
        return false;
    }
    return true;
}

/**
 * EN: Advances the game state by one frame using the supplied input and time.
 * PT: Avança o estado do jogo em um quadro usando a entrada e o intervalo dados.
 */
bool game_update(Game *game, const GameInput *input, float delta_time)
{
    if (game == NULL || input == NULL || !isfinite(delta_time) ||
        delta_time < 0.0f) {
        return false;
    }

    if (game->state == GAME_STATE_MENU) {
        return input->confirm ? game_start(game) : true;
    }
    if (game->state == GAME_STATE_PAUSED) {
        if (input->confirm) {
            game->state = GAME_STATE_PLAYING;
        }
        return true;
    }
    if (game->state == GAME_STATE_GAME_OVER) {
        return input->confirm ? game_start(game) : true;
    }
    if (game->state != GAME_STATE_PLAYING) {
        return true;
    }
    if (input->pause) {
        game->state = GAME_STATE_PAUSED;
        return true;
    }
    if (delta_time == 0.0f) {
        return input->fire ? game_fire(game) : true;
    }
    if (delta_time > MAX_FRAME_DELTA) {
        delta_time = MAX_FRAME_DELTA;
    }

    update_player(game, input, delta_time);
    update_enemies(game, delta_time);
    if (input->fire && !game_fire(game)) {
        return false;
    }
    update_projectiles(game, delta_time);
    check_projectile_hits(game);
    check_enemy_player_contacts(game);
    remove_inactive_entities(game);
    if (game->state == GAME_STATE_PLAYING && game->enemy_count == 0 &&
        !advance_wave(game)) {
        return false;
    }
    return true;
}
