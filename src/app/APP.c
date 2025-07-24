// TODO: Refactor this into multiple sources for each subsystem. E.g., move
// video-related stuff into APP_video.h/APP_video.c, audio-related stuff into
// APP_audio.h/APP_audio.c, etc..

#include "APP.h"
#include "APP_bdf.h"
#include "APP_filesystem.h"
#include "nanotime.h"

#define		APP_GAME_CAPTION		"Heboris C.E."

#define		APP_TEXTURE_MAX		100
#define		APP_WAVE_MAX		100
#define		APP_TEXTLAYER_MAX	16

#define 	APP_VOLUME_MAX		MIX_MAX_VOLUME

#define		APP_WIDE_SCREEN (4.0f / 3.0f)
#define		APP_NARROW_SCREEN (3.0f / 4.0f)

typedef struct APP_TextLayer
{
	bool enable;
	int x;
	int y;
	SDL_Color color;
	int size;
	char string[256];
	bool updateTexture;
	SDL_Texture* texture;
	int textureW;
	int textureH;
} APP_TextLayer;

typedef struct APP_Wave
{
	Mix_Chunk *chunk;
	int loops;
} APP_Wave;

static bool 			APP_InitFast = false;
static SDL_Window		*APP_ScreenWindow = NULL;
static SDL_Renderer		*APP_ScreenRenderer = NULL;
static SDL_Texture		*APP_ScreenRenderTarget = NULL;
static float			APP_ScreenSubpixelOffset = 0.0f;

static SDL_Texture		*APP_Textures[APP_TEXTURE_MAX];

static APP_Wave		APP_Waves[APP_WAVE_MAX];
static Mix_Music		*APP_Music;
static bool			APP_WaveFormatsSupported[APP_WAVE_MAXFORMAT];

#define		APP_BDFFONTFILE_MAX	3
static APP_BDFFont		*APP_BDFFonts[APP_BDFFONTFILE_MAX];

static int			APP_LogicalWidth;
static int			APP_LogicalHeight;

static bool			APP_NoFrameskip;
static nanotime_step_data	APP_StepData = { 0 };
static bool			APP_LastFrameSkipped;
static uint64_t			APP_FPSCount;
static unsigned int		APP_FPSCounting;
static unsigned int		APP_FPS;
static unsigned int		APP_NowFPS;
static unsigned int		APP_CursorCounting;

static APP_TextLayer		APP_TextLayers[APP_TEXTLAYER_MAX];
static int32_t			APP_ScreenMode;
static int32_t			APP_ScreenIndex;

static int			APP_NewOffsetX = 0, APP_NewOffsetY = 0;
static int			APP_OffsetX = 0, APP_OffsetY = 0;

static int			APP_QuitLevel;

#ifndef APP_ONLY_INPUT_TYPE
static APP_InputType APP_LastInputType = APP_INPUT_NULL;
#endif

static void APP_PrivateBDFFontFinalize();
static void APP_PrivateBDFFontInitialize();
static bool APP_OpenInputs();
static void APP_CloseInputs();
static bool APP_PlayerSlotsChanged();

static float APP_GetScreenSubpixelOffset()
{
	// The returned subpixel offset nudges all draws to have pixel coordinates
	// that end up centered in the floating point coordinate space. Without
	// this offset, pixel coordinates are at the upper left of the intended
	// pixels, resulting in off-by-one drawing errors sometimes.
	//
	// The numerator of the return value is the fraction of a pixel to adjust
	// draw position rightwards and downwards, and the division by the current
	// scale converts that subpixel amount to the amount to adjust by within
	// the currently set logical resolution (320x240 or 640x480), so it adjusts
	// by less than 0.375 if the render resolution is above logical resolution,
	// or greater than 0.375 if below logical resolution. If the numerator is
	// exactly 0.5f, then system-dependent rounding errors can occur, because
	// the coordinate is located exactly in the pixel center, producing
	// sampling artifacts on some systems, but not on others. By using a
	// numerator less than 0.5f, the rounding of draw coordinates by the
	// graphics implementation should place drawn pixels in the center. I also
	// saw scaling artifacts happen with a numerator of 0.25f, so maybe the
	// numerator needs to be somewhere in the open range (0.25f, 0.5f) to
	// likely work everywhere; for now, the midpoint, 0.375f, is used, and
	// seems to work correctly on some systems I've tested.
	//
	// Getting the scale from SDL_RenderGetScale is always appropriate here,
	// being an integer value when integer scaling is in effect, or a
	// non-integer scale when fill-screen scaling is in effect. Even if the
	// scale value is below 1.0f, the formula is still correct.
	//
	// -Brandon McGriff <nightmareci@gmail.com>
	if ( APP_ScreenRenderer )
	{
		float scale;
		SDL_RenderGetScale(APP_ScreenRenderer, &scale, NULL);
		return 0.375f / scale;
	}
	else
	{
		return 0.0f;
	}
}

void APP_Init(const int soundBufferSize)
{
	if (soundBufferSize <= 0) {
		APP_Exit(EXIT_FAILURE);
	}

	APP_QuitLevel = 0;

#ifdef __EMSCRIPTEN__
	/* Keyboard input gets a bit broken without this hint set. */
	//SDL_SetHint(SDL_HINT_EMSCRIPTEN_KEYBOARD_ELEMENT, "#canvas");
#endif

	/* SDLの初期化 || SDL initialization */
	if ( !APP_InitFast && SDL_Init(
		SDL_INIT_AUDIO | SDL_INIT_VIDEO
		#ifdef APP_ENABLE_JOYSTICK
		| SDL_INIT_JOYSTICK
		#endif
		#ifdef APP_ENABLE_GAME_CONTROLLER
		| SDL_INIT_GAMECONTROLLER
		#endif
	) < 0 )
	{
		fprintf(stderr, "Couldn't initialize SDL: %s\n", SDL_GetError());
		APP_Exit(EXIT_FAILURE);
	}

	// If this fails, it doesn't matter, the game will still work. But it's
	// called because if it works, the game might perform better.
	if (!APP_InitFast) SDL_SetThreadPriority(SDL_THREAD_PRIORITY_HIGH);

	APP_QuitLevel++;
	if (!APP_InitFast) SDL_SetHint(SDL_HINT_RENDER_BATCHING, "1");

	/* 画像の初期化 || Image initialization */
	if ( !APP_InitFast && IMG_Init(IMG_INIT_PNG) != IMG_INIT_PNG )
	{
		fprintf(stderr, "Couldn't initialize image support: %s\n", IMG_GetError());
		APP_Exit(EXIT_FAILURE);
	}
	APP_QuitLevel++;

	/* サウンドの初期化 || Sound initialization */
	if (!APP_InitFast) {
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
			APP_Exit(EXIT_FAILURE);
		}

		APP_WaveFormatsSupported[APP_WAVE_MID] = !!(formatsInitialized & MIX_INIT_MID);
		APP_WaveFormatsSupported[APP_WAVE_WAV] = 1; // WAVEはいつでも利用可能 || WAVE is always supported
		APP_WaveFormatsSupported[APP_WAVE_OGG] = !!(formatsInitialized & MIX_INIT_OGG);
		APP_WaveFormatsSupported[APP_WAVE_MP3] = !!(formatsInitialized & MIX_INIT_MP3);
		APP_WaveFormatsSupported[APP_WAVE_FLAC] = !!(formatsInitialized & MIX_INIT_FLAC);
		APP_WaveFormatsSupported[APP_WAVE_OPUS] = !!(formatsInitialized & MIX_INIT_OPUS);
		APP_WaveFormatsSupported[APP_WAVE_MOD] = !!(formatsInitialized & MIX_INIT_MOD);
		APP_WaveFormatsSupported[APP_WAVE_IT] = !!(formatsInitialized & MIX_INIT_MOD);
		APP_WaveFormatsSupported[APP_WAVE_XM] = !!(formatsInitialized & MIX_INIT_MOD);
		APP_WaveFormatsSupported[APP_WAVE_S3M] = !!(formatsInitialized & MIX_INIT_MOD);
	}
	APP_QuitLevel++;

	if (!APP_InitFast) {
#ifdef __EMSCRIPTEN__
		// In testing on desktop Linux, Firefox seems to cope with a 1024 byte
		// buffer fine, but Chrome produces a fair bit of audio breakup. 2048
		// seems to work fine in both, though.
		if ( Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0 )
#else
		// A 1024 byte buffer seems to be a good choice for all native code
		// ports. It's reasonably low latency but doesn't result in audio
		// breakup.
		if ( Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024) < 0 )
#endif
		{
			fprintf(stderr, "Couldn't open audio: %s\n", Mix_GetError());
			APP_Exit(EXIT_FAILURE);
		}
	}
	APP_QuitLevel++;

	if (!APP_InitFast) Mix_AllocateChannels(100);

	int		configChanged = 0;

	APP_NewOffsetX = 0;	APP_NewOffsetY = 0;
	APP_OffsetX = 0;		APP_OffsetY = 0;

	APP_SetFPS(60);

	if (!APP_InitFast) APP_OpenInputs();

	/* テクスチャ領域の初期化 || Initialize the texture pointers */
	if (!APP_InitFast) memset(APP_Textures, 0, sizeof(APP_Textures));

	/* サウンドの初期化 || Initialize sound data */
	for ( int i = 0 ; i < APP_WAVE_MAX ; i ++ )
	{
		if ( !APP_InitFast )
		{
			APP_Waves[i] = (APP_Wave) { 0 };
		}
	}

	if ( !APP_InitFast ) APP_Music = NULL;

	/* テキストレイヤーの初期化 || Initialize the text layers */
	for ( int i = 0 ; i < APP_TEXTLAYER_MAX ; i ++ )
	{
		memset(&APP_TextLayers[i], 0, sizeof(APP_TextLayer));
		APP_TextLayers[i].color.r = APP_TextLayers[i].color.g = APP_TextLayers[i].color.b = 255;
		APP_TextLayers[i].color.a = SDL_ALPHA_OPAQUE;
		APP_TextLayers[i].size = 16;
	}

	if ( !APP_InitFast ) APP_PrivateBDFFontInitialize();

	APP_LastFrameSkipped = false;
	APP_FPSCount = 0u;
	APP_FPS			= 0;
	APP_NoFrameskip		= false;

	srand((unsigned)time(NULL));
	APP_InitFast = true;
}

void APP_Quit(void)
{
	APP_CloseInputs();

	/* サウンドの解放 */
	for ( int i = 0 ; i < APP_WAVE_MAX ; i ++ )
	{
		if ( APP_Waves[i].chunk )
		{
			Mix_FreeChunk(APP_Waves[i].chunk);
			APP_Waves[i].chunk = NULL;
		}
	}
	if ( APP_Music )
	{
		Mix_FreeMusic(APP_Music);
		APP_Music = NULL;
	}

	if ( APP_ScreenRenderer )
	{
		for ( int i = 0 ; i < APP_TEXTLAYER_MAX ; i ++ )
		{
			if ( APP_TextLayers[i].texture )
			{
				SDL_DestroyTexture(APP_TextLayers[i].texture);
				APP_TextLayers[i].texture = NULL;
			}
		}
		memset(APP_TextLayers, 0, sizeof(APP_TextLayers));

		/* テクスチャ領域の解放 */
		for ( int i = 0 ; i < APP_TEXTURE_MAX ; i ++ )
		{
			if ( APP_Textures[i] )
			{
				SDL_DestroyTexture(APP_Textures[i]);
				APP_Textures[i] = NULL;
			}
		}
	}

	if ( APP_ScreenRenderer ) {
		SDL_SetRenderTarget( APP_ScreenRenderer, NULL );
	}
	if ( APP_ScreenRenderTarget ) {
		SDL_DestroyTexture( APP_ScreenRenderTarget );
		APP_ScreenRenderTarget = NULL;
	}
	if ( APP_ScreenRenderer ) {
		SDL_DestroyRenderer( APP_ScreenRenderer );
		APP_ScreenRenderer = NULL;
	}

	if ( APP_ScreenWindow ) {
		SDL_DestroyWindow( APP_ScreenWindow );
		APP_ScreenWindow = NULL;
	}

	APP_PrivateBDFFontFinalize();

	switch ( APP_QuitLevel )
	{
	case 4: Mix_CloseAudio();
	case 3: Mix_Quit();
	case 2: IMG_Quit();
	case 1: SDL_Quit();
	default: break;
	}
	APP_QuitLevel = 0;

	APP_InitFast = false;
}

void APP_Exit(int exitStatus)
{
	APP_Quit();
	APP_QuitFilesystem();
	exit(exitStatus);
}

bool APP_Update(void)
{
	if ( APP_ScreenRenderer )
	{
		SDL_RenderFlush( APP_ScreenRenderer );

		#ifndef NDEBUG
		APP_NoFrameskip = true;
		#endif
		if ( APP_NoFrameskip )
		{
			/* バックサーフェスをフロントに転送 */
			if ( APP_ScreenRenderTarget )
			{
				SDL_SetRenderTarget(APP_ScreenRenderer, NULL);
				SDL_RenderClear( APP_ScreenRenderer );
				SDL_RenderCopy(APP_ScreenRenderer, APP_ScreenRenderTarget, NULL, NULL);
				SDL_RenderPresent(APP_ScreenRenderer);
				SDL_SetRenderTarget(APP_ScreenRenderer, APP_ScreenRenderTarget);
			}
			else {
				SDL_RenderPresent(APP_ScreenRenderer);
			}

			/* フレームレート待ち || Frame rate waiting */
			APP_LastFrameSkipped = !nanotime_step(&APP_StepData);

			/* 画面塗りつぶし || Fill screen */
			SDL_RenderClear( APP_ScreenRenderer );
		}
		else
		{
			if ( !APP_LastFrameSkipped )
			{
				/* バックサーフェスをフロントに転送 */
				if ( APP_ScreenRenderTarget )
				{
					SDL_SetRenderTarget(APP_ScreenRenderer, NULL);
					SDL_RenderClear( APP_ScreenRenderer );
					SDL_RenderCopy(APP_ScreenRenderer, APP_ScreenRenderTarget, NULL, NULL);
					SDL_RenderPresent(APP_ScreenRenderer);
					SDL_SetRenderTarget(APP_ScreenRenderer, APP_ScreenRenderTarget);
				}
				else {
					SDL_RenderPresent(APP_ScreenRenderer);
				}

				/* 画面塗りつぶし */
				SDL_RenderClear( APP_ScreenRenderer );
			}

			/* フレームレート待ち || Frame rate waiting */
			APP_LastFrameSkipped = !nanotime_step(&APP_StepData);
		}
	}

	/* フレームレート計算 || Frame rate calculation */
	APP_FPSCounting ++;

	if ( nanotime_interval(APP_FPSCount, nanotime_now(), APP_StepData.now_max) >= NANOTIME_NSEC_PER_SEC )
	{
		APP_FPS = APP_FPSCounting;
		APP_FPSCounting = 0;
		APP_FPSCount = nanotime_now();
	}

	/* イベント処理 || Process events */

	SDL_PumpEvents();
	SDL_Event	ev;
	int showCursor = SDL_DISABLE;
	#if defined(APP_ENABLE_JOYSTICK) || defined(APP_ENABLE_GAME_CONTROLLER)
	bool slotsChanged = false;
	#endif
	while (SDL_PeepEvents(&ev, 1, SDL_GETEVENT, 0, SDL_LASTEVENT) == 1)
	{
		switch(ev.type){
			// ウィンドウの×ボタンが押された時など || When the window's X-button is pressed, etc.
			case SDL_QUIT:
				return false;

			case SDL_WINDOWEVENT:
				if (ev.window.event == SDL_WINDOWEVENT_SIZE_CHANGED) {
					APP_ScreenSubpixelOffset = APP_SCREEN_SUBPIXEL_OFFSET;
				}
				break;

			#ifdef APP_ENABLE_JOYSTICK
			case SDL_JOYDEVICEADDED:
			case SDL_JOYDEVICEREMOVED:
				slotsChanged = true;
			#endif

			#ifdef APP_ENABLE_GAME_CONTROLLER
			case SDL_CONTROLLERDEVICEADDED:
			case SDL_CONTROLLERDEVICEREMOVED:
				slotsChanged = true;
				break;
			#endif

			case SDL_MOUSEMOTION:
			case SDL_MOUSEBUTTONDOWN:
			case SDL_MOUSEBUTTONUP:
			case SDL_MOUSEWHEEL:
				showCursor = SDL_ENABLE;
				break;
			
			default:
				break;
		}
	}

	if (showCursor == SDL_ENABLE)
	{
		SDL_ShowCursor(SDL_ENABLE);
		APP_CursorCounting = 0u;
	}

	if (SDL_ShowCursor(SDL_QUERY) == SDL_ENABLE && APP_CursorCounting++ >= APP_NowFPS)
	{
		SDL_ShowCursor(SDL_DISABLE);
	}

	#if defined(APP_ENABLE_JOYSTICK) || defined(APP_ENABLE_GAME_CONTROLLER)
	if (slotsChanged) {
		if (!APP_PlayerSlotsChanged()) {
			APP_Exit(EXIT_FAILURE);
		}
	}
	#endif

	/* 画面ずらし量の反映 */
	APP_OffsetX = APP_NewOffsetX;
	APP_OffsetY = APP_NewOffsetY;

	return true;
}

int APP_IsPlayMusic()
{
	return Mix_PlayingMusic();
}

#ifdef __EMSCRIPTEN__
static EM_BOOL APP_EmscriptenResizeCallback(int eventType, const EmscriptenUiEvent* uiEvent, void* userData)
{
	if (
		eventType != EMSCRIPTEN_EVENT_RESIZE ||
		!APP_ScreenWindow
	) {
		return false;
	}

	SDL_SetWindowSize(APP_ScreenWindow, uiEvent->windowInnerWidth, uiEvent->windowInnerHeight);
	return true;
}
#endif

bool APP_SetScreen(APP_ScreenModeFlag *screenMode, int32_t *screenIndex)
{
	Uint32		windowFlags = SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE;
	int		windowX, windowY;
	int		logicalWidth, logicalHeight;

	/* 画面の設定 || Set up the screen */

	/* Validate the window type */
	APP_ScreenModeFlag windowType = *screenMode & APP_SCREENMODE_WINDOWTYPE;
	if ( windowType < 0 || windowType >= APP_SCREENMODE_NUMWINDOWTYPES )
	{
		goto fail;
	}

	int displayIndex = APP_SCREENINDEX_DISPLAY_TOVALUE(*screenIndex);
	int modeIndex = APP_SCREENINDEX_MODE_TOVALUE(*screenIndex);
	int numVideoDisplays = SDL_GetNumVideoDisplays();
	int numDisplayModes = SDL_GetNumDisplayModes(displayIndex);
	if ( numVideoDisplays < 0 || numDisplayModes < 0 )
	{
		goto fail;
	}

	if (
		displayIndex >= numVideoDisplays ||
		((windowType == APP_SCREENMODE_FULLSCREEN || windowType == APP_SCREENMODE_FULLSCREEN_DESKTOP) && modeIndex >= numDisplayModes)
	) {
		*screenMode = APP_DEFAULT_SCREEN_MODE;
		*screenIndex = 0;
		windowX = SDL_WINDOWPOS_CENTERED_DISPLAY(*screenIndex);
		windowY = SDL_WINDOWPOS_CENTERED_DISPLAY(*screenIndex);
	}
	else
	{
		windowX = SDL_WINDOWPOS_CENTERED_DISPLAY(displayIndex);
		windowY = SDL_WINDOWPOS_CENTERED_DISPLAY(displayIndex);
		switch ( windowType & APP_SCREENMODE_WINDOWTYPE )
		{
		case APP_SCREENMODE_WINDOW_MAXIMIZED:
			windowFlags |= SDL_WINDOW_MAXIMIZED;
			break;

		case APP_SCREENMODE_FULLSCREEN_DESKTOP:
			windowFlags |= SDL_WINDOW_FULLSCREEN_DESKTOP;
			break;

		case APP_SCREENMODE_FULLSCREEN: {
			windowFlags |= SDL_WINDOW_FULLSCREEN;
			break;
		}
		}
	}

	if ( *screenMode & APP_SCREENMODE_DETAILLEVEL )
	{
		logicalWidth  = 640;
		logicalHeight = 480;
	}
	else
	{
		logicalWidth  = 320;
		logicalHeight = 240;
	}

	APP_LogicalWidth  = logicalWidth;
	APP_LogicalHeight = logicalHeight;

	/* ウィンドウの作成 || Create and set up the window */
	if (
		windowType == APP_SCREENMODE_FULLSCREEN ||
		windowType == APP_SCREENMODE_FULLSCREEN_DESKTOP
	)
	{
		SDL_DisplayMode displayMode;
		int status;
		if ( windowType == APP_SCREENMODE_FULLSCREEN )
		{
			status = SDL_GetDisplayMode(displayIndex, modeIndex, &displayMode);
		}
		else
		{
			status = SDL_GetDesktopDisplayMode(displayIndex, &displayMode);
		}
		if ( status < 0 )
		{
			*screenMode &= ~APP_SCREENMODE_WINDOWTYPE;
			*screenMode |= APP_SCREENMODE_WINDOW;
			*screenIndex = 0;
			if ( !APP_ScreenWindow )
			{
				APP_ScreenWindow = SDL_CreateWindow(
					APP_GAME_CAPTION,
					SDL_WINDOWPOS_CENTERED_DISPLAY(displayIndex),
					SDL_WINDOWPOS_CENTERED_DISPLAY(displayIndex),
					logicalWidth,
					logicalHeight,
					SDL_WINDOW_HIDDEN | SDL_WINDOW_RESIZABLE
				);
				if ( !APP_ScreenWindow )
				{
					goto fail;
				}
			}
			else
			{
				SDL_RestoreWindow(APP_ScreenWindow);
				SDL_SetWindowSize(APP_ScreenWindow, logicalWidth, logicalHeight);
				SDL_SetWindowResizable(APP_ScreenWindow, SDL_TRUE);
				SDL_SetWindowPosition(APP_ScreenWindow, SDL_WINDOWPOS_CENTERED_DISPLAY(displayIndex), SDL_WINDOWPOS_CENTERED_DISPLAY(displayIndex));
				if ( SDL_SetWindowFullscreen(APP_ScreenWindow, 0u) < 0 )
				{
					goto fail;
				}
			}
		}
		else {
			if ( !APP_ScreenWindow )
			{
				APP_ScreenWindow = SDL_CreateWindow(APP_GAME_CAPTION, windowX, windowY, displayMode.w, displayMode.h, windowFlags);
				if ( !APP_ScreenWindow )
				{
					goto fail;
				}
			}
			else
			{
				SDL_SetWindowPosition(APP_ScreenWindow, windowX, windowY);
				int fullscreenError = -1;
				if ( (windowFlags & SDL_WINDOW_FULLSCREEN_DESKTOP) == SDL_WINDOW_FULLSCREEN_DESKTOP )
				{
					fullscreenError = SDL_SetWindowFullscreen(APP_ScreenWindow, SDL_WINDOW_FULLSCREEN_DESKTOP);
				}
				else if ( (windowFlags & SDL_WINDOW_FULLSCREEN) == SDL_WINDOW_FULLSCREEN )
				{
					fullscreenError = SDL_SetWindowFullscreen(APP_ScreenWindow, SDL_WINDOW_FULLSCREEN);
				}
				if ( fullscreenError < 0 )
				{
					goto fail;
				}
			}
			if ( SDL_SetWindowDisplayMode(APP_ScreenWindow, &displayMode) < 0 )
			{
				goto fail;
			}
		}
	}
	else if (
		windowType == APP_SCREENMODE_WINDOW ||
		windowType == APP_SCREENMODE_WINDOW_MAXIMIZED
	)
	{
		SDL_DisplayMode displayMode;
		if ( SDL_GetDesktopDisplayMode(displayIndex, &displayMode) < 0 )
		{
			goto fail;
		}

		int maxScale;
		if ( displayMode.w <= logicalWidth || displayMode.h <= logicalHeight )
		{
			maxScale = 1;
		}
		else if ( displayMode.w > displayMode.h )
		{
			maxScale = (displayMode.h / logicalHeight) - (displayMode.h % logicalHeight == 0);
		}
		else
		{
			maxScale = (displayMode.w / logicalWidth) - (displayMode.w % logicalWidth == 0);
		}
		int maxWidth = maxScale * logicalWidth;
		int maxHeight = maxScale * logicalHeight;
		int scale = modeIndex + 1;
		int windowW = scale * logicalWidth;
		int windowH = scale * logicalHeight;
		if ( scale > maxScale) {
			windowW = logicalWidth;
			windowH = logicalHeight;
			*screenIndex = APP_SCREENINDEX_TOSETTING(displayIndex, 0);
		}
		if ( !APP_ScreenWindow )
		{
			APP_ScreenWindow = SDL_CreateWindow(APP_GAME_CAPTION, windowX, windowY, windowW, windowH, windowFlags);
			if ( !APP_ScreenWindow )
			{
				goto fail;
			}
		}
		else
		{
			if ( windowFlags & SDL_WINDOW_MAXIMIZED ) {
				SDL_MaximizeWindow(APP_ScreenWindow);
			}
			else {
				SDL_RestoreWindow(APP_ScreenWindow);
			}
			SDL_SetWindowResizable(APP_ScreenWindow, SDL_TRUE);
			if ( SDL_SetWindowFullscreen(APP_ScreenWindow, 0) < 0 )
			{
				goto fail;
			}
			SDL_SetWindowSize(APP_ScreenWindow, windowW, windowH);
			SDL_SetWindowPosition(APP_ScreenWindow, windowX, windowY);
		}
	}

	// Create the renderer, if not already created. It's important to not
	// recreate the renderer if it's already created, so restarting without
	// changing the detail level doesn't require reloading graphics. If the
	// renderer were destroyed/created anew every restart, it would be required
	// to reload the graphics every restart, even when detail level isn't
	// changed.
	// TODO: fix to allow rendering to the texture.
	if ( !APP_ScreenRenderer )
	{
		
		APP_ScreenRenderer = SDL_CreateRenderer(APP_ScreenWindow, -1, SDL_RENDERER_TARGETTEXTURE); // ask for render to texture support.
		if (!APP_ScreenRenderer)
		{
			goto fail;
		}
	}
	SDL_RenderClear(APP_ScreenRenderer);
	SDL_RenderPresent(APP_ScreenRenderer);

	// Unset the render target, if currently set, for making renderer setting
	// changes. The render target should only be set once all settings have been
	// set, with no setting changes made after the render target has been set;
	// various bugs have been observed when attempting setting changes while the
	// render target is non-NULL, such bugs disappearing when setting changes
	// are made only while the render target is NULL.
	if ( APP_ScreenRenderTarget && SDL_SetRenderTarget(APP_ScreenRenderer, NULL) < 0 )
	{
		goto fail;
	}

	/* Clear the whole screen, as the framebuffer might be uninitialized */
	if ( SDL_RenderClear(APP_ScreenRenderer) < 0 )
	{
		goto fail;
	}

	// This should be somewhere after the renderer has been created, as
	// APP_SCREEN_SUBPIXEL_OFFSET queries the renderer when a given platform uses
	// nonzero offsets.
	APP_ScreenSubpixelOffset = APP_SCREEN_SUBPIXEL_OFFSET;

	if ( SDL_RenderSetVSync(APP_ScreenRenderer, !!(*screenMode & APP_SCREENMODE_VSYNC)) )
	{
		goto fail;
	}

	if ( SDL_RenderSetLogicalSize(APP_ScreenRenderer, logicalWidth, logicalHeight) < 0 )
	{
		goto fail;
	}

	/* Use integer scaling, if required */
	if ( *screenMode & APP_SCREENMODE_SCALEMODE )
	{
		if ( SDL_RenderSetIntegerScale(APP_ScreenRenderer, SDL_TRUE) < 0 )
		{
			goto fail;
		}
	}
	else
	{
		if ( SDL_RenderSetIntegerScale(APP_ScreenRenderer, SDL_FALSE) < 0 )
		{
			goto fail;
		}
	}

	/* Set up the render target, if required */
	// TODO: Figure out how to get render targets in Emscripten working with
	// SDL_BLENDMODE_BLEND textures. This #ifdef is a workaround for now. It's
	// probably a bug in the Emscripten SDL2 port.
	#ifndef __EMSCRIPTEN__
	if ( SDL_RenderTargetSupported(APP_ScreenRenderer) )
	{
		if (!(*screenMode & APP_SCREENMODE_RENDERLEVEL))
		{
			// There's no need to create a render target texture if the
			// currently created render target texture is already the current
			// logicalWidth x logicalHeight.
			bool createNewRenderTarget = true;

			if ( APP_ScreenRenderTarget ) {
				int w, h;
				if ( SDL_QueryTexture(APP_ScreenRenderTarget, NULL, NULL, &w, &h) < 0 ) {
					goto fail;
				}

				if ( w == logicalWidth && h == logicalHeight )
				{
					createNewRenderTarget = false;
				}
				else {
					SDL_DestroyTexture(APP_ScreenRenderTarget);
					APP_ScreenRenderTarget = NULL;
				}
			}

			if ( createNewRenderTarget )
			{
				APP_ScreenRenderTarget = SDL_CreateTexture(APP_ScreenRenderer, SDL_PIXELFORMAT_RGBX8888, SDL_TEXTUREACCESS_TARGET, logicalWidth, logicalHeight);
				if ( !APP_ScreenRenderTarget )
				{
					goto fail;
				}
			}

			if (
				SDL_SetRenderTarget(APP_ScreenRenderer, APP_ScreenRenderTarget) < 0 ||
				SDL_RenderClear(APP_ScreenRenderer) < 0
			)
			{
				goto fail;
			}
		}
		else if ( APP_ScreenRenderTarget )
		{
			SDL_DestroyTexture(APP_ScreenRenderTarget);
			APP_ScreenRenderTarget = NULL;
		}
	}
	else
	#endif
	{
		APP_ScreenRenderTarget = NULL;
		*screenMode |= APP_SCREENMODE_RENDERLEVEL;
	}

	// WARNING: Make no changes to the renderer settings from here on down, as
	// the render target has been set, and bugs have been observed when
	// attempting renderer setting changes while a non-NULL render target is
	// set.

	#ifdef __EMSCRIPTEN__
	static bool emscriptenCallbackSet = false;
	if ( !emscriptenCallbackSet && emscripten_set_resize_callback(EMSCRIPTEN_EVENT_TARGET_WINDOW, NULL, false, APP_EmscriptenResizeCallback) < 0 )
	{
		goto fail;
	}
	else
	{
		emscriptenCallbackSet = true;
		int width = -1, height = -1;
		width = EM_ASM_INT({ return window.innerWidth; });
		height = EM_ASM_INT({ return window.innerHeight; });
		if ( width > 0 && height > 0 )
		{
			SDL_SetWindowSize(APP_ScreenWindow, width, height);
		}
	}
	#endif

	APP_ScreenMode = *screenMode;
	APP_ScreenIndex = *screenIndex;

	/* Hide the mouse cursor at first */
	if ( SDL_ShowCursor(SDL_DISABLE) < 0 )
	{
		goto fail;
	}
	APP_CursorCounting = 0u;

	/* Setup was successful, so return with success */
	return true;

	/* A failure condition was encountered, so clean up and return with error */
	fail:
	if ( APP_ScreenRenderTarget )
	{
		SDL_SetRenderTarget(APP_ScreenRenderer, NULL);
		SDL_DestroyTexture(APP_ScreenRenderTarget);
	}
	if ( APP_ScreenRenderer ) SDL_DestroyRenderer(APP_ScreenRenderer);
	if ( APP_ScreenWindow ) SDL_DestroyWindow(APP_ScreenWindow);
	APP_ScreenRenderTarget = NULL;
	APP_ScreenRenderer = NULL;
	APP_ScreenWindow = NULL;
	return false;
}

int APP_GetMaxDisplayIndex()
{
	return SDL_GetNumVideoDisplays();
}

int APP_GetMaxDisplayMode( int displayIndex )
{
	return SDL_GetNumDisplayModes(displayIndex);
}

bool APP_RenderLevelLowSupported()
{
	return APP_ScreenRenderer ? SDL_RenderTargetSupported(APP_ScreenRenderer) : false;
}

int APP_Rand ( int max )
{
	return rand() % max;
}

void APP_PauseMusic()
{
	Mix_PauseMusic();
}

void APP_ReplayMusic()
{
	Mix_ResumeMusic();
}

void APP_PlayWave ( int num )
{
	if ( APP_Waves[num].chunk )
	{
		Mix_PlayChannel(num, APP_Waves[num].chunk, APP_Waves[num].loops);
	}
}

void APP_ReplayWave ( int num )
{
	Mix_Resume(num);
}

void APP_StopWave ( int num )
{
	Mix_HaltChannel(num);
}

void APP_PauseWave ( int num )
{
	Mix_Pause(num);
}

void APP_SetVolumeWave( int num, int vol )
{
	int volume = (int)((vol / 100.0f) * APP_VOLUME_MAX);
	if ( volume > APP_VOLUME_MAX ) { volume = APP_VOLUME_MAX; }
	if ( volume < 0 )   { volume = 0; }
	Mix_Volume(num, volume);
}

int APP_IsPlayWave( int num )
{
	return Mix_Playing(num);
}

void APP_LoadWave( const char* filename, int num )
{
	int len = strlen(filename);
	if ( len < 4 ) { return; }

	SDL_RWops *src;
	src = APP_OpenRead(filename);
	if ( !src ) return;
	if ( APP_Waves[num].chunk )
	{
		Mix_FreeChunk(APP_Waves[num].chunk);
		APP_Waves[num].chunk = NULL;
	}
	APP_Waves[num].chunk = Mix_LoadWAV_RW(src, 1);
	Mix_VolumeChunk(APP_Waves[num].chunk, APP_VOLUME_MAX);
	APP_Waves[num].loops = 0;
}

void APP_SetLoopModeWave( int num, int mode )
{
	if ( mode )
	{
		// SDL2 SDL_mixer doesn't really support infinite looping of
		// chunks, but INT_MAX is plenty enough.
		APP_Waves[num].loops = INT_MAX;
	}
	else
	{
		APP_Waves[num].loops = 0;
	}
}

void APP_LoadMusic( const char* filename )
{
	if ( APP_Music )
	{
		Mix_FreeMusic(APP_Music);
		APP_Music = NULL;
	}

	SDL_RWops* src;
	if (!(src = APP_OpenRead(filename))) return;
	APP_Music = Mix_LoadMUS_RW(src, SDL_TRUE);
	Mix_VolumeMusic(APP_VOLUME_MAX);
}

void APP_LoadBitmap( const char* filename, int plane, int val )
{
	if ( !APP_ScreenRenderer )
	{
		return;
	}

	if ( APP_Textures[plane] )
	{
		SDL_DestroyTexture(APP_Textures[plane]);
		APP_Textures[plane] = NULL;
	}

	SDL_RWops* src;
	if (!(src = APP_OpenRead(filename))) return;
	if (!(APP_Textures[plane] = IMG_LoadTexture_RW(APP_ScreenRenderer, src, SDL_TRUE))) return;
	SDL_SetTextureBlendMode(APP_Textures[plane], SDL_BLENDMODE_BLEND);
}

void APP_PlayMusic()
{
	if ( APP_Music )
	{
		Mix_PlayMusic(APP_Music, -1);
	}
}

void APP_StopMusic()
{
	Mix_HaltMusic();
}

void APP_SetVolumeMusic(int vol)
{
	int volume = (int)((vol / 100.0f) * APP_VOLUME_MAX);
	if ( volume > APP_VOLUME_MAX ) { volume = APP_VOLUME_MAX; }
	if ( volume < 0 )   { volume = 0; }
	Mix_VolumeMusic(volume);
}

bool APP_WaveFormatSupported(APP_WaveFormat format) {
	return APP_WaveFormatsSupported[format & APP_WAVE_FORMAT];
}

void APP_SetColorKeyPos(int plane, int x, int y)
{
	// TODO
	// sets transparent color to the specified pixel.  Since we use actual alpha channel in our assets, this is a no-oop
}

void APP_EnableBlendColorKey(int plane, int key)
{
	// TODO
	// alows for parial transparency.   again, because we use real transparency, it's a no-op.
}

void APP_CreateSurface(int surf, int w, int h)
{
	// TODO
	// required for orignal YGS2K engine. not needed at all for SDL2 renderer.
}

void APP_ClearSecondary()
{
	// TODO
	// used to write the listed color to all pixels of the rendering area.
	// with SDL2 renderer, we never need to do this, so it's a no-op
}

void APP_SetFillColor(int col)
{
	// TODO
	// sets the color that APP_ClearSecondary uses to fill the render target. since APP_ClearSecondary is a no-op, so is this.
}

void APP_LoadFile( const char* filename, void* buf, size_t size )
{
	SDL_RWops	*src = APP_OpenRead(filename);

	if ( src )
	{
		SDL_RWread(src, buf, size, 1);
		SDL_RWclose(src);

		/* エンディアン変換 */
		int32_t* buf2 = (int32_t*)buf;
		for ( size_t i = 0 ; i < size / sizeof(int32_t) ; i ++ )
		{
			buf2[i] = SDL_SwapLE32(buf2[i]);
		}
	}
}

void APP_ReadFile( const char* filename, void* buf, size_t size, size_t offset )
{
	SDL_RWops	*src = APP_OpenRead(filename);

	if ( src )
	{
		if (SDL_RWseek(src, offset, RW_SEEK_SET) < 0) return;
		SDL_RWread(src, buf, size, 1);
		SDL_RWclose(src);

		/* エンディアン変換 */
		int32_t* buf2 = (int32_t*)buf;
		for ( size_t i = 0 ; i < size / sizeof(int32_t) ; i ++ )
		{
			buf2[i] = SDL_SwapLE32(buf2[i]);
		}
	}
}


void APP_SaveFile( const char* filename, void* buf, size_t size )
{
	/* エンディアン変換 */
	int32_t* buf2 = (int32_t*)buf;
	for ( size_t i = 0 ; i < size / sizeof(int32_t) ; i ++ )
	{
		buf2[i] = SDL_SwapLE32(buf2[i]);
	}

	SDL_RWops	*dst = APP_OpenWrite(filename);

	if ( dst )
	{
		SDL_RWwrite(dst, buf, size, 1);
		if ( SDL_RWclose(dst) < 0 )
		{
			fprintf(stderr, "Error closing: %s\n", SDL_GetError());
		}
		// TODO: Create a custom SDL_RWops object type for Emscripten and put this in that.
		#ifdef __EMSCRIPTEN__
		EM_ASM({
			FS.syncfs(function (err) {
				assert(!err);
			});
		});
		#endif
	}

	/* もどす */
	for ( size_t i = 0 ; i < size / sizeof(int32_t) ; i ++ )
	{
		buf2[i] = SDL_SwapLE32(buf2[i]);
	}
}

void APP_AppendFile( const char* filename, void* buf, size_t size ) {
	/* エンディアン変換 */
	int32_t* buf2 = (int32_t*)buf;
	for ( size_t i = 0 ; i < size / sizeof(int32_t) ; i ++ )
	{
		buf2[i] = SDL_SwapLE32(buf2[i]);
	}

	SDL_RWops	*dst = APP_OpenAppend(filename);

	if ( dst )
	{
		SDL_RWwrite(dst, buf, size, 1);
		// TODO: Create a custom SDL_RWops object type for Emscripten and put this in that.
		SDL_RWclose(dst);
		#ifdef __EMSCRIPTEN__
		EM_ASM({
			FS.syncfs(function (err) {
				assert(!err);
			});
		});
		#endif
	}

	/* もどす */
	for ( size_t i = 0 ; i < size / sizeof(int32_t) ; i ++ )
	{
		buf2[i] = SDL_SwapLE32(buf2[i]);
	}
}

void APP_TextLayerOn ( int layer, int x, int y )
{
	APP_TextLayers[layer].enable = true;
	APP_TextLayers[layer].x = x;
	APP_TextLayers[layer].y = y;
}

void APP_TextMove ( int layer, int x, int y )
{
	APP_TextLayers[layer].x = x;
	APP_TextLayers[layer].y = y;
}

void APP_TextColor ( int layer, int r, int g, int b )
{
	if (
		APP_TextLayers[layer].color.r != r ||
		APP_TextLayers[layer].color.g != g ||
		APP_TextLayers[layer].color.b != b
	) {
		APP_TextLayers[layer].color.r = r;
		APP_TextLayers[layer].color.g = g;
		APP_TextLayers[layer].color.b = b;
		APP_TextLayers[layer].color.a = SDL_ALPHA_OPAQUE;
		APP_TextLayers[layer].updateTexture = true;
	}
}

void APP_TextBackColorDisable ( int layer )
{
	// TODO
	// turns off the shadow effect for text in the listed layer. since we don't even use said shadow effect to begin with, it's a no-op.
}

void APP_TextSize ( int layer, int size )
{
	if (APP_TextLayers[layer].size != size) {
		APP_TextLayers[layer].size = size;
		APP_TextLayers[layer].updateTexture = true;
	}
}

void APP_TextHeight ( int layer, int height )
{
	// TODO
	// only used in flexdraw.c for ExTextHeight. But since ExTextHeight is unused, we don't need to bother implementing it. 
}

void APP_TextOut ( int layer, const char* text )
{
	if ( strcmp(text, APP_TextLayers[layer].string) != 0 )
	{
		strcpy(APP_TextLayers[layer].string, text);
		APP_TextLayers[layer].updateTexture = true;
	}
}

void APP_TextBlt ( int layer )
{
	if ( !APP_ScreenRenderer || !APP_TextLayers[layer].enable)
	{
		return;
	}

	int font = 0;

	if ( APP_TextLayers[layer].size >= 12 )
	{
		font ++;
	}
	if ( APP_TextLayers[layer].size >= 16 )
	{
		font ++;
	}

	if ( !APP_BDFFonts[font] )
	{
		return;
	}

	if ( APP_TextLayers[layer].updateTexture )
	{
		if ( APP_TextLayers[layer].texture )
		{
			SDL_DestroyTexture(APP_TextLayers[layer].texture);
		}
		APP_TextLayers[layer].texture = APP_CreateBDFTextTexture(APP_BDFFonts[font], APP_ScreenRenderer, APP_TextLayers[layer].string, APP_TextLayers[layer].color, 32);
		if ( !APP_TextLayers[layer].texture )
		{
			SDL_Log("Error creating texture to blit text: %s\n", SDL_GetError());
			APP_Exit(EXIT_FAILURE);
		}
		if ( SDL_QueryTexture(APP_TextLayers[layer].texture, NULL, NULL, &APP_TextLayers[layer].textureW, &APP_TextLayers[layer].textureH) < 0 )
		{
			SDL_Log("Error getting size of a text layer texture: %s\n", SDL_GetError());
			APP_Exit(EXIT_FAILURE);
		}
		APP_TextLayers[layer].updateTexture = false;
	}

	if ( APP_TextLayers[layer].texture )
	{
		if ( APP_ScreenRenderTarget )
		{
			const SDL_Rect dstRect =
			{
				APP_TextLayers[layer].x + APP_OffsetX,
				APP_TextLayers[layer].y + APP_OffsetY,
				APP_TextLayers[layer].textureW,
				APP_TextLayers[layer].textureH
			};
			if ( SDL_RenderCopy(APP_ScreenRenderer, APP_TextLayers[layer].texture, NULL, &dstRect) < 0 )
			{
				SDL_Log("Error rendering text layer: %s\n", SDL_GetError());
				APP_Exit(EXIT_FAILURE);
			}
		}
		else
		{
			const SDL_FRect dstRect =
			{
				APP_TextLayers[layer].x + APP_OffsetX + APP_ScreenSubpixelOffset,
				APP_TextLayers[layer].y + APP_OffsetY + APP_ScreenSubpixelOffset,
				APP_TextLayers[layer].textureW,
				APP_TextLayers[layer].textureH
			};
			if ( SDL_RenderCopyF(APP_ScreenRenderer, APP_TextLayers[layer].texture, NULL, &dstRect) < 0 )
			{
				SDL_Log("Error rendering text layer: %s\n", SDL_GetError());
				APP_Exit(EXIT_FAILURE);
			}
		}
	}
}

void APP_TextLayerOff ( int layer )
{
	APP_TextLayers[layer].enable = false;
}

void APP_BltAlways(bool always)
{
	APP_NoFrameskip = always;
}

void APP_Blt(int pno, int dx, int dy)
{
	if ( APP_Textures[pno] == NULL ) { return; }
	int w, h;
	SDL_QueryTexture(APP_Textures[pno], NULL, NULL, &w, &h);
	APP_BltRect(pno, dx, dy, 0, 0, w, h);
}
void APP_BltRect(int pno, int dx, int dy, int sx, int sy, int hx, int hy)
{
	if ( !APP_ScreenRenderer )
	{
		return;
	}

	if ( APP_Textures[pno] == NULL ) return;

	if ( APP_ScreenRenderTarget )
	{
		SDL_Rect	src = { 0 };
		SDL_Rect	dst = { 0 };

		src.x = sx;			src.y = sy;
		src.w = hx;			src.h = hy;
		dst.x = dx + APP_OffsetX;	dst.y = dy + APP_OffsetY;
		dst.w = hx;			dst.h = hy;

		SDL_RenderCopy(APP_ScreenRenderer, APP_Textures[pno], &src, &dst);
	}
	else
	{
		SDL_Rect	src = { 0 };
		SDL_FRect	dst = { APP_ScreenSubpixelOffset, APP_ScreenSubpixelOffset };

		src.x  = sx;			src.y  = sy;
		src.w  = hx;			src.h  = hy;
		dst.x += dx + APP_OffsetX;	dst.y += dy + APP_OffsetY;
		dst.w  = hx;			dst.h  = hy;

		SDL_RenderCopyF(APP_ScreenRenderer, APP_Textures[pno], &src, &dst);
	}
}

void APP_BltFast(int pno, int dx, int dy)
{
	APP_Blt(pno, dx, dy);
}

void APP_BltFastRect(int pno, int dx, int dy, int sx, int sy, int hx, int hy)
{
	APP_BltRect(pno, dx, dy, sx, sy, hx, hy);
}

void APP_BlendBlt(int pno, int dx, int dy, int ar, int ag, int ab, int br, int bg, int bb)
{
	if ( APP_Textures[pno] == NULL ) return;

	SDL_SetTextureAlphaMod(APP_Textures[pno], ar);
	APP_Blt(pno, dx, dy);
	SDL_SetTextureAlphaMod(APP_Textures[pno], SDL_ALPHA_OPAQUE);
}

void APP_BlendBltRect(int pno, int dx, int dy, int sx, int sy, int hx, int hy, int ar, int ag, int ab, int br, int bg, int bb)
{
	if ( APP_Textures[pno] == NULL ) return;

	SDL_SetTextureAlphaMod(APP_Textures[pno], ar);
	APP_BltRect(pno, dx, dy, sx, sy, hx, hy);
	SDL_SetTextureAlphaMod(APP_Textures[pno], SDL_ALPHA_OPAQUE);
}

void APP_BltR(int pno, int dx, int dy, int scx, int scy)
{
	APP_BltRectR(pno, dx, dy, 0, 0, APP_LogicalWidth, APP_LogicalHeight, scx, scy);
}

void APP_BltRectR(int pno, int dx, int dy, int sx, int sy, int hx, int hy, int scx, int scy)
{
	if ( !APP_ScreenRenderer )
	{
		return;
	}
	if ( APP_Textures[pno] == NULL ) return;

	// ちゃんと拡大して描画する
	if ( APP_ScreenRenderTarget )
	{
		SDL_Rect	src = { 0 };
		SDL_Rect	dst = { 0 };

		src.x = sx;					src.y = sy;
		src.w = hx;					src.h = hy;
		dst.x = dx + APP_OffsetX;	dst.y = dy + APP_OffsetY;
		dst.w = hx * (scx / 65536.0f);
		dst.h = hy * (scy / 65536.0f);

		if ( src.w == 0 || src.h == 0 || dst.w == 0 || dst.h == 0 ) { return; }

		SDL_RenderCopy( APP_ScreenRenderer, APP_Textures[pno], &src, &dst );
	}
	else
	{
		SDL_Rect	src = { 0 };
		SDL_FRect	dst = { APP_ScreenSubpixelOffset, APP_ScreenSubpixelOffset };

		src.x  = sx;			src.y  = sy;
		src.w  = hx;			src.h  = hy;
		dst.x += dx + APP_OffsetX;	dst.y += dy + APP_OffsetY;
		dst.w  = (int)(hx * (scx / 65536.0f));
		dst.h  = (int)(hy * (scy / 65536.0f));

		if ( src.w == 0 || src.h == 0 || dst.w == 0 || dst.h == 0 ) { return; }

		SDL_RenderCopyF( APP_ScreenRenderer, APP_Textures[pno], &src, &dst );
	}
}

void APP_BltFastR(int pno, int dx, int dy, int scx, int scy)
{
	APP_BltR(pno, dx, dy, scx, scy);
}

void APP_BltFastRectR(int pno, int dx, int dy, int sx, int sy, int hx, int hy, int scx, int scy)
{
	APP_BltRectR(pno, dx, dy, sx, sy, hx, hy, scx, scy);
}

void APP_BltTrans(int pno, int dx, int dy)
{
	// TODO
	// completely unused.  so we don't need to care what it even does.
}

void APP_BlendBltR(int pno, int dx, int dy, int ar, int ag, int ab, int br, int bg, int bb, int scx, int scy)
{
	APP_BlendBltRectR(pno, dx, dy, 0, 0, APP_LogicalWidth, APP_LogicalHeight, ar, ag, ab, br, bg, bb, scx, scy);
}

void APP_BlendBltRectR(int pno, int dx, int dy, int sx, int sy, int hx, int hy, int ar, int ag, int ab, int br, int bg, int bb, int scx, int scy)
{
	if ( !APP_ScreenRenderer )
	{
		return;
	}

	if ( APP_Textures[pno] == NULL ) return;

	// ちゃんと拡大して描画する
	if ( APP_ScreenRenderTarget )
	{
		SDL_Rect	src = { 0 };
		SDL_Rect	dst = { 0 };

		src.x = sx;					src.y = sy;
		src.w = hx;					src.h = hy;
		dst.x = dx + APP_OffsetX;	dst.y = dy + APP_OffsetY;
		dst.w = hx * (scx / 65536.0f);
		dst.h = hy * (scy / 65536.0f);

		if ( src.w == 0 || src.h == 0 || dst.w == 0 || dst.h == 0 ) { return; }

		SDL_SetTextureAlphaMod(APP_Textures[pno], ar);
		SDL_RenderCopy( APP_ScreenRenderer, APP_Textures[pno], &src, &dst );
		SDL_SetTextureAlphaMod(APP_Textures[pno], SDL_ALPHA_OPAQUE);
	}
	else
	{
		SDL_Rect	src = { 0 };
		SDL_FRect	dst = { APP_ScreenSubpixelOffset, APP_ScreenSubpixelOffset };

		src.x  = sx;			src.y  = sy;
		src.w  = hx;			src.h  = hy;
		dst.x += dx + APP_OffsetX;	dst.y += dy + APP_OffsetY;
		dst.w  = hx * (scx / 65536.0f);
		dst.h  = hy * (scy / 65536.0f);

		if ( src.w == 0 || src.h == 0 || dst.w == 0 || dst.h == 0 ) { return; }

		SDL_SetTextureAlphaMod(APP_Textures[pno], ar);
		SDL_RenderCopyF( APP_ScreenRenderer, APP_Textures[pno], &src, &dst );
		SDL_SetTextureAlphaMod(APP_Textures[pno], SDL_ALPHA_OPAQUE);
	}
}

void APP_SetSecondaryOffset(int x, int y)
{
	APP_NewOffsetX = x;
	APP_NewOffsetY = y;
}

void APP_SetColorKeyRGB(int pno, int r, int g, int b)
{
	// TODO
	//  again because we have actual transparency in our assets, this is a no-op.
}

void APP_ResetFrameStep()
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

int APP_GetFPS()
{
	return APP_NowFPS;
}

int APP_GetRealFPS()
{
	return APP_FPS;
}

void APP_StrCpy(char *dest, const char *src)
{
	strcpy(dest, src);
}

void APP_StrCat(char *str1, const char *str2)
{
	strcat(str1, str2);
}

int APP_StrLen(const char *stri)
{
	return strlen(stri);
}

void APP_MidStr(const char *src, int start, int len, char *dest)
{
	int		i;
	for ( i = 0 ; i < len ; i ++ )
	{
		dest[i] = src[start - 1 + i];
	}
	dest[len] = '\0';
}

void APP_LeftStr(const char *src, int len, char *dest)
{
	APP_MidStr(src, 1, len, dest);
}

char APP_CharAt(const char *stri, int pos)
{
	return stri[pos];
}

int APP_ValLong(const char *stri)
{
	return atoi(stri);
}

void APP_FillMemory(void* buf, int size, int val)
{
	memset(buf, val, size);
}

////////////////////////////////////////////////////

static void APP_PrivateBDFFontInitialize()
{
	const char* const filenames[APP_BDFFONTFILE_MAX] = {
		"res/font/font10.bdf",
		"res/font/font12.bdf",
		"res/font/font16.bdf"
	};

	/* フォント読み込み */
	/* Load fonts */
	for (int i = 0; i < APP_BDFFONTFILE_MAX; i++) {
		if (APP_BDFFonts[i]) {
			continue;
		}
		SDL_RWops *src = APP_OpenRead(filenames[i]);
		if ( !src ) {
			SDL_Log("Failed to open file for font \"%s\"; continuing without it.", filenames[i]);
			APP_BDFFonts[i] = NULL;
			continue;
		}
		APP_BDFFonts[i] = APP_OpenBDFFont(src);
		if ( !APP_BDFFonts[i] ) {
			SDL_Log("Failed to load font \"%s\": %s\n", filenames[i], SDL_GetError());
			SDL_RWclose(src);
			APP_Exit(EXIT_FAILURE);
		}
		SDL_RWclose(src);
	}
}

static void APP_PrivateBDFFontFinalize()
{
	for (int i = 0; i < APP_BDFFONTFILE_MAX; i++) {
		if (APP_BDFFonts[i]) {
			APP_CloseBDFFont(APP_BDFFonts[i]);
		}
		APP_BDFFonts[i] = NULL;
	}
}

#ifdef APP_ENABLE_LINUX_GPIO
static int			APP_GPIORepeat[10];
static struct gpiod_chip	*APP_GPIOChip;
static struct gpiod_line	*APP_GPIOLines[10];
#endif

#ifdef APP_ENABLE_KEYBOARD
static int			APP_KeyRepeat[APP_KEY_MAX];
#endif

#ifdef APP_ENABLE_JOYSTICK
typedef struct APP_Joy
{
	SDL_Joystick* joystick;
	int numAxes;
	int numHats;
	int numButtons;
	int* axesRepeat;
	int* hatsRepeat;
	int* buttonsRepeat;
} APP_Joy;
#endif

#ifdef APP_ENABLE_GAME_CONTROLLER
typedef struct APP_Con
{
	SDL_GameController* controller;
	int axesRepeat[APP_CONAXIS_MAX];
	int buttonsRepeat[APP_CONBUTTON_MAX];
} APP_Con;
static int APP_LastActiveCon = -1;
#endif

#if defined(APP_ENABLE_JOYSTICK) || defined(APP_ENABLE_GAME_CONTROLLER)
typedef struct APP_PlayerSlot
{
	APP_PlayerSlotType type;
	union
	{
		#ifdef APP_ENABLE_JOYSTICK
		APP_Joy joy;
		#endif
		#ifdef APP_ENABLE_GAME_CONTROLLER
		APP_Con con;
		#endif
	};
} APP_PlayerSlot;

/*
Builtin controllers are controllers that can't be detached from the host system
(normally...), and are always active, even while docked; an external display is
attached and external controllers are attached. The builtin controller is
specially treated as always controlling player 1, with external controllers then
assigned player slots 1, 2, etc. So, in the case of a builtin controller and
only one attached external controller, both controllers control player 1, then
if one more external controller is attached, that controller controls player 2;
this is the behavior desired when docking a handheld system.

If a device has a builtin controller, but its host software automatically
detaches the controller upon docking, that device's builtin controller *should
not* be placed here. This list is *only* for when the builtin controller can
still be used even while external controllers are attached while docked.
*/
#define APP_CON_ID(vendor, product) (Uint32)(((Uint32)(Uint16)(vendor) << 16) | (Uint16)(product))
static Uint32 APP_BuiltinConIDs[] = {
	APP_CON_ID(0x28de, 0x1205) // Steam Deck
};
static APP_Con APP_BuiltinCon = { 0 };
static bool APP_LastInputBuiltinCon = true;

static APP_PlayerSlot* APP_PlayerSlots = NULL;
static int APP_NumPlayerSlots = 0;
#endif

APP_InputType APP_GetLastInputType ()
{
	#ifdef APP_ONLY_INPUT_TYPE
	return APP_ONLY_INPUT_TYPE;
	#else
	return APP_LastInputType;
	#endif
}

#ifdef APP_ENABLE_LINUX_GPIO
bool APP_IsPushGPIO ( int key )
{
	return key >= 0 && key < NUMBTNS && APP_GPIORepeat[key] == 1;
}

bool APP_IsPressGPIO ( int key )
{
	return key >= 0 && key < NUMBTNS && APP_GPIORepeat[key] != 0;
}

int APP_GetGPIORepeat( int key )
{
	return key >= 0 && key < NUMBTNS ? APP_GPIORepeat[key] : 0;
}
#endif

#ifdef APP_ENABLE_KEYBOARD
bool APP_IsPushKey (int key)
{
	return key >= 0 && key < APP_KEY_MAX && APP_KeyRepeat[key] == 1;
}

bool APP_IsPressKey (int key)
{
	return key >= 0 && key < APP_KEY_MAX && APP_KeyRepeat[key] != 0;
}

int APP_GetKeyRepeat( int key )
{
	return key >= 0 && key < APP_KEY_MAX ? APP_KeyRepeat[key] : 0;
}

int APP_GetMaxKey ()
{
	return APP_KEY_MAX;
}
#endif

#if defined(APP_ENABLE_JOYSTICK) || defined(APP_ENABLE_GAME_CONTROLLER)

static bool APP_IsCon ( int device )
{
	if (device < 0) {
		return false;
	}

	return
		(SDL_IsGameController(device) && SDL_GameControllerTypeForIndex(device) != SDL_CONTROLLER_TYPE_UNKNOWN) ||
		SDL_GameControllerMappingForGUID(SDL_JoystickGetDeviceGUID(device)) != NULL;
}

static bool APP_ResizePlayerSlots(int numSlots) {
	if (numSlots == APP_NumPlayerSlots) {
		return true;
	}
	else if (numSlots == 0) {
		free(APP_PlayerSlots);
		APP_PlayerSlots = NULL;
		APP_NumPlayerSlots = 0;
		return true;
	}

	APP_PlayerSlot* const oldSlots = APP_PlayerSlots;
	if (APP_PlayerSlots) {
		APP_PlayerSlots = realloc(APP_PlayerSlots, sizeof(APP_PlayerSlot) * numSlots);
	}
	else {
		APP_PlayerSlots = malloc(sizeof(APP_PlayerSlot) * numSlots);
	}
	if (!APP_PlayerSlots) {
		fprintf(stderr, "Failed to resize joysticks/controllers array\n");
		APP_PlayerSlots = oldSlots;
		return false;
	}
	for (int player = APP_NumPlayerSlots; player < numSlots; player++) {
		APP_PlayerSlots[player] = (APP_PlayerSlot) { 0 };
	}
	APP_NumPlayerSlots = numSlots;
	return true;
}

static bool APP_ConIsBuiltin(SDL_GameController* const controller) {
	for (size_t i = 0; i < SDL_arraysize(APP_BuiltinConIDs); i++) {
		const Uint16 vendor = SDL_GameControllerGetVendor(controller);
		const Uint16 product = SDL_GameControllerGetProduct(controller);
		const Uint32 conID = APP_CON_ID(vendor, product);
		if (conID == APP_BuiltinConIDs[i]) {
			return true;
		}
	}
	return false;
}

static bool APP_PlayerSlotsChanged() {
	#ifdef APP_ENABLE_GAME_CONTROLLER
	// This should never be needed, as the builtin controller most likely will
	// always be connected, but you never know, maybe a modder detached it.
	if (APP_BuiltinCon.controller && !SDL_GameControllerGetAttached(APP_BuiltinCon.controller)) {
		APP_BuiltinCon = (APP_Con) { 0 };
	}
	#endif
	for (int player = 0; player < APP_NumPlayerSlots; player++) {
		APP_PlayerSlot* const slot = &APP_PlayerSlots[player];
		switch (slot->type) {
		#ifdef APP_ENABLE_JOYSTICK
		case APP_PLAYERSLOT_JOY:
			if (!SDL_JoystickGetAttached(slot->joy.joystick)) {
				if (slot->joy.axesRepeat) free(slot->joy.axesRepeat);
				if (slot->joy.hatsRepeat) free(slot->joy.hatsRepeat);
				if (slot->joy.buttonsRepeat) free(slot->joy.buttonsRepeat);
				*slot = (APP_PlayerSlot) { 0 };
			}
			break;
		#endif

		#ifdef APP_ENABLE_GAME_CONTROLLER
		case APP_PLAYERSLOT_CON:
			if (!SDL_GameControllerGetAttached(slot->con.controller)) {
				*slot = (APP_PlayerSlot) { 0 };
			}
			break;
		#endif

		default:
		case APP_PLAYERSLOT_NULL:
			break;
		}
	}

	const int numJoys = SDL_NumJoysticks();

	// Only opened joysticks will have a non-NULL return value from
	// SDL_JoystickFromInstanceID(), so we can use it to skip already opened
	// devices.
	//
	// Internally, SDL uses joystick APIs for game controllers, so it also skips
	// already opened game controllers.

	#ifdef APP_ENABLE_GAME_CONTROLLER
	for (int device = 0; device < numJoys; device++) {
		const SDL_JoystickID id = SDL_JoystickGetDeviceInstanceID(device);
		if (id >= 0 && SDL_JoystickFromInstanceID(id)) {
			continue;
		}
		if (!APP_IsCon(device)) {
			continue;
		}

		SDL_GameController* const controller = SDL_GameControllerOpen(device);
		if (!controller) {
			fprintf(stderr, "Failed to open controller: %s\n", SDL_GetError());
			return false;
		}
		if (!APP_BuiltinCon.controller && APP_ConIsBuiltin(controller)) {
			APP_BuiltinCon.controller = controller;
			continue;
		}
		bool playerIndexFound = false;
		int player;
		for (player = 0; player < APP_NumPlayerSlots; player++) {
			if (APP_PlayerSlots[player].type == APP_PLAYERSLOT_NULL) {
				playerIndexFound = true;
				break;
			}
		}
		if (!playerIndexFound) {
			if (!APP_ResizePlayerSlots(APP_NumPlayerSlots + 1)) {
				fprintf(stderr, "Failed to open controller: %s\n", SDL_GetError());
				SDL_GameControllerClose(controller);
				return false;
			}
			player = APP_NumPlayerSlots - 1;
		}
		APP_PlayerSlots[player].con.controller = controller;
		APP_PlayerSlots[player].type = APP_PLAYERSLOT_CON;
	}
	#endif

	#ifdef APP_ENABLE_JOYSTICK
	for (int device = 0; device < numJoys; device++) {
		const SDL_JoystickID id = SDL_JoystickGetDeviceInstanceID(device);
		if (id >= 0 && SDL_JoystickFromInstanceID(id)) {
			continue;
		}
		if (APP_IsCon(device)) {
			continue;
		}

		SDL_Joystick* const joystick = SDL_JoystickOpen(device);
		if (!joystick) {
			fprintf(stderr, "Failed to open joystick: %s\n", SDL_GetError());
			goto fail;
		}
		bool playerIndexFound = false;
		int player;
		for (player = 0; player < APP_NumPlayerSlots; player++) {
			if (APP_PlayerSlots[player].type == APP_PLAYERSLOT_NULL) {
				playerIndexFound = true;
				break;
			}
		}
		if (!playerIndexFound) {
			if (!APP_ResizePlayerSlots(APP_NumPlayerSlots + 1)) {
				goto fail;
			}
			player = APP_NumPlayerSlots - 1;
		}

		if (
			(APP_PlayerSlots[player].joy.numAxes = SDL_JoystickNumAxes(joystick)) >= 0 &&
			(APP_PlayerSlots[player].joy.numHats = SDL_JoystickNumHats(joystick)) >= 0 &&
			(APP_PlayerSlots[player].joy.numButtons = SDL_JoystickNumButtons(joystick)) >= 0
		) {
			if (APP_PlayerSlots[player].joy.numAxes > 0 && !(APP_PlayerSlots[player].joy.axesRepeat = (int*)calloc((size_t)APP_PlayerSlots[player].joy.numAxes * 2, sizeof(int)))) {
				fprintf(stderr, "Failed to allocate axes repeat array for joystick\n");
				goto fail;
			}
			if (APP_PlayerSlots[player].joy.numHats > 0 && !(APP_PlayerSlots[player].joy.hatsRepeat = (int*)calloc((size_t)APP_PlayerSlots[player].joy.numHats * 4, sizeof(int)))) {
				fprintf(stderr, "Failed to allocate hats repeat array for joystick\n");
				goto fail;
			}
			if (APP_PlayerSlots[player].joy.numButtons > 0 && !(APP_PlayerSlots[player].joy.buttonsRepeat = (int*)calloc((size_t)APP_PlayerSlots[player].joy.numButtons, sizeof(int)))) {
				fprintf(stderr, "Failed to allocate buttons repeat array for joystick\n");
				goto fail;
			}
		}
		if (false) {
		fail:
			if (APP_PlayerSlots[player].joy.axesRepeat) free(APP_PlayerSlots[player].joy.axesRepeat);
			if (APP_PlayerSlots[player].joy.hatsRepeat) free(APP_PlayerSlots[player].joy.hatsRepeat);
			if (APP_PlayerSlots[player].joy.buttonsRepeat) free(APP_PlayerSlots[player].joy.buttonsRepeat);
			if (joystick) SDL_JoystickClose(joystick);
			return false;
		}
		APP_PlayerSlots[player].joy.joystick = joystick;
		APP_PlayerSlots[player].type = APP_PLAYERSLOT_JOY;
	}
	#endif

	int numRequiredSlots = 0;
	for (int player = 0; player < APP_NumPlayerSlots; player++) {
		if (APP_PlayerSlots[player].type != APP_PLAYERSLOT_NULL) {
			numRequiredSlots = player + 1;
		}
	}
	return APP_ResizePlayerSlots(numRequiredSlots);
}

static void APP_PlayerSlotsClose()
{
	if (APP_PlayerSlots)
	{
		for (int player = 0; player < APP_NumPlayerSlots; player++)
		{
			switch (APP_PlayerSlots[player].type)
			{
			#ifdef APP_ENABLE_JOYSTICK
			case APP_PLAYERSLOT_JOY: {
				APP_Joy* const joy = &APP_PlayerSlots[player].joy;
				if (SDL_JoystickGetAttached(joy->joystick))
				{
					SDL_JoystickClose(joy->joystick);
				}
				if (joy->axesRepeat) free(joy->axesRepeat);
				if (joy->hatsRepeat) free(joy->hatsRepeat);
				if (joy->buttonsRepeat) free(joy->buttonsRepeat);
				break;
			}
			#endif

			#ifdef APP_ENABLE_GAME_CONTROLLER
			case APP_PLAYERSLOT_CON: {
				if (SDL_GameControllerGetAttached(APP_PlayerSlots[player].con.controller))
				{
					SDL_GameControllerClose(APP_PlayerSlots[player].con.controller);
				}
				break;
			}
			#endif

			default:
			case APP_PLAYERSLOT_NULL:
				break;
			}
		}
		free(APP_PlayerSlots);
	}
	APP_PlayerSlots = NULL;
	APP_NumPlayerSlots = 0;
}

int APP_GetNumPlayerSlots()
{
	if (APP_NumPlayerSlots <= 0 && APP_BuiltinCon.controller) {
		return 1;
	}
	else {
		return APP_NumPlayerSlots;
	}
}

APP_PlayerSlotType APP_GetPlayerSlotType(int player)
{
	if (player == 0 && APP_BuiltinCon.controller && APP_LastInputBuiltinCon) {
		return APP_PLAYERSLOT_CON;
	}
	else if (player < 0 || player >= APP_NumPlayerSlots) {
		return APP_PLAYERSLOT_NULL;
	}
	else {
		return APP_PlayerSlots[player].type;
	}
}

#endif

#ifdef APP_ENABLE_JOYSTICK

static void APP_JoyInputsUpdate ()
{
	if (!APP_PlayerSlots) return;

	for (int player = 0; player < APP_NumPlayerSlots; player++)
	{
		if (
			APP_PlayerSlots[player].type != APP_PLAYERSLOT_JOY ||
			!SDL_JoystickGetAttached(APP_PlayerSlots[player].joy.joystick)
		) {
			continue;
		}

		const APP_JoyGUID checkGUID = APP_GetJoyGUID(player);
		const APP_JoyGUID zeroGUID = { 0 };
		if (memcmp(checkGUID.data, zeroGUID.data, sizeof(checkGUID.data)) == 0) continue;

		APP_Joy* const joy = &APP_PlayerSlots[player].joy;

		for (int axis = 0; axis < joy->numAxes; axis++)
		{
			const int value = SDL_JoystickGetAxis(joy->joystick, axis);

			if (value < -APP_DEADZONE_MAX)
			{
				if (++joy->axesRepeat[axis*2 + 0] == 1) APP_LastInputType = APP_INPUT_JOYSTICK;
			}
			else
			{
				joy->axesRepeat[axis*2 + 0] = 0;
			}

			if (value > +APP_DEADZONE_MAX)
			{
				if (++joy->axesRepeat[axis*2 + 1] == 1) APP_LastInputType = APP_INPUT_JOYSTICK;
			}
			else
			{
				joy->axesRepeat[axis*2 + 1] = 0;
			}
		}

		for (int hat = 0; hat < joy->numHats; hat++)
		{
			Uint8 value = SDL_JoystickGetHat(joy->joystick, hat);
			const Uint8 hatValues[4] =
			{
				SDL_HAT_LEFT,
				SDL_HAT_RIGHT,
				SDL_HAT_UP,
				SDL_HAT_DOWN
			};
			for (int valueIndex = 0; valueIndex < 4; valueIndex++)
			{
				if (value & hatValues[valueIndex])
				{
					if (++joy->hatsRepeat[hat*4 + valueIndex] == 1) APP_LastInputType = APP_INPUT_JOYSTICK;
				}
				else
				{
					joy->hatsRepeat[hat*4 + valueIndex] = 0;
				}
			}
		}

		for (int button = 0; button < joy->numButtons; button++)
		{
			if (SDL_JoystickGetButton(joy->joystick, button))
			{
				if (++joy->buttonsRepeat[button] == 1) APP_LastInputType = APP_INPUT_JOYSTICK;
			}
			else
			{
				joy->buttonsRepeat[button] = 0;
			}
		}
	}
}

bool APP_IsPushJoyKey ( const APP_JoyKey* const key )
{
	if (!APP_PlayerSlots || APP_NumPlayerSlots <= 0 || key == NULL || key->player >= APP_NumPlayerSlots) return false;

	int player = 0;
	int playerMax = 0;
	if (key->player >= 0)
	{
		APP_JoyGUID checkGUID = APP_GetJoyGUID(key->player);
		APP_JoyGUID zeroGUID = { 0 };
		if (memcmp(checkGUID.data, zeroGUID.data, sizeof(checkGUID.data)) != 0 && memcmp(key->guid.data, checkGUID.data, sizeof(checkGUID.data)) == 0)
		{
			player = key->player;
			playerMax = key->player + 1;
		}
	}
	else
	{
		player = 0;
		playerMax = APP_NumPlayerSlots;
	}
	for (; player < playerMax; player++)
	{
		APP_Joy* const joy = &APP_PlayerSlots[player].joy;

		switch (key->type)
		{
		case APP_JOYKEY_ANY:
			for (int axis = 0; axis < joy->numAxes; axis++)
			{
				if (joy->axesRepeat[axis*2 + 0] == 1) return true;
				if (joy->axesRepeat[axis*2 + 1] == 1) return true;
			}
			for (int hat = 0; hat < joy->numHats; hat++)
			{
				if (joy->hatsRepeat[hat] == 1) return true;
			}
			for (int button = 0; button < joy->numButtons; button++)
			{
				if (joy->buttonsRepeat[button] == 1) return true;
			}
			break;
		case APP_JOYKEY_AXIS:
			if (key->setting.index < joy->numAxes)
			{
				if (key->setting.value == -APP_DEADZONE_MAX)
				{
					return joy->axesRepeat[key->setting.index * 2 + 0] == 1;
				}
				else if (key->setting.value == +APP_DEADZONE_MAX)
				{
					return joy->axesRepeat[key->setting.index * 2 + 1] == 1;
				}
			}
			break;
		case APP_JOYKEY_HAT:
			if (key->setting.index < joy->numHats)
			{
				switch (key->setting.value)
				{
				case SDL_HAT_LEFT:
					return joy->hatsRepeat[key->setting.index * 4 + 0] == 1;
				case SDL_HAT_RIGHT:
					return joy->hatsRepeat[key->setting.index * 4 + 1] == 1;
				case SDL_HAT_UP:
					return joy->hatsRepeat[key->setting.index * 4 + 2] == 1;
				case SDL_HAT_DOWN:
					return joy->hatsRepeat[key->setting.index * 4 + 3] == 1;
				default:
					break;
				}
			}
			break;
		case APP_JOYKEY_BUTTON:
			if (key->setting.button < joy->numButtons) return joy->buttonsRepeat[key->setting.button] == 1;
			break;
		default:
			break;
		}
	}
	return false;
}

bool APP_IsPressJoyKey ( const APP_JoyKey* const key )
{
	if (!APP_PlayerSlots || APP_NumPlayerSlots <= 0 || key == NULL || key->player >= APP_NumPlayerSlots) return false;

	int player = 0;
	int playerMax = 0;
	if (key->player >= 0)
	{
		APP_JoyGUID checkGUID = APP_GetJoyGUID(key->player);
		APP_JoyGUID zeroGUID = { 0 };
		if (memcmp(checkGUID.data, zeroGUID.data, sizeof(checkGUID.data)) != 0 && memcmp(key->guid.data, checkGUID.data, sizeof(checkGUID.data)) == 0)
		{
			player = key->player;
			playerMax = key->player + 1;
		}
	}
	else
	{
		player = 0;
		playerMax = APP_NumPlayerSlots;
	}
	for (; player < playerMax; player++)
	{
		APP_Joy* const joy = &APP_PlayerSlots[player].joy;

		switch (key->type)
		{
		case APP_JOYKEY_ANY:
			for (int axis = 0; axis < joy->numAxes; axis++)
			{
				if (joy->axesRepeat[axis*2 + 0] != 0) return true;
				if (joy->axesRepeat[axis*2 + 1] != 0) return true;
			}
			for (int hat = 0; hat < joy->numHats; hat++)
			{
				if (joy->hatsRepeat[hat] != 0) return true;
			}
			for (int button = 0; button < joy->numButtons; button++)
			{
				if (joy->buttonsRepeat[button] != 0) return true;
			}
			break;
		case APP_JOYKEY_AXIS:
			if (key->setting.index < joy->numAxes)
			{
				if (key->setting.value == -APP_DEADZONE_MAX)
				{
					return joy->axesRepeat[key->setting.index * 2 + 0] != 0;
				}
				else if (key->setting.value == +APP_DEADZONE_MAX)
				{
					return joy->axesRepeat[key->setting.index * 2 + 1] != 0;
				}
			}
			break;
		case APP_JOYKEY_HAT:
			if (key->setting.index < joy->numHats)
			{
				switch (key->setting.value)
				{
				case SDL_HAT_LEFT:
					return joy->hatsRepeat[key->setting.index * 4 + 0] != 0;
				case SDL_HAT_RIGHT:
					return joy->hatsRepeat[key->setting.index * 4 + 1] != 0;
				case SDL_HAT_UP:
					return joy->hatsRepeat[key->setting.index * 4 + 2] != 0;
				case SDL_HAT_DOWN:
					return joy->hatsRepeat[key->setting.index * 4 + 3] != 0;
				default:
					break;
				}
			}
			break;
		case APP_JOYKEY_BUTTON:
			if (key->setting.button < joy->numButtons) return joy->buttonsRepeat[key->setting.button] != 0;
			break;
		default:
			break;
		}
	}
	return false;
}

int APP_GetJoyKeyRepeat ( const APP_JoyKey* const key )
{
	if (!APP_PlayerSlots || APP_NumPlayerSlots <= 0 || key == NULL || key->player >= APP_NumPlayerSlots) return 0;

	bool multi;
	int player = 0;
	int playerMax = 0;
	int maxRepeat = 0;
	if (key->player >= 0)
	{
		APP_JoyGUID checkGUID = APP_GetJoyGUID(key->player);
		APP_JoyGUID zeroGUID = { 0 };
		if (memcmp(checkGUID.data, zeroGUID.data, sizeof(checkGUID.data)) != 0 && memcmp(key->guid.data, checkGUID.data, sizeof(checkGUID.data)) == 0)
		{
			player = key->player;
			playerMax = key->player + 1;
			multi = false;
		}
	}
	else
	{
		player = 0;
		playerMax = APP_NumPlayerSlots;
		multi = true;
	}
	for (; player < playerMax; player++)
	{
		APP_Joy* const joy = &APP_PlayerSlots[player].joy;

		switch (key->type)
		{
		case APP_JOYKEY_ANY:
			for (int axis = 0; axis < joy->numAxes; axis++)
			{
				if (joy->axesRepeat[axis*2 + 0] > maxRepeat) maxRepeat = joy->axesRepeat[axis*2 + 0];
				if (joy->axesRepeat[axis*2 + 1] > maxRepeat) maxRepeat = joy->axesRepeat[axis*2 + 1];
			}
			for (int hat = 0; hat < joy->numHats; hat++)
			{
				if (joy->hatsRepeat[hat] > maxRepeat) maxRepeat = joy->hatsRepeat[hat];
			}
			for (int button = 0; button < joy->numButtons; button++)
			{
				if (joy->buttonsRepeat[button] > maxRepeat) maxRepeat = joy->buttonsRepeat[button];
			}
			break;
		case APP_JOYKEY_AXIS:
			if (key->setting.index < joy->numAxes)
			{
				if (key->setting.value == -APP_DEADZONE_MAX)
				{
					if (multi)
					{
						if (joy->axesRepeat[key->setting.index * 2 + 0] > maxRepeat) maxRepeat = joy->axesRepeat[key->setting.index * 2 + 0];
					}
					else return joy->axesRepeat[key->setting.index * 2 + 0];
				}
				else if (key->setting.value == +APP_DEADZONE_MAX)
				{
					if (multi)
					{
						if (joy->axesRepeat[key->setting.index * 2 + 1] > maxRepeat) maxRepeat = joy->axesRepeat[key->setting.index * 2 + 1];
					}
					else return joy->axesRepeat[key->setting.index * 2 + 1];
				}
			}
			break;
		case APP_JOYKEY_HAT:
			if (key->setting.index < joy->numHats)
			{
				switch (key->setting.value)
				{
				case SDL_HAT_LEFT:
					if (multi)
					{
						if (joy->hatsRepeat[key->setting.index * 4 + 0] > maxRepeat) maxRepeat = joy->hatsRepeat[key->setting.index * 4 + 0];
					}
					else return joy->hatsRepeat[key->setting.index * 4 + 0];
				case SDL_HAT_RIGHT:
					if (multi)
					{
						if (joy->hatsRepeat[key->setting.index * 4 + 1] > maxRepeat) maxRepeat = joy->hatsRepeat[key->setting.index * 4 + 1];
					}
					else return joy->hatsRepeat[key->setting.index * 4 + 1];
				case SDL_HAT_UP:
					if (multi)
					{
						if (joy->hatsRepeat[key->setting.index * 4 + 2] > maxRepeat) maxRepeat = joy->hatsRepeat[key->setting.index * 4 + 2];
					}
					else return joy->hatsRepeat[key->setting.index * 4 + 2];
				case SDL_HAT_DOWN:
					if (multi)
					{
						if (joy->hatsRepeat[key->setting.index * 4 + 3] > maxRepeat) maxRepeat = joy->hatsRepeat[key->setting.index * 4 + 3];
					}
					else return joy->hatsRepeat[key->setting.index * 4 + 3];
				default:
					break;
				}
			}
			break;
		case APP_JOYKEY_BUTTON:
			if (key->setting.button < joy->numButtons)
			{
				if (multi)
				{
					if (joy->buttonsRepeat[key->setting.button] > maxRepeat) maxRepeat = joy->buttonsRepeat[key->setting.button];
				}
				else return joy->buttonsRepeat[key->setting.button];
			}
			break;
		default:
			break;
		}
	}
	return maxRepeat;
}

int APP_GetNumJoys()
{
	int foundJoys = 0;
	for (int player = 0; player < APP_NumPlayerSlots; player++) {
		if (APP_PlayerSlots[player].type == APP_PLAYERSLOT_JOY) {
			foundJoys++;
		}
	}
	return foundJoys;
}

APP_JoyGUID APP_GetJoyGUID(int player)
{
	if (
		player < 0 ||
		APP_NumPlayerSlots <= 0 ||
		player >= APP_NumPlayerSlots ||
		APP_PlayerSlots[player].type != APP_PLAYERSLOT_JOY
	) {
		return (APP_JoyGUID) { 0 };
	}

	SDL_JoystickGUID sdlGUID;
	sdlGUID = SDL_JoystickGetGUID(APP_PlayerSlots[player].joy.joystick);

	APP_JoyGUID joyGUID = { 0 };
	for (int32_t i = 0; i < 4; i++)
	{
		joyGUID.data[i] = 0;
		for (int32_t j = 0; j < 4; j++)
		{
			joyGUID.data[i] |= (int32_t)((uint32_t)sdlGUID.data[i * 4 + j] << (j * 8));
		}
	}
	return joyGUID;
}

int APP_GetMaxJoyAxis(int player)
{
	if (player >= 0 && APP_NumPlayerSlots > 0 && player < APP_NumPlayerSlots)
	{
		if (APP_PlayerSlots[player].type == APP_PLAYERSLOT_JOY)
		{
			return APP_PlayerSlots[player].joy.numAxes;
		}
		else
		{
			return -1;
		}
	}
	else
	{
		return -1;
	}
}

int APP_GetMaxJoyHat(int player)
{
	if (player >= 0 && APP_NumPlayerSlots > 0 && player < APP_NumPlayerSlots)
	{
		if (APP_PlayerSlots[player].type == APP_PLAYERSLOT_JOY)
		{
			return APP_PlayerSlots[player].joy.numHats;
		}
		else
		{
			return -1;
		}
	}
	else
	{
		return -1;
	}
}

int APP_GetMaxJoyButton(int player)
{
	if (player >= 0 && APP_NumPlayerSlots > 0 && player < APP_NumPlayerSlots)
	{
		if (APP_PlayerSlots[player].type == APP_PLAYERSLOT_JOY)
		{
			return APP_PlayerSlots[player].joy.numButtons;
		}
		else
		{
			return -1;
		}
	}
	else
	{
		return -1;
	}
}
#endif

#ifdef APP_ENABLE_GAME_CONTROLLER

static SDL_GameControllerType APP_GetSDLGameControllerType(SDL_GameController* const controller)
{
	#ifdef APP_ONLY_SDL_CONTROLLER_TYPE
	return APP_ONLY_SDL_CONTROLLER_TYPE;
	#else
	return SDL_GameControllerGetType(controller);
	#endif
}

static void APP_ConInputsUpdate()
{
	bool backupLastInputBuiltinCon;
	if (APP_BuiltinCon.controller && SDL_GameControllerGetAttached(APP_BuiltinCon.controller)) {
		backupLastInputBuiltinCon = APP_LastInputBuiltinCon;
		APP_LastInputBuiltinCon = true;
		APP_InputType const inputType = APP_GetConType(-1);
		APP_LastInputBuiltinCon = backupLastInputBuiltinCon;
		Sint16 axisValue;

		axisValue = SDL_GameControllerGetAxis(APP_BuiltinCon.controller, SDL_CONTROLLER_AXIS_LEFTX);
		if (axisValue > 0 && axisValue > +APP_DEADZONE_MAX)
		{
			if (++APP_BuiltinCon.axesRepeat[0] == 1)
			{
				APP_LastInputType = inputType;
				APP_LastActiveCon = 0;
				APP_LastInputBuiltinCon = true;
			}
			APP_BuiltinCon.axesRepeat[1] = 0;
		}
		else if (axisValue < 0 && axisValue < -APP_DEADZONE_MAX)
		{
			APP_BuiltinCon.axesRepeat[0] = 0;
			if (++APP_BuiltinCon.axesRepeat[1] == 1)
			{
				APP_LastInputType = inputType;
				APP_LastActiveCon = 0;
				APP_LastInputBuiltinCon = true;
			}
		}
		else
		{
			APP_BuiltinCon.axesRepeat[0] = 0;
			APP_BuiltinCon.axesRepeat[1] = 0;
		}

		axisValue = SDL_GameControllerGetAxis(APP_BuiltinCon.controller, SDL_CONTROLLER_AXIS_LEFTY);
		if (axisValue > 0 && axisValue > +APP_DEADZONE_MAX)
		{
			if (++APP_BuiltinCon.axesRepeat[2] == 1)
			{
				APP_LastInputType = inputType;
				APP_LastActiveCon = 0;
				APP_LastInputBuiltinCon = true;
			}
			APP_BuiltinCon.axesRepeat[3] = 0;
		}
		else if (axisValue < 0 && axisValue < -APP_DEADZONE_MAX)
		{
			APP_BuiltinCon.axesRepeat[2] = 0;
			if (++APP_BuiltinCon.axesRepeat[3] == 1)
			{
				APP_LastInputType = inputType;
				APP_LastActiveCon = 0;
				APP_LastInputBuiltinCon = true;
			}
		}
		else
		{
			APP_BuiltinCon.axesRepeat[2] = 0;
			APP_BuiltinCon.axesRepeat[3] = 0;
		}

		axisValue = SDL_GameControllerGetAxis(APP_BuiltinCon.controller, SDL_CONTROLLER_AXIS_RIGHTX);
		if (axisValue > 0 && axisValue > +APP_DEADZONE_MAX)
		{
			if (++APP_BuiltinCon.axesRepeat[4] == 1)
			{
				APP_LastInputType = inputType;
				APP_LastActiveCon = 0;
				APP_LastInputBuiltinCon = true;
			}
			APP_BuiltinCon.axesRepeat[5] = 0;
		}
		else if (axisValue < 0 && axisValue < -APP_DEADZONE_MAX)
		{
			APP_BuiltinCon.axesRepeat[4] = 0;
			if (++APP_BuiltinCon.axesRepeat[5] == 1)
			{
				APP_LastInputType = inputType;
				APP_LastActiveCon = 0;
				APP_LastInputBuiltinCon = true;
			}
		}
		else
		{
			APP_BuiltinCon.axesRepeat[4] = 0;
			APP_BuiltinCon.axesRepeat[5] = 0;
		}

		axisValue = SDL_GameControllerGetAxis(APP_BuiltinCon.controller, SDL_CONTROLLER_AXIS_RIGHTY);
		if (axisValue > 0 && axisValue > +APP_DEADZONE_MAX)
		{
			if (++APP_BuiltinCon.axesRepeat[6] == 1)
			{
				APP_LastInputType = inputType;
				APP_LastActiveCon = 0;
				APP_LastInputBuiltinCon = true;
			}
			APP_BuiltinCon.axesRepeat[7] = 0;
		}
		else if (axisValue < 0 && axisValue < -APP_DEADZONE_MAX)
		{
			APP_BuiltinCon.axesRepeat[6] = 0;
			if (++APP_BuiltinCon.axesRepeat[7] == 1)
			{
				APP_LastInputType = inputType;
				APP_LastActiveCon = 0;
				APP_LastInputBuiltinCon = true;
			}
		}
		else
		{
			APP_BuiltinCon.axesRepeat[6] = 0;
			APP_BuiltinCon.axesRepeat[7] = 0;
		}

		axisValue = SDL_GameControllerGetAxis(APP_BuiltinCon.controller, SDL_CONTROLLER_AXIS_TRIGGERLEFT);
		if (axisValue > APP_DEADZONE_MAX)
		{
			if (++APP_BuiltinCon.axesRepeat[8] == 1)
			{
				APP_LastInputType = inputType;
				APP_LastActiveCon = 0;
				APP_LastInputBuiltinCon = true;
			}
		}
		else
		{
			APP_BuiltinCon.axesRepeat[8] = 0;
		}

		axisValue = SDL_GameControllerGetAxis(APP_BuiltinCon.controller, SDL_CONTROLLER_AXIS_TRIGGERRIGHT);
		if (axisValue > APP_DEADZONE_MAX)
		{
			if (++APP_BuiltinCon.axesRepeat[9] == 1)
			{
				APP_LastInputType = inputType;
				APP_LastActiveCon = 0;
				APP_LastInputBuiltinCon = true;
			}
		}
		else
		{
			APP_BuiltinCon.axesRepeat[9] = 0;
		}

		for (SDL_GameControllerButton button = 0; button < SDL_CONTROLLER_BUTTON_MAX; button++)
		{
			if (SDL_GameControllerGetButton(APP_BuiltinCon.controller, button))
			{
				if (++APP_BuiltinCon.buttonsRepeat[button] == 1)
				{
					APP_LastInputType = inputType;
					APP_LastActiveCon = 0;
					APP_LastInputBuiltinCon = true;
				}
			}
			else
			{
				APP_BuiltinCon.buttonsRepeat[button] = 0;
			}
		}
	}

	if (!APP_PlayerSlots) return;

	for (int player = 0; player < APP_NumPlayerSlots; player++)
	{
		APP_PlayerSlot* const slot = &APP_PlayerSlots[player];
		if (slot->type != APP_PLAYERSLOT_CON) {
			continue;
		}
		APP_Con* const con = &slot->con;

		if (!SDL_GameControllerGetAttached(con->controller)) continue;

		backupLastInputBuiltinCon = APP_LastInputBuiltinCon;
		APP_LastInputBuiltinCon = false;
		APP_InputType const inputType = APP_GetConType(player);
		APP_LastInputBuiltinCon = backupLastInputBuiltinCon;
		Sint16 axisValue;

		axisValue = SDL_GameControllerGetAxis(con->controller, SDL_CONTROLLER_AXIS_LEFTX);
		if (axisValue > 0 && axisValue > +APP_DEADZONE_MAX)
		{
			if (++con->axesRepeat[0] == 1)
			{
				APP_LastInputType = inputType;
				APP_LastActiveCon = player;
				if (player == 0) APP_LastInputBuiltinCon = false;
			}
			con->axesRepeat[1] = 0;
		}
		else if (axisValue < 0 && axisValue < -APP_DEADZONE_MAX)
		{
			con->axesRepeat[0] = 0;
			if (++con->axesRepeat[1] == 1)
			{
				APP_LastInputType = inputType;
				APP_LastActiveCon = player;
				if (player == 0) APP_LastInputBuiltinCon = false;
			}
		}
		else
		{
			con->axesRepeat[0] = 0;
			con->axesRepeat[1] = 0;
		}

		axisValue = SDL_GameControllerGetAxis(con->controller, SDL_CONTROLLER_AXIS_LEFTY);
		if (axisValue > 0 && axisValue > +APP_DEADZONE_MAX)
		{
			if (++con->axesRepeat[2] == 1)
			{
				APP_LastInputType = inputType;
				APP_LastActiveCon = player;
				if (player == 0) APP_LastInputBuiltinCon = false;
			}
			con->axesRepeat[3] = 0;
		}
		else if (axisValue < 0 && axisValue < -APP_DEADZONE_MAX)
		{
			con->axesRepeat[2] = 0;
			if (++con->axesRepeat[3] == 1)
			{
				APP_LastInputType = inputType;
				APP_LastActiveCon = player;
				if (player == 0) APP_LastInputBuiltinCon = false;
			}
		}
		else
		{
			con->axesRepeat[2] = 0;
			con->axesRepeat[3] = 0;
		}

		axisValue = SDL_GameControllerGetAxis(con->controller, SDL_CONTROLLER_AXIS_RIGHTX);
		if (axisValue > 0 && axisValue > +APP_DEADZONE_MAX)
		{
			if (++con->axesRepeat[4] == 1)
			{
				APP_LastInputType = inputType;
				APP_LastActiveCon = player;
				if (player == 0) APP_LastInputBuiltinCon = false;
			}
			con->axesRepeat[5] = 0;
		}
		else if (axisValue < 0 && axisValue < -APP_DEADZONE_MAX)
		{
			con->axesRepeat[4] = 0;
			if (++con->axesRepeat[5] == 1)
			{
				APP_LastInputType = inputType;
				APP_LastActiveCon = player;
				if (player == 0) APP_LastInputBuiltinCon = false;
			}
		}
		else
		{
			con->axesRepeat[4] = 0;
			con->axesRepeat[5] = 0;
		}

		axisValue = SDL_GameControllerGetAxis(con->controller, SDL_CONTROLLER_AXIS_RIGHTY);
		if (axisValue > 0 && axisValue > +APP_DEADZONE_MAX)
		{
			if (++con->axesRepeat[6] == 1)
			{
				APP_LastInputType = inputType;
				APP_LastActiveCon = player;
				if (player == 0) APP_LastInputBuiltinCon = false;
			}
			con->axesRepeat[7] = 0;
		}
		else if (axisValue < 0 && axisValue < -APP_DEADZONE_MAX)
		{
			con->axesRepeat[6] = 0;
			if (++con->axesRepeat[7] == 1)
			{
				APP_LastInputType = inputType;
				APP_LastActiveCon = player;
				if (player == 0) APP_LastInputBuiltinCon = false;
			}
		}
		else
		{
			con->axesRepeat[6] = 0;
			con->axesRepeat[7] = 0;
		}

		axisValue = SDL_GameControllerGetAxis(con->controller, SDL_CONTROLLER_AXIS_TRIGGERLEFT);
		if (axisValue > APP_DEADZONE_MAX)
		{
			if (++con->axesRepeat[8] == 1)
			{
				APP_LastInputType = inputType;
				APP_LastActiveCon = player;
				if (player == 0) APP_LastInputBuiltinCon = false;
			}
		}
		else
		{
			con->axesRepeat[8] = 0;
		}

		axisValue = SDL_GameControllerGetAxis(con->controller, SDL_CONTROLLER_AXIS_TRIGGERRIGHT);
		if (axisValue > APP_DEADZONE_MAX)
		{
			if (++con->axesRepeat[9] == 1)
			{
				APP_LastInputType = inputType;
				APP_LastActiveCon = player;
				if (player == 0) APP_LastInputBuiltinCon = false;
			}
		}
		else
		{
			con->axesRepeat[9] = 0;
		}

		for (SDL_GameControllerButton button = 0; button < SDL_CONTROLLER_BUTTON_MAX; button++)
		{
			if (SDL_GameControllerGetButton(con->controller, button))
			{
				if (++con->buttonsRepeat[button] == 1)
				{
					APP_LastInputType = inputType;
					APP_LastActiveCon = player;
					if (player == 0) APP_LastInputBuiltinCon = false;
				}
			}
			else
			{
				con->buttonsRepeat[button] = 0;
			}
		}
	}
}

static bool APP_IsPushConKeyPrivate ( const APP_Con* const con, const APP_ConKey* const key )
{
	switch (key->type)
	{
	case APP_CONKEY_ANY:
		for (int axis = 0; axis < APP_CONAXIS_MAX; axis++)
		{
			if (con->axesRepeat[axis] == 1) return true;
		}
		for (int button = 0; button < APP_CONBUTTON_MAX; button++)
		{
			if (con->buttonsRepeat[button] == 1) return true;
		}
		break;
	case APP_CONKEY_AXIS:
		if (key->index < APP_CONAXIS_MAX)
		{
			if (con->axesRepeat[key->index] == 1) return true;
		}
		break;
	case APP_CONKEY_BUTTON:
		if (key->index < APP_CONBUTTON_MAX)
		{
			if (con->buttonsRepeat[key->index] == 1) return true;
		}
		break;
	default:
		break;
	}
	return false;
}

bool APP_IsPushConKey ( const int player, const APP_ConKey* const key )
{
	if (!key) return false;
	if (player <= 0 && APP_BuiltinCon.controller && APP_IsPushConKeyPrivate(&APP_BuiltinCon, key)) return true;
	if (!APP_PlayerSlots || APP_NumPlayerSlots == 0 || player >= APP_NumPlayerSlots) return false;

	int playerStart = 0;
	int playerMax = 0;
	if (player >= 0)
	{
		playerStart = player;
		playerMax = player + 1;
	}
	else
	{
		playerStart = 0;
		playerMax = APP_NumPlayerSlots;
	}
	for (; playerStart < playerMax; playerStart++)
	{
		if (APP_PlayerSlots[playerStart].type != APP_PLAYERSLOT_CON) continue;
		APP_Con* const con = &APP_PlayerSlots[playerStart].con;
		if (APP_IsPushConKeyPrivate(con, key)) return true;
	}
	return false;
}

static bool APP_IsPressConKeyPrivate ( const APP_Con* const con, const APP_ConKey* const key)
{
	switch (key->type)
	{
	case APP_CONKEY_ANY:
		for (int axis = 0; axis < APP_CONAXIS_MAX; axis++)
		{
			if (con->axesRepeat[axis] != 0) return true;
		}
		for (int button = 0; button < APP_CONBUTTON_MAX; button++)
		{
			if (con->buttonsRepeat[button] != 0) return true;
		}
		break;
	case APP_CONKEY_AXIS:
		if (key->index < APP_CONAXIS_MAX)
		{
			if (con->axesRepeat[key->index] != 0) return true;
		}
		break;
	case APP_CONKEY_BUTTON:
		if (key->index < APP_CONBUTTON_MAX)
		{
			if (con->buttonsRepeat[key->index] != 0) return true;
		}
		break;
	default:
		break;
	}
	return false;
}

bool APP_IsPressConKey ( const int player, const APP_ConKey* const key )
{
	if (!key) return false;
	if (player <= 0 && APP_BuiltinCon.controller && APP_IsPressConKeyPrivate(&APP_BuiltinCon, key)) return true;
	if (!APP_PlayerSlots || APP_NumPlayerSlots == 0 || player >= APP_NumPlayerSlots) return false;

	int playerStart = 0;
	int playerMax = 0;
	if (player >= 0)
	{
		playerStart = player;
		playerMax = player + 1;
	}
	else
	{
		playerStart = 0;
		playerMax = APP_NumPlayerSlots;
	}
	for (; playerStart < playerMax; playerStart++)
	{
		if (APP_PlayerSlots[playerStart].type != APP_PLAYERSLOT_CON) continue;
		APP_Con* const con = &APP_PlayerSlots[playerStart].con;
		if (APP_IsPressConKeyPrivate(con, key)) return true;
	}
	return false;
}

static void APP_GetConKeyRepeatPrivate ( const APP_Con* const con, const APP_ConKey* const key, int* const maxRepeat)
{
	switch (key->type)
	{
	case APP_CONKEY_ANY:
		for (int axis = 0; axis < APP_CONAXIS_MAX; axis++)
		{
			if (con->axesRepeat[axis] > *maxRepeat) *maxRepeat = con->axesRepeat[axis];
		}
		for (int button = 0; button < APP_CONBUTTON_MAX; button++)
		{
			if (con->buttonsRepeat[button] > *maxRepeat) *maxRepeat = con->buttonsRepeat[button];
		}
		break;
	case APP_CONKEY_AXIS:
		if (key->index < APP_CONAXIS_MAX)
		{
			*maxRepeat = con->axesRepeat[key->index];
		}
		break;
	case APP_CONKEY_BUTTON:
		if (key->index < APP_CONBUTTON_MAX)
		{
			*maxRepeat = con->buttonsRepeat[key->index];
		}
		break;
	default:
		break;
	}
}

static void APP_GetConKeyRepeatMultiPrivate ( const APP_Con* const con, const APP_ConKey* const key, int* const maxRepeat)
{
	switch (key->type)
	{
	case APP_CONKEY_ANY:
		for (int axis = 0; axis < APP_CONAXIS_MAX; axis++)
		{
			if (con->axesRepeat[axis] > *maxRepeat) *maxRepeat = con->axesRepeat[axis];
		}
		for (int button = 0; button < APP_CONBUTTON_MAX; button++)
		{
			if (con->buttonsRepeat[button] > *maxRepeat) *maxRepeat = con->buttonsRepeat[button];
		}
		break;
	case APP_CONKEY_AXIS:
		if (key->index < APP_CONAXIS_MAX)
		{
			if (con->axesRepeat[key->index] > *maxRepeat) *maxRepeat = con->axesRepeat[key->index];
		}
		break;
	case APP_CONKEY_BUTTON:
		if (key->index < APP_CONBUTTON_MAX)
		{
			if (con->buttonsRepeat[key->index] > *maxRepeat) *maxRepeat = con->buttonsRepeat[key->index];
		}
		break;
	default:
		break;
	}
}

int APP_GetConKeyRepeat ( const int player, const APP_ConKey* const key )
{
	if (!key) return false;

	int maxRepeat = 0;
	if (player <= 0 && APP_BuiltinCon.controller)
	{
		APP_GetConKeyRepeatPrivate(&APP_BuiltinCon, key, &maxRepeat);
	}
	if (!APP_PlayerSlots || APP_NumPlayerSlots == 0 || player >= APP_NumPlayerSlots) return maxRepeat;

	bool multi;
	int playerStart = 0;
	int playerMax = 0;
	if (player >= 0)
	{
		playerStart = player;
		playerMax = player + 1;
		multi = false;
	}
	else
	{
		playerStart = 0;
		playerMax = APP_NumPlayerSlots;
		multi = true;
	}
	for (; playerStart < playerMax; playerStart++)
	{
		if (APP_PlayerSlots[playerStart].type != APP_PLAYERSLOT_CON) continue;
		APP_Con* const con = &APP_PlayerSlots[playerStart].con;
		if (multi) APP_GetConKeyRepeatMultiPrivate(con, key, &maxRepeat);
		else APP_GetConKeyRepeatPrivate(con, key, &maxRepeat);
	}
	return maxRepeat;
}

int APP_GetNumCons()
{
	int foundCons = 0;
	for (int player = 0; player < APP_NumPlayerSlots; player++) {
		if (APP_PlayerSlots[player].type == APP_PLAYERSLOT_CON) {
			foundCons++;
		}
	}
	if (foundCons == 0 && APP_BuiltinCon.controller) {
		foundCons = 1;
	}
	return foundCons;
}

void APP_ResetLastActiveCon()
{
	APP_LastActiveCon = -1;
}

int APP_GetLastActiveCon()
{
	return APP_LastActiveCon;
}

/*
 * Controller type constants were introduced in SDL 2.0.12, with later versions
 * introducing more.
 *
 * SDL 2.0.12:
 * SDL_CONTROLLER_TYPE_UNKNOWN
 * SDL_CONTROLLER_TYPE_XBOX360
 * SDL_CONTROLLER_TYPE_XBOXONE
 * SDL_CONTROLLER_TYPE_PS3
 * SDL_CONTROLLER_TYPE_PS4
 * SDL_CONTROLLER_TYPE_NINTENDO_SWITCH_PRO
 *
 * SDL 2.0.14:
 * SDL_CONTROLLER_TYPE_VIRTUAL
 * SDL_CONTROLLER_TYPE_PS5
 *
 * SDL 2.0.16:
 * SDL_CONTROLLER_TYPE_AMAZON_LUNA
 * SDL_CONTROLLER_TYPE_GOOGLE_STADIA
 *
 * SDL 2.24.0:
 * SDL_CONTROLLER_TYPE_NVIDIA_SHIELD
 * SDL_CONTROLLER_TYPE_NINTENDO_SWITCH_JOYCON_LEFT
 * SDL_CONTROLLER_TYPE_NINTENDO_SWITCH_JOYCON_RIGHT
 * SDL_CONTROLLER_TYPE_NINTENDO_SWITCH_JOYCON_PAIR
 */
APP_InputType APP_GetConType(const int player)
{
	SDL_GameController* controller = NULL;
	if (player <= 0 && APP_BuiltinCon.controller && APP_LastInputBuiltinCon) {
		controller = APP_BuiltinCon.controller;
	}
	else if (player >= 0 && player < APP_NumPlayerSlots && APP_PlayerSlots && APP_NumPlayerSlots > 0 && APP_PlayerSlots[player].type == APP_PLAYERSLOT_CON) {
		controller = APP_PlayerSlots[player].con.controller;
	}
	else {
		return APP_INPUT_NULL;
	}

	#ifdef APP_ONLY_INPUT_TYPE
	return APP_ONLY_INPUT_TYPE;
	#else
	switch(APP_GetSDLGameControllerType(controller))
	{
	default:
	case SDL_CONTROLLER_TYPE_XBOX360:
	case SDL_CONTROLLER_TYPE_XBOXONE:
		return APP_INPUT_XBOX;

	case SDL_CONTROLLER_TYPE_PS3:
	case SDL_CONTROLLER_TYPE_PS4:
		return APP_INPUT_PLAYSTATION;

	case SDL_CONTROLLER_TYPE_NINTENDO_SWITCH_PRO:
		return APP_INPUT_NINTENDO;

	#if SDL_VERSION_ATLEAST(2, 0, 14)
	case SDL_CONTROLLER_TYPE_VIRTUAL:
		return APP_INPUT_XBOX;

	case SDL_CONTROLLER_TYPE_PS5:
		return APP_INPUT_PLAYSTATION;
	#endif

	#if SDL_VERSION_ATLEAST(2, 0, 16)
	case SDL_CONTROLLER_TYPE_AMAZON_LUNA:
	case SDL_CONTROLLER_TYPE_GOOGLE_STADIA:
		return APP_INPUT_XBOX;
	#endif

	#if SDL_VERSION_ATLEAST(2, 24, 0)
	case SDL_CONTROLLER_TYPE_NVIDIA_SHIELD:
		return APP_INPUT_XBOX;

	case SDL_CONTROLLER_TYPE_NINTENDO_SWITCH_JOYCON_LEFT:
	case SDL_CONTROLLER_TYPE_NINTENDO_SWITCH_JOYCON_RIGHT:
	case SDL_CONTROLLER_TYPE_NINTENDO_SWITCH_JOYCON_PAIR:
		return APP_INPUT_NINTENDO;
	#endif
	}
	#endif
}

bool APP_GetConKeyDesc(const int player, const APP_ConKey* const key, const char** text, APP_Button* button)
{
	SDL_GameController* controller;
	if (player == 0 && APP_BuiltinCon.controller && APP_LastInputBuiltinCon)
	{
		controller = APP_BuiltinCon.controller;
	}
	else if (
		APP_PlayerSlots &&
		APP_NumPlayerSlots > 0 &&
		player >= 0 &&
		player < APP_NumPlayerSlots &&
		APP_PlayerSlots[player].type == APP_PLAYERSLOT_CON &&
		key &&
		text &&
		button
	)
	{
		controller = APP_PlayerSlots[player].con.controller;
	}
	else
	{
		return false;
	}

	*text = NULL;
	*button = APP_BUTTON_NULL;
	const SDL_GameControllerType sdlGameControllerType = APP_GetSDLGameControllerType(controller);
	switch (key->type)
	{
	case APP_CONKEY_AXIS:
		switch (key->index)
		{
		case 0:
			*text = "LS";
			*button = APP_BUTTON_RIGHT;
			break;
		case 1:
			*text = "LS";
			*button = APP_BUTTON_LEFT;
			break;
		case 2:
			*text = "LS";
			*button = APP_BUTTON_DOWN;
			break;
		case 3:
			*text = "LS";
			*button = APP_BUTTON_UP;
			break;
		case 4:
			*text = "RS";
			*button = APP_BUTTON_RIGHT;
			break;
		case 5:
			*text = "RS";
			*button = APP_BUTTON_LEFT;
			break;
		case 6:
			*text = "RS";
			*button = APP_BUTTON_DOWN;
			break;
		case 7:
			*text = "RS";
			*button = APP_BUTTON_UP;
			break;
		case 8:
			switch(sdlGameControllerType)
			{
			default:
			case SDL_CONTROLLER_TYPE_XBOX360:
			case SDL_CONTROLLER_TYPE_XBOXONE:
			#if SDL_VERSION_ATLEAST(2, 0, 14)
			case SDL_CONTROLLER_TYPE_VIRTUAL:
			#endif
			#if SDL_VERSION_ATLEAST(2, 0, 16)
			case SDL_CONTROLLER_TYPE_AMAZON_LUNA:
			#endif
			#if SDL_VERSION_ATLEAST(2, 24, 0)
			case SDL_CONTROLLER_TYPE_NVIDIA_SHIELD:
			#endif
				*text = "LT";
				break;

			case SDL_CONTROLLER_TYPE_PS3:
			case SDL_CONTROLLER_TYPE_PS4:
			#if SDL_VERSION_ATLEAST(2, 0, 14)
			case SDL_CONTROLLER_TYPE_PS5:
			#endif
			#if SDL_VERSION_ATLEAST(2, 0, 16)
			case SDL_CONTROLLER_TYPE_GOOGLE_STADIA:
			#endif
				*text = "L2";
				break;

			case SDL_CONTROLLER_TYPE_NINTENDO_SWITCH_PRO:
			#if SDL_VERSION_ATLEAST(2, 24, 0)
			case SDL_CONTROLLER_TYPE_NINTENDO_SWITCH_JOYCON_PAIR:
			#endif
				*text = "ZL";
				break;

			#if SDL_VERSION_ATLEAST(2, 24, 0)
			case SDL_CONTROLLER_TYPE_NINTENDO_SWITCH_JOYCON_LEFT:
			case SDL_CONTROLLER_TYPE_NINTENDO_SWITCH_JOYCON_RIGHT:
				*text = "SL";
				break;
			#endif
			}
			break;
		case 9:
			switch(sdlGameControllerType)
			{
			default:
			case SDL_CONTROLLER_TYPE_XBOX360:
			case SDL_CONTROLLER_TYPE_XBOXONE:
			#if SDL_VERSION_ATLEAST(2, 0, 14)
			case SDL_CONTROLLER_TYPE_VIRTUAL:
			#endif
			#if SDL_VERSION_ATLEAST(2, 0, 16)
			case SDL_CONTROLLER_TYPE_AMAZON_LUNA:
			#endif
			#if SDL_VERSION_ATLEAST(2, 24, 0)
			case SDL_CONTROLLER_TYPE_NVIDIA_SHIELD:
			#endif
				*text = "RT";
				break;

			case SDL_CONTROLLER_TYPE_PS3:
			case SDL_CONTROLLER_TYPE_PS4:
			#if SDL_VERSION_ATLEAST(2, 0, 14)
			case SDL_CONTROLLER_TYPE_PS5:
			#endif
			#if SDL_VERSION_ATLEAST(2, 0, 16)
			case SDL_CONTROLLER_TYPE_GOOGLE_STADIA:
			#endif
				*text = "R2";
				break;

			case SDL_CONTROLLER_TYPE_NINTENDO_SWITCH_PRO:
			#if SDL_VERSION_ATLEAST(2, 24, 0)
			case SDL_CONTROLLER_TYPE_NINTENDO_SWITCH_JOYCON_PAIR:
			#endif
				*text = "ZR";
				break;

			#if SDL_VERSION_ATLEAST(2, 24, 0)
			case SDL_CONTROLLER_TYPE_NINTENDO_SWITCH_JOYCON_LEFT:
			case SDL_CONTROLLER_TYPE_NINTENDO_SWITCH_JOYCON_RIGHT:
				*text = "SR";
				break;
			#endif
			}
			break;
		default:
			break;
		}
		break;
	case APP_CONKEY_BUTTON:
		switch (key->index)
		{
		case SDL_CONTROLLER_BUTTON_BACK:
		case SDL_CONTROLLER_BUTTON_START:
			// "Start" and "Back" are reserved for "Pause" and "Give Up", and may not be remapped.
			break;

		case SDL_CONTROLLER_BUTTON_GUIDE:
			*text = "HOME";
			break;

		case SDL_CONTROLLER_BUTTON_A:
		case SDL_CONTROLLER_BUTTON_B:
		case SDL_CONTROLLER_BUTTON_X:
		case SDL_CONTROLLER_BUTTON_Y:
			*button = APP_BUTTON_A + key->index;
			break;

		case SDL_CONTROLLER_BUTTON_DPAD_UP:
		case SDL_CONTROLLER_BUTTON_DPAD_DOWN:
		case SDL_CONTROLLER_BUTTON_DPAD_LEFT:
		case SDL_CONTROLLER_BUTTON_DPAD_RIGHT:
			*text = "DPAD";
			*button = APP_BUTTON_UP + key->index - SDL_CONTROLLER_BUTTON_DPAD_UP;
			break;

		case SDL_CONTROLLER_BUTTON_MISC1:
			switch(sdlGameControllerType)
			{
			case SDL_CONTROLLER_TYPE_XBOX360:
			case SDL_CONTROLLER_TYPE_XBOXONE:
				*text = "SHARE";
				break;

			case SDL_CONTROLLER_TYPE_NINTENDO_SWITCH_PRO:
			#if SDL_VERSION_ATLEAST(2, 24, 0)
			case SDL_CONTROLLER_TYPE_NINTENDO_SWITCH_JOYCON_LEFT:
			case SDL_CONTROLLER_TYPE_NINTENDO_SWITCH_JOYCON_PAIR:
			#endif
				*text = "CAPTURE";
				break;

			#if SDL_VERSION_ATLEAST(2, 0, 14)
			case SDL_CONTROLLER_TYPE_PS5:
			#if SDL_VERSION_ATLEAST(2, 0, 16)
			case SDL_CONTROLLER_TYPE_AMAZON_LUNA:
			case SDL_CONTROLLER_TYPE_GOOGLE_STADIA:
			#endif
			#if SDL_VERSION_ATLEAST(2, 24, 0)
			case SDL_CONTROLLER_TYPE_NVIDIA_SHIELD:
			#endif
				*text = "MICROPHONE";
				break;
			#endif

			#if SDL_VERSION_ATLEAST(2, 0, 14)
			case SDL_CONTROLLER_TYPE_VIRTUAL:
			default:
				*text = "MISC1";
				break;
			#endif
			}
			break;
		case SDL_CONTROLLER_BUTTON_PADDLE1:
			*text = "P1";
			break;
		case SDL_CONTROLLER_BUTTON_PADDLE2:
			*text = "P2";
			break;
		case SDL_CONTROLLER_BUTTON_PADDLE3:
			*text = "P3";
			break;
		case SDL_CONTROLLER_BUTTON_PADDLE4:
			*text = "P4";
			break;
		case SDL_CONTROLLER_BUTTON_TOUCHPAD:
			*text = "TOUCHPAD";
			break;
		case SDL_CONTROLLER_BUTTON_LEFTSTICK:
			switch(sdlGameControllerType)
			{
			default:
			case SDL_CONTROLLER_TYPE_XBOX360:
			case SDL_CONTROLLER_TYPE_XBOXONE:
			case SDL_CONTROLLER_TYPE_NINTENDO_SWITCH_PRO:
			#if SDL_VERSION_ATLEAST(2, 0, 14)
			case SDL_CONTROLLER_TYPE_VIRTUAL:
			#endif
			#if SDL_VERSION_ATLEAST(2, 24, 0)
			case SDL_CONTROLLER_TYPE_NVIDIA_SHIELD:
			#endif
			#if SDL_VERSION_ATLEAST(2, 24, 0)
			case SDL_CONTROLLER_TYPE_NINTENDO_SWITCH_JOYCON_LEFT:
			case SDL_CONTROLLER_TYPE_NINTENDO_SWITCH_JOYCON_RIGHT:
			case SDL_CONTROLLER_TYPE_NINTENDO_SWITCH_JOYCON_PAIR:
			#endif
				*text = "LS";
				break;

			case SDL_CONTROLLER_TYPE_PS3:
			case SDL_CONTROLLER_TYPE_PS4:
			#if SDL_VERSION_ATLEAST(2, 0, 14)
			case SDL_CONTROLLER_TYPE_PS5:
			#endif
			#if SDL_VERSION_ATLEAST(2, 0, 16)
			case SDL_CONTROLLER_TYPE_AMAZON_LUNA:
			case SDL_CONTROLLER_TYPE_GOOGLE_STADIA:
			#endif
				*text = "L3";
				break;
			}
			break;
		case SDL_CONTROLLER_BUTTON_RIGHTSTICK:
			switch(sdlGameControllerType)
			{
			default:
			case SDL_CONTROLLER_TYPE_XBOX360:
			case SDL_CONTROLLER_TYPE_XBOXONE:
			case SDL_CONTROLLER_TYPE_NINTENDO_SWITCH_PRO:
			#if SDL_VERSION_ATLEAST(2, 0, 14)
			case SDL_CONTROLLER_TYPE_VIRTUAL:
			#endif
			#if SDL_VERSION_ATLEAST(2, 24, 0)
			case SDL_CONTROLLER_TYPE_NVIDIA_SHIELD:
			case SDL_CONTROLLER_TYPE_NINTENDO_SWITCH_JOYCON_PAIR:
			#endif
				*text = "RS";
				break;

			case SDL_CONTROLLER_TYPE_PS3:
			case SDL_CONTROLLER_TYPE_PS4:
			#if SDL_VERSION_ATLEAST(2, 0, 14)
			case SDL_CONTROLLER_TYPE_PS5:
			#endif
			#if SDL_VERSION_ATLEAST(2, 0, 16)
			case SDL_CONTROLLER_TYPE_AMAZON_LUNA:
			case SDL_CONTROLLER_TYPE_GOOGLE_STADIA:
			#endif
				*text = "R3";
				break;
			}
			break;
		case SDL_CONTROLLER_BUTTON_LEFTSHOULDER:
			switch(sdlGameControllerType)
			{
			default:
			case SDL_CONTROLLER_TYPE_XBOX360:
			case SDL_CONTROLLER_TYPE_XBOXONE:
			#if SDL_VERSION_ATLEAST(2, 0, 14)
			case SDL_CONTROLLER_TYPE_VIRTUAL:
			#endif
			#if SDL_VERSION_ATLEAST(2, 0, 16)
			case SDL_CONTROLLER_TYPE_AMAZON_LUNA:
			#endif
			#if SDL_VERSION_ATLEAST(2, 24, 0)
			case SDL_CONTROLLER_TYPE_NVIDIA_SHIELD:
			#endif
				*text = "LB";
				break;

			case SDL_CONTROLLER_TYPE_PS3:
			case SDL_CONTROLLER_TYPE_PS4:
			#if SDL_VERSION_ATLEAST(2, 0, 14)
			case SDL_CONTROLLER_TYPE_PS5:
			#endif
			#if SDL_VERSION_ATLEAST(2, 0, 16)
			case SDL_CONTROLLER_TYPE_GOOGLE_STADIA:
			#endif
				*text = "L1";
				break;

			case SDL_CONTROLLER_TYPE_NINTENDO_SWITCH_PRO:
			#if SDL_VERSION_ATLEAST(2, 24, 0)
			case SDL_CONTROLLER_TYPE_NINTENDO_SWITCH_JOYCON_PAIR:
			#endif
				*text = "L";
				break;
			}
			break;
		case SDL_CONTROLLER_BUTTON_RIGHTSHOULDER:
			switch(sdlGameControllerType)
			{
			default:
			case SDL_CONTROLLER_TYPE_XBOX360:
			case SDL_CONTROLLER_TYPE_XBOXONE:
			#if SDL_VERSION_ATLEAST(2, 0, 14)
			case SDL_CONTROLLER_TYPE_VIRTUAL:
			#endif
			#if SDL_VERSION_ATLEAST(2, 0, 16)
			case SDL_CONTROLLER_TYPE_AMAZON_LUNA:
			#endif
			#if SDL_VERSION_ATLEAST(2, 24, 0)
			case SDL_CONTROLLER_TYPE_NVIDIA_SHIELD:
			#endif
				*text = "RB";
				break;

			case SDL_CONTROLLER_TYPE_PS3:
			case SDL_CONTROLLER_TYPE_PS4:
			#if SDL_VERSION_ATLEAST(2, 0, 14)
			case SDL_CONTROLLER_TYPE_PS5:
			#endif
			#if SDL_VERSION_ATLEAST(2, 0, 16)
			case SDL_CONTROLLER_TYPE_GOOGLE_STADIA:
			#endif
				*text = "R1";
				break;

			case SDL_CONTROLLER_TYPE_NINTENDO_SWITCH_PRO:
			#if SDL_VERSION_ATLEAST(2, 24, 0)
			case SDL_CONTROLLER_TYPE_NINTENDO_SWITCH_JOYCON_PAIR:
			#endif
				*text = "R";
				break;
			}
			break;
		default:
			break;
		}
		break;
	default:
		break;
	}
	return *text != NULL || *button != APP_BUTTON_NULL;
}
#endif

static bool APP_OpenInputs()
{
	#ifdef ENABLE_LINUXGPIO
	const char* chipName = "gpiochip0";

	APP_GPIOChip = gpiod_chip_open_by_name(chipName);
	if ( !APP_GPIOChip )
	{
		fprintf(stderr, "Failed opening GPIO chip \"%s\". Continuing without GPIO input support.\n", chipName);
	}
	else
	{
		memset(APP_GPIORepeat, 0, sizeof(APP_GPIORepeat));
		memset(APP_GPIOLines, 0, sizeof(APP_GPIOLines));
		if (
			!(APP_GPIOLines[0] = gpiod_chip_get_line(APP_GPIOChip,  5)) ||
			!(APP_GPIOLines[1] = gpiod_chip_get_line(APP_GPIOChip, 13)) ||
			!(APP_GPIOLines[2] = gpiod_chip_get_line(APP_GPIOChip,  6)) ||
			!(APP_GPIOLines[3] = gpiod_chip_get_line(APP_GPIOChip, 12)) ||
			!(APP_GPIOLines[4] = gpiod_chip_get_line(APP_GPIOChip, 19)) ||
			!(APP_GPIOLines[5] = gpiod_chip_get_line(APP_GPIOChip, 16)) ||
			!(APP_GPIOLines[6] = gpiod_chip_get_line(APP_GPIOChip, 26)) ||
			!(APP_GPIOLines[7] = gpiod_chip_get_line(APP_GPIOChip, 20)) ||
			!(APP_GPIOLines[8] = gpiod_chip_get_line(APP_GPIOChip, 21)) ||
			!(APP_GPIOLines[9] = gpiod_chip_get_line(APP_GPIOChip,  4))
		)
		{
			for (int line = 0; line < 10; line++)
			{
				if (APP_GPIOLines[line])
				{
					gpiod_line_release(APP_GPIOLines[line]);
					APP_GPIOLines[line] = NULL;
				}
			}
			gpiod_chip_close(APP_GPIOChip);
			APP_GPIOChip = NULL;
			fprintf(stderr, "Failed opening GPIO lines. Continuing without GPIO input support.\n");
		}
		else
		{
			for (int line = 0; line < 10; line++)
			{
				if (gpiod_line_request_input(APP_GPIOLines[line], "input") < 0)
				{
					for (line = 0; line < 10; line++)
					{
						gpiod_line_release(APP_GPIOLines[line]);
						APP_GPIOLines[line] = NULL;
					}
					gpiod_chip_close(APP_GPIOChip);
					APP_GPIOChip = NULL;
					fprintf(stderr, "Failed setting GPIO lines for input. Continuing without GPIO input support.\n");
					break;
				}
			}
		}
	}
	#endif

	#ifdef APP_ENABLE_KEYBOARD
	/* キーリピートバッファ初期化 */
	memset(APP_KeyRepeat, 0, sizeof(APP_KeyRepeat));
	#endif

	/* パッドの初期化 */
	#ifdef APP_APP_ENABLE_GAME_CONTROLLER_DB
	// The game will just go without the database if it's missing or fails to load.
	SDL_RWops* db = APP_OpenRead("gamecontrollerdb.txt");
	if (db) {
		if (SDL_RWsize(db) > 0) {
			SDL_GameControllerAddMappingsFromRW(db, 0);
		}
		SDL_RWclose(db);
	}
	#endif
	#ifdef APP_ENABLE_GAME_CONTROLLER
	SDL_SetHint(SDL_HINT_GAMECONTROLLER_USE_BUTTON_LABELS, "0");
	#endif

	return true;
}

static void APP_CloseInputs()
{
	#ifdef APP_ENABLE_LINUX_GPIO
	if ( APP_GPIOChip )
	{
		for ( int i = 0 ; i < 10 ; i ++ )
		{
			gpiod_line_release(APP_GPIOLines[i]);
		}
		gpiod_chip_close(APP_GPIOChip);

		APP_GPIOChip = NULL;
		memset(APP_GPIOLines, 0, sizeof(APP_GPIOLines));
	}
	#endif

	#ifdef APP_ENABLE_KEYBOARD
	memset(APP_KeyRepeat, 0, sizeof(APP_KeyRepeat));
	#endif

	#if defined(APP_ENABLE_JOYSTICK) || defined(APP_ENABLE_GAME_CONTROLLER)
	#ifdef APP_ENABLE_GAME_CONTROLLER
	if (APP_BuiltinCon.controller && SDL_GameControllerGetAttached(APP_BuiltinCon.controller)) {
		SDL_GameControllerClose(APP_BuiltinCon.controller);
		APP_BuiltinCon.controller = NULL;
	}
	#endif
	APP_PlayerSlotsClose();
	#endif
}

void APP_InputsUpdate()
{
	#ifdef APP_ENABLE_LINUX_GPIO
	for (int line = 0; line < 10; line++)
	{
		if (gpiod_line_get_value(APP_GPIOLines[line]) == 1)
		{
			if (++APP_GPIORepeat[line] == 1) LastControllerType = APP_INPUT_LINUXGPIO;
		}
		else
		{
			APP_GPIORepeat[line] = 0;
		}
	}
	#endif

	#ifdef APP_ENABLE_KEYBOARD
	int numKeys = 0;
	const Uint8* keyStates = SDL_GetKeyboardState(&numKeys);

	for (int i = 0; i < APP_KEY_MAX; i++)
	{
		if (i < numKeys && keyStates[i] == SDL_PRESSED)
		{
			if (++APP_KeyRepeat[i] == 1) APP_LastInputType = APP_INPUT_KEYBOARD;
		}
		else
		{
			APP_KeyRepeat[i] = 0;
		}
	}
	#endif

	#ifdef APP_ENABLE_JOYSTICK
	APP_JoyInputsUpdate();
	#endif

	#ifdef APP_ENABLE_GAME_CONTROLLER
	APP_ConInputsUpdate();
	#endif
}
