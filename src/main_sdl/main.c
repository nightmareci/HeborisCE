#include "main_sdl/include.h"
#include "script/config.h"
#include "gamestart.h"
#include "physfs.h"
#ifdef ENABLE_GAME_CONTROLLER
#include "main_sdl/gamecontroller.h"
#endif
#include <assert.h>

static int quitLevel = 0;
static int quit(int status) {
	switch ( quitLevel )
	{
	case 5: if ( !PHYSFS_deinit() ) fprintf(stderr, "Failed closing access to files: %s\n", PHYSFS_getErrorByCode(PHYSFS_getLastErrorCode()));
	case 4: Mix_CloseAudio();
	case 3: Mix_Quit();
	case 2: IMG_Quit();
	case 1: SDL_Quit();
	default: break;
	}
	return status;
}

int main(int argc, char* argv[])
{
	/* SDLの初期化 */
	if ( SDL_Init(
		SDL_INIT_AUDIO |
		SDL_INIT_VIDEO |
		SDL_INIT_JOYSTICK |
		SDL_INIT_GAMECONTROLLER
	) < 0 )
	{
		fprintf(stderr, "Couldn't initialize SDL: %s\n", SDL_GetError());
		return quit(EXIT_FAILURE);
	}

	/* If this fails, it doesn't matter, the game will still work. But it's called because if it works, the game might perform better. */
	SDL_SetThreadPriority(SDL_THREAD_PRIORITY_HIGH);

	quitLevel++;
	SDL_SetHint(SDL_HINT_RENDER_BATCHING, "1");

	/* 画像の初期化 */
	if ( IMG_Init(IMG_INIT_PNG) != IMG_INIT_PNG )
	{
		fprintf(stderr, "Couldn't initialize image support: %s\n", IMG_GetError());
		return quit(EXIT_FAILURE);
	}
	quitLevel++;

	/* サウンドの初期化 */
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
		fprintf(stderr, "Couldn't initialize audio mixing: %s\n", Mix_GetError());
		return quit(EXIT_FAILURE);
	}
	quitLevel++;

	wavebgm_supported[0] = !!(formatsInitialized & MIX_INIT_MID);
	wavebgm_supported[1] = !!(formatsInitialized & MIX_INIT_MID);
	wavebgm_supported[2] = 1; // WAVEはいつでも利用可能
	wavebgm_supported[3] = !!(formatsInitialized & MIX_INIT_OGG);
	wavebgm_supported[4] = !!(formatsInitialized & MIX_INIT_MP3);
	wavebgm_supported[5] = !!(formatsInitialized & MIX_INIT_FLAC);
	wavebgm_supported[6] = !!(formatsInitialized & MIX_INIT_OPUS);
	wavebgm_supported[7] = !!(formatsInitialized & MIX_INIT_MOD);
	wavebgm_supported[8] = !!(formatsInitialized & MIX_INIT_MOD);
	wavebgm_supported[9] = !!(formatsInitialized & MIX_INIT_MOD);
	wavebgm_supported[10] = !!(formatsInitialized & MIX_INIT_MOD);

	if ( Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024) < 0 )
	{
		fprintf(stderr, "Couldn't open audio: %s\n", Mix_GetError());
		return quit(EXIT_FAILURE);
	}
	quitLevel++;

	Mix_AllocateChannels(100);

	if ( !PHYSFS_init(argv[0]) )
	{
		fprintf(stderr, "Couldn't initialize file access: %s\n", PHYSFS_getErrorByCode(PHYSFS_getLastErrorCode()));
		return quit(EXIT_FAILURE);
	}
	quitLevel++;

	if ( argc > 1 && strlen(argv[1]) > 0 )
	{
		char *specifiedPath = argv[1];
		if ( !PHYSFS_mount(specifiedPath, NULL, 0) )
		{
			fprintf(stderr, "Error mounting specified path \"%s\": %s\n", specifiedPath, PHYSFS_getErrorByCode(PHYSFS_getLastErrorCode()));
			return quit(EXIT_FAILURE);
		}
		if ( !PHYSFS_setWriteDir(specifiedPath) )
		{
			fprintf(stderr, "Error setting specified path \"%s\" for writing: %s\n", specifiedPath, PHYSFS_getErrorByCode(PHYSFS_getLastErrorCode()));
			return quit(EXIT_FAILURE);
		}
	}
#if FILESYSTEM_TYPE == FILESYSTEM_WORKINGDIR
	else
	{
		if ( !PHYSFS_mount("./", NULL, 0) )
		{
			fprintf(stderr, "Error mounting working directory: %s\n", PHYSFS_getErrorByCode(PHYSFS_getLastErrorCode()));
			return quit(EXIT_FAILURE);
		}
		if ( !PHYSFS_setWriteDir("./") )
		{
			fprintf(stderr, "Error setting working directory for writing: %s\n", PHYSFS_getErrorByCode(PHYSFS_getLastErrorCode()));
			return quit(EXIT_FAILURE);
		}
	}
#elif FILESYSTEM_TYPE == FILESYSTEM_PORTABLE
	else
	{
		char *basePath = SDL_GetBasePath();
		if ( !basePath )
		{
			fprintf(stderr, "Failed getting base path: %s\n", SDL_GetError());
			return quit(EXIT_FAILURE);
		}
		if ( !PHYSFS_mount(basePath, NULL, 0) )
		{
			fprintf(stderr, "Error mounting base path \"%s\": %s\n", basePath, PHYSFS_getErrorByCode(PHYSFS_getLastErrorCode()));
			free(basePath);
			return quit(EXIT_FAILURE);
		}
		if ( !PHYSFS_setWriteDir(basePath) )
		{
			fprintf(stderr, "Error setting working directory for writing: %s\n", PHYSFS_getErrorByCode(PHYSFS_getLastErrorCode()));
			SDL_free(basePath);
			return quit(EXIT_FAILURE);
		}
		SDL_free(basePath);
	}
#elif FILESYSTEM_TYPE == FILESYSTEM_INSTALLABLE
	else
	{
		char *basePath = SDL_GetBasePath();
		if ( !basePath )
		{
			fprintf(stderr, "Failed getting base path: %s\n", SDL_GetError());
			return quit(EXIT_FAILURE);
		}
		char* fullBasePath;
		if ( !(fullBasePath = malloc(strlen(basePath) + strlen(BASE_PATH_APPEND) + 1)) )
		{
			fprintf(stderr, "Failed creating full base path.\n");
			SDL_free(basePath);
			return quit(EXIT_FAILURE);
		}
		sprintf(fullBasePath, "%s%s", basePath, BASE_PATH_APPEND);
		SDL_free(basePath);
		if ( !PHYSFS_mount(fullBasePath, NULL, 0) )
		{
			fprintf(stderr, "Error mounting full base path \"%s\": %s\n", fullBasePath, PHYSFS_getErrorByCode(PHYSFS_getLastErrorCode()));
			free(fullBasePath);
			return quit(EXIT_FAILURE);
		}
		free(fullBasePath);

		char *prefPath = SDL_GetPrefPath(PROJECT_ORG, PROJECT_APP);
		if ( !prefPath )
		{
			fprintf(stderr, "Failed getting pref path: %s\n", SDL_GetError());
			return quit(EXIT_FAILURE);
		}
		if ( !PHYSFS_setWriteDir(prefPath) )
		{
			fprintf(stderr, "Error setting pref path \"%s\" for writing: %s\n", prefPath, PHYSFS_getErrorByCode(PHYSFS_getLastErrorCode()));
			SDL_free(prefPath);
			return quit(EXIT_FAILURE);
		}
		if ( !PHYSFS_mount(prefPath, NULL, 0) )
		{
			fprintf(stderr, "Error mounting pref path \"%s\": %s\n", prefPath, PHYSFS_getErrorByCode(PHYSFS_getLastErrorCode()));
			SDL_free(prefPath);
			return quit(EXIT_FAILURE);
		}
		SDL_free(prefPath);
	}
#elif FILESYSTEM_TYPE == FILESYSTEM_PHYSFS
	else if ( !PHYSFS_setSaneConfig(PROJECT_ORG, PROJECT_APP, "ZIP", 0, 0) )
	{
		fprintf(stderr, "Error setting sane PhysicsFS config: %s\n", PHYSFS_getErrorByCode(PHYSFS_getLastErrorCode()));
		return quit(EXIT_FAILURE);
	}
#else
	#error FILESYSTEM_TYPE must be defined as FILESYSTEM_WORKINGDIR, FILESYSTEM_PORTABLE, FILESYSTEM_INSTALLABLE, or FILESYSTEM_PHYSFS.
#endif

	if (
		!PHYSFS_mkdir("replay") ||
		!PHYSFS_mkdir("config/data") ||
		!PHYSFS_mkdir("config/mission") ||
		!PHYSFS_mkdir("config/stage")
	)
	{
		fprintf(stderr, "Error creating save data directories: %s\n", PHYSFS_getErrorByCode(PHYSFS_getLastErrorCode()));
		return quit(EXIT_FAILURE);
	}

#ifdef ENABLE_GAME_CONTROLLER
	/* Must be called after PhysicsFS setup, as it reads gamecontrollerdb.txt in the res directory on some platforms. */
	OpenGameControllers();
#endif

	gameMain();

	/* 辞める */
	return quit(EXIT_SUCCESS);
}
