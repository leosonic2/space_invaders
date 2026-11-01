#include "game_config.h"

#ifndef EXPECTED_FINAL_WAVE
#error EXPECTED_FINAL_WAVE must be provided by CMake
#endif

/**
 * EN: Verifies that the configured final wave matches the expected build value.
 * PT: Verifica se a onda final configurada corresponde ao valor esperado do build.
 */
int main(void)
{
    return SPACE_INVADERS_FINAL_WAVE == EXPECTED_FINAL_WAVE ? 0 : 1;
}
