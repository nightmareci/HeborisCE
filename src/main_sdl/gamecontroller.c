#include "main_sdl/gamecontroller.h"
#include "main_sdl/filesystem.h"

void OpenGameControllerDB() {
	// The game will just go without the database if it's missing or fails to load.

	SDL_RWops* db = FSOpenRead("res/gamecontrollerdb.txt");
	if (!db) {
		return;
	}

	Sint64 size = SDL_RWsize(db);
	if (size < 0) {
		SDL_RWclose(db);
		return;
	}
	if (size == 0) {
		return;
	}

	SDL_GameControllerAddMappingsFromRW(db, 1);
}
