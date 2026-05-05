#include <MAIN.h>
#include <VIDEO.h>
#include <AUDIO.h>
#include <FILESYSTEM.h>
#include <INPUT.h>
#include <ERROR.h>
#include "GLOBAL.h"
#include <game.h>
#define SDL_MAIN_USE_CALLBACKS 1
#include <SDL3/SDL_main.h>
#include <setjmp.h>

static int MAIN_argc;
static char** MAIN_argv;
static bool MAIN_WasSetResourceSettings = false;
static const char* const* MAIN_WriteDirectories = NULL;
static size_t MAIN_WriteDirectoryCount = 0;
static SDL_IOStream* MAIN_LogFile = NULL;
static int MAIN_WaveCount = 0;
static int MAIN_PlaneCount = 0;
static int MAIN_TextLayerCount = 0;
static uint64_t MAIN_LastRealFPSNS;
static unsigned int MAIN_FramesThisSecond;
static unsigned int MAIN_RealFPS;
static unsigned int MAIN_SettingFPS = 1;
static unsigned int MAIN_CursorFrames = 0;
static bool MAIN_RenderWhileSkippingFrames;
static bool MAIN_LastFrameSkipped;
static uint64_t MAIN_NowNS;
static int64_t MAIN_FrameNS;
static int64_t MAIN_AccumulatedNS = 0;
#if defined(INPUT_ENABLE_JOYSTICK) || defined(INPUT_ENABLE_GAME_CONTROLLER)
static bool MAIN_UpdatePlayerSlotsNow = false;
#endif
static bool MAIN_ShowCursorNow = false;
static int MAIN_QuitLevel;
static bool MAIN_QuitNow = false;
static jmp_buf MAIN_QuitPoint;

static bool MAIN_FrameStep(void);

SDL_AppResult SDLCALL SDL_AppInit(void** appstate, int argc, char** argv)
{
	(void)appstate;
	MAIN_argc = argc;
	MAIN_argv = argv;

	MAIN_QuitLevel = 0;

	if (!SDL_Init(
		SDL_INIT_AUDIO | SDL_INIT_VIDEO
#ifdef INPUT_ENABLE_JOYSTICK
		| SDL_INIT_JOYSTICK
#endif
#ifdef INPUT_ENABLE_GAME_CONTROLLER
		| SDL_INIT_GAMEPAD
#endif
	)) {
		ERROR_Set("Couldn't initialize SDL: %s", SDL_GetError());
		return SDL_APP_FAILURE;
	}

	return SDL_APP_CONTINUE;
}

SDL_AppResult SDLCALL SDL_AppEvent(void* appstate, SDL_Event* event)
{
	(void)appstate;
	switch (event->type) {
	case SDL_EVENT_QUIT:
		MAIN_QuitNow = true;
		break;

#ifdef INPUT_ENABLE_JOYSTICK
	case SDL_EVENT_JOYSTICK_ADDED:
	case SDL_EVENT_JOYSTICK_REMOVED:
		MAIN_UpdatePlayerSlotsNow = true;
		break;
#endif

#ifdef INPUT_ENABLE_GAME_CONTROLLER
	case SDL_EVENT_GAMEPAD_ADDED:
	case SDL_EVENT_GAMEPAD_REMOVED:
		MAIN_UpdatePlayerSlotsNow = true;
		break;
#endif

	case SDL_EVENT_MOUSE_MOTION:
	case SDL_EVENT_MOUSE_BUTTON_DOWN:
	case SDL_EVENT_MOUSE_BUTTON_UP:
	case SDL_EVENT_MOUSE_WHEEL:
		MAIN_ShowCursorNow = true;
		break;

	default:
		break;
	}

	return SDL_APP_CONTINUE;
}

SDL_AppResult SDLCALL SDL_AppIterate(void* appstate)
{
	(void)appstate;
	switch (setjmp(MAIN_QuitPoint)) {
	case SDL_APP_CONTINUE:
	default:
		break;

	case SDL_APP_SUCCESS:
		return SDL_APP_SUCCESS;

	case SDL_APP_FAILURE:
		return SDL_APP_FAILURE;
	}

	if (MAIN_ShowCursorNow) {
		if (!SDL_ShowCursor()) {
			ERROR_Set("Failed showing mouse cursor: %s", SDL_GetError());
			return SDL_APP_FAILURE;
		}
		MAIN_ShowCursorNow = false;
		MAIN_CursorFrames = MAIN_SettingFPS;
	}

	if (MAIN_CursorFrames > 0 && --MAIN_CursorFrames == 0 && SDL_CursorVisible() && !SDL_HideCursor()) {
		ERROR_Set("Failed hiding mouse cursor: %s", SDL_GetError());
		return SDL_APP_FAILURE;
	}

#if defined(INPUT_ENABLE_JOYSTICK) || defined(INPUT_ENABLE_GAME_CONTROLLER)
	if (MAIN_UpdatePlayerSlotsNow) {
		if (!INPUT_UpdatePlayerSlots()) {
			ERROR_Set("Failed changing player joystick/controller device slots: %s", SDL_GetError());
			return SDL_APP_FAILURE;
		}
		MAIN_UpdatePlayerSlotsNow = false;
	}
#endif

	GLOBAL_ScreenSubpixelOffset = VIDEO_GetScreenSubpixelOffset();

	gameUpdate();

	return SDL_APP_CONTINUE;
}

void SDLCALL SDL_AppQuit(void* appstate, SDL_AppResult result)
{
	(void)appstate;
	if (result == SDL_APP_FAILURE) {
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, PROJECT_TITLE " Error", SDL_GetError(), GLOBAL_ScreenWindow);
	}
	MAIN_Quit();
}

void MAIN_ResetFrameStep(void)
{
	MAIN_FrameNS = SDL_NS_PER_SECOND / MAIN_GetFPS();
	MAIN_AccumulatedNS = 0;
	MAIN_NowNS = SDL_GetTicksNS();
}

static bool MAIN_FrameStep(void)
{
	const int64_t initialNS = SDL_GetTicksNS() - MAIN_NowNS;
	uint64_t now;
	if (MAIN_AccumulatedNS >= MAIN_FrameNS + 100 * SDL_NS_PER_MS) {
		MAIN_NowNS = SDL_GetTicksNS();
		MAIN_AccumulatedNS = 0;
		return false;
	}
	bool skipped = MAIN_AccumulatedNS >= MAIN_FrameNS;
	if (skipped) {
		now = SDL_GetTicksNS();
		MAIN_AccumulatedNS -= MAIN_FrameNS;
	}
	else {
		if (MAIN_FrameNS - initialNS - MAIN_AccumulatedNS > 0) {
			SDL_DelayPrecise(MAIN_FrameNS - initialNS - MAIN_AccumulatedNS);
		}
		now = SDL_GetTicksNS();
		MAIN_AccumulatedNS += now - MAIN_NowNS - MAIN_FrameNS;
	}
	MAIN_NowNS = now;
	return skipped;
}

static void SDLCALL MAIN_LogOutput(void* userdata, int category, SDL_LogPriority priority, const char* message)
{
	const char* priorityString;
	switch (priority) {
	default:
		priorityString = "INVALID";
		break;

	case SDL_LOG_PRIORITY_TRACE:
		priorityString = "TRACE";
		break;

    	case SDL_LOG_PRIORITY_VERBOSE:
     		priorityString = "VERBOSE";
       		break;

	case SDL_LOG_PRIORITY_DEBUG:
		priorityString = "DEBUG";
	 	break;

	case SDL_LOG_PRIORITY_INFO:
		priorityString = "INFO";
		break;

	case SDL_LOG_PRIORITY_WARN:
		priorityString = "WARN";
		break;

	case SDL_LOG_PRIORITY_ERROR:
		priorityString = "ERROR";
		break;

	case SDL_LOG_PRIORITY_CRITICAL:
		priorityString = "CRITICAL";
		break;
	}

	SDL_IOprintf(MAIN_LogFile, "%s: %s\n", priorityString, message);
}

void MAIN_SetResourceSettings(int waveCount, const char* const* writeDirectories, size_t writeDirectoryCount, int planeCount, int textLayerCount)
{
	if (MAIN_WasSetResourceSettings) {
		ERROR_Set("Resource settings already set, they can only be set once");
		MAIN_Exit(SDL_APP_FAILURE);
	}

	MAIN_WaveCount = waveCount;
	MAIN_WriteDirectories = writeDirectories;
	MAIN_WriteDirectoryCount = writeDirectoryCount;
	MAIN_PlaneCount = planeCount;
	MAIN_TextLayerCount = textLayerCount;

	MAIN_WasSetResourceSettings = true;
}

void MAIN_WaitToFinishLoading(void)
{
	if (!WORKER_WaitForJobCompletion(GLOBAL_LoadingWorker, 0)) {
		ERROR_Set("%s", SDL_GetError());
		MAIN_Exit(SDL_APP_FAILURE);
	}
	VIDEO_HavePlane(-1);
}

void MAIN_Init(void)
{
	if (!MAIN_WasSetResourceSettings) {
		ERROR_Set("MAIN_SetResourceSettings() must be called only once for the app's whole lifetime before calling MAIN_Init()");
		MAIN_Exit(SDL_APP_FAILURE);
	}

	if (MAIN_QuitLevel == 0) {
		SDL_SetCurrentThreadPriority(SDL_THREAD_PRIORITY_HIGH);

		if (!FILESYSTEM_Init(MAIN_argc, MAIN_argv)) {
			ERROR_Set("%s", SDL_GetError());
			MAIN_Exit(SDL_APP_FAILURE);
		}
		MAIN_QuitLevel++;

		MAIN_LogFile = FILESYSTEM_OpenWrite("log.txt");
		if (!MAIN_LogFile) {
			ERROR_Set("Failed opening log file");
			MAIN_Exit(SDL_APP_FAILURE);
		}
		MAIN_QuitLevel++;
		SDL_SetLogOutputFunction(MAIN_LogOutput, NULL);

		if (MAIN_WriteDirectories) {
			for (size_t i = 0u; i < MAIN_WriteDirectoryCount; i++) {
				if (!FILESYSTEM_CreateDirectory(MAIN_WriteDirectories[i])) {
					ERROR_Set("%s", SDL_GetError());
					MAIN_Exit(SDL_APP_FAILURE);
				}
			}
		}

		if (!AUDIO_Init(MAIN_WaveCount)) {
			ERROR_Set("%s", SDL_GetError());
			MAIN_Exit(SDL_APP_FAILURE);
		}
		MAIN_QuitLevel++;

		VIDEO_Init(MAIN_PlaneCount, MAIN_TextLayerCount);
		MAIN_QuitLevel++;

		INPUT_Open();
		MAIN_QuitLevel++;

		GLOBAL_LoadingWorker = WORKER_Create();
		if (!GLOBAL_LoadingWorker) {
			ERROR_Set("%s", SDL_GetError);
			MAIN_Exit(SDL_APP_FAILURE);
		}
		MAIN_QuitLevel++;
	}

	VIDEO_SetPlaneDrawOffset(0, 0);
	MAIN_RealFPS = 0;
	MAIN_RenderWhileSkippingFrames = false;
	MAIN_LastFrameSkipped = false;
	SDL_srand(0);
	MAIN_SetFPS(60);
}

void MAIN_Quit(void)
{
	switch (MAIN_QuitLevel) {
	case 6: WORKER_Destroy(GLOBAL_LoadingWorker); GLOBAL_LoadingWorker = NULL; SDL_FALLTHROUGH;
	case 5: INPUT_Close(); SDL_FALLTHROUGH;
	case 4: VIDEO_Quit(); SDL_FALLTHROUGH;
	case 3: AUDIO_Quit(); SDL_FALLTHROUGH;
	case 2: SDL_CloseIO(MAIN_LogFile); MAIN_LogFile = NULL; SDL_FALLTHROUGH;
	case 1: FILESYSTEM_Quit(); SDL_FALLTHROUGH;
	default: break;
	}
	MAIN_QuitLevel = 0;
}

bool MAIN_Update(void)
{
	if (!GLOBAL_ScreenRenderer) {
		ERROR_Set("Renderer is not initialized");
		MAIN_Exit(SDL_APP_FAILURE);
	}

	#ifdef NDEBUG
	if (MAIN_RenderWhileSkippingFrames || !MAIN_LastFrameSkipped) {
	#endif
		if (GLOBAL_ScreenRenderTarget) {
			if (
				!SDL_SetRenderTarget(GLOBAL_ScreenRenderer, NULL) ||
				!SDL_RenderClear(GLOBAL_ScreenRenderer) ||
				!SDL_RenderTexture(GLOBAL_ScreenRenderer, GLOBAL_ScreenRenderTarget, NULL, NULL) ||
				!SDL_RenderPresent(GLOBAL_ScreenRenderer) ||
				!SDL_SetRenderTarget(GLOBAL_ScreenRenderer, GLOBAL_ScreenRenderTarget)
			) {
				ERROR_Set("Failed render present with render target: %s", SDL_GetError());
				MAIN_Exit(SDL_APP_FAILURE);
			}
		}
		else if (!SDL_RenderPresent(GLOBAL_ScreenRenderer)) {
			ERROR_Set("Failed render present with no render target: %s", SDL_GetError());
			MAIN_Exit(SDL_APP_FAILURE);
		}

		MAIN_LastFrameSkipped = MAIN_FrameStep();

		if (!SDL_RenderClear(GLOBAL_ScreenRenderer)) {
			ERROR_Set("Failed render clear: %s", SDL_GetError());
			MAIN_Exit(SDL_APP_FAILURE);
		}
	#ifdef NDEBUG
	}
	else {
		if (!SDL_FlushRenderer(GLOBAL_ScreenRenderer)) {
			ERROR_Set("Failed flushing renderer: %s", SDL_GetError());
			MAIN_Exit(SDL_APP_FAILURE);
		}

		MAIN_LastFrameSkipped = MAIN_FrameStep();
	}
	#endif

	MAIN_FramesThisSecond++;
	if (MAIN_NowNS - MAIN_LastRealFPSNS >= SDL_NS_PER_SECOND) {
		MAIN_RealFPS = MAIN_FramesThisSecond;
		MAIN_FramesThisSecond = 0;
		MAIN_LastRealFPSNS = MAIN_NowNS;
	}

	return !MAIN_QuitNow;
}

SDL_NORETURN void MAIN_Exit(SDL_AppResult result)
{
	longjmp(MAIN_QuitPoint, result);
}

void MAIN_SetFPS(unsigned fps)
{
	if (fps == 0) {
		MAIN_SettingFPS = 1;
	}
	else {
		MAIN_SettingFPS = fps;
	}
	MAIN_ResetFrameStep();
}

int MAIN_GetFPS(void)
{
	return MAIN_SettingFPS;
}

int MAIN_GetRealFPS(void)
{
	return MAIN_RealFPS;
}

void MAIN_SetRenderWhileSkippingFrames(bool render)
{
	MAIN_RenderWhileSkippingFrames = render;
}

bool MAIN_RenderThisFrame(void)
{
	return MAIN_RenderWhileSkippingFrames || !MAIN_LastFrameSkipped;
}
