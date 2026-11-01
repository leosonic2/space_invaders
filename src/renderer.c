#include "renderer.h"

#include <glad/gl.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define STAR_COUNT 40
#define FONT_HEIGHT 7
#define PLAYER_SPRITE_PIXEL_SIZE 4.0f
#define PLAYER_SPRITE_WIDTH 16
#define PLAYER_SPRITE_HEIGHT 20

typedef struct {
    char character;
    unsigned char rows[FONT_HEIGHT];
} BitmapGlyph;

typedef struct {
    float x;
    float y;
    float size;
} Star;

static void draw_rectangle(Renderer *renderer, float x, float y, float width,
                           float height, float red, float green, float blue);

static const Star stars[STAR_COUNT] = {
    {32.0f, 42.0f, 2.0f},   {124.0f, 111.0f, 1.5f}, {250.0f, 63.0f, 2.0f},
    {381.0f, 152.0f, 1.5f}, {492.0f, 48.0f, 2.0f},  {610.0f, 101.0f, 1.5f},
    {731.0f, 55.0f, 2.0f},  {844.0f, 143.0f, 1.5f}, {961.0f, 81.0f, 2.0f},
    {75.0f, 207.0f, 1.5f},  {198.0f, 263.0f, 2.0f}, {335.0f, 221.0f, 1.5f},
    {448.0f, 302.0f, 2.0f}, {560.0f, 250.0f, 1.5f}, {688.0f, 285.0f, 2.0f},
    {799.0f, 221.0f, 1.5f}, {920.0f, 310.0f, 2.0f}, {1002.0f, 251.0f, 1.5f},
    {41.0f, 391.0f, 2.0f},  {163.0f, 353.0f, 1.5f}, {280.0f, 417.0f, 2.0f},
    {407.0f, 367.0f, 1.5f}, {535.0f, 442.0f, 2.0f}, {647.0f, 384.0f, 1.5f},
    {766.0f, 429.0f, 2.0f}, {875.0f, 372.0f, 1.5f}, {989.0f, 447.0f, 2.0f},
    {96.0f, 521.0f, 1.5f},  {218.0f, 584.0f, 2.0f}, {347.0f, 539.0f, 1.5f},
    {472.0f, 621.0f, 2.0f}, {589.0f, 562.0f, 1.5f}, {710.0f, 645.0f, 2.0f},
    {826.0f, 579.0f, 1.5f}, {944.0f, 652.0f, 2.0f}, {1010.0f, 566.0f, 1.5f},
    {51.0f, 710.0f, 2.0f},  {301.0f, 731.0f, 1.5f}, {643.0f, 720.0f, 2.0f},
    {892.0f, 735.0f, 1.5f}
};

static const char player_sprite[PLAYER_SPRITE_HEIGHT][PLAYER_SPRITE_WIDTH + 1] = {
    "......####......",
    "......#WW#......",
    "......#WW#......",
    ".....##WW##.....",
    ".....#WWWW#.....",
    "....##WWWW##....",
    "...##WW..WW##...",
    "..##WW....WW##..",
    ".###WW....WW###.",
    "##RR#......#RR##",
    "##BB#......#BB##",
    "##WW########WW##",
    "#WW.##RRRR##.WW#",
    "#WW.##RRRR##.WW#",
    "#WW..#RRRR#..WW#",
    "#WW.##WWWW##.WW#",
    "#WW.###..###.WW#",
    "#WW##......##WW#",
    "##RR##....##RR##",
    "..##.##..##.##.."
};

static const BitmapGlyph glyphs[] = {
    {'A', {14, 17, 17, 31, 17, 17, 17}}, {'B', {30, 17, 17, 30, 17, 17, 30}},
    {'C', {14, 17, 16, 16, 16, 17, 14}}, {'D', {30, 17, 17, 17, 17, 17, 30}},
    {'E', {31, 16, 16, 30, 16, 16, 31}}, {'F', {31, 16, 16, 30, 16, 16, 16}},
    {'G', {14, 17, 16, 23, 17, 17, 15}}, {'H', {17, 17, 17, 31, 17, 17, 17}},
    {'I', {14, 4, 4, 4, 4, 4, 14}},      {'J', {7, 2, 2, 2, 18, 18, 12}},
    {'K', {17, 18, 20, 24, 20, 18, 17}}, {'L', {16, 16, 16, 16, 16, 16, 31}},
    {'M', {17, 27, 21, 21, 17, 17, 17}}, {'N', {17, 25, 21, 19, 17, 17, 17}},
    {'O', {14, 17, 17, 17, 17, 17, 14}}, {'P', {30, 17, 17, 30, 16, 16, 16}},
    {'Q', {14, 17, 17, 17, 21, 18, 13}}, {'R', {30, 17, 17, 30, 20, 18, 17}},
    {'S', {15, 16, 16, 14, 1, 1, 30}},   {'T', {31, 4, 4, 4, 4, 4, 4}},
    {'U', {17, 17, 17, 17, 17, 17, 14}}, {'V', {17, 17, 17, 17, 10, 10, 4}},
    {'W', {17, 17, 17, 21, 21, 21, 10}}, {'X', {17, 17, 10, 4, 10, 17, 17}},
    {'Y', {17, 17, 10, 4, 4, 4, 4}},     {'Z', {31, 1, 2, 4, 8, 16, 31}},
    {'0', {14, 17, 19, 21, 25, 17, 14}}, {'1', {4, 12, 4, 4, 4, 4, 14}},
    {'2', {14, 17, 1, 2, 4, 8, 31}},     {'3', {30, 1, 1, 14, 1, 1, 30}},
    {'4', {2, 6, 10, 18, 31, 2, 2}},     {'5', {31, 16, 16, 30, 1, 1, 30}},
    {'6', {14, 16, 16, 30, 17, 17, 14}}, {'7', {31, 1, 2, 4, 8, 8, 8}},
    {'8', {14, 17, 17, 14, 17, 17, 14}}, {'9', {14, 17, 17, 15, 1, 1, 14}},
    {':', {0, 4, 4, 0, 4, 4, 0}}
};

/**
 * EN: Finds the bitmap glyph associated with a character.
 * PT: Localiza o glifo bitmap associado a um caractere.
 */
static const BitmapGlyph *find_glyph(char character)
{
    for (size_t i = 0; i < sizeof(glyphs) / sizeof(glyphs[0]); i++) {
        if (glyphs[i].character == character) {
            return &glyphs[i];
        }
    }
    return NULL;
}

/**
 * EN: Draws text by rendering the lit pixels of each bitmap glyph.
 * PT: Desenha texto renderizando os pixels acesos de cada glifo bitmap.
 */
static void draw_text(Renderer *renderer, const char *text, float x, float y,
                      float scale, float red, float green, float blue)
{
    for (size_t character_index = 0; text[character_index] != '\0';
         character_index++) {
        char character = text[character_index];
        const BitmapGlyph *glyph = find_glyph(character);
        if (glyph != NULL) {
            for (int row = 0; row < FONT_HEIGHT; row++) {
                for (int column = 0; column < 5; column++) {
                    if ((glyph->rows[row] & (1U << (4 - column))) != 0) {
                        draw_rectangle(renderer,
                                       x + (float)column * scale,
                                       y + (float)row * scale,
                                       scale, scale, red, green, blue);
                    }
                }
            }
        }
        x += 6.0f * scale;
    }
}

/**
 * EN: Reads an entire file into a null-terminated heap buffer.
 * PT: Lê um arquivo inteiro para um buffer de heap terminado em nulo.
 */
static char *read_file(const char *path)
{
    FILE *file = fopen(path, "rb");
    if (file == NULL) {
        perror(path);
        return NULL;
    }

    if (fseek(file, 0, SEEK_END) != 0) {
        perror(path);
        fclose(file);
        return NULL;
    }
    long length = ftell(file);
    if (length < 0 || fseek(file, 0, SEEK_SET) != 0) {
        perror(path);
        fclose(file);
        return NULL;
    }

    char *contents = malloc((size_t)length + 1U);
    if (contents == NULL) {
        fprintf(stderr, "Failed to allocate memory for shader %s.\n", path);
        fclose(file);
        return NULL;
    }
    size_t read_count = fread(contents, 1U, (size_t)length, file);
    if (read_count != (size_t)length || ferror(file)) {
        fprintf(stderr, "Failed to read shader %s.\n", path);
        free(contents);
        fclose(file);
        return NULL;
    }
    contents[length] = '\0';
    fclose(file);
    return contents;
}

/**
 * EN: Loads and compiles a shader, reporting compilation diagnostics on failure.
 * PT: Carrega e compila um shader, informando erros de compilação se necessário.
 */
static unsigned int compile_shader(unsigned int type, const char *path)
{
    char *source = read_file(path);
    if (source == NULL) {
        return 0;
    }

    unsigned int shader = glCreateShader(type);
    glShaderSource(shader, 1, (const char *const *)&source, NULL);
    glCompileShader(shader);
    free(source);

    int compiled = GL_FALSE;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
    if (compiled != GL_TRUE) {
        int log_length = 0;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &log_length);
        char *log = malloc((size_t)log_length + 1U);
        if (log != NULL) {
            glGetShaderInfoLog(shader, log_length, NULL, log);
            log[log_length] = '\0';
            fprintf(stderr, "Failed to compile shader %s:\n%s\n", path, log);
            free(log);
        } else {
            fprintf(stderr, "Failed to compile shader %s.\n", path);
        }
        glDeleteShader(shader);
        return 0;
    }
    return shader;
}

/**
 * EN: Compiles and links the vertex and fragment shaders into a program.
 * PT: Compila e vincula os shaders de vértice e fragmento em um programa.
 */
static unsigned int create_program(void)
{
    unsigned int vertex_shader = compile_shader(GL_VERTEX_SHADER, "shaders/vertex.glsl");
    if (vertex_shader == 0) {
        return 0;
    }

    unsigned int fragment_shader =
        compile_shader(GL_FRAGMENT_SHADER, "shaders/fragment.glsl");
    if (fragment_shader == 0) {
        glDeleteShader(vertex_shader);
        return 0;
    }

    unsigned int program = glCreateProgram();
    glAttachShader(program, vertex_shader);
    glAttachShader(program, fragment_shader);
    glLinkProgram(program);
    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);

    int linked = GL_FALSE;
    glGetProgramiv(program, GL_LINK_STATUS, &linked);
    if (linked != GL_TRUE) {
        int log_length = 0;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &log_length);
        char *log = malloc((size_t)log_length + 1U);
        if (log != NULL) {
            glGetProgramInfoLog(program, log_length, NULL, log);
            log[log_length] = '\0';
            fprintf(stderr, "Failed to link the renderer shader program:\n%s\n", log);
            free(log);
        } else {
            fprintf(stderr, "Failed to link the renderer shader program.\n");
        }
        glDeleteProgram(program);
        return 0;
    }
    return program;
}

/**
 * EN: Creates the shader program and vertex resources required for drawing.
 * PT: Cria o programa de shaders e os recursos de vértice usados nos desenhos.
 */
bool renderer_init(Renderer *renderer)
{
    if (renderer == NULL) {
        return false;
    }

    *renderer = (Renderer){0};
    renderer->program = create_program();
    if (renderer->program == 0) {
        return false;
    }
    renderer->color_location = glGetUniformLocation(renderer->program, "shape_color");
    if (renderer->color_location < 0) {
        fprintf(stderr, "Renderer shader is missing the shape_color uniform.\n");
        renderer_destroy(renderer);
        return false;
    }

    glGenVertexArrays(1, &renderer->vertex_array);
    glGenBuffers(1, &renderer->vertex_buffer);
    glBindVertexArray(renderer->vertex_array);
    glBindBuffer(GL_ARRAY_BUFFER, renderer->vertex_buffer);
    glBufferData(GL_ARRAY_BUFFER, 12 * (GLsizeiptr)sizeof(float), NULL,
                 GL_DYNAMIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * (GLsizei)sizeof(float), NULL);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    return true;
}

/**
 * EN: Releases the renderer's OpenGL resources and clears its fields.
 * PT: Libera os recursos OpenGL do renderizador e limpa seus campos.
 */
void renderer_destroy(Renderer *renderer)
{
    if (renderer == NULL) {
        return;
    }
    if (renderer->vertex_buffer != 0) {
        glDeleteBuffers(1, &renderer->vertex_buffer);
    }
    if (renderer->vertex_array != 0) {
        glDeleteVertexArrays(1, &renderer->vertex_array);
    }
    if (renderer->program != 0) {
        glDeleteProgram(renderer->program);
    }
    *renderer = (Renderer){0};
}

/**
 * EN: Draws a colored rectangle in the game's logical coordinate system.
 * PT: Desenha um retângulo colorido no sistema lógico de coordenadas do jogo.
 */
static void draw_rectangle(Renderer *renderer, float x, float y, float width,
                           float height, float red, float green, float blue)
{
    float left = -1.0f + 2.0f * x / GAME_WORLD_WIDTH;
    float right = -1.0f + 2.0f * (x + width) / GAME_WORLD_WIDTH;
    float top = 1.0f - 2.0f * y / GAME_WORLD_HEIGHT;
    float bottom = 1.0f - 2.0f * (y + height) / GAME_WORLD_HEIGHT;
    const float vertices[] = {
        left, top, right, top, right, bottom,
        left, top, right, bottom, left, bottom
    };

    glBindVertexArray(renderer->vertex_array);
    glBindBuffer(GL_ARRAY_BUFFER, renderer->vertex_buffer);
    glBufferSubData(GL_ARRAY_BUFFER, 0, (GLsizeiptr)sizeof(vertices), vertices);
    glUniform4f(renderer->color_location, red, green, blue, 1.0f);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}

/**
 * EN: Renders the player's pixel-art sprite as horizontal colored runs.
 * PT: Renderiza o sprite pixel art do jogador em faixas horizontais coloridas.
 */
static void draw_player(Renderer *renderer, const Player *player)
{
    for (int row = 0; row < PLAYER_SPRITE_HEIGHT; row++) {
        int column = 0;
        while (column < PLAYER_SPRITE_WIDTH) {
            char pixel = player_sprite[row][column];
            if (pixel == '.') {
                column++;
                continue;
            }

            int run_start = column;
            while (column < PLAYER_SPRITE_WIDTH &&
                   player_sprite[row][column] == pixel) {
                column++;
            }

            float red;
            float green;
            float blue;
            switch (pixel) {
            case '#':
                red = 0.005f;
                green = 0.008f;
                blue = 0.018f;
                break;
            case 'R':
                red = 0.95f;
                green = 0.20f;
                blue = 0.17f;
                break;
            case 'B':
                red = 0.25f;
                green = 0.32f;
                blue = 0.72f;
                break;
            default:
                red = 0.92f;
                green = 0.94f;
                blue = 0.98f;
                break;
            }

            draw_rectangle(
                renderer,
                player->x + (float)run_start * PLAYER_SPRITE_PIXEL_SIZE,
                player->y + (float)row * PLAYER_SPRITE_PIXEL_SIZE,
                (float)(column - run_start) * PLAYER_SPRITE_PIXEL_SIZE,
                PLAYER_SPRITE_PIXEL_SIZE,
                red, green, blue);
        }
    }
}

/**
 * EN: Calculates a centered viewport that preserves the logical world aspect.
 * PT: Calcula uma área centralizada que preserva a proporção do mundo lógico.
 */
RendererViewport renderer_calculate_viewport(int width, int height)
{
    if (width <= 0 || height <= 0) {
        return (RendererViewport){0};
    }

    const float logical_aspect = GAME_WORLD_WIDTH / GAME_WORLD_HEIGHT;
    float viewport_aspect = (float)width / (float)height;
    RendererViewport viewport = {.width = width, .height = height};

    if (viewport_aspect > logical_aspect) {
        viewport.width = (int)((float)height * logical_aspect);
        viewport.x = (width - viewport.width) / 2;
    } else {
        viewport.height = (int)((float)width / logical_aspect);
        viewport.y = (height - viewport.height) / 2;
    }
    return viewport;
}

/**
 * EN: Formats the text and visibility flags required for the current game state.
 * PT: Formata os textos e indicadores de exibição do estado atual do jogo.
 */
bool renderer_format_text(const Game *game, RendererText *text)
{
    if (game == NULL || text == NULL) {
        return false;
    }

    *text = (RendererText){0};
    switch (game->state) {
    case GAME_STATE_MENU:
        text->show_title = true;
        text->show_prompt = true;
        snprintf(text->title, sizeof(text->title), "SPACE INVADERS");
        snprintf(text->prompt, sizeof(text->prompt), "PRESS ENTER TO START");
        return true;
    case GAME_STATE_PLAYING:
    case GAME_STATE_PAUSED:
        text->show_hud = true;
        snprintf(text->score, sizeof(text->score), "SCORE: %d", game->score);
        snprintf(text->lives, sizeof(text->lives), "LIVES: %d", game->lives);
        snprintf(text->wave, sizeof(text->wave), "WAVE: %d", game->wave);
        if (game->state == GAME_STATE_PAUSED) {
            text->show_message = true;
            snprintf(text->message, sizeof(text->message), "PAUSED");
        }
        return true;
    case GAME_STATE_GAME_OVER:
    case GAME_STATE_VICTORY:
        text->show_hud = true;
        text->show_message = true;
        text->show_final_score = true;
        text->show_restart_prompt = true;
        snprintf(text->score, sizeof(text->score), "SCORE: %d", game->score);
        snprintf(text->lives, sizeof(text->lives), "LIVES: %d", game->lives);
        snprintf(text->wave, sizeof(text->wave), "WAVE: %d", game->wave);
        snprintf(text->message, sizeof(text->message), "%s",
                 game->state == GAME_STATE_VICTORY ? "VICTORY" : "GAME OVER");
        snprintf(text->final_score, sizeof(text->final_score),
                 "FINAL SCORE: %d", game->score);
        snprintf(text->restart_prompt, sizeof(text->restart_prompt),
                 "PRESS ENTER TO RESTART");
        return true;
    default:
        return false;
    }
}

/**
 * EN: Applies the aspect-preserving viewport for the logical game world.
 * PT: Aplica a área de desenho que preserva a proporção do mundo lógico.
 */
static void set_logical_viewport(int width, int height)
{
    RendererViewport viewport = renderer_calculate_viewport(width, height);
    if (viewport.width > 0 && viewport.height > 0) {
        glViewport(viewport.x, viewport.y, viewport.width, viewport.height);
    }
}

/**
 * EN: Draws the background, entities, and interface for the current game frame.
 * PT: Desenha o fundo, as entidades e a interface do quadro atual do jogo.
 */
void renderer_render(Renderer *renderer, const Game *game, int width, int height)
{
    if (renderer == NULL || game == NULL || renderer->program == 0) {
        return;
    }

    glClearColor(0.015f, 0.02f, 0.08f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    set_logical_viewport(width, height);
    glUseProgram(renderer->program);

    for (size_t i = 0; i < STAR_COUNT; i++) {
        draw_rectangle(renderer, stars[i].x, stars[i].y, stars[i].size,
                       stars[i].size, 0.45f, 0.55f, 0.8f);
    }

    RendererText text;
    if (!renderer_format_text(game, &text)) {
        glBindVertexArray(0);
        glUseProgram(0);
        return;
    }
    if (text.show_title) {
        draw_text(renderer, text.title, 180.0f, 290.0f, 8.0f,
                  0.35f, 0.95f, 1.0f);
        draw_text(renderer, text.prompt, 272.0f, 410.0f, 4.0f,
                  0.9f, 0.95f, 1.0f);
    } else {
        draw_player(renderer, &game->player);
        for (size_t i = 0; i < game->enemy_count; i++) {
            float red = (i / (size_t)game->wave_enemy_columns) % 2 == 0
                            ? 0.25f
                            : 0.9f;
            draw_rectangle(renderer, game->enemies[i].x, game->enemies[i].y,
                           game->enemies[i].width, game->enemies[i].height,
                           red, 0.95f, 0.35f);
        }
        for (size_t i = 0; i < game->projectile_count; i++) {
            draw_rectangle(renderer, game->projectiles[i].x,
                           game->projectiles[i].y,
                           game->projectiles[i].width,
                           game->projectiles[i].height,
                           1.0f, 0.75f, 0.2f);
        }

        if (text.show_hud) {
            draw_text(renderer, text.score, 32.0f, 24.0f, 3.0f,
                      0.95f, 0.95f, 1.0f);
            draw_text(renderer, text.lives, 465.0f, 24.0f, 3.0f,
                      0.95f, 0.95f, 1.0f);
            draw_text(renderer, text.wave, 800.0f, 24.0f, 3.0f,
                      0.95f, 0.95f, 1.0f);
        }

        if (text.show_message && game->state == GAME_STATE_PAUSED) {
            draw_text(renderer, text.message, 368.0f, 330.0f, 8.0f,
                      1.0f, 0.85f, 0.2f);
        } else if (text.show_message) {
            bool victory = game->state == GAME_STATE_VICTORY;
            draw_text(renderer, text.message,
                      victory ? 360.0f : 296.0f, 300.0f, 8.0f,
                      victory ? 0.35f : 1.0f, victory ? 1.0f : 0.35f, 0.3f);
            if (text.show_final_score) {
                draw_text(renderer, text.final_score, 320.0f, 400.0f, 4.0f,
                          0.95f, 0.95f, 1.0f);
            }
            if (text.show_restart_prompt) {
                draw_text(renderer, text.restart_prompt, 260.0f, 460.0f, 4.0f,
                          0.9f, 0.95f, 1.0f);
            }
        }
    }

    glBindVertexArray(0);
    glUseProgram(0);
}
