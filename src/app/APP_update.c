#include "APP_update.h"
#include "APP_video.h"
#include "APP_input.h"
#include "APP_main.h"
#include "APP_global.h"
#include "nanotime.h"

static uint64_t			APP_FPSCount;
static unsigned int		APP_FPSCounting;
static unsigned int		APP_FPS;
static unsigned int		APP_NowFPS;
static unsigned int		APP_CursorCounting;

void APP_Start(void)
{
	APP_FPSCount = 0;
	APP_FPS = 0;

	SDL_srand(SDL_GetTicksNS());
}

bool APP_Update(void)
{
	if (!APP_RenderScreen()) {
		return false;
	}

	// フレームレート計算
	// Frame rate calculation
	APP_FPSCounting++;
	if ( nanotime_interval(APP_FPSCount, nanotime_now(), APP_StepData.now_max) >= NANOTIME_NSEC_PER_SEC )
	{
		APP_FPS = APP_FPSCounting;
		APP_FPSCounting = 0;
		APP_FPSCount = nanotime_now();
	}

	// イベント処理
	// Process events
	SDL_PumpEvents();
	SDL_Event	ev;
	bool showCursor = false;
	#if defined(APP_ENABLE_JOYSTICK) || defined(APP_ENABLE_GAME_CONTROLLER)
	bool slotsChanged = false;
	#endif
	while (SDL_PeepEvents(&ev, 1, SDL_GETEVENT, 0, SDL_EVENT_LAST) == 1)
	{
		switch(ev.type){
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

	if (showCursor)
	{
		if (!SDL_ShowCursor()) {
			return false;
		}
		APP_CursorCounting = 0u;
	}

	if (SDL_CursorVisible() && APP_CursorCounting++ >= APP_NowFPS)
	{
		if (!SDL_HideCursor()) {
			return false;
		}
	}

	#if defined(APP_ENABLE_JOYSTICK) || defined(APP_ENABLE_GAME_CONTROLLER)
	if (slotsChanged) {
		if (!APP_PlayerSlotsChanged()) {
			return false;
		}
	}
	#endif

	return true;
}

void APP_ResetFrameStep(void)
{
	nanotime_step_init(&APP_StepData, NANOTIME_NSEC_PER_SEC / APP_NowFPS, nanotime_now_max(), nanotime_now, nanotime_sleep);
}

void APP_SetFPS(unsigned fps)
{
	if (fps == 0) {
		APP_NowFPS = 1u;
	}
	else {
		APP_NowFPS = fps;
	}
	APP_ResetFrameStep();
}

int APP_GetFPS(void)
{
	return APP_NowFPS;
}

int APP_GetRealFPS(void)
{
	return APP_FPS;
}
