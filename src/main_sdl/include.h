#pragma once

#include "SDL.h"
#include "SDL_image.h"
#include "SDL_mixer.h"
#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#include <emscripten/html5.h>
#else
#include "physfs.h"
#include "physfsrwops.h"
#endif
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>
#include <stdbool.h>
#include <ctype.h>
#include <assert.h>
#include <limits.h>
#include "defs.h"
#ifdef ENABLE_LINUX_GPIO
#include <gpiod.h>
#endif
#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#endif
