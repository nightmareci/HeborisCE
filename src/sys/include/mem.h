#ifndef mem_h_
#define mem_h_

#include <stdinc.h>

// Leftover allocated memory will be cleaned up when exiting. Still, this memory
// subsystem isn't a garbage collector, so you should deallocate memory when
// you're done with it. The cleanup feature is more for cleaning up remaining
// allocations if an error occurs during gameplay.

// Dynamically allocate memory.
void* MEM_Allocate(void* old, size_t size, bool zero);

// Deallocate memory previously allocated with MEM_Allocate().
void MEM_Deallocate(void* mem);

#endif
