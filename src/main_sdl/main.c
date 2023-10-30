#include "main_sdl/include.h"
#include "script/config.h"
#include "gamestart.h"
#include "filesystem.h"
#ifdef ENABLE_GAME_CONTROLLER
#include "main_sdl/gamecontroller.h"
#endif
#include <stdarg.h>

static int quitLevel = 0;
int quit(int status) {
	switch ( quitLevel )
	{
	case 5: FSDeInit();
	case 4: Mix_CloseAudio();
	case 3: Mix_Quit();
	case 2: IMG_Quit();
	case 1: SDL_Quit();
	default: break;
	}
	return status;
}

#ifdef __EMSCRIPTEN__
void startup() {
	emscripten_set_main_loop(mainUpdate, 0, true);
}
#endif

int main(int argc, char** argv)
{
	/* SDLの初期化 || SDL initialization */
	if ( SDL_Init(
		SDL_INIT_AUDIO | SDL_INIT_VIDEO
		#ifdef ENABLE_JOYSTICK
		| SDL_INIT_JOYSTICK
		#endif
		#ifdef ENABLE_GAME_CONTROLLER
		| SDL_INIT_GAMECONTROLLER
		#endif
	) < 0 )
	{
		fprintf(stderr, "Couldn't initialize SDL: %s\n", SDL_GetError());
		return quit(EXIT_FAILURE);
	}

	/* If this fails, it doesn't matter, the game will still work. But it's called because if it works, the game might perform better. */
	SDL_SetThreadPriority(SDL_THREAD_PRIORITY_HIGH);

	quitLevel++;
	SDL_SetHint(SDL_HINT_RENDER_BATCHING, "1");

	/* 画像の初期化 || Image initialization */
	if ( IMG_Init(IMG_INIT_PNG) != IMG_INIT_PNG )
	{
		fprintf(stderr, "Couldn't initialize image support: %s\n", IMG_GetError());
		return quit(EXIT_FAILURE);
	}
	quitLevel++;

	/* サウンドの初期化 || Sound initialization */
	const int formatsInitialized = Mix_Init(
		MIX_INIT_MID |
		MIX_INIT_OGG |
		MIX_INIT_MP3 |
		MIX_INIT_FLAC |
		MIX_INIT_OPUS |
		MIX_INIT_MOD
	);
	if ( !formatsInitialized )
	{
		fprintf(stderr, "Couldn't initialize sound support: %s\n", Mix_GetError());
		return quit(EXIT_FAILURE);
	}
	quitLevel++;

	wavebgm_supported[WAVEBGM_MID] = !!(formatsInitialized & MIX_INIT_MID);
	wavebgm_supported[WAVEBGM_WAV] = 1; // WAVEはいつでも利用可能 || WAVE is always supported
	wavebgm_supported[WAVEBGM_OGG] = !!(formatsInitialized & MIX_INIT_OGG);
	wavebgm_supported[WAVEBGM_MP3] = !!(formatsInitialized & MIX_INIT_MP3);
	wavebgm_supported[WAVEBGM_FLAC] = !!(formatsInitialized & MIX_INIT_FLAC);
	wavebgm_supported[WAVEBGM_OPUS] = !!(formatsInitialized & MIX_INIT_OPUS);
	wavebgm_supported[WAVEBGM_MOD] = !!(formatsInitialized & MIX_INIT_MOD);
	wavebgm_supported[WAVEBGM_IT] = !!(formatsInitialized & MIX_INIT_MOD);
	wavebgm_supported[WAVEBGM_XM] = !!(formatsInitialized & MIX_INIT_MOD);
	wavebgm_supported[WAVEBGM_S3M] = !!(formatsInitialized & MIX_INIT_MOD);

	if ( Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024) < 0 )
	{
		fprintf(stderr, "Couldn't open audio: %s\n", Mix_GetError());
		return quit(EXIT_FAILURE);
	}
	quitLevel++;

	Mix_AllocateChannels(100);

	if (!FSInit(argc, argv)) {
		return quit(EXIT_FAILURE);
	}
	quitLevel++;

#ifdef ENABLE_GAME_CONTROLLER
	/* Must be called after FSInit, as it reads gamecontrollerdb.txt in the res directory on some platforms. */
	OpenGameControllers();
#endif

	const char* directories[] = {
		"replay",
		"config",
		"config/data",
		"config/mission",
		"config/stage"
	};
	for (size_t i = 0u; i < sizeof(directories) / sizeof(*directories); i++) {
		if (!FSMkdir(directories[i])) {
			return EXIT_FAILURE;
		}
	}

	// This EM_ASM block ensures that the filesystem sync completes fully before
	// the game starts. The pre-startup sync is required, to ensure the writable
	// directories have been created before the game starts.
	#ifdef __EMSCRIPTEN__
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
