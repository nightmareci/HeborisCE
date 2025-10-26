#include "APP_main.h"
#include "APP_update.h"
#include "APP_filesystem.h"
#include "APP_input.h"
#include "game/config.h"
#include "game/gamestart.h"
#include <SDL3/SDL_main.h>

static bool APP_InitFast = false;
static int APP_QuitLevel;

// TODO: Move as much as possible of the Emscripten code into its own
// source(s). The __EMSCRIPTEN__ checks really ugly up the code.

#ifdef __EMSCRIPTEN__
void startup() {
	emscripten_set_main_loop(mainUpdate, 0, true);
}
#endif

int main(int argc, char** argv)
{
	if (!APP_InitFilesystem(argc, argv)) {
		return EXIT_FAILURE;
	}

	const char* directories[] = {
		"replay",
		"config",
		"config/data",
		"config/mission",
		"config/stage"
	};
	for (size_t i = 0u; i < sizeof(directories) / sizeof(*directories); i++) {
		if (!APP_CreateDirectory(directories[i])) {
			APP_QuitFilesystem();
			return EXIT_FAILURE;
		}
	}

	// This EM_ASM block ensures that the filesystem sync completes fully before
	// the game starts. The pre-startup sync is required, to ensure the writable
	// directories have been created before the game starts.
	#ifdef __EMSCRIPTEN__
	// TODO: This filesystem sync might not be needed with SDL3
	EM_ASM({
		FS.syncfs(true, function (err) {
			assert(!err || err.errno == 10);
			Module.ccall('startup', 'v', 'v')
		});
	});

	emscripten_exit_with_live_runtime();

	#else
	while (true) {
		mainUpdate();
	}

	#endif

	return EXIT_SUCCESS;
}

void APP_Init(size_t wavesCount)
{
	APP_QuitLevel = 0;

#ifdef __EMSCRIPTEN__
	/* Keyboard input gets a bit broken without this hint set. */
	//SDL_SetHint(SDL_HINT_EMSCRIPTEN_KEYBOARD_ELEMENT, "#canvas");
#endif

	/* SDLの初期化 || SDL initialization */
	if (!APP_InitFast &&
		!SDL_Init(
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
	APP_QuitLevel++;

	// If this fails, it doesn't matter, the game will still work. But it's
	// called because if it works, the game might perform better.
	if (!APP_InitFast) {
		SDL_SetCurrentThreadPriority(SDL_THREAD_PRIORITY_HIGH);
	}

	if (!APP_InitFast) {
		if (!APP_InitAudio(wavesCount)) {
			fprintf(stderr, "%s", SDL_GetError());
			APP_Exit(EXIT_FAILURE);
		}
		APP_QuitLevel++;
		APP_InitVideo();
		APP_QuitLevel++;
	}

	APP_SetFPS(60);

	if (!APP_InitFast) {
		APP_OpenInputs();
	}

	APP_SetSecondaryOffset(0, 0);

	if (!APP_InitFast) {
		APP_InitVideo();
	}

	APP_Start();
	APP_InitFast = true;
}

void APP_Quit(void)
{
	APP_CloseInputs();

	switch (APP_QuitLevel)
	{
	case 3: APP_QuitVideo();
	case 2: APP_QuitAudio();
	case 1: SDL_Quit();
	default: break;
	}
	APP_QuitLevel = 0;

	APP_InitFast = false;
}

#undef APP_Exit
void APP_Exit(int status)
{
	APP_Quit();
	APP_QuitFilesystem();
	exit(status);
}
