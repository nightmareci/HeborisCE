#pragma once

#include "SDL.h"

typedef enum RWmode {
	RWMODE_APPEND,
	RWMODE_READ,
	RWMODE_WRITE
} RWmode;

// Create SDL_RWops from PhysicsFS files. PhysicsFS must be initialized before calling this.
// This can be used with existing SDL code, using PhysicsFS behind-the-scenes.
SDL_RWops *RWFromFile( const char *filename, RWmode mode );