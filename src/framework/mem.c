#include <framework/mem.h>
#include "mem_private.h"
#include "error.h"
#include "main_private.h"

static size_t MEM_AllocationsCount = 0;
static void** MEM_Allocations = NULL;

void* MEM_Allocate(void* mem, size_t size, bool zero)
{
	size_t i;
	for (i = 0; i < MEM_AllocationsCount; i++) {
		if (!MEM_Allocations[i]) {
			break;
		}
	}
	if (i == MEM_AllocationsCount) {
		void** newAllocations = SDL_realloc(MEM_Allocations, (MEM_AllocationsCount + 1) * sizeof(void*));
		if (!newAllocations) {
			ERROR_Set("%s", SDL_GetError());
			MAIN_Exit(SDL_APP_FAILURE);
		}
		MEM_Allocations = newAllocations;
		MEM_Allocations[i] = NULL;
		MEM_AllocationsCount++;
	}
	void* allocation;
	if (mem) {
		allocation = SDL_realloc(mem, size);
		if (!allocation) {
			ERROR_Set("%s", SDL_GetError());
			MAIN_Exit(SDL_APP_FAILURE);
		}
		if (zero) {
			SDL_memset(allocation, 0, size);
		}
	}
	else if (zero) {
		allocation = SDL_calloc(1, size);
		if (!allocation) {
			ERROR_Set("%s", SDL_GetError());
			MAIN_Exit(SDL_APP_FAILURE);
		}
	}
	else {
		allocation = SDL_malloc(size);
		if (!allocation) {
			ERROR_Set("%s", SDL_GetError());
			MAIN_Exit(SDL_APP_FAILURE);
		}
	}
	MEM_Allocations[i] = allocation;
	return allocation;
}

void MEM_Deallocate(void* mem)
{
	if (!mem) {
		return;
	}
	for (size_t i = 0; i < MEM_AllocationsCount; i++) {
		if (MEM_Allocations[i] == mem) {
			SDL_free(mem);
			MEM_Allocations[i] = NULL;
			return;
		}
	}
	ERROR_Set("Attempted to deallocate memory with invalid pointer");
	MAIN_Exit(SDL_APP_FAILURE);
}

void MEM_Cleanup(void)
{
	for (size_t i = 0; i < MEM_AllocationsCount; i++) {
		SDL_free(MEM_Allocations[i]);
	}
	SDL_free(MEM_Allocations);
	MEM_Allocations = NULL;
	MEM_AllocationsCount = 0;
}
