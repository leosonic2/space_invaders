#include "game.h"
#include "game_config.h"
#include "renderer.h"

#include <glad/gl.h>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <stdio.h>
#include <stdlib.h>

/**
 * EN: Updates the OpenGL viewport when the window framebuffer is resized.
 * PT: Atualiza a área de desenho do OpenGL quando o framebuffer é redimensionado.
 */
static void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    (void)window;
    glViewport(0, 0, width, height);
}

/**
 * EN: Converts GLFW key presses into one-shot game input events.
 * PT: Converte pressionamentos de teclas do GLFW em eventos pontuais do jogo.
 */
static void key_callback(GLFWwindow *window, int key, int scancode, int action,
                         int modifiers)
{
    (void)scancode;
    (void)modifiers;
    if (action != GLFW_PRESS) {
        return;
    }

    GameInput *input = glfwGetWindowUserPointer(window);
    if (input == NULL) {
        return;
    }

    switch (key) {
    case GLFW_KEY_SPACE:
        game_input_press(input, GAME_KEY_FIRE);
        break;
    case GLFW_KEY_ESCAPE:
        game_input_press(input, GAME_KEY_PAUSE);
        break;
    case GLFW_KEY_ENTER:
        game_input_press(input, GAME_KEY_CONFIRM);
        break;
    default:
        break;
    }
}

/**
 * EN: Writes GLFW diagnostics to the standard error stream.
 * PT: Escreve diagnósticos do GLFW no fluxo de erro padrão.
 */
static void report_glfw_error(int code, const char *description)
{
    fprintf(stderr, "GLFW error %d: %s\n", code, description);
}

/**
 * EN: Initializes the window, game, and renderer, then runs the main loop.
 * PT: Inicializa janela, jogo e renderizador e executa o loop principal.
 */
int main(void)
{
    GLFWwindow *window = NULL;
    Game game;
    Renderer renderer = {0};
    bool game_initialized = false;
    GameInput input_events = {0};
    int result = EXIT_FAILURE;
    const char *fps_option = getenv("SPACE_INVADERS_LOG_FPS");
    bool log_fps = fps_option != NULL && fps_option[0] != '\0';

    glfwSetErrorCallback(report_glfw_error);
    if (glfwInit() != GLFW_TRUE) {
        fprintf(stderr, "Failed to initialize GLFW.\n");
        return EXIT_FAILURE;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    window = glfwCreateWindow(1024, 768, "Space Invaders", NULL, NULL);
    if (window == NULL) {
        fprintf(stderr, "Failed to create the OpenGL 3.3 window.\n");
        goto cleanup;
    }

    glfwSetWindowSizeLimits(window, 800, 600, GLFW_DONT_CARE, GLFW_DONT_CARE);
    glfwMakeContextCurrent(window);
    if (gladLoadGL((GLADloadfunc)glfwGetProcAddress) == 0) {
        fprintf(stderr, "Failed to load OpenGL functions with GLAD.\n");
        goto cleanup;
    }

    if (!game_init(&game, SPACE_INVADERS_FINAL_WAVE)) {
        fprintf(stderr, "Failed to initialize the game model.\n");
        goto cleanup;
    }
    game_initialized = true;
    if (!renderer_init(&renderer)) {
        fprintf(stderr, "Failed to initialize the OpenGL renderer.\n");
        goto cleanup;
    }
    glfwSetWindowUserPointer(window, &input_events);
    glfwSetKeyCallback(window, key_callback);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    int framebuffer_width;
    int framebuffer_height;
    glfwGetFramebufferSize(window, &framebuffer_width, &framebuffer_height);
    framebuffer_size_callback(window, framebuffer_width, framebuffer_height);

    glfwSwapInterval(1);
    result = EXIT_SUCCESS;
    double previous_time = glfwGetTime();
    double fps_sample_start = previous_time;
    unsigned int fps_sample_frames = 0;
    while (glfwWindowShouldClose(window) == GLFW_FALSE) {
        glfwPollEvents();
        GameInput input = input_events;
        input.move_left = glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS ||
                          glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS;
        input.move_right = glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS ||
                           glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS;
        double current_time = glfwGetTime();
        float delta_time = (float)(current_time - previous_time);
        previous_time = current_time;
        if (!game_update(&game, &input, delta_time)) {
            fprintf(stderr, "Game update failed while allocating or updating entities.\n");
            result = EXIT_FAILURE;
            break;
        }
        input_events.fire = false;
        input_events.pause = false;
        input_events.confirm = false;

        glfwGetFramebufferSize(window, &framebuffer_width, &framebuffer_height);
        renderer_render(&renderer, &game, framebuffer_width, framebuffer_height);
        glfwSwapBuffers(window);
        if (log_fps) {
            fps_sample_frames++;
            double sample_end = glfwGetTime();
            double sample_duration = sample_end - fps_sample_start;
            if (sample_duration >= 5.0) {
                fprintf(stderr, "Average render FPS: %.1f over %.1f seconds\n",
                        (double)fps_sample_frames / sample_duration,
                        sample_duration);
                fflush(stderr);
                fps_sample_frames = 0;
                fps_sample_start = sample_end;
            }
        }
    }

cleanup:
    renderer_destroy(&renderer);
    if (game_initialized) {
        game_destroy(&game);
    }
    if (window != NULL) {
        glfwDestroyWindow(window);
    }
    glfwTerminate();
    return result;
}
