#ifndef APP_common_h_
#define APP_common_h_

#include "app/APP_build_config.h"
#include <SDL3/SDL.h>
#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#include <emscripten/html5.h>
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
#ifdef APP_ENABLE_LINUX_GPIO
#include <gpiod.h>
#endif
#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

#endif
