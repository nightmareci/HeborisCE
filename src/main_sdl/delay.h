#pragma once

#ifdef _WIN32

#include <stdint.h>

void Delay(uint32_t milliseconds);

#else

#include "SDL_timer.h"

#define Delay(milliseconds) SDL_Delay((Uint32)(milliseconds))

#endif