#include "renderer.h"

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

/**
 * EN: Checks viewport dimensions at the minimum and reference resolutions.
 * PT: Verifica as dimensões da área de desenho nas resoluções mínima e padrão.
 */
static void test_reference_resolutions(void)
{
    RendererViewport minimum = renderer_calculate_viewport(800, 600);
    CHECK(minimum.x == 0);
    CHECK(minimum.y == 0);
    CHECK(minimum.width == 800);
    CHECK(minimum.height == 600);

    RendererViewport reference = renderer_calculate_viewport(1024, 768);
    CHECK(reference.x == 0);
    CHECK(reference.y == 0);
    CHECK(reference.width == 1024);
    CHECK(reference.height == 768);
}

/**
 * EN: Verifies letterboxing preserves the logical aspect and handles invalid size.
 * PT: Verifica a proporção com barras laterais e o tratamento de tamanho inválido.
 */
static void test_letterboxing_preserves_logical_aspect(void)
{
    RendererViewport viewport = renderer_calculate_viewport(1280, 720);
    CHECK(viewport.x == 160);
    CHECK(viewport.y == 0);
    CHECK(viewport.width == 960);
    CHECK(viewport.height == 720);

    RendererViewport invalid = renderer_calculate_viewport(0, 720);
    CHECK(invalid.width == 0);
    CHECK(invalid.height == 0);
}

/**
 * EN: Runs viewport tests and reports whether all checks passed.
 * PT: Executa os testes da área de desenho e informa se passaram.
 */
int main(void)
{
    test_reference_resolutions();
    test_letterboxing_preserves_logical_aspect();
    return failures == 0 ? 0 : 1;
}
