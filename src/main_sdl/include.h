#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include "physfsrwops.h"
#include <time.h>
#include <stdint.h>
#include <stdbool.h>

#define		FRAMEWORK_NAME		"SDL2"

#if		defined(WIN32)
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
