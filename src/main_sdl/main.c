#include "main_sdl/include.h"
#include "gamestart.h"
#include "physfs.h"

int main(int argc, char* argv[])
{
	/* SDLの初期化 */
	if ( SDL_Init(SDL_INIT_AUDIO | SDL_INIT_VIDEO | SDL_INIT_JOYSTICK) < 0 )
	{
		fprintf(stderr, "Couldn't initialize SDL: %s\n", SDL_GetError());
		return EXIT_FAILURE;
	}
	SDL_SetHint(SDL_HINT_RENDER_BATCHING, "1");

	/* 画像の初期化 */
	IMG_Init(
		IMG_INIT_JPG |
		IMG_INIT_PNG |
		IMG_INIT_TIF |
		IMG_INIT_WEBP
	);

	/* サウンドの初期化 */
	Mix_Init(
		MIX_INIT_FLAC |
		MIX_INIT_MOD |
		MIX_INIT_MP3 |
		MIX_INIT_OGG |
		MIX_INIT_MID |
		MIX_INIT_OPUS
	);
	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024);
	Mix_AllocateChannels(100);

	if ( !PHYSFS_init(argv[0]) )
	{
		fprintf(stderr, "Error with PHYSFS_init: %s\n", PHYSFS_getErrorByCode(PHYSFS_getLastErrorCode()));
		return EXIT_FAILURE;
	}

	#if defined(PACKAGETYPE_INSTALLABLE) || defined(PACKAGETYPE_PORTABLE)
	char *basePath;
	if ( !(basePath = SDL_GetBasePath()) ) {
		fprintf(stderr, "Failed getting base path: %s\n", SDL_GetError());
		return EXIT_FAILURE;
	}
	if ( !PHYSFS_mount(basePath, NULL, 0) )
	{
		fprintf(stderr, "Error mounting base path: %s\n", PHYSFS_getErrorByCode(PHYSFS_getLastErrorCode()));
		return EXIT_FAILURE;
	}

	#ifdef PACKAGETYPE_INSTALLABLE
	char *prefPath;
	if ( !(prefPath = SDL_GetPrefPath("nightmareci", "HeborisC7EX SDL2")) )
	{
		fprintf(stderr, "Failed getting pref path: %s\n", SDL_GetError());
		return EXIT_FAILURE;
	}
	if ( !PHYSFS_setWriteDir(prefPath) )
	{
		fprintf(stderr, "Error setting pref path for writing: %s\n", PHYSFS_getErrorByCode(PHYSFS_getLastErrorCode()));
		return EXIT_FAILURE;
	}
	if ( !PHYSFS_mount(prefPath, NULL, 0) )
	{
		fprintf(stderr, "Error mounting pref path: %s\n", PHYSFS_getErrorByCode(PHYSFS_getLastErrorCode()));
		return EXIT_FAILURE;
	}
	SDL_free(prefPath);
	prefPath = NULL;
	#else
	if ( !PHYSFS_setWriteDir(basePath) )
	{
		fprintf(stderr, "Error setting pref path for writing: %s\n", PHYSFS_getErrorByCode(PHYSFS_getLastErrorCode()));
		return EXIT_FAILURE;
	}
	#endif

	SDL_free(basePath);
	basePath = NULL;
	#else
	const char *baseDir;
	if ( argc > 1 )
	{
		baseDir = argv[1];
	}
	else
	{
		baseDir = "./";
	}
	if ( !PHYSFS_mount(baseDir, NULL, 0) )
	{
		fprintf(stderr, "Error mounting base dir: %s\n", PHYSFS_getErrorByCode(PHYSFS_getLastErrorCode()));
		return EXIT_FAILURE;
	}
	if ( !PHYSFS_setWriteDir(baseDir) )
	{
		fprintf(stderr, "Error setting base dir for writing: %s\n", PHYSFS_getErrorByCode(PHYSFS_getLastErrorCode()));
		return EXIT_FAILURE;
	}
	baseDir = NULL;
	#endif

	if (
		!PHYSFS_mkdir("replay") ||
		!PHYSFS_mkdir("config/data") ||
		!PHYSFS_mkdir("config/mission") ||
		!PHYSFS_mkdir("config/stage")
	)
	{
		fprintf(stderr, "Error creating save data dirs: %s\n", PHYSFS_getErrorByCode(PHYSFS_getLastErrorCode()));
		return EXIT_FAILURE;
	}

	gameMain();

	/* 辞める */
	if ( !PHYSFS_deinit() )
	{
		fprintf(stderr, "Failed PhysicsFS deinit: %s\n", PHYSFS_getErrorByCode(PHYSFS_getLastErrorCode()));
	}
	Mix_CloseAudio();
	Mix_Quit();
	IMG_Quit();
	SDL_Quit();
	return EXIT_SUCCESS;
}
