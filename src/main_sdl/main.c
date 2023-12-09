#include "main_sdl/include.h"
#include "script/config.h"
#include "gamestart.h"
#include "filesystem.h"
#ifdef ENABLE_GAME_CONTROLLER
#include "main_sdl/gamecontroller.h"
#endif

// TODO: Move as much as possible of the Emscripten code into its own
// source(s). The __EMSCRIPTEN__ checks really ugly up the code.

#ifdef __EMSCRIPTEN__
void startup() {
	emscripten_set_main_loop(mainUpdate, 0, true);
}
#endif

int main(int argc, char** argv)
{
	if (!FSInit(argc, argv)) {
		return EXIT_FAILURE;
	}

#ifdef ENABLE_GAME_CONTROLLER
	// Must be called after FSInit, as it reads gamecontrollerdb.txt in the
	// res directory on some platforms.
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
			FSDeInit();
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
