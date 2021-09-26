#pragma once

#include "SDL.h"

typedef enum PHYSFSRWops_Mode {
	PHYSFSRWOPS_MODE_APPEND,
	PHYSFSRWOPS_MODE_READ,
	PHYSFSRWOPS_MODE_WRITE
} PHYSFSRWops_Mode;

// Create SDL_RWops from PhysicsFS files. PhysicsFS must be initialized before calling this.
// This can be used with existing SDL code, using PhysicsFS behind-the-scenes.
SDL_RWops *PHYSFSRWops_FromFile( const char *filename, PHYSFSRWops_Mode mode );