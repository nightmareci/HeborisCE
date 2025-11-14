#include "APP_update.h"
#include "APP_main.h"
#include "APP_video.h"
#include "APP_input.h"
#include "APP_error.h"
#include "APP_global.h"

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

void APP_ResetFrameStep(void)
{
	APP_FrameNS = SDL_NS_PER_SECOND / APP_GetFPS();
	APP_AccumulatedNS = 0;
	APP_NowNS = SDL_GetTicksNS();
}

static bool APP_FrameStep(void)
{
	uint64_t now;
	if (APP_AccumulatedNS >= APP_FrameNS + 100 * SDL_NS_PER_MS) {
		APP_AccumulatedNS = 0;
	}
	const bool skipped = APP_AccumulatedNS >= APP_FrameNS;
	if (skipped) {
		now = SDL_GetTicksNS();
		APP_AccumulatedNS -= APP_FrameNS;
	}
	else {
		SDL_DelayPrecise(APP_FrameNS - APP_AccumulatedNS);
		now = SDL_GetTicksNS();
		APP_AccumulatedNS += now - APP_NowNS - APP_FrameNS;
	}
	APP_NowNS = now;
	return skipped;
}

void APP_Start(void)
{
	APP_RealFPS = 0;
	APP_RenderWhileSkippingFrames = false;
	APP_LastFrameSkipped = false;
	SDL_srand(0);
	APP_SetFPS(60);
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
				break;
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
			APP_SetError("Failed showing mouse cursor: %s", SDL_GetError());
			APP_Exit(EXIT_FAILURE);
		}
		APP_CursorFrames = APP_SettingFPS;
	}
	if (APP_CursorFrames > 0 && SDL_CursorVisible() && --APP_CursorFrames == 0 && !SDL_HideCursor()) {
		APP_SetError("Failed hiding mouse cursor: %s", SDL_GetError());
		APP_Exit(EXIT_FAILURE);
	}

	#if defined(APP_ENABLE_JOYSTICK) || defined(APP_ENABLE_GAME_CONTROLLER)
	if (slotsChanged) {
		if (!APP_PlayerSlotsChanged()) {
			APP_SetError("Failed changing player joystick/controller device slots: %s", SDL_GetError());
			APP_Exit(EXIT_FAILURE);
		}
	}
	#endif

	return true;
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
