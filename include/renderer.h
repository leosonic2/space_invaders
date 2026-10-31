#ifndef SPACE_INVADERS_RENDERER_H
#define SPACE_INVADERS_RENDERER_H

#include "game.h"

#include <stdbool.h>

typedef struct {
    int x;
    int y;
    int width;
    int height;
} RendererViewport;

typedef struct {
    unsigned int program;
    unsigned int vertex_array;
    unsigned int vertex_buffer;
    int color_location;
} Renderer;

typedef struct {
    bool show_title;
    bool show_prompt;
    bool show_hud;
    bool show_message;
    bool show_final_score;
    bool show_restart_prompt;
    char title[32];
    char prompt[48];
    char score[48];
    char lives[48];
    char wave[48];
    char message[32];
    char final_score[48];
    char restart_prompt[48];
} RendererText;

bool renderer_init(Renderer *renderer);
void renderer_destroy(Renderer *renderer);
RendererViewport renderer_calculate_viewport(int width, int height);
bool renderer_format_text(const Game *game, RendererText *text);
void renderer_render(Renderer *renderer, const Game *game, int width, int height);

#endif
