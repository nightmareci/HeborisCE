#ifndef STD_h_
#define STD_h_

#include <SYS_build_config.h>
#include <SDL3/SDL.h>
#include <stdint.h>
#include <inttypes.h>
#include <stdbool.h>
#include <limits.h>
#ifdef INPUT_ENABLE_LINUX_GPIO
#include <gpiod.h>
#endif
#ifdef SDL_PLATFORM_EMSCRIPTEN
#include <emscripten.h>
#endif

#endif
