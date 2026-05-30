#ifndef stdinc_h_
#define stdinc_h_

#include <build_config.h>
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
