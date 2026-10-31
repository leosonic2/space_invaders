#include "game_config.h"

#ifndef EXPECTED_FINAL_WAVE
#error EXPECTED_FINAL_WAVE must be provided by CMake
#endif

int main(void)
{
    return SPACE_INVADERS_FINAL_WAVE == EXPECTED_FINAL_WAVE ? 0 : 1;
}
