#pragma once

#include "SDL.h"

typedef enum PHYSFS_RWmode {
	PHYSFS_RWMODE_APPEND,
	PHYSFS_RWMODE_READ,
	PHYSFS_RWMODE_WRITE
} PHYSFS_RWmode;

// Create SDL_RWops from PhysicsFS files. PhysicsFS must be initialized before calling this.
// This can be used with existing SDL code, using PhysicsFS behind-the-scenes.
SDL_RWops *PHYSFS_RWFromFile( const char *filename, PHYSFS_RWmode mode );