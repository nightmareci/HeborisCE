// TODO: Refactor this into multiple sources for each subsystem. E.g., move
// video-related stuff into APP_video.h/APP_video.c, audio-related stuff into
// APP_audio.h/APP_audio.c, etc..

#include "APP.h"
#include "APP_bdf.h"
#include "APP_filesystem.h"
#include "nanotime.h"

#define STB_IMAGE_STATIC
#define STBI_NO_THREAD_LOCALS
#define STBI_FAILURE_USERMSG
#if defined(SDL_NEON_INTRINSICS)
#define STBI_NEON
#endif
#define STBI_ONLY_PNG
#define STBI_NO_HDR
#define STBI_NO_LINEAR
#define STBI_NO_STDIO
#define STBI_ASSERT SDL_assert
#define STBI_MALLOC SDL_malloc
#define STBI_REALLOC SDL_realloc
#define STBI_FREE SDL_free
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define		APP_GAME_CAPTION		"Heboris C.E."

#define		APP_TEXTURE_MAX		100
#define		APP_WAVE_MAX		100
#define		APP_TEXTLAYER_MAX	16

#if 0
// TODO: Implement audio
#define 	APP_VOLUME_MAX		MIX_MAX_VOLUME
#endif

#define		APP_WIDE_SCREEN (4.0f / 3.0f)
#define		APP_NARROW_SCREEN (3.0f / 4.0f)

#define APP_TEXTLAYER_STRING_MAX 256

typedef struct APP_TextLayer
{
	bool enable;
	int x;
	int y;
	SDL_Color color;
	int size;
	char string[APP_TEXTLAYER_STRING_MAX];
	bool updateTexture;
	SDL_Texture* texture;
	float textureW;
	float textureH;
} APP_TextLayer;

typedef struct APP_Wave
{
	SDL_AudioStream* stream;
	void* data;
	bool looping;
} APP_Wave;

static bool 			APP_InitFast = false;
static SDL_Window		*APP_ScreenWindow = NULL;
static SDL_Renderer		*APP_ScreenRenderer = NULL;
static SDL_Texture		*APP_ScreenRenderTarget = NULL;
static float			APP_ScreenSubpixelOffset = 0.0f;

static SDL_Texture		*APP_Textures[APP_TEXTURE_MAX];

static APP_Wave			APP_Waves[APP_WAVE_MAX];
static SDL_AudioDeviceID	APP_AudioDevice;
static APP_Wave			APP_Music;
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

#ifdef APP_ONLY_INPUT_TYPE
static APP_InputType APP_LastInputType = APP_ONLY_INPUT_TYPE;
#else
static APP_InputType APP_LastInputType = APP_INPUT_NULL;
#endif

static void APP_PrivateBDFFontFinalize();
static void APP_PrivateBDFFontInitialize();
static bool APP_OpenInputs();
static void APP_CloseInputs();
static bool APP_PlayerSlotsChanged();

static float APP_GetScreenSubpixelOffset(void)
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
	// -Brandon McGriff <nightmareci@gmail.com>
	if ( APP_ScreenRenderer )
	{
		int w;
		if (!SDL_GetRenderLogicalPresentation(APP_ScreenRenderer, &w, NULL, NULL)) {
			APP_Exit(EXIT_FAILURE);
		}
		SDL_FRect rect;
		if (!SDL_GetRenderLogicalPresentationRect(APP_ScreenRenderer, &rect)) {
			APP_Exit(EXIT_FAILURE);
		}
		const float scale = rect.w / w;
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

	if (!APP_InitFast) {
		APP_WaveFormatsSupported[APP_WAVE_WAV] = true;
	}
	APP_QuitLevel = 0;

#ifdef __EMSCRIPTEN__
	/* Keyboard input gets a bit broken without this hint set. */
	//SDL_SetHint(SDL_HINT_EMSCRIPTEN_KEYBOARD_ELEMENT, "#canvas");
#endif

	/* SDLの初期化 || SDL initialization */
	if ( !APP_InitFast && !SDL_Init(
					SDL_INIT_AUDIO | SDL_INIT_VIDEO
					#ifdef APP_ENABLE_JOYSTICK
					| SDL_INIT_JOYSTICK
					#endif
					#ifdef APP_ENABLE_GAME_CONTROLLER
					| SDL_INIT_GAMEPAD
					#endif
	))
	{
		fprintf(stderr, "Couldn't initialize SDL: %s\n", SDL_GetError());
		APP_Exit(EXIT_FAILURE);
	}

	// If this fails, it doesn't matter, the game will still work. But it's
	// called because if it works, the game might perform better.
	if (!APP_InitFast) SDL_SetCurrentThreadPriority(SDL_THREAD_PRIORITY_HIGH);

	APP_QuitLevel++;

	if (!APP_InitFast) {
		APP_AudioDevice = SDL_OpenAudioDevice(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, NULL);
		if (!APP_AudioDevice) {
			fprintf(stderr, "Couldn't open audio device: %s\n", SDL_GetError());
			APP_Exit(EXIT_FAILURE);
		}
	}
	APP_QuitLevel++;

	int configChanged = 0;

	APP_NewOffsetX = 0;
	APP_NewOffsetY = 0;
	APP_OffsetX = 0;
	APP_OffsetY = 0;

	APP_SetFPS(60);

	if (!APP_InitFast) APP_OpenInputs();

	/* テクスチャ領域の初期化 || Initialize the texture pointers */
	if (!APP_InitFast) SDL_memset(APP_Textures, 0, sizeof(APP_Textures));

	/* サウンドの初期化 || Initialize sound data */
	for ( int i = 0 ; i < APP_WAVE_MAX ; i ++ )
	{
		if ( !APP_InitFast )
		{
			APP_Waves[i] = (APP_Wave) { 0 };
		}
	}

	if ( !APP_InitFast ) APP_Music = (APP_Wave) { 0 };

	/* テキストレイヤーの初期化 || Initialize the text layers */
	for ( int i = 0 ; i < APP_TEXTLAYER_MAX ; i ++ )
	{
		SDL_memset(&APP_TextLayers[i], 0, sizeof(APP_TextLayer));
		APP_TextLayers[i].color.r = APP_TextLayers[i].color.g = APP_TextLayers[i].color.b = 255;
		APP_TextLayers[i].color.a = SDL_ALPHA_OPAQUE;
		APP_TextLayers[i].size = 16;
	}

	if ( !APP_InitFast ) APP_PrivateBDFFontInitialize();

	APP_LastFrameSkipped = false;
	APP_FPSCount = 0u;
	APP_FPS			= 0;
	APP_NoFrameskip		= false;

	SDL_srand(SDL_GetTicksNS());
	APP_InitFast = true;
}

void APP_Quit(void)
{
	APP_CloseInputs();

	/* サウンドの解放 */
	for ( int i = 0 ; i < APP_WAVE_MAX ; i ++ )
	{
		SDL_DestroyAudioStream(APP_Waves[i].stream);
		SDL_free(APP_Waves[i].data);
		APP_Waves[i] = (APP_Wave) { 0 };
	}
	SDL_DestroyAudioStream(APP_Music.stream);
	SDL_free(APP_Music.data);

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
		SDL_memset(APP_TextLayers, 0, sizeof(APP_TextLayers));

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
	case 2: SDL_CloseAudioDevice(APP_AudioDevice);
	case 1: SDL_Quit();
	default: break;
	}
	APP_QuitLevel = 0;

	APP_InitFast = false;
}

bool APP_Update(void)
{
	if ( APP_ScreenRenderer )
	{
		SDL_FlushRenderer( APP_ScreenRenderer );

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
				SDL_RenderTexture(APP_ScreenRenderer, APP_ScreenRenderTarget, NULL, NULL);
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
					SDL_RenderTexture(APP_ScreenRenderer, APP_ScreenRenderTarget, NULL, NULL);
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
	bool showCursor = false;
	#if defined(APP_ENABLE_JOYSTICK) || defined(APP_ENABLE_GAME_CONTROLLER)
	bool slotsChanged = false;
	#endif
	while (SDL_PeepEvents(&ev, 1, SDL_GETEVENT, 0, SDL_EVENT_LAST) == 1)
	{
		switch(ev.type){
			// ウィンドウの×ボタンが押された時など || When the window's X-button is pressed, etc.
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
		SDL_ShowCursor();
		APP_CursorCounting = 0u;
	}

	if (SDL_CursorVisible() && APP_CursorCounting++ >= APP_NowFPS)
	{
		SDL_HideCursor();
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

bool APP_IsPlayMusic(void)
{
	return APP_Music.stream && SDL_GetAudioStreamAvailable(APP_Music.stream) > 0;
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

#ifdef NDEBUG
#define SET_SCREEN_ERROR goto fail
#else
#define SET_SCREEN_ERROR SDL_Log("APP_SetScreen error: %d", __LINE__); goto fail
#endif

bool APP_SetScreen(APP_ScreenModeFlag *screenMode, int32_t *screenIndex)
{
	int windowX, windowY;
	int logicalWidth, logicalHeight;
	SDL_DisplayID* displays = NULL;
	SDL_DisplayMode** displayModes = NULL;

	/* 画面の設定 || Set up the screen */

	/* Validate the window type */
	APP_ScreenModeFlag windowType = *screenMode & APP_SCREENMODE_WINDOWTYPE;
	if ( windowType < 0 || windowType >= APP_SCREENMODE_NUMWINDOWTYPES )
	{
		SET_SCREEN_ERROR;
	}

	int displayIndex = APP_SCREENINDEX_DISPLAY_TOVALUE(*screenIndex);
	int modeIndex = APP_SCREENINDEX_MODE_TOVALUE(*screenIndex);
	int displayCount;
	displays = SDL_GetDisplays(&displayCount);
	if (!displays) {
	    SET_SCREEN_ERROR;
	}
	const SDL_DisplayID display = displays[displayIndex];
	SDL_free(displays);
	displays = NULL;
	int displayModeCount;
	displayModes = SDL_GetFullscreenDisplayModes(display, &displayModeCount);
	if (!displayModes) {
	    SET_SCREEN_ERROR;
	}

	windowX = SDL_WINDOWPOS_CENTERED_DISPLAY(display);
	windowY = SDL_WINDOWPOS_CENTERED_DISPLAY(display);
	if (
		displayIndex >= displayCount ||
		((windowType == APP_SCREENMODE_FULLSCREEN || windowType == APP_SCREENMODE_FULLSCREEN_DESKTOP) && modeIndex >= displayModeCount)
	) {
		*screenMode = APP_DEFAULT_SCREEN_MODE;
		*screenIndex = 0;
	}
	const bool maximized = windowType == APP_SCREENMODE_WINDOW_MAXIMIZED;

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
		if ( !APP_ScreenWindow )
		{
			SDL_PropertiesID props = SDL_CreateProperties();
			if (!props) {
			    SET_SCREEN_ERROR;
			}
			if (
				!SDL_SetStringProperty(props, SDL_PROP_WINDOW_CREATE_TITLE_STRING, APP_GAME_CAPTION) ||
				!SDL_SetBooleanProperty(props, SDL_PROP_WINDOW_CREATE_FULLSCREEN_BOOLEAN, true) ||
				!SDL_SetNumberProperty(props, SDL_PROP_WINDOW_CREATE_X_NUMBER, windowX) ||
				!SDL_SetNumberProperty(props, SDL_PROP_WINDOW_CREATE_Y_NUMBER, windowY) ||
				!SDL_SetNumberProperty(props, SDL_PROP_WINDOW_CREATE_WIDTH_NUMBER, logicalWidth) ||
				!SDL_SetNumberProperty(props, SDL_PROP_WINDOW_CREATE_HEIGHT_NUMBER, logicalHeight) ||
				!SDL_SetBooleanProperty(props, SDL_PROP_WINDOW_CREATE_HIDDEN_BOOLEAN, true) ||
				!SDL_SetBooleanProperty(props, SDL_PROP_WINDOW_CREATE_RESIZABLE_BOOLEAN, true)
			) {
				SET_SCREEN_ERROR;
			}
			APP_ScreenWindow = SDL_CreateWindowWithProperties(props);
			SDL_DestroyProperties(props);
			if ( !APP_ScreenWindow || (windowType == APP_SCREENMODE_FULLSCREEN && !SDL_SetWindowFullscreenMode(APP_ScreenWindow, displayModes[modeIndex])))
			{
				SET_SCREEN_ERROR;
			}
		}
		else
		{
			if (
				!SDL_HideWindow(APP_ScreenWindow) ||
				!SDL_SetWindowFullscreen(APP_ScreenWindow, true) ||
				!SDL_SetWindowSize(APP_ScreenWindow, logicalWidth, logicalHeight) ||
				!SDL_SetWindowFullscreenMode(APP_ScreenWindow, (windowType == APP_SCREENMODE_FULLSCREEN) ? displayModes[modeIndex] : NULL)
			) {
				SET_SCREEN_ERROR;
			}
			// NOTE: This can fail on some platforms due to not being supported (Wayland), so don't check the return status
			SDL_SetWindowPosition(APP_ScreenWindow, windowX, windowY);
		}
	}
	else if (
		windowType == APP_SCREENMODE_WINDOW ||
		windowType == APP_SCREENMODE_WINDOW_MAXIMIZED
	)
	{
		const SDL_DisplayMode* const displayMode = SDL_GetDesktopDisplayMode(display);
		if (!displayMode) {
			SET_SCREEN_ERROR;
		}
		int maxScale;
		if ( displayMode->w <= logicalWidth || displayMode->h <= logicalHeight )
		{
			maxScale = 1;
		}
		else if ( displayMode->w > displayMode->h )
		{
			maxScale = (displayMode->h / logicalHeight) - (displayMode->h % logicalHeight == 0);
		}
		else
		{
			maxScale = (displayMode->w / logicalWidth) - (displayMode->w % logicalWidth == 0);
		}
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
			SDL_PropertiesID props = SDL_CreateProperties();
			if (!props) {
			    SET_SCREEN_ERROR;
			}
			if (
				!SDL_SetStringProperty(props, SDL_PROP_WINDOW_CREATE_TITLE_STRING, APP_GAME_CAPTION) ||
				!SDL_SetNumberProperty(props, SDL_PROP_WINDOW_CREATE_X_NUMBER, windowX) ||
				!SDL_SetNumberProperty(props, SDL_PROP_WINDOW_CREATE_Y_NUMBER, windowY) ||
				!SDL_SetNumberProperty(props, SDL_PROP_WINDOW_CREATE_WIDTH_NUMBER, windowW) ||
				!SDL_SetNumberProperty(props, SDL_PROP_WINDOW_CREATE_HEIGHT_NUMBER, windowH) ||
				!SDL_SetBooleanProperty(props, SDL_PROP_WINDOW_CREATE_HIDDEN_BOOLEAN, true) ||
				!SDL_SetBooleanProperty(props, SDL_PROP_WINDOW_CREATE_RESIZABLE_BOOLEAN, true) ||
				!SDL_SetBooleanProperty(props, SDL_PROP_WINDOW_CREATE_MAXIMIZED_BOOLEAN, maximized)
			) {
				SET_SCREEN_ERROR;
			}
			APP_ScreenWindow = SDL_CreateWindowWithProperties(props);
			SDL_DestroyProperties(props);
			if ( !APP_ScreenWindow )
			{
				SET_SCREEN_ERROR;
			}
		}
		else
		{
			if (!SDL_HideWindow(APP_ScreenWindow)) {
				SET_SCREEN_ERROR;
			}
			if ( maximized ) {
				if (!SDL_MaximizeWindow(APP_ScreenWindow)) {
					SET_SCREEN_ERROR;
				}
			}
			else if (!SDL_RestoreWindow(APP_ScreenWindow)) {
				SET_SCREEN_ERROR;
			}
			if (
				!SDL_SetWindowFullscreen(APP_ScreenWindow, false) ||
				!SDL_SetWindowSize(APP_ScreenWindow, windowW, windowH) ||
				!SDL_SetWindowResizable(APP_ScreenWindow, true)
			) {
				SET_SCREEN_ERROR;
			}
			// NOTE: This can fail on some platforms due to not being supported (Wayland), so don't check the return status
			SDL_SetWindowPosition(APP_ScreenWindow, windowX, windowY);
		}
	}
	SDL_free(displayModes);
	displayModes = NULL;

	// Create the renderer, if not already created. It's important to not
	// recreate the renderer if it's already created, so restarting without
	// changing the detail level doesn't require reloading graphics. If the
	// renderer were destroyed/created anew every restart, it would be required
	// to reload the graphics every restart, even when detail level isn't
	// changed.
	// TODO: fix to allow rendering to the texture.
	if ( !APP_ScreenRenderer )
	{
		APP_ScreenRenderer = SDL_CreateRenderer(APP_ScreenWindow, NULL);
		if (!APP_ScreenRenderer)
		{
			SET_SCREEN_ERROR;
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
	if ( APP_ScreenRenderTarget && !SDL_SetRenderTarget(APP_ScreenRenderer, NULL))
	{
		SET_SCREEN_ERROR;
	}

	/* Clear the whole screen, as the framebuffer might be uninitialized */
	if (!SDL_RenderClear(APP_ScreenRenderer))
	{
		SET_SCREEN_ERROR;
	}

	// This should be somewhere after the renderer has been created, as
	// APP_SCREEN_SUBPIXEL_OFFSET queries the renderer when a given platform uses
	// nonzero offsets.
	APP_ScreenSubpixelOffset = APP_SCREEN_SUBPIXEL_OFFSET;

	if (!SDL_SetRenderVSync(APP_ScreenRenderer, (*screenMode & APP_SCREENMODE_VSYNC) ? 1 : SDL_RENDERER_VSYNC_DISABLED) )
	{
		SET_SCREEN_ERROR;
	}

	if ( *screenMode & APP_SCREENMODE_SCALEMODE )
	{
		if (!SDL_SetRenderLogicalPresentation(APP_ScreenRenderer, logicalWidth, logicalHeight, SDL_LOGICAL_PRESENTATION_INTEGER_SCALE))
		{
			SET_SCREEN_ERROR;
		}
	}
	else {
		if (!SDL_SetRenderLogicalPresentation(APP_ScreenRenderer, logicalWidth, logicalHeight, SDL_LOGICAL_PRESENTATION_LETTERBOX))
		{
			SET_SCREEN_ERROR;
		}
	}

	/* Set up the render target, if required */
	// TODO: Figure out how to get render targets in Emscripten working with
	// SDL_BLENDMODE_BLEND textures. This #ifdef is a workaround for now. It's
	// probably a bug in the Emscripten SDL2 port.
	if (!(*screenMode & APP_SCREENMODE_RENDERLEVEL))
	{
		// There's no need to create a render target texture if the
		// currently created render target texture is already the current
		// logicalWidth x logicalHeight.
		bool createNewRenderTarget = true;

		if ( APP_ScreenRenderTarget ) {
			float w, h;
			if (!SDL_GetTextureSize(APP_ScreenRenderTarget, &w, &h)) {
				SET_SCREEN_ERROR;
			}

			if ( (int)w == logicalWidth && (int)h == logicalHeight )
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
				SET_SCREEN_ERROR;
			}
			if (!SDL_SetTextureScaleMode(APP_ScreenRenderTarget, SDL_SCALEMODE_NEAREST)) {
				SET_SCREEN_ERROR;
			}
		}

		if (!SDL_SetRenderTarget(APP_ScreenRenderer, APP_ScreenRenderTarget) ||
			!SDL_RenderClear(APP_ScreenRenderer)
		)
		{
			SET_SCREEN_ERROR;
		}
	}
	else if ( APP_ScreenRenderTarget )
	{
		SDL_DestroyTexture(APP_ScreenRenderTarget);
		APP_ScreenRenderTarget = NULL;
	}

	// WARNING: Make no changes to the renderer settings from here on down, as
	// the render target has been set, and bugs have been observed when
	// attempting renderer setting changes while a non-NULL render target is
	// set.

	#ifdef __EMSCRIPTEN__
	static bool emscriptenCallbackSet = false;
	if ( !emscriptenCallbackSet && emscripten_set_resize_callback(EMSCRIPTEN_EVENT_TARGET_WINDOW, NULL, false, APP_EmscriptenResizeCallback) < 0 )
	{
		SET_SCREEN_ERROR;
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
	if (!SDL_HideCursor())
	{
		SET_SCREEN_ERROR;
	}
	APP_CursorCounting = 0u;

	if (!SDL_ShowWindow(APP_ScreenWindow)) {
		SET_SCREEN_ERROR;
	}
	while (!SDL_SyncWindow(APP_ScreenWindow)) {
		SDL_Delay(1);
	}

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
	SDL_free(displayModes);
	SDL_free(displays);
	APP_ScreenRenderTarget = NULL;
	APP_ScreenRenderer = NULL;
	APP_ScreenWindow = NULL;
	return false;
}

int APP_GetMaxDisplayIndex(void)
{
	int displayCount;
	SDL_DisplayID* displays = SDL_GetDisplays(&displayCount);
	if (!displays) {
		return -1;
	}
	SDL_free(displays);
	return displayCount;
}

int APP_GetMaxDisplayMode( int displayIndex )
{
	int displayCount;
	SDL_DisplayID* displays = SDL_GetDisplays(&displayCount);
	if (!displays) {
		return -1;
	}
	if (displayIndex >= displayCount) {
		SDL_free(displays);
		return -1;
	}
	int modeCount;
	SDL_DisplayMode** modes = SDL_GetFullscreenDisplayModes(displays[displayIndex], &modeCount);
	if (!modes) {
		SDL_free(displays);
		return -1;
	}
	SDL_free(displays);
	SDL_free(modes);
	return modeCount;
}

bool APP_GetDisplayMode( int displayIndex, int modeIndex, SDL_DisplayMode *mode )
{
	int displayCount;
	SDL_DisplayID* displays = SDL_GetDisplays(&displayCount);
	if (!displays) {
		APP_Exit(EXIT_FAILURE);
	}
	if (displayIndex >= displayCount) {
		SDL_free(displays);
		return false;
	}
	int modeCount;
	SDL_DisplayMode** modes = SDL_GetFullscreenDisplayModes(displays[displayIndex], &modeCount);
	SDL_free(displays);
	if (modeIndex >= modeCount) {
		SDL_free(modes);
		return false;
	}
	*mode = *modes[modeIndex];
	return true;
}

const SDL_DisplayMode* APP_GetDesktopDisplayMode(unsigned displayIndex) {
	int displayCount;
	SDL_DisplayID* displays = SDL_GetDisplays(&displayCount);
	if (!displays) {
		APP_Exit(EXIT_FAILURE);
	}
	if (displayIndex >= displayCount) {
		SDL_free(displays);
		return NULL;
	}
	const SDL_DisplayMode* mode = SDL_GetDesktopDisplayMode(displays[displayIndex]);
	SDL_free(displays);
	if (!mode) {
		APP_Exit(EXIT_FAILURE);
	}
	return mode;
}

int APP_Rand ( int max )
{
	return rand() % max;
}

void APP_PauseMusic(void)
{
	// TODO
#if 0
	Mix_PauseMusic();
#endif
}

void APP_ReplayMusic(void)
{
	// TODO
#if 0
	Mix_ResumeMusic();
#endif
}

void APP_PlayWave ( int num )
{
	// TODO
#if 0
	if ( APP_Waves[num].chunk )
	{
		Mix_PlayChannel(num, APP_Waves[num].chunk, APP_Waves[num].loops);
	}
#endif
}

void APP_ReplayWave ( int num )
{
	// TODO
#if 0
	Mix_Resume(num);
#endif
}

void APP_StopWave ( int num )
{
	// TODO
#if 0
	Mix_HaltChannel(num);
#endif
}

void APP_PauseWave ( int num )
{
	// TODO
#if 0
	Mix_Pause(num);
#endif
}

void APP_SetVolumeWave( int num, int vol )
{
	// TODO
#if 0
	int volume = (int)((vol / 100.0f) * APP_VOLUME_MAX);
	if ( volume > APP_VOLUME_MAX ) { volume = APP_VOLUME_MAX; }
	if ( volume < 0 )   { volume = 0; }
	Mix_Volume(num, volume);
#endif
}

bool APP_IsPlayWave( int num )
{
	return APP_Waves[num].stream && SDL_GetAudioStreamAvailable(APP_Waves[num].stream) > 0;
}

void APP_LoadWave( const char* filename, int num )
{
#if 0
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
#endif

#if 0
	// TODO
	if (num >= APP_WAVE_MAX) {
		APP_Exit(EXIT_FAILURE);
	}
	SDL_IOStream* file = APP_OpenRead(filename);
	if (!file) {
		APP_Exit(EXIT_FAILURE);
	}
	if (APP_Waves[num].stream) {
		if (!SDL_ClearAudioStream(APP_Waves[num].stream)) {
			APP_Exit(EXIT_FAILURE);
		}
	}
#endif
}

void APP_SetLoopModeWave( int num, bool looping )
{
	if (num >= APP_WAVE_MAX) {
		APP_Exit(EXIT_FAILURE);
	}
	APP_Waves[num].looping = looping;
}

void APP_LoadMusic( const char* filename )
{
	// TODO
#if 0
	if ( APP_Music )
	{
		Mix_FreeMusic(APP_Music);
		APP_Music = NULL;
	}

	SDL_RWops* src;
	if (!(src = APP_OpenRead(filename))) return;
	APP_Music = Mix_LoadMUS_RW(src, true);
	Mix_VolumeMusic(APP_VOLUME_MAX);
#endif
}

void APP_LoadBitmap( const char* filename, unsigned plane )
{
	if (!APP_ScreenRenderer || !filename || plane >= APP_TEXTURE_MAX) {
		APP_Exit(EXIT_FAILURE);
	}

	// TODO: Use I/O streams instead, as that allows only returning without error if the file doesn't exist, which is an expected behavior, but can exit with error if there's errors while loading an existent file. It also has the benefit of using less memory while loading
	size_t size;
	void* data = SDL_LoadFile(filename, &size);
	if (!data) {
		return;
	}
	if (size > INT_MAX) {
		SDL_free(data);
		APP_Exit(EXIT_FAILURE);
	}

	int w, h;
	int comp;
	stbi_uc* pixels = stbi_load_from_memory(data, (int)size, &w, &h, &comp, STBI_rgb_alpha);
	SDL_free(data);
	if (!pixels) {
		APP_Exit(EXIT_FAILURE);
	}
	SDL_Surface* surface = SDL_CreateSurfaceFrom(w, h, SDL_PIXELFORMAT_RGBA32, pixels, w * 4);
	if (!surface) {
		SDL_free(pixels);
		APP_Exit(EXIT_FAILURE);
	}

	if (APP_Textures[plane]) {
		SDL_DestroyTexture(APP_Textures[plane]);
	}
	APP_Textures[plane] = SDL_CreateTextureFromSurface(APP_ScreenRenderer, surface);
	SDL_DestroySurface(surface);
	SDL_free(pixels);
	if (!APP_Textures[plane]) {
		APP_Exit(EXIT_FAILURE);
	}
	if (
		!SDL_SetTextureScaleMode(APP_Textures[plane], SDL_SCALEMODE_NEAREST) ||
		!SDL_SetTextureBlendMode(APP_Textures[plane], SDL_BLENDMODE_BLEND)
	) {
		APP_Exit(EXIT_FAILURE);
	}
}

void APP_PlayMusic(void)
{
	// TODO
#if 0
	if ( APP_Music )
	{
		Mix_PlayMusic(APP_Music, -1);
	}
#endif
}

void APP_StopMusic(void)
{
	// TODO
#if 0
	Mix_HaltMusic();
#endif
}

void APP_SetVolumeMusic(int vol)
{
	// TODO
#if 0
	int volume = (int)((vol / 100.0f) * APP_VOLUME_MAX);
	if ( volume > APP_VOLUME_MAX ) { volume = APP_VOLUME_MAX; }
	if ( volume < 0 )   { volume = 0; }
	Mix_VolumeMusic(volume);
#endif
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

void APP_ClearSecondary(void)
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
	SDL_IOStream	*src = APP_OpenRead(filename);

	if ( src )
	{
		SDL_ReadIO(src, buf, size);
		SDL_CloseIO(src);

#if SDL_BYTEORDER != SDL_LIL_ENDIAN
		/* エンディアン変換 */
		int32_t* buf2 = (int32_t*)buf;
		for ( size_t i = 0 ; i < size / sizeof(int32_t) ; i ++ )
		{
			buf2[i] = SDL_Swap32LE(buf2[i]);
		}
#endif
	}
}

void APP_ReadFile( const char* filename, void* buf, size_t size, size_t offset )
{
	SDL_IOStream	*src = APP_OpenRead(filename);

	if ( src )
	{
		if (SDL_SeekIO(src, offset, SDL_IO_SEEK_SET) < 0) return;
		SDL_ReadIO(src, buf, size);
		SDL_CloseIO(src);

#if SDL_BYTEORDER != SDL_LIL_ENDIAN
		/* エンディアン変換 */
		int32_t* buf2 = (int32_t*)buf;
		for ( size_t i = 0 ; i < size / sizeof(int32_t) ; i ++ )
		{
			buf2[i] = SDL_Swap32LE(buf2[i]);
		}
#endif
	}
}


void APP_SaveFile( const char* filename, void* buf, size_t size )
{
	SDL_IOStream	*dst = APP_OpenWrite(filename);

	if ( dst )
	{
		#if SDL_BYTEORDER != SDL_LIL_ENDIAN
		/* エンディアン変換 */
		int32_t* buf2 = (int32_t*)buf;
		for ( size_t i = 0 ; i < size / sizeof(int32_t) ; i ++ )
		{
			buf2[i] = SDL_Swap32LE(buf2[i]);
		}
		#endif

		SDL_WriteIO(dst, buf, size);
		if (!SDL_CloseIO(dst))
		{
			fprintf(stderr, "Error closing: %s\n", SDL_GetError());
		}

		// TODO: Create a custom SDL_IOStream object type for Emscripten and put this in that.
		#ifdef __EMSCRIPTEN__
		EM_ASM({
			FS.syncfs(function (err) {
				assert(!err);
			});
		});
		#endif

		#if SDL_BYTEORDER != SDL_LIL_ENDIAN
		/* もどす */
		for ( size_t i = 0 ; i < size / sizeof(int32_t) ; i ++ )
		{
			buf2[i] = SDL_Swap32LE(buf2[i]);
		}
		#endif
	}
}

void APP_AppendFile( const char* filename, void* buf, size_t size ) {
	/* エンディアン変換 */
	int32_t* buf2 = (int32_t*)buf;
	for ( size_t i = 0 ; i < size / sizeof(int32_t) ; i ++ )
	{
		buf2[i] = SDL_Swap32LE(buf2[i]);
	}

	SDL_IOStream	*dst = APP_OpenAppend(filename);

	if ( dst )
	{
		SDL_WriteIO(dst, buf, size);
		// TODO: Create a custom SDL_RWops object type for Emscripten and put this in that.
		SDL_CloseIO(dst);
		#ifdef __EMSCRIPTEN__
		EM_ASM({
			FS.syncfs(function (err) {
				assert(!err);
			});
		});
		#endif
	}

#if SDL_BYTEORDER != SDL_LIL_ENDIAN
	/* もどす */
	for ( size_t i = 0 ; i < size / sizeof(int32_t) ; i ++ )
	{
		buf2[i] = SDL_Swap32LE(buf2[i]);
	}
#endif
}

void APP_TextLayerOn ( int layer, int x, int y )
{
	if (layer < 0 || layer >= APP_TEXTLAYER_MAX) {
		APP_Exit(EXIT_FAILURE);
	}
	APP_TextLayers[layer].enable = true;
	APP_TextLayers[layer].x = x;
	APP_TextLayers[layer].y = y;
}

void APP_TextMove ( int layer, int x, int y )
{
	if (layer < 0 || layer >= APP_TEXTLAYER_MAX) {
		APP_Exit(EXIT_FAILURE);
	}
	APP_TextLayers[layer].x = x;
	APP_TextLayers[layer].y = y;
}

void APP_TextColor ( int layer, int r, int g, int b )
{
	if (layer < 0 || layer >= APP_TEXTLAYER_MAX) {
		APP_Exit(EXIT_FAILURE);
	}
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
	if (layer < 0 || layer >= APP_TEXTLAYER_MAX) {
		APP_Exit(EXIT_FAILURE);
	}
	// TODO
	// turns off the shadow effect for text in the listed layer. since we don't even use said shadow effect to begin with, it's a no-op.
}

void APP_TextSize ( int layer, int size )
{
	if (layer < 0 || layer >= APP_TEXTLAYER_MAX) {
		APP_Exit(EXIT_FAILURE);
	}
	if (APP_TextLayers[layer].size != size) {
		APP_TextLayers[layer].size = size;
		APP_TextLayers[layer].updateTexture = true;
	}
}

void APP_TextHeight ( int layer, int height )
{
	if (layer < 0 || layer >= APP_TEXTLAYER_MAX) {
		APP_Exit(EXIT_FAILURE);
	}
	// TODO
	// only used in flexdraw.c for ExTextHeight. But since ExTextHeight is unused, we don't need to bother implementing it. 
}

void APP_TextOut ( int layer, const char* text )
{
	if (layer < 0 || layer >= APP_TEXTLAYER_MAX) {
		APP_Exit(EXIT_FAILURE);
	}
	if ( SDL_strcmp(text, APP_TextLayers[layer].string) != 0 )
	{
		SDL_strlcpy(APP_TextLayers[layer].string, text, APP_TEXTLAYER_STRING_MAX - 1);
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
		APP_TextLayers[layer].texture = APP_CreateBDFTextTexture(APP_BDFFonts[font], APP_ScreenRenderer, APP_TextLayers[layer].string, APP_TextLayers[layer].color, 32, SDL_SCALEMODE_NEAREST);
		if ( !APP_TextLayers[layer].texture )
		{
			SDL_Log("Error creating texture to blit text: %s\n", SDL_GetError());
			APP_Exit(EXIT_FAILURE);
		}
		if ( !SDL_GetTextureSize(APP_TextLayers[layer].texture, &APP_TextLayers[layer].textureW, &APP_TextLayers[layer].textureH) )
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
			const SDL_FRect dstRect =
			{
				APP_TextLayers[layer].x + APP_OffsetX,
				APP_TextLayers[layer].y + APP_OffsetY,
				APP_TextLayers[layer].textureW,
				APP_TextLayers[layer].textureH
			};
			if (!SDL_RenderTexture(APP_ScreenRenderer, APP_TextLayers[layer].texture, NULL, &dstRect))
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
			if (!SDL_RenderTexture(APP_ScreenRenderer, APP_TextLayers[layer].texture, NULL, &dstRect))
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
	float w, h;
	if (!SDL_GetTextureSize(APP_Textures[pno], &w, &h)) {
		APP_Exit(EXIT_FAILURE);
	}
	APP_BltRect(pno, dx, dy, 0, 0, (int)w, (int)h);
}

void APP_BltRect(int pno, int dx, int dy, int sx, int sy, int hx, int hy)
{
	if ( !APP_ScreenRenderer )
	{
		return;
	}

	if ( APP_Textures[pno] == NULL ) return;

	SDL_FRect	src = { 0 };
	SDL_FRect	dst = { 0 };
	if ( APP_ScreenRenderTarget )
	{
		src.x = sx;			src.y = sy;
		src.w = hx;			src.h = hy;
		dst.x = dx + APP_OffsetX;	dst.y = dy + APP_OffsetY;
		dst.w = hx;			dst.h = hy;
	}
	else
	{
		src.x  = sx;			src.y  = sy;
		src.w  = hx;			src.h  = hy;
		dst.x = dx + APP_OffsetX + APP_SCREEN_SUBPIXEL_OFFSET;
		dst.y = dy + APP_OffsetY + APP_SCREEN_SUBPIXEL_OFFSET;
		dst.w  = hx;			dst.h  = hy;
	}

	if (!SDL_RenderTexture(APP_ScreenRenderer, APP_Textures[pno], &src, &dst)) {
		APP_Exit(EXIT_FAILURE);
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

	if (!SDL_SetTextureAlphaMod(APP_Textures[pno], ar)) {
		APP_Exit(EXIT_FAILURE);
	}
	APP_Blt(pno, dx, dy);
	if (!SDL_SetTextureAlphaMod(APP_Textures[pno], SDL_ALPHA_OPAQUE)) {
		APP_Exit(EXIT_FAILURE);
	}
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
		SDL_FRect	src = { 0 };
		SDL_FRect	dst = { 0 };

		src.x = sx;					src.y = sy;
		src.w = hx;					src.h = hy;
		dst.x = dx + APP_OffsetX;	dst.y = dy + APP_OffsetY;
		dst.w = hx * (int)(scx / 65536.0f);
		dst.h = hy * (int)(scy / 65536.0f);

		if ( src.w == 0 || src.h == 0 || dst.w == 0 || dst.h == 0 ) { return; }

		SDL_RenderTexture( APP_ScreenRenderer, APP_Textures[pno], &src, &dst );
	}
	else
	{
		SDL_FRect	src = { 0 };
		SDL_FRect	dst = { APP_ScreenSubpixelOffset, APP_ScreenSubpixelOffset };

		src.x  = sx;			src.y  = sy;
		src.w  = hx;			src.h  = hy;
		dst.x += dx + APP_OffsetX;	dst.y += dy + APP_OffsetY;
		dst.w  = (int)(hx * (scx / 65536.0f));
		dst.h  = (int)(hy * (scy / 65536.0f));

		if ( src.w == 0 || src.h == 0 || dst.w == 0 || dst.h == 0 ) { return; }

		SDL_RenderTexture( APP_ScreenRenderer, APP_Textures[pno], &src, &dst );
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

	SDL_FRect	src = { 0 };
	SDL_FRect	dst = { 0 };

	// ちゃんと拡大して描画する
	if ( APP_ScreenRenderTarget )
	{
		src.x = sx;					src.y = sy;
		src.w = hx;					src.h = hy;
		dst.x = dx + APP_OffsetX;	dst.y = dy + APP_OffsetY;
		dst.w = hx * (int)(scx / 65536.0f);
		dst.h = hy * (int)(scy / 65536.0f);

		if ( src.w == 0 || src.h == 0 || dst.w == 0 || dst.h == 0 ) { return; }

		SDL_SetTextureAlphaMod(APP_Textures[pno], ar);
		SDL_RenderTexture( APP_ScreenRenderer, APP_Textures[pno], &src, &dst );
		SDL_SetTextureAlphaMod(APP_Textures[pno], SDL_ALPHA_OPAQUE);
	}
	else
	{
		src.x  = sx;			src.y  = sy;
		src.w  = hx;			src.h  = hy;
		dst.x = dx + APP_OffsetX + APP_SCREEN_SUBPIXEL_OFFSET;
		dst.y = dy + APP_OffsetY + APP_SCREEN_SUBPIXEL_OFFSET;
		dst.w  = hx * (int)(scx / 65536.0f);
		dst.h  = hy * (int)(scy / 65536.0f);

		if ( src.w == 0 || src.h == 0 || dst.w == 0 || dst.h == 0 ) { return; }

		SDL_SetTextureAlphaMod(APP_Textures[pno], ar);
		SDL_RenderTexture( APP_ScreenRenderer, APP_Textures[pno], &src, &dst );
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

void APP_StrCpy(char *dest, const char *src)
{
	// TODO: Use SDL_strlcpy() instead, but that requires adding a parameter of the size of the dest argument
	strcpy(dest, src);
}

void APP_StrCat(char *str1, const char *str2)
{
	// TODO: Use SDL_strlcat() instead, but that requires adding a parameter of the size of the str1 argument
	strcat(str1, str2);
}

size_t APP_StrLen(const char *stri)
{
	return SDL_strlen(stri);
}

void APP_MidStr(const char *src, size_t start, size_t len, char *dest)
{
	int		i;
	for ( i = 0 ; i < len ; i ++ )
	{
		dest[i] = src[start - 1 + i];
	}
	dest[len] = '\0';
}

void APP_LeftStr(const char *src, size_t len, char *dest)
{
	APP_MidStr(src, 1, len, dest);
}

char APP_CharAt(const char *stri, size_t pos)
{
	return stri[pos];
}

int APP_ValLong(const char *stri)
{
	return SDL_atoi(stri);
}

void APP_FillMemory(void* buf, size_t size, int val)
{
	SDL_memset(buf, val, size);
}

////////////////////////////////////////////////////

static void APP_PrivateBDFFontInitialize(void)
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
		SDL_IOStream *src = APP_OpenRead(filenames[i]);
		if ( !src ) {
			SDL_Log("Failed to open file for font \"%s\"; continuing without it.", filenames[i]);
			APP_BDFFonts[i] = NULL;
			continue;
		}
		APP_BDFFonts[i] = APP_OpenBDFFont(src);
		if ( !APP_BDFFonts[i] ) {
			SDL_Log("Failed to load font \"%s\": %s\n", filenames[i], SDL_GetError());
			SDL_CloseIO(src);
			APP_Exit(EXIT_FAILURE);
		}
		SDL_CloseIO(src);
	}
}

static void APP_PrivateBDFFontFinalize(void)
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
	SDL_Gamepad* controller;
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

APP_InputType APP_GetLastInputType(void)
{
	return APP_LastInputType;
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

int APP_GetMaxKey(void)
{
	return APP_KEY_MAX;
}
#endif

#if defined(APP_ENABLE_JOYSTICK) || defined(APP_ENABLE_GAME_CONTROLLER)

static bool APP_IsCon ( SDL_JoystickID joy )
{
	if ( !joy ) {
		return false;
	}

	if (
		SDL_IsGamepad(joy) &&
		SDL_GetGamepadTypeForID(joy) != SDL_GAMEPAD_TYPE_UNKNOWN
	) {
		return true;
	}

	char* const mapping = SDL_GetGamepadMappingForID(joy);
	const bool isGamepad = mapping != NULL;
	SDL_free(mapping);
	return isGamepad;
}

static bool APP_ResizePlayerSlots(int numSlots) {
	if (numSlots == APP_NumPlayerSlots) {
		return true;
	}
	else if (numSlots == 0) {
		SDL_free(APP_PlayerSlots);
		APP_PlayerSlots = NULL;
		APP_NumPlayerSlots = 0;
		return true;
	}

	APP_PlayerSlot* const oldSlots = APP_PlayerSlots;
	if (APP_PlayerSlots) {
		APP_PlayerSlots = SDL_realloc(APP_PlayerSlots, sizeof(APP_PlayerSlot) * numSlots);
	}
	else {
		APP_PlayerSlots = SDL_malloc(sizeof(APP_PlayerSlot) * numSlots);
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

static bool APP_ConIsBuiltin(SDL_Gamepad * const controller) {
	for (size_t i = 0; i < SDL_arraysize(APP_BuiltinConIDs); i++) {
		const Uint16 vendor = SDL_GetGamepadVendor(controller);
		const Uint16 product = SDL_GetGamepadProduct(controller);
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
	if (APP_BuiltinCon.controller && !SDL_GamepadConnected(APP_BuiltinCon.controller)) {
		APP_BuiltinCon = (APP_Con) { 0 };
	}
	#endif
	for (int player = 0; player < APP_NumPlayerSlots; player++) {
		APP_PlayerSlot* const slot = &APP_PlayerSlots[player];
		switch (slot->type) {
		#ifdef APP_ENABLE_JOYSTICK
		case APP_PLAYERSLOT_JOY:
			if (!SDL_JoystickConnected(slot->joy.joystick)) {
				if (slot->joy.axesRepeat) SDL_free(slot->joy.axesRepeat);
				if (slot->joy.hatsRepeat) SDL_free(slot->joy.hatsRepeat);
				if (slot->joy.buttonsRepeat) SDL_free(slot->joy.buttonsRepeat);
				*slot = (APP_PlayerSlot) { 0 };
			}
			break;
		#endif

		#ifdef APP_ENABLE_GAME_CONTROLLER
		case APP_PLAYERSLOT_CON:
			if (!SDL_GamepadConnected(slot->con.controller)) {
				*slot = (APP_PlayerSlot) { 0 };
			}
			break;
		#endif

		default:
		case APP_PLAYERSLOT_NULL:
			break;
		}
	}

	int numJoys;
	SDL_JoystickID* const joys = SDL_GetJoysticks(&numJoys);
	if (!joys) {
		fprintf(stderr, "Failed to get joysticks list: %s\n", SDL_GetError());
		return false;
	}

	#ifdef APP_ENABLE_GAME_CONTROLLER
	for (int device = 0; device < numJoys; device++) {
		if (!APP_IsCon(joys[device])) {
			continue;
		}

		SDL_Gamepad * const controller = SDL_OpenGamepad(joys[device]);
		if (!controller) {
			fprintf(stderr, "Failed to open controller: %s\n", SDL_GetError());
			SDL_free(joys);
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
				SDL_CloseGamepad(controller);
				SDL_free(joys);
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
		if (APP_IsCon(joys[device])) {
			continue;
		}

		SDL_Joystick* const joystick = SDL_OpenJoystick(joys[device]);
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
			(APP_PlayerSlots[player].joy.numAxes = SDL_GetNumJoystickAxes(joystick)) >= 0 &&
			(APP_PlayerSlots[player].joy.numHats = SDL_GetNumJoystickHats(joystick)) >= 0 &&
			(APP_PlayerSlots[player].joy.numButtons = SDL_GetNumJoystickButtons(joystick)) >= 0
		) {
			if (APP_PlayerSlots[player].joy.numAxes > 0 && !(APP_PlayerSlots[player].joy.axesRepeat = (int*)SDL_calloc((size_t)APP_PlayerSlots[player].joy.numAxes * 2, sizeof(int)))) {
				fprintf(stderr, "Failed to allocate axes repeat array for joystick\n");
				goto fail;
			}
			if (APP_PlayerSlots[player].joy.numHats > 0 && !(APP_PlayerSlots[player].joy.hatsRepeat = (int*)SDL_calloc((size_t)APP_PlayerSlots[player].joy.numHats * 4, sizeof(int)))) {
				fprintf(stderr, "Failed to allocate hats repeat array for joystick\n");
				goto fail;
			}
			if (APP_PlayerSlots[player].joy.numButtons > 0 && !(APP_PlayerSlots[player].joy.buttonsRepeat = (int*)SDL_calloc((size_t)APP_PlayerSlots[player].joy.numButtons, sizeof(int)))) {
				fprintf(stderr, "Failed to allocate buttons repeat array for joystick\n");
				goto fail;
			}
		}
		if (false) {
		fail:
			if (APP_PlayerSlots[player].joy.axesRepeat) SDL_free(APP_PlayerSlots[player].joy.axesRepeat);
			if (APP_PlayerSlots[player].joy.hatsRepeat) SDL_free(APP_PlayerSlots[player].joy.hatsRepeat);
			if (APP_PlayerSlots[player].joy.buttonsRepeat) SDL_free(APP_PlayerSlots[player].joy.buttonsRepeat);
			if (joystick) SDL_CloseJoystick(joystick);
			SDL_free(joys);
			return false;
		}
		APP_PlayerSlots[player].joy.joystick = joystick;
		APP_PlayerSlots[player].type = APP_PLAYERSLOT_JOY;
	}
	#endif

	SDL_free(joys);

	int numRequiredSlots = 0;
	for (int player = 0; player < APP_NumPlayerSlots; player++) {
		if (APP_PlayerSlots[player].type != APP_PLAYERSLOT_NULL) {
			numRequiredSlots = player + 1;
		}
	}
	return APP_ResizePlayerSlots(numRequiredSlots);
}

static void APP_PlayerSlotsClose(void)
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
				if (SDL_JoystickConnected(joy->joystick))
				{
					SDL_CloseJoystick(joy->joystick);
				}
				if (joy->axesRepeat) SDL_free(joy->axesRepeat);
				if (joy->hatsRepeat) SDL_free(joy->hatsRepeat);
				if (joy->buttonsRepeat) SDL_free(joy->buttonsRepeat);
				break;
			}
			#endif

			#ifdef APP_ENABLE_GAME_CONTROLLER
			case APP_PLAYERSLOT_CON: {
				if (SDL_GamepadConnected(APP_PlayerSlots[player].con.controller))
				{
					SDL_CloseGamepad(APP_PlayerSlots[player].con.controller);
				}
				break;
			}
			#endif

			default:
			case APP_PLAYERSLOT_NULL:
				break;
			}
		}
		SDL_free(APP_PlayerSlots);
	}
	APP_PlayerSlots = NULL;
	APP_NumPlayerSlots = 0;
}

int APP_GetNumPlayerSlots(void)
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

static void APP_JoyInputsUpdate(void)
{
	if (!APP_PlayerSlots) return;

	for (int player = 0; player < APP_NumPlayerSlots; player++)
	{
		if (
			APP_PlayerSlots[player].type != APP_PLAYERSLOT_JOY ||
			!SDL_JoystickConnected(APP_PlayerSlots[player].joy.joystick)
		) {
			continue;
		}

		const APP_JoyGUID checkGUID = APP_GetJoyGUID(player);
		const APP_JoyGUID zeroGUID = { 0 };
		if (SDL_memcmp(checkGUID.data, zeroGUID.data, sizeof(checkGUID.data)) == 0) continue;

		APP_Joy* const joy = &APP_PlayerSlots[player].joy;

		for (int axis = 0; axis < joy->numAxes; axis++)
		{
			const int value = SDL_GetJoystickAxis(joy->joystick, axis);

			if (value < APP_DEADZONE_MIN)
			{
				if (++joy->axesRepeat[axis*2 + 0] == 1) APP_LastInputType = APP_INPUT_JOYSTICK;
			}
			else
			{
				joy->axesRepeat[axis*2 + 0] = 0;
			}

			if (value > APP_DEADZONE_MAX)
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
			Uint8 value = SDL_GetJoystickHat(joy->joystick, hat);
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
			if (SDL_GetJoystickButton(joy->joystick, button))
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
		if (SDL_memcmp(checkGUID.data, zeroGUID.data, sizeof(checkGUID.data)) != 0 && SDL_memcmp(key->guid.data, checkGUID.data, sizeof(checkGUID.data)) == 0)
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
				if (key->setting.value == APP_DEADZONE_MIN)
				{
					return joy->axesRepeat[key->setting.index * 2 + 0] == 1;
				}
				else if (key->setting.value == APP_DEADZONE_MAX)
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
		if (SDL_memcmp(checkGUID.data, zeroGUID.data, sizeof(checkGUID.data)) != 0 && SDL_memcmp(key->guid.data, checkGUID.data, sizeof(checkGUID.data)) == 0)
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
				if (key->setting.value == APP_DEADZONE_MIN)
				{
					return joy->axesRepeat[key->setting.index * 2 + 0] != 0;
				}
				else if (key->setting.value == APP_DEADZONE_MAX)
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
		if (SDL_memcmp(checkGUID.data, zeroGUID.data, sizeof(checkGUID.data)) != 0 && SDL_memcmp(key->guid.data, checkGUID.data, sizeof(checkGUID.data)) == 0)
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
				if (key->setting.value == APP_DEADZONE_MIN)
				{
					if (multi)
					{
						if (joy->axesRepeat[key->setting.index * 2 + 0] > maxRepeat) maxRepeat = joy->axesRepeat[key->setting.index * 2 + 0];
					}
					else return joy->axesRepeat[key->setting.index * 2 + 0];
				}
				else if (key->setting.value == APP_DEADZONE_MAX)
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

int APP_GetNumJoys(void)
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

	SDL_GUID sdlGUID;
	sdlGUID = SDL_GetJoystickGUID(APP_PlayerSlots[player].joy.joystick);

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

static SDL_GamepadType APP_GetSDLGameControllerType(SDL_Gamepad * const controller)
{
	#ifdef APP_ONLY_SDL_CONTROLLER_TYPE
	return APP_ONLY_SDL_CONTROLLER_TYPE;
	#else
	return SDL_GetGamepadType(controller);
	#endif
}

static void APP_ConInputsUpdate(void)
{
	bool backupLastInputBuiltinCon;
	if (APP_BuiltinCon.controller && SDL_GamepadConnected(APP_BuiltinCon.controller)) {
		backupLastInputBuiltinCon = APP_LastInputBuiltinCon;
		APP_LastInputBuiltinCon = true;
		APP_InputType const inputType = APP_GetConType(-1);
		APP_LastInputBuiltinCon = backupLastInputBuiltinCon;
		Sint16 axisValue;

		axisValue = SDL_GetGamepadAxis(APP_BuiltinCon.controller, SDL_GAMEPAD_AXIS_LEFTX);
		if (axisValue > 0 && axisValue > APP_DEADZONE_MAX)
		{
			if (++APP_BuiltinCon.axesRepeat[0] == 1)
			{
				APP_LastInputType = inputType;
				APP_LastActiveCon = 0;
				APP_LastInputBuiltinCon = true;
			}
			APP_BuiltinCon.axesRepeat[1] = 0;
		}
		else if (axisValue < 0 && axisValue < APP_DEADZONE_MIN)
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

		axisValue = SDL_GetGamepadAxis(APP_BuiltinCon.controller, SDL_GAMEPAD_AXIS_LEFTY);
		if (axisValue > 0 && axisValue > APP_DEADZONE_MAX)
		{
			if (++APP_BuiltinCon.axesRepeat[2] == 1)
			{
				APP_LastInputType = inputType;
				APP_LastActiveCon = 0;
				APP_LastInputBuiltinCon = true;
			}
			APP_BuiltinCon.axesRepeat[3] = 0;
		}
		else if (axisValue < 0 && axisValue < APP_DEADZONE_MIN)
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

		axisValue = SDL_GetGamepadAxis(APP_BuiltinCon.controller, SDL_GAMEPAD_AXIS_RIGHTX);
		if (axisValue > 0 && axisValue > APP_DEADZONE_MAX)
		{
			if (++APP_BuiltinCon.axesRepeat[4] == 1)
			{
				APP_LastInputType = inputType;
				APP_LastActiveCon = 0;
				APP_LastInputBuiltinCon = true;
			}
			APP_BuiltinCon.axesRepeat[5] = 0;
		}
		else if (axisValue < 0 && axisValue < APP_DEADZONE_MIN)
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

		axisValue = SDL_GetGamepadAxis(APP_BuiltinCon.controller, SDL_GAMEPAD_AXIS_RIGHTY);
		if (axisValue > 0 && axisValue > APP_DEADZONE_MAX)
		{
			if (++APP_BuiltinCon.axesRepeat[6] == 1)
			{
				APP_LastInputType = inputType;
				APP_LastActiveCon = 0;
				APP_LastInputBuiltinCon = true;
			}
			APP_BuiltinCon.axesRepeat[7] = 0;
		}
		else if (axisValue < 0 && axisValue < APP_DEADZONE_MIN)
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

		axisValue = SDL_GetGamepadAxis(APP_BuiltinCon.controller, SDL_GAMEPAD_AXIS_LEFT_TRIGGER);
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

		axisValue = SDL_GetGamepadAxis(APP_BuiltinCon.controller, SDL_GAMEPAD_AXIS_RIGHT_TRIGGER);
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

		for (SDL_GamepadButton button = 0; button < SDL_GAMEPAD_BUTTON_COUNT; button++)
		{
			if (SDL_GetGamepadButton(APP_BuiltinCon.controller, button))
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

		if (!SDL_GamepadConnected(con->controller)) continue;

		backupLastInputBuiltinCon = APP_LastInputBuiltinCon;
		APP_LastInputBuiltinCon = false;
		APP_InputType const inputType = APP_GetConType(player);
		APP_LastInputBuiltinCon = backupLastInputBuiltinCon;
		Sint16 axisValue;

		axisValue = SDL_GetGamepadAxis(con->controller, SDL_GAMEPAD_AXIS_LEFTX);
		if (axisValue > 0 && axisValue > APP_DEADZONE_MAX)
		{
			if (++con->axesRepeat[0] == 1)
			{
				APP_LastInputType = inputType;
				APP_LastActiveCon = player;
				if (player == 0) APP_LastInputBuiltinCon = false;
			}
			con->axesRepeat[1] = 0;
		}
		else if (axisValue < 0 && axisValue < APP_DEADZONE_MIN)
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

		axisValue = SDL_GetGamepadAxis(con->controller, SDL_GAMEPAD_AXIS_LEFTY);
		if (axisValue > 0 && axisValue > APP_DEADZONE_MAX)
		{
			if (++con->axesRepeat[2] == 1)
			{
				APP_LastInputType = inputType;
				APP_LastActiveCon = player;
				if (player == 0) APP_LastInputBuiltinCon = false;
			}
			con->axesRepeat[3] = 0;
		}
		else if (axisValue < 0 && axisValue < APP_DEADZONE_MIN)
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

		axisValue = SDL_GetGamepadAxis(con->controller, SDL_GAMEPAD_AXIS_RIGHTX);
		if (axisValue > 0 && axisValue > APP_DEADZONE_MAX)
		{
			if (++con->axesRepeat[4] == 1)
			{
				APP_LastInputType = inputType;
				APP_LastActiveCon = player;
				if (player == 0) APP_LastInputBuiltinCon = false;
			}
			con->axesRepeat[5] = 0;
		}
		else if (axisValue < 0 && axisValue < APP_DEADZONE_MIN)
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

		axisValue = SDL_GetGamepadAxis(con->controller, SDL_GAMEPAD_AXIS_RIGHTY);
		if (axisValue > 0 && axisValue > APP_DEADZONE_MAX)
		{
			if (++con->axesRepeat[6] == 1)
			{
				APP_LastInputType = inputType;
				APP_LastActiveCon = player;
				if (player == 0) APP_LastInputBuiltinCon = false;
			}
			con->axesRepeat[7] = 0;
		}
		else if (axisValue < 0 && axisValue < APP_DEADZONE_MIN)
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

		axisValue = SDL_GetGamepadAxis(con->controller, SDL_GAMEPAD_AXIS_LEFT_TRIGGER);
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

		axisValue = SDL_GetGamepadAxis(con->controller, SDL_GAMEPAD_AXIS_RIGHT_TRIGGER);
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

		for (SDL_GamepadButton button = 0; button < SDL_GAMEPAD_BUTTON_COUNT; button++)
		{
			if (SDL_GetGamepadButton(con->controller, button))
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

int APP_GetNumCons(void)
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

void APP_ResetLastActiveCon(void)
{
	APP_LastActiveCon = -1;
}

int APP_GetLastActiveCon(void)
{
	return APP_LastActiveCon;
}

APP_InputType APP_GetConType(const int player)
{
	SDL_Gamepad * controller = NULL;
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
	if (APP_ONLY_INPUT_TYPE >= APP_INPUT_FIRSTGAMECONTROLLERTYPE && APP_ONLY_INPUT_TYPE <= APP_INPUT_LASTGAMECONTROLLERTYPE) {
		return APP_ONLY_INPUT_TYPE;
	}
	else {
		return APP_INPUT_NULL;
	}
	#else
	switch(APP_GetSDLGameControllerType(controller))
	{
	default:
	case SDL_GAMEPAD_TYPE_STANDARD :
	case SDL_GAMEPAD_TYPE_XBOX360 :
	case SDL_GAMEPAD_TYPE_XBOXONE :
		return APP_INPUT_XBOX;

	case SDL_GAMEPAD_TYPE_PS3 :
	case SDL_GAMEPAD_TYPE_PS4 :
	case SDL_GAMEPAD_TYPE_PS5 :
		return APP_INPUT_PLAYSTATION;

	case SDL_GAMEPAD_TYPE_NINTENDO_SWITCH_PRO :
	case SDL_GAMEPAD_TYPE_NINTENDO_SWITCH_JOYCON_LEFT :
	case SDL_GAMEPAD_TYPE_NINTENDO_SWITCH_JOYCON_RIGHT :
	case SDL_GAMEPAD_TYPE_NINTENDO_SWITCH_JOYCON_PAIR :
		return APP_INPUT_NINTENDO;
	}
	#endif
}

bool APP_GetConKeyDesc(const int player, const APP_ConKey* const key, const char** text, APP_Button* button)
{
	SDL_Gamepad * controller;
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
	const SDL_GamepadType type = APP_GetSDLGameControllerType(controller);
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
			switch(type)
			{
			default:
			case SDL_GAMEPAD_TYPE_STANDARD :
			case SDL_GAMEPAD_TYPE_XBOX360 :
			case SDL_GAMEPAD_TYPE_XBOXONE :
				*text = "LT";
				break;

			case SDL_GAMEPAD_TYPE_PS3 :
			case SDL_GAMEPAD_TYPE_PS4 :
			case SDL_GAMEPAD_TYPE_PS5 :
				*text = "L2";
				break;

			case SDL_GAMEPAD_TYPE_NINTENDO_SWITCH_PRO :
			case SDL_GAMEPAD_TYPE_NINTENDO_SWITCH_JOYCON_PAIR :
				*text = "ZL";
				break;

			case SDL_GAMEPAD_TYPE_NINTENDO_SWITCH_JOYCON_LEFT :
			case SDL_GAMEPAD_TYPE_NINTENDO_SWITCH_JOYCON_RIGHT :
				*text = "SL";
				break;
			}
			break;
		case 9:
			switch(type)
			{
			default:
			case SDL_GAMEPAD_TYPE_STANDARD :
			case SDL_GAMEPAD_TYPE_XBOX360 :
			case SDL_GAMEPAD_TYPE_XBOXONE :
				*text = "RT";
				break;

			case SDL_GAMEPAD_TYPE_PS3 :
			case SDL_GAMEPAD_TYPE_PS4 :
			case SDL_GAMEPAD_TYPE_PS5 :
				*text = "R2";
				break;

			case SDL_GAMEPAD_TYPE_NINTENDO_SWITCH_PRO :
			case SDL_GAMEPAD_TYPE_NINTENDO_SWITCH_JOYCON_PAIR :
				*text = "ZR";
				break;

			case SDL_GAMEPAD_TYPE_NINTENDO_SWITCH_JOYCON_LEFT :
			case SDL_GAMEPAD_TYPE_NINTENDO_SWITCH_JOYCON_RIGHT :
				*text = "SR";
				break;
			}
			break;
		default:
			break;
		}
		break;
	case APP_CONKEY_BUTTON:
		switch (key->index)
		{
		case SDL_GAMEPAD_BUTTON_BACK :
		case SDL_GAMEPAD_BUTTON_START :
			// "Start" and "Back" are reserved for "Pause" and "Give Up", and may not be remapped.
			break;

		case SDL_GAMEPAD_BUTTON_GUIDE :
			*text = "HOME";
			break;

		case SDL_GAMEPAD_BUTTON_SOUTH :
		case SDL_GAMEPAD_BUTTON_EAST :
		case SDL_GAMEPAD_BUTTON_WEST :
		case SDL_GAMEPAD_BUTTON_NORTH :
			*button = APP_BUTTON_A + key->index;
			break;

		case SDL_GAMEPAD_BUTTON_DPAD_UP :
		case SDL_GAMEPAD_BUTTON_DPAD_DOWN :
		case SDL_GAMEPAD_BUTTON_DPAD_LEFT :
		case SDL_GAMEPAD_BUTTON_DPAD_RIGHT :
			*text = "DPAD";
			*button = APP_BUTTON_UP + key->index - SDL_GAMEPAD_BUTTON_DPAD_UP;
			break;

		case SDL_GAMEPAD_BUTTON_MISC1 :
			switch(type)
			{
			case SDL_GAMEPAD_TYPE_XBOXONE :
				*text = "SHARE";
				break;

			case SDL_GAMEPAD_TYPE_NINTENDO_SWITCH_PRO :
			case SDL_GAMEPAD_TYPE_NINTENDO_SWITCH_JOYCON_LEFT :
			case SDL_GAMEPAD_TYPE_NINTENDO_SWITCH_JOYCON_PAIR :
				*text = "CAPTURE";
				break;

			case SDL_GAMEPAD_TYPE_PS5 :
				*text = "MICROPHONE";
				break;

			default:
				*text = "MISC1";
				break;
			}
			break;
		case SDL_GAMEPAD_BUTTON_RIGHT_PADDLE1 :
			*text = "P1";
			break;
		case SDL_GAMEPAD_BUTTON_LEFT_PADDLE1 :
			*text = "P2";
			break;
		case SDL_GAMEPAD_BUTTON_RIGHT_PADDLE2 :
			*text = "P3";
			break;
		case SDL_GAMEPAD_BUTTON_LEFT_PADDLE2 :
			*text = "P4";
			break;
		case SDL_GAMEPAD_BUTTON_TOUCHPAD :
			*text = "TOUCHPAD";
			break;
		case SDL_GAMEPAD_BUTTON_LEFT_STICK :
			switch(type)
			{
			default:
				*text = "LS";
				break;

			case SDL_GAMEPAD_TYPE_PS3 :
			case SDL_GAMEPAD_TYPE_PS4 :
			case SDL_GAMEPAD_TYPE_PS5 :
				*text = "L3";
				break;
			}
			break;
		case SDL_GAMEPAD_BUTTON_RIGHT_STICK :
			switch(type)
			{
			default:
				*text = "RS";
				break;

			case SDL_GAMEPAD_TYPE_PS3 :
			case SDL_GAMEPAD_TYPE_PS4 :
				*text = "R3";
				break;
			}
			break;
		case SDL_GAMEPAD_BUTTON_LEFT_SHOULDER :
			switch(type)
			{
			default:
				*text = "LB";
				break;

			case SDL_GAMEPAD_TYPE_PS3 :
			case SDL_GAMEPAD_TYPE_PS4 :
			case SDL_GAMEPAD_TYPE_PS5 :
				*text = "L1";
				break;

			case SDL_GAMEPAD_TYPE_NINTENDO_SWITCH_PRO :
			case SDL_GAMEPAD_TYPE_NINTENDO_SWITCH_JOYCON_PAIR :
				*text = "L";
				break;
			}
			break;
		case SDL_GAMEPAD_BUTTON_RIGHT_SHOULDER :
			switch(type)
			{
			default:
				*text = "RB";
				break;

			case SDL_GAMEPAD_TYPE_PS3 :
			case SDL_GAMEPAD_TYPE_PS4 :
			case SDL_GAMEPAD_TYPE_PS5 :
				*text = "R1";
				break;

			case SDL_GAMEPAD_TYPE_NINTENDO_SWITCH_PRO :
			case SDL_GAMEPAD_TYPE_NINTENDO_SWITCH_JOYCON_PAIR :
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

static bool APP_OpenInputs(void)
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
		SDL_memset(APP_GPIORepeat, 0, sizeof(APP_GPIORepeat));
		SDL_memset(APP_GPIOLines, 0, sizeof(APP_GPIOLines));
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
	SDL_memset(APP_KeyRepeat, 0, sizeof(APP_KeyRepeat));
	#endif

	/* パッドの初期化 */
	#ifdef APP_ENABLE_GAME_CONTROLLER_DB
	// The game will just go without the database if it's missing or fails to load.
	SDL_IOStream* db = APP_OpenRead("gamecontrollerdb.txt");
	if (db) {
		if (SDL_GetIOSize(db) > 0) {
			SDL_AddGamepadMappingsFromIO(db, false);
		}
		SDL_CloseIO(db);
	}
	#endif

	return true;
}

static void APP_CloseInputs(void)
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
		SDL_memset(APP_GPIOLines, 0, sizeof(APP_GPIOLines));
	}
	#endif

	#ifdef APP_ENABLE_KEYBOARD
	SDL_memset(APP_KeyRepeat, 0, sizeof(APP_KeyRepeat));
	#endif

	#if defined(APP_ENABLE_JOYSTICK) || defined(APP_ENABLE_GAME_CONTROLLER)
	#ifdef APP_ENABLE_GAME_CONTROLLER
	if (APP_BuiltinCon.controller && SDL_GamepadConnected(APP_BuiltinCon.controller)) {
		SDL_CloseGamepad(APP_BuiltinCon.controller);
		APP_BuiltinCon.controller = NULL;
	}
	#endif
	APP_PlayerSlotsClose();
	#endif
}

void APP_InputsUpdate(void)
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
	const bool* keyStates = SDL_GetKeyboardState(&numKeys);

	for (int i = 0; i < APP_KEY_MAX; i++)
	{
		if (i < numKeys && keyStates[i])
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

#undef APP_Exit
void APP_Exit(int exitStatus)
{
	APP_Quit();
	APP_QuitFilesystem();
	exit(exitStatus);
}
