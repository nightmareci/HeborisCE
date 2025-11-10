#include "APP_update.h"
#include "APP_main.h"
#include "APP_video.h"
#include "APP_input.h"
#include "APP_global.h"

static uint64_t			APP_LastNS;
static unsigned int		APP_FramesThisSecond;
static unsigned int		APP_RealFPS;
static unsigned int		APP_FPS = 1;
static unsigned int		APP_CursorFrames;

void APP_Start(void)
{
	APP_LastNS = SDL_GetTicksNS();
	APP_RealFPS = 0;

	SDL_srand(0);
}

bool APP_Update(void)
{
	if (!APP_RenderScreen()) {
		APP_Exit(__FUNCTION__, __LINE__, "Failed updating: %s", SDL_GetError());
	}

	// フレームレート計算
	// Frame rate calculation
	const uint64_t now = SDL_GetTicksNS();
	APP_FramesThisSecond++;
	if (now - APP_LastNS >= SDL_NS_PER_SECOND) {
		APP_RealFPS = APP_FramesThisSecond;
		APP_FramesThisSecond = 0;
		APP_LastNS = now;
	}

	// イベント処理
	// Process events
	SDL_PumpEvents();
	SDL_Event	ev;
	bool showCursor = false;
	#if defined(APP_ENABLE_JOYSTICK) || defined(APP_ENABLE_GAME_CONTROLLER)
	bool slotsChanged = false;
	#endif
	while (SDL_PeepEvents(&ev, 1, SDL_GETEVENT, 0, SDL_EVENT_LAST) == 1) {
		switch (ev.type) {
			// ウィンドウの×ボタンが押された時など
			// When the window's X-button was pressed, etc.
			case SDL_EVENT_QUIT:
				return false;

			case SDL_EVENT_WINDOW_RESIZED:
				APP_ScreenSubpixelOffset = APP_SCREEN_SUBPIXEL_OFFSET;
				break;

			#ifdef APP_ENABLE_JOYSTICK
			case SDL_EVENT_JOYSTICK_ADDED:
			case SDL_EVENT_JOYSTICK_REMOVED:
				slotsChanged = true;
			#endif

			#ifdef APP_ENABLE_GAME_CONTROLLER
			case SDL_EVENT_GAMEPAD_ADDED:
			case SDL_EVENT_GAMEPAD_REMOVED:
				slotsChanged = true;
				break;
			#endif

			case SDL_EVENT_MOUSE_MOTION:
			case SDL_EVENT_MOUSE_BUTTON_DOWN:
			case SDL_EVENT_MOUSE_BUTTON_UP:
			case SDL_EVENT_MOUSE_WHEEL:
				showCursor = true;
				break;

			default:
				break;
		}
	}

	if (showCursor) {
		if (!SDL_ShowCursor()) {
			APP_Exit(__FUNCTION__, __LINE__, "Failed showing mouse cursor: %s", SDL_GetError());
		}
		APP_CursorFrames = 0u;
	}

	if (SDL_CursorVisible() && APP_CursorFrames++ >= APP_FPS) {
		if (!SDL_HideCursor()) {
			APP_Exit(__FUNCTION__, __LINE__, "Failed hiding mouse cursor: %s", SDL_GetError());
		}
	}

	#if defined(APP_ENABLE_JOYSTICK) || defined(APP_ENABLE_GAME_CONTROLLER)
	if (slotsChanged) {
		if (!APP_PlayerSlotsChanged()) {
			APP_Exit(__FUNCTION__, __LINE__, "Failed changing player joystick/controller device slots: %s", SDL_GetError());
		}
	}
	#endif

	return true;
}

void APP_SetFPS(unsigned fps)
{
	if (fps == 0) {
		APP_FPS = 1u;
	}
	else {
		APP_FPS = fps;
	}
	APP_ResetFrameStep();
}

int APP_GetFPS(void)
{
	return APP_FPS;
}

int APP_GetRealFPS(void)
{
	return APP_RealFPS;
}
