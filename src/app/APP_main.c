#include "APP_main.h"
#include "APP_update.h"
#include "APP_filesystem.h"
#include "APP_input.h"
#include "game/config.h"
#include "game/gamestart.h"
#include <SDL3/SDL_main.h>

static int APP_argc;
static char** APP_argv;
static int APP_QuitLevel;

int main(int argc, char** argv)
{
	APP_argc = argc;
	APP_argv = argv;

	APP_QuitLevel = 0;

	// TODO: Remove this once the issue with WASAPI crackling with the move sound in-game is fixed
	#ifdef SDL_PLATFORM_WIN32
	if (!SDL_SetHint(SDL_HINT_AUDIO_DRIVER, "directsound")) {
		fprintf(stderr, "Couldn't set audio driver to DirectSound: %s\n", SDL_GetError());
		APP_Exit(EXIT_FAILURE);
	}
	#endif

	/* SDLの初期化 || SDL initialization */
	if (!SDL_Init(
			SDL_INIT_AUDIO | SDL_INIT_VIDEO
			#ifdef APP_ENABLE_JOYSTICK
			| SDL_INIT_JOYSTICK
			#endif
			#ifdef APP_ENABLE_GAME_CONTROLLER
			| SDL_INIT_GAMEPAD
			#endif
		)
	) {
		fprintf(stderr, "Couldn't initialize SDL: %s\n", SDL_GetError());
		APP_Exit(EXIT_FAILURE);
	}

	while (true) {
		mainUpdate();
	}

	return EXIT_SUCCESS;
}

void APP_Init(size_t wavesCount, const char* const* writeDirectories, size_t writeDirectoriesCount)
{
	if (APP_QuitLevel == 0) {
		// If this fails, it doesn't matter, the game will still work. But it's
		// called because if it works, the game might perform better.
		SDL_SetCurrentThreadPriority(SDL_THREAD_PRIORITY_HIGH);

		if (!APP_InitFilesystem(APP_argc, APP_argv)) {
			APP_Exit(EXIT_FAILURE);
		}
		APP_QuitLevel++;

		for (size_t i = 0u; i < writeDirectoriesCount; i++) {
			if (!APP_CreateDirectory(writeDirectories[i])) {
				APP_Exit(EXIT_FAILURE);
			}
		}

		if (!APP_InitAudio(wavesCount)) {
			fprintf(stderr, "%s", SDL_GetError());
			APP_Exit(EXIT_FAILURE);
		}
		APP_QuitLevel++;

		APP_InitVideo();
		APP_QuitLevel++;

		if (!APP_OpenInputs()) {
			fprintf(stderr, "%s", SDL_GetError());
			APP_Exit(EXIT_FAILURE);
		}
		APP_QuitLevel++;
	}

	APP_SetSecondaryOffset(0, 0);
	APP_SetFPS(60);

	APP_Start();
}

void APP_Quit(void)
{
	switch (APP_QuitLevel)
	{
	case 4: APP_CloseInputs();
	case 3: APP_QuitVideo();
	case 2: APP_QuitAudio();
	case 1: APP_QuitFilesystem();
	default: break;
	}
	APP_QuitLevel = 0;
}

#undef APP_Exit
void APP_Exit(int status)
{
	APP_Quit();
	SDL_Quit();
	exit(status);
}
