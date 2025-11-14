#include "APP_main.h"
#include "APP_video.h"
#include "APP_audio.h"
#include "APP_filesystem.h"
#include "APP_input.h"
#include "APP_error.h"
#include "APP_global.h"
#include "game/gamestart.h"
#define SDL_MAIN_USE_CALLBACKS 1
#include <SDL3/SDL_main.h>
#include <stdarg.h>

static int APP_argc;
static char** APP_argv;
static uint64_t APP_LastRealFPSNS;
static unsigned int APP_FramesThisSecond;
static unsigned int APP_RealFPS;
static unsigned int APP_SettingFPS = 1;
static unsigned int APP_CursorFrames = 0;
static bool APP_RenderWhileSkippingFrames;
static bool APP_LastFrameSkipped;
static uint64_t APP_NowNS;
static int64_t APP_FrameNS;
static int64_t APP_AccumulatedNS = 0;
#if defined(APP_ENABLE_JOYSTICK) || defined(APP_ENABLE_GAME_CONTROLLER)
static bool APP_UpdatePlayerSlotsNow = false;
#endif
static bool APP_ShowCursorNow = false;
static int APP_QuitLevel;
static bool APP_QuitNow = false;

void APP_ResetFrameStep(void)
{
	APP_FrameNS = SDL_NS_PER_SECOND / APP_GetFPS();
	APP_AccumulatedNS = 0;
	APP_NowNS = SDL_GetTicksNS();
}

static bool APP_FrameStep(void)
{
	const uint64_t initialNS = SDL_GetTicksNS() - APP_NowNS;
	uint64_t now;
	if (APP_AccumulatedNS >= APP_FrameNS + 100 * SDL_NS_PER_MS) {
		APP_AccumulatedNS = 0;
		APP_NowNS = SDL_GetTicksNS();
	}
	const bool skipped = APP_AccumulatedNS + initialNS >= APP_FrameNS;
	if (skipped) {
		now = SDL_GetTicksNS();
		APP_AccumulatedNS -= APP_FrameNS - initialNS;
	}
	else {
		SDL_DelayPrecise(APP_FrameNS - initialNS - APP_AccumulatedNS);
		now = SDL_GetTicksNS();
		APP_AccumulatedNS += now - APP_NowNS - APP_FrameNS;
	}
	APP_NowNS = now;
	return skipped;
}

void APP_Init(size_t wavesCount, const char* const* writeDirectories, size_t writeDirectoriesCount)
{
	if (APP_QuitLevel == 0) {
		// If this fails, it doesn't matter, the game will still work. But it's
		// called because if it works, the game might perform better.
		SDL_SetCurrentThreadPriority(SDL_THREAD_PRIORITY_HIGH);

		if (!APP_InitFilesystem(APP_argc, APP_argv)) {
			APP_SetError("%s", SDL_GetError());
			APP_Exit(EXIT_FAILURE);
		}
		APP_QuitLevel++;

		for (size_t i = 0u; i < writeDirectoriesCount; i++) {
			if (!APP_CreateDirectory(writeDirectories[i])) {
				APP_SetError("%s", SDL_GetError());
				APP_Exit(EXIT_FAILURE);
			}
		}

		if (!APP_InitAudio(wavesCount)) {
			APP_SetError("%s", SDL_GetError());
			APP_Exit(EXIT_FAILURE);
		}
		APP_QuitLevel++;

		APP_InitVideo();
		APP_QuitLevel++;

		APP_OpenInputs();
		APP_QuitLevel++;
	}

	APP_SetPlaneDrawOffset(0, 0);
	APP_RealFPS = 0;
	APP_RenderWhileSkippingFrames = false;
	APP_LastFrameSkipped = false;
	SDL_srand(0);
	APP_SetFPS(60);
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

bool APP_Update(void)
{
	if (!APP_ScreenRenderer) {
		APP_SetError("Renderer is not initialized");
		APP_Exit(EXIT_FAILURE);
	}
	if (!SDL_FlushRenderer(APP_ScreenRenderer)) {
		APP_SetError("Failed flushing renderer: %s", SDL_GetError());
		APP_Exit(EXIT_FAILURE);
	}

	#ifdef NDEBUG
	if (APP_RenderWhileSkippingFrames || !APP_LastFrameSkipped)
	#endif
	{
		/* バックサーフェスをフロントに転送 */
		if (APP_ScreenRenderTarget) {
			if (
				!SDL_SetRenderTarget(APP_ScreenRenderer, NULL) ||
				!SDL_RenderClear(APP_ScreenRenderer) ||
				!SDL_RenderTexture(APP_ScreenRenderer, APP_ScreenRenderTarget, NULL, NULL) ||
				!SDL_RenderPresent(APP_ScreenRenderer) ||
				!SDL_SetRenderTarget(APP_ScreenRenderer, APP_ScreenRenderTarget)
			) {
				APP_SetError("Failed render present with render target: %s", SDL_GetError());
				APP_Exit(EXIT_FAILURE);
			}
		}
		else if (!SDL_RenderPresent(APP_ScreenRenderer)) {
			APP_SetError("Failed render present with no render target: %s", SDL_GetError());
			APP_Exit(EXIT_FAILURE);
		}

		/* フレームレート待ち || Frame rate waiting */
		APP_LastFrameSkipped = APP_FrameStep();

		/* 画面塗りつぶし || Fill screen */
		if (!SDL_RenderClear(APP_ScreenRenderer)) {
			APP_SetError("Failed render clear: %s", SDL_GetError());
			APP_Exit(EXIT_FAILURE);
		}
	}

	// フレームレート計算
	// Frame rate calculation
	APP_FramesThisSecond++;
	if (APP_NowNS - APP_LastRealFPSNS >= SDL_NS_PER_SECOND) {
		APP_RealFPS = APP_FramesThisSecond;
		APP_FramesThisSecond = 0;
		APP_LastRealFPSNS = APP_NowNS;
	}

	return !APP_QuitNow;
}

SDL_NORETURN void APP_Exit(int exitStatus)
{
	if (exitStatus != EXIT_SUCCESS) {
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, APP_PROJECT_NAME " Error", SDL_GetError(), APP_ScreenWindow);
	}
	APP_Quit();
	SDL_Quit();
	exit(exitStatus);
}

void APP_SetFPS(unsigned fps)
{
	if (fps == 0) {
		APP_SettingFPS = 1;
	}
	else {
		APP_SettingFPS = fps;
	}
	APP_ResetFrameStep();
}

int APP_GetFPS(void)
{
	return APP_SettingFPS;
}

int APP_GetRealFPS(void)
{
	return APP_RealFPS;
}

void APP_SetRenderWhileSkippingFrames(bool draw)
{
	APP_RenderWhileSkippingFrames = draw;
}

SDL_AppResult SDLCALL SDL_AppInit(void** appstate, int argc, char** argv)
{
	APP_argc = argc;
	APP_argv = argv;

	APP_QuitLevel = 0;

	// TODO: Remove this once the issue with WASAPI crackling with the move sound in-game is fixed
	#ifdef SDL_PLATFORM_WIN32
	if (!SDL_SetHint(SDL_HINT_AUDIO_DRIVER, "directsound")) {
		APP_SetError("Couldn't set audio driver to DirectSound: %s", SDL_GetError());
		return SDL_APP_FAILURE;
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
	)) {
		APP_SetError("Couldn't initialize SDL: %s", SDL_GetError());
		return SDL_APP_FAILURE;
	}

	return SDL_APP_CONTINUE;
}

SDL_AppResult SDLCALL SDL_AppEvent(void* appstate, SDL_Event* event)
{
	switch (event->type) {
		// ウィンドウの×ボタンが押された時など
		// When the window's X-button was pressed, etc.
		case SDL_EVENT_QUIT:
			APP_QuitNow = true;
			break;

		case SDL_EVENT_WINDOW_RESIZED:
			APP_ScreenSubpixelOffset = APP_SCREEN_SUBPIXEL_OFFSET;
			break;

		#ifdef APP_ENABLE_JOYSTICK
		case SDL_EVENT_JOYSTICK_ADDED:
		case SDL_EVENT_JOYSTICK_REMOVED:
			APP_UpdatePlayerSlotsNow = true;
			break;
		#endif

		#ifdef APP_ENABLE_GAME_CONTROLLER
		case SDL_EVENT_GAMEPAD_ADDED:
		case SDL_EVENT_GAMEPAD_REMOVED:
			APP_UpdatePlayerSlotsNow = true;
			break;
		#endif

		case SDL_EVENT_MOUSE_MOTION:
		case SDL_EVENT_MOUSE_BUTTON_DOWN:
		case SDL_EVENT_MOUSE_BUTTON_UP:
		case SDL_EVENT_MOUSE_WHEEL:
			APP_ShowCursorNow = true;
			break;

		default:
			break;
	}

	return SDL_APP_CONTINUE;
}

SDL_AppResult SDLCALL SDL_AppIterate(void* appstate)
{
	if (APP_ShowCursorNow) {
		if (!SDL_ShowCursor()) {
			APP_SetError("Failed showing mouse cursor: %s", SDL_GetError());
			return SDL_APP_FAILURE;
		}
		APP_ShowCursorNow = false;
		APP_CursorFrames = APP_SettingFPS;
	}

	if (APP_CursorFrames > 0 && --APP_CursorFrames == 0 && SDL_CursorVisible() && !SDL_HideCursor()) {
		APP_SetError("Failed hiding mouse cursor: %s", SDL_GetError());
		return SDL_APP_FAILURE;
	}

	#if defined(APP_ENABLE_JOYSTICK) || defined(APP_ENABLE_GAME_CONTROLLER)
	if (APP_UpdatePlayerSlotsNow) {
		if (!APP_UpdatePlayerSlots()) {
			APP_SetError("Failed changing player joystick/controller device slots: %s", SDL_GetError());
			return SDL_APP_FAILURE;
		}
	}
	#endif

	mainUpdate();

	return SDL_APP_CONTINUE;
}

void SDLCALL SDL_AppQuit(void* appstate, SDL_AppResult result)
{
	if (result != SDL_APP_SUCCESS) {
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, APP_PROJECT_NAME " Error", SDL_GetError(), APP_ScreenWindow);
	}
	APP_Quit();
	SDL_Quit();
}
