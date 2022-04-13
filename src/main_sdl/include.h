#pragma once

#include "SDL.h"
#include "SDL_image.h"
#include "SDL_mixer.h"
#include "physfsrwops.h"
#include "platform.h"
#include <time.h>
#include <stdint.h>
#include <stdbool.h>
#if defined(LINUX_GPIO)
#include <gpiod.h>
#endif

#define		FRAMEWORK_NAME		"SDL2"

#if		defined(VITA)
#define		FRAMEWORK_VER		"VITA-" FRAMEWORK_NAME
#elif		defined(WIN32)
#define		FRAMEWORK_VER		"WIN-" FRAMEWORK_NAME
#elif	defined(MACOSX)
#define		FRAMEWORK_VER		"OSX-" FRAMEWORK_NAME
#elif	defined(LINUX)
#define		FRAMEWORK_VER		"LNX-" FRAMEWORK_NAME
#else
#define		FRAMEWORK_VER		FRAMEWORK_NAME
#endif

#if SDL_BYTEORDER == SDL_LIL_ENDIAN
#define SWAP16(X)    (X)
#define SWAP32(X)    (X)
#else
#define SWAP16(X)    SDL_Swap16(X)
#define SWAP32(X)    SDL_Swap32(X)
#endif
