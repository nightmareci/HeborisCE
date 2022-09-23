#pragma once

#include "include.h"

#ifdef _WIN32

void Delay(uint32_t milliseconds);

#else

#define Delay(milliseconds) SDL_Delay((Uint32)(milliseconds))

#endif