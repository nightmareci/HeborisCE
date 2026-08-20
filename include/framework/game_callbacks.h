#ifndef game_callbacks_h_
#define game_callbacks_h_

// Values that GAME_Update() must return.
typedef enum GAME_UpdateResult
{
	// Continue running the game normally.
	GAME_UPDATE_CONTINUE,

	// Unload all loaded resources then continue running the game normally.
	GAME_UPDATE_RELOAD,

	// Quit the game.
	GAME_UPDATE_QUIT
} GAME_UpdateResult;

// Initializes the game.
// MAIN_SetResourceSettings() must be called only once in GAME_Init().
// The game must implement this.
void GAME_Init(void);

// Updates the game for one frame.
// The game must implement this.
GAME_UpdateResult GAME_Update(void);

#endif
