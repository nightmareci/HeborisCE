#include "APP_video.h"
#include "APP_filesystem.h"
#include "APP_main.h"
#include "APP_global.h"
#include "APP_bdf.h"

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

SDL_Window* APP_ScreenWindow = NULL;
SDL_Renderer* APP_ScreenRenderer = NULL;
SDL_Texture* APP_ScreenRenderTarget = NULL;

static SDL_Texture		*APP_Textures[APP_TEXTURE_MAX];

#define		APP_BDFFONTFILE_MAX	3
static APP_BDFFont		*APP_BDFFonts[APP_BDFFONTFILE_MAX];

static int			APP_LogicalWidth;
static int			APP_LogicalHeight;

static APP_TextLayer		APP_TextLayers[APP_TEXTLAYER_MAX];
static int32_t			APP_ScreenMode;
static int32_t			APP_ScreenIndex;

static int			APP_NewOffsetX = 0, APP_NewOffsetY = 0;
static int			APP_OffsetX = 0, APP_OffsetY = 0;

static bool APP_NoFrameskip;
static bool APP_LastFrameSkipped;

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

void APP_InitVideo(void)
{
	/* テクスチャ領域の初期化 || Initialize the texture pointers */
	SDL_zero(APP_Textures);

	/* テキストレイヤーの初期化 || Initialize the text layers */
	SDL_zero(APP_TextLayers);
	for ( int i = 0 ; i < APP_TEXTLAYER_MAX ; i ++ )
	{
		APP_TextLayers[i].color.r = APP_TextLayers[i].color.g = APP_TextLayers[i].color.b = 255;
		APP_TextLayers[i].color.a = SDL_ALPHA_OPAQUE;
		APP_TextLayers[i].size = 16;
	}

	APP_PrivateBDFFontInitialize();

	APP_NoFrameskip = false;
	APP_LastFrameSkipped = false;
}

void APP_QuitVideo(void)
{
	if (APP_ScreenRenderer) {
		for (int i = 0; i < APP_TEXTLAYER_MAX; i++) {
			if (APP_TextLayers[i].texture) {
				SDL_DestroyTexture(APP_TextLayers[i].texture);
				APP_TextLayers[i].texture = NULL;
			}
		}
		SDL_memset(APP_TextLayers, 0, sizeof(APP_TextLayers));

		// テクスチャメモリの解放
		// Free texture memory
		for (int i = 0; i < APP_TEXTURE_MAX; i++) {
			if (APP_Textures[i]) {
				SDL_DestroyTexture(APP_Textures[i]);
				APP_Textures[i] = NULL;
			}
		}
	}

	if (APP_ScreenRenderer) {
		SDL_SetRenderTarget(APP_ScreenRenderer, NULL);
	}
	if (APP_ScreenRenderTarget) {
		SDL_DestroyTexture(APP_ScreenRenderTarget);
		APP_ScreenRenderTarget = NULL;
	}
	if (APP_ScreenRenderer) {
		SDL_DestroyRenderer(APP_ScreenRenderer);
		APP_ScreenRenderer = NULL;
	}

	if (APP_ScreenWindow) {
		SDL_DestroyWindow(APP_ScreenWindow);
		APP_ScreenWindow = NULL;
	}

	APP_PrivateBDFFontFinalize();
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

bool APP_RenderScreen(void)
{
	if (!APP_ScreenRenderer) {
		return true;
	}
	SDL_FlushRenderer(APP_ScreenRenderer);

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

	/* 画面ずらし量の反映 */
	APP_OffsetX = APP_NewOffsetX;
	APP_OffsetY = APP_NewOffsetY;
	return true;
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

static int APP_STBI_Read(void* user, char* data, int size)
{
	return (int)SDL_ReadIO((SDL_IOStream*)user, data, size);
}

static void APP_STBI_Skip(void* user, int n)
{
	if (SDL_SeekIO((SDL_IOStream*)user, n, SDL_IO_SEEK_CUR) < 0) {
		APP_Exit(EXIT_FAILURE);
	}
}

static int APP_STBI_EOF(void* user)
{
	switch (SDL_GetIOStatus((SDL_IOStream*)user)) {
	case SDL_IO_STATUS_READY:
		return 0;

	case SDL_IO_STATUS_EOF:
		return 1;

	default:
		APP_Exit(EXIT_FAILURE);
		return 1;
	}
}

void APP_LoadBitmap(const char* filename, unsigned plane)
{
	if (!APP_ScreenRenderer || !filename || plane >= APP_TEXTURE_MAX) {
		APP_Exit(EXIT_FAILURE);
	}

	SDL_IOStream* file = APP_OpenRead(filename);
	if (!file) {
		return;
	}

	int w, h;
	int comp;
	const stbi_io_callbacks callbacks = { APP_STBI_Read, APP_STBI_Skip, APP_STBI_EOF };
	stbi_uc* pixels = stbi_load_from_callbacks(&callbacks, file, &w, &h, &comp, STBI_default);
	if (!pixels) {
		APP_Exit(EXIT_FAILURE);
	}
	SDL_Surface* surface;
	const char* compString;
	switch (comp) {
	case STBI_rgb:
		surface = SDL_CreateSurfaceFrom(w, h, SDL_PIXELFORMAT_RGB24, pixels, w * 3);
		break;

	case STBI_rgb_alpha:
		surface = SDL_CreateSurfaceFrom(w, h, SDL_PIXELFORMAT_RGBA32, pixels, w * 4);
		break;

	case STBI_default:
		compString = "STBI_default";
		goto logUnhandledComp;

	case STBI_grey:
		compString = "STBI_grey";
		goto logUnhandledComp;

	case STBI_grey_alpha:
		compString = "STBI_grey_alpha";
		goto logUnhandledComp;

	default:
		compString = "UNKNOWN";
		goto logUnhandledComp;
	}
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
	return;

logUnhandledComp:
	SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Unhandled stb_image comp of %s", compString);
	APP_Exit(EXIT_FAILURE);
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

float APP_GetScreenSubpixelOffset(void)
{
	// The returned subpixel offset nudges all draws to have pixel coordinates
	// that end up centered in the floating point coordinate space. Without
	// this offset, pixel coordinates are at the upper left of the intended
	// pixels, resulting in off-by-one drawing errors sometimes.
	//
	// The numerator of the return value is the fraction of a pixel to adjust
	// draw position rightwards and downwards, and the division by the current
	// scale converts that subpixel amount to the amount to adjust by within
	// the currently set logical presentation (320x240 or 640x480), so it
	// adjusts by less than 0.49 if the render resolution is above logical
	// resolution, or greater than 0.49 if below logical resolution. If the
	// numerator is exactly 0.5f, then system-dependent rounding errors can
	// occur, because the coordinate is located exactly in the pixel center,
	// producing sampling artifacts on some systems, but not on others. By
	// using a numerator less than 0.5f, the rounding of draw coordinates by
	// the graphics implementation should place drawn pixels in the center. I
	// also saw scaling artifacts happen with a numerator of 0.25f, so maybe
	// the numerator needs to be somewhere in the open range (0.25f, 0.5f) to
	// likely work everywhere; 0.49f seems to work correctly on some systems
	// I've tested.
	//
	// The subpixel adjustment isn't needed when a render target is active,
	// as the logical and framebuffer resolutions are the same in such a
	// case.
	//
	// -Brandon McGriff <nightmareci@gmail.com>
	if (APP_ScreenRenderer && !APP_ScreenRenderTarget) {
		int w;
		if (!SDL_GetRenderLogicalPresentation(APP_ScreenRenderer, &w, NULL, NULL)) {
			APP_Exit(EXIT_FAILURE);
		}
		SDL_FRect rect;
		if (!SDL_GetRenderLogicalPresentationRect(APP_ScreenRenderer, &rect)) {
			APP_Exit(EXIT_FAILURE);
		}
		const float scale = rect.w / w;
		return 0.49f / scale;
	}
	else {
		return 0.0f;
	}
}

void APP_BltAlways(bool always)
{
	APP_NoFrameskip = always;
}
