#include "APP_video.h"
#include "APP_build_config.h"
#include "APP_update.h"
#include "APP_filesystem.h"
#include "APP_main.h"
#include "APP_global.h"
#include "APP_bdf.h"
#include "APP_error.h"
#include <SDL3/SDL_error.h>
#include <SDL3/SDL_render.h>
#include <SDL3_image/SDL_image.h>

#define APP_WINDOW_TITLE "Heboris C.E."

#define APP_WIDE_SCREEN (4.0f / 3.0f)
#define APP_NARROW_SCREEN (3.0f / 4.0f)

#define APP_TEXT_LAYER_STRING_LENGTH 256
typedef struct APP_TextLayer
{
	bool enable;
	int x;
	int y;
	SDL_Color color;
	int size;
	char string[APP_TEXT_LAYER_STRING_LENGTH];
	bool updateTexture;
	SDL_Texture* texture;
	float textureW;
	float textureH;
} APP_TextLayer;

SDL_Window* APP_ScreenWindow = NULL;
static SDL_Renderer* APP_ScreenRenderer = NULL;
static SDL_Texture* APP_ScreenRenderTarget = NULL;

#define APP_PLANE_COUNT 100
static SDL_Texture* APP_Planes[APP_PLANE_COUNT];

#define APP_BDF_FONT_FILE_COUNT	3
static APP_BDFFont* APP_BDFFonts[APP_BDF_FONT_FILE_COUNT];

static int APP_LogicalWidth;
static int APP_LogicalHeight;

#define APP_TEXT_LAYER_COUNT 16
static APP_TextLayer APP_TextLayers[APP_TEXT_LAYER_COUNT];

static int32_t APP_ScreenMode;
static int32_t APP_ScreenIndex;

static size_t APP_TextDataLength = 0;
static SDL_Vertex* APP_TextVertices = NULL;
static int* APP_TextIndices = NULL;

static int APP_NewPlaneDrawOffsetX = 0, APP_NewPlaneDrawOffsetY = 0;
static int APP_PlaneDrawOffsetX = 0, APP_PlaneDrawOffsetY = 0;

static bool APP_DrawWhileSkippingFrames;
static bool APP_LastFrameSkipped;
static uint64_t APP_FrameNS;
static int64_t APP_AccumulatedNS = 0;
static uint64_t APP_LastFrameStepNS;

void APP_ResetFrameStep(void)
{
	APP_FrameNS = SDL_NS_PER_SECOND / APP_GetFPS();
	APP_AccumulatedNS = 0;
	APP_LastFrameStepNS = SDL_GetTicksNS();
}

static bool APP_FrameStep(void)
{
	const bool skipped = APP_AccumulatedNS >= APP_FrameNS;
	uint64_t now;
	if (skipped) {
		now = SDL_GetTicksNS();
		APP_AccumulatedNS -= APP_FrameNS;
	}
	else {
		SDL_DelayNS(APP_FrameNS - APP_AccumulatedNS);
		now = SDL_GetTicksNS();
		APP_AccumulatedNS += now - APP_LastFrameStepNS - APP_FrameNS;
	}
	APP_LastFrameStepNS = now;
	return skipped;
}

static void APP_PrivateBDFFontInitialize(void)
{
	const char* const filenames[APP_BDF_FONT_FILE_COUNT] = {
		"res/font/font10.bdf",
		"res/font/font12.bdf",
		"res/font/font16.bdf"
	};

	/* フォント読み込み */
	/* Load fonts */
	for (int i = 0; i < APP_BDF_FONT_FILE_COUNT; i++) {
		if (APP_BDFFonts[i]) {
			continue;
		}
		SDL_IOStream* const src = APP_OpenRead(filenames[i]);
		if (!src) {
			SDL_Log("Failed to open font file \"%s\"; continuing without it.", filenames[i]);
			APP_BDFFonts[i] = NULL;
			continue;
		}
		APP_BDFFonts[i] = APP_OpenBDFFont(src);
		if (!APP_BDFFonts[i]) {
			SDL_CloseIO(src);
			APP_SetError("Failed to load font file \"%s\": %s", filenames[i], SDL_GetError());
			APP_Exit(EXIT_FAILURE);
		}
		SDL_CloseIO(src);
	}
}

static void APP_PrivateBDFFontFinalize(void)
{
	for (int i = 0; i < APP_BDF_FONT_FILE_COUNT; i++) {
		if (APP_BDFFonts[i]) {
			APP_CloseBDFFont(APP_BDFFonts[i]);
		}
		APP_BDFFonts[i] = NULL;
	}
}

void APP_InitVideo(void)
{
	/* テクスチャ領域の初期化 || Initialize the texture pointers */
	SDL_zero(APP_Planes);

	/* テキストレイヤーの初期化 || Initialize the text layers */
	SDL_zero(APP_TextLayers);
	for (int i = 0; i < APP_TEXT_LAYER_COUNT; i++) {
		APP_TextLayers[i].color.r = APP_TextLayers[i].color.g = APP_TextLayers[i].color.b = 255;
		APP_TextLayers[i].color.a = SDL_ALPHA_OPAQUE;
		APP_TextLayers[i].size = 16;
	}

	APP_PrivateBDFFontInitialize();

	APP_DrawWhileSkippingFrames = false;
	APP_LastFrameSkipped = false;
}

void APP_QuitVideo(void)
{
	if (APP_ScreenRenderer) {
		for (int i = 0; i < APP_TEXT_LAYER_COUNT; i++) {
			if (APP_TextLayers[i].texture) {
				SDL_DestroyTexture(APP_TextLayers[i].texture);
				APP_TextLayers[i].texture = NULL;
			}
		}
		SDL_memset(APP_TextLayers, 0, sizeof(APP_TextLayers));

		// テクスチャメモリの解放
		// Free texture memory
		for (int i = 0; i < APP_PLANE_COUNT; i++) {
			if (APP_Planes[i]) {
				SDL_DestroyTexture(APP_Planes[i]);
				APP_Planes[i] = NULL;
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

	SDL_free(APP_TextVertices);
	APP_TextVertices = NULL;
	SDL_free(APP_TextIndices);
	APP_TextIndices = NULL;
	APP_TextDataLength = 0;

	APP_PrivateBDFFontFinalize();
}

void APP_SetScreen(APP_ScreenModeFlag* screenMode, int32_t* screenIndex)
{
	int windowX, windowY;
	int logicalWidth, logicalHeight;
	SDL_DisplayID* displays = NULL;
	SDL_DisplayMode** displayModes = NULL;

	/* 画面の設定 || Set up the screen */

	/* Validate the window type */
	APP_ScreenModeFlag windowType = *screenMode & APP_SCREEN_MODE_WINDOW_TYPE;
	if (windowType < 0 || windowType >= APP_SCREEN_MODE_WINDOW_TYPES_COUNT) {
		APP_SetError("Invalid window type value of %d", (int)windowType);
		goto fail;
	}

	int displayIndex = APP_SCREEN_INDEX_DISPLAY_TO_VALUE(*screenIndex);
	int modeIndex = APP_SCREEN_INDEX_MODE_TO_VALUE(*screenIndex);
	int displayCount;
	displays = SDL_GetDisplays(&displayCount);
	if (!displays) {
		APP_SetError("Could not get list of displays: %s", SDL_GetError());
		goto fail;
	}
	const SDL_DisplayID display = displays[displayIndex];
	SDL_free(displays);
	displays = NULL;
	int displayModeCount;
	displayModes = SDL_GetFullscreenDisplayModes(display, &displayModeCount);
	if (!displayModes) {
		APP_SetError("Could not get list of fullscreen display modes: %s", SDL_GetError());
		goto fail;
	}

	windowX = SDL_WINDOWPOS_CENTERED_DISPLAY(display);
	windowY = SDL_WINDOWPOS_CENTERED_DISPLAY(display);
	if (
		displayIndex >= displayCount ||
		((windowType == APP_SCREEN_MODE_FULLSCREEN || windowType == APP_SCREEN_MODE_FULLSCREEN_DESKTOP) && modeIndex >= displayModeCount)
	) {
		*screenMode = APP_DEFAULT_SCREEN_MODE;
		*screenIndex = 0;
	}
	const bool maximized = windowType == APP_SCREEN_MODE_WINDOW_MAXIMIZED;

	if (*screenMode & APP_SCREEN_MODE_DETAIL_LEVEL) {
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
		windowType == APP_SCREEN_MODE_FULLSCREEN ||
		windowType == APP_SCREEN_MODE_FULLSCREEN_DESKTOP
	)
	{
		if (!APP_ScreenWindow) {
			SDL_PropertiesID props = SDL_CreateProperties();
			if (!props) {
				APP_SetError("Could not create properties object: %s", SDL_GetError());
				goto fail;
			}
			if (
				!SDL_SetStringProperty(props, SDL_PROP_WINDOW_CREATE_TITLE_STRING, APP_WINDOW_TITLE) ||
				!SDL_SetBooleanProperty(props, SDL_PROP_WINDOW_CREATE_FULLSCREEN_BOOLEAN, true) ||
				!SDL_SetNumberProperty(props, SDL_PROP_WINDOW_CREATE_X_NUMBER, windowX) ||
				!SDL_SetNumberProperty(props, SDL_PROP_WINDOW_CREATE_Y_NUMBER, windowY) ||
				!SDL_SetNumberProperty(props, SDL_PROP_WINDOW_CREATE_WIDTH_NUMBER, logicalWidth) ||
				!SDL_SetNumberProperty(props, SDL_PROP_WINDOW_CREATE_HEIGHT_NUMBER, logicalHeight) ||
				!SDL_SetBooleanProperty(props, SDL_PROP_WINDOW_CREATE_RESIZABLE_BOOLEAN, true)
			) {
				APP_SetError("Could not set up properties object: %s", SDL_GetError());
				goto fail;
			}
			APP_ScreenWindow = SDL_CreateWindowWithProperties(props);
			SDL_DestroyProperties(props);
			if (!APP_ScreenWindow || (windowType == APP_SCREEN_MODE_FULLSCREEN && !SDL_SetWindowFullscreenMode(APP_ScreenWindow, displayModes[modeIndex]))) {
				APP_SetError("Could not set up window: %s", SDL_GetError());
				goto fail;
			}
		}
		else
		{
			if (
				!SDL_SetWindowFullscreen(APP_ScreenWindow, true) ||
				!SDL_SetWindowSize(APP_ScreenWindow, logicalWidth, logicalHeight) ||
				!SDL_SetWindowFullscreenMode(APP_ScreenWindow, (windowType == APP_SCREEN_MODE_FULLSCREEN) ? displayModes[modeIndex] : NULL)
			) {
				APP_SetError("Could not set up window: %s", SDL_GetError());
				goto fail;
			}
			// NOTE: This can fail on some platforms due to not being supported (Wayland), so don't check the return status
			SDL_SetWindowPosition(APP_ScreenWindow, windowX, windowY);
		}
	}
	else if (
		windowType == APP_SCREEN_MODE_WINDOW ||
		windowType == APP_SCREEN_MODE_WINDOW_MAXIMIZED
	)
	{
		const SDL_DisplayMode* const displayMode = SDL_GetDesktopDisplayMode(display);
		if (!displayMode) {
			APP_SetError("Could not get desktop display mode: %s", SDL_GetError());
			goto fail;
		}
		int maxScale;
		if (displayMode->w <= logicalWidth || displayMode->h <= logicalHeight) {
			maxScale = 1;
		}
		else if (displayMode->w > displayMode->h) {
			maxScale = (displayMode->h / logicalHeight) - (displayMode->h % logicalHeight == 0);
		}
		else {
			maxScale = (displayMode->w / logicalWidth) - (displayMode->w % logicalWidth == 0);
		}
		int scale = modeIndex + 1;
		int windowW = scale * logicalWidth;
		int windowH = scale * logicalHeight;
		if (scale > maxScale) {
			windowW = logicalWidth;
			windowH = logicalHeight;
			*screenIndex = APP_SCREEN_INDEX_TO_SETTING(displayIndex, 0);
		}
		if (!APP_ScreenWindow) {
			SDL_PropertiesID props = SDL_CreateProperties();
			if (!props) {
				APP_SetError("Could not create properties object: %s", SDL_GetError());
				goto fail;
			}
			if (
				!SDL_SetStringProperty(props, SDL_PROP_WINDOW_CREATE_TITLE_STRING, APP_WINDOW_TITLE) ||
				!SDL_SetNumberProperty(props, SDL_PROP_WINDOW_CREATE_X_NUMBER, windowX) ||
				!SDL_SetNumberProperty(props, SDL_PROP_WINDOW_CREATE_Y_NUMBER, windowY) ||
				!SDL_SetNumberProperty(props, SDL_PROP_WINDOW_CREATE_WIDTH_NUMBER, windowW) ||
				!SDL_SetNumberProperty(props, SDL_PROP_WINDOW_CREATE_HEIGHT_NUMBER, windowH) ||
				!SDL_SetBooleanProperty(props, SDL_PROP_WINDOW_CREATE_RESIZABLE_BOOLEAN, true) ||
				!SDL_SetBooleanProperty(props, SDL_PROP_WINDOW_CREATE_MAXIMIZED_BOOLEAN, maximized)
			) {
				APP_SetError("Could not set up properties object: %s", SDL_GetError());
				goto fail;
			}
			APP_ScreenWindow = SDL_CreateWindowWithProperties(props);
			SDL_DestroyProperties(props);
			if (!APP_ScreenWindow) {
				APP_SetError("Could not create window: %s", SDL_GetError());
				goto fail;
			}
		}
		else {
			if (maximized) {
				if (!SDL_MaximizeWindow(APP_ScreenWindow)) {
					APP_SetError("Could not maximize window: %s", SDL_GetError());
					goto fail;
				}
			}
			if (
				!SDL_SetWindowFullscreen(APP_ScreenWindow, false) ||
				!SDL_SetWindowResizable(APP_ScreenWindow, true) ||
				!SDL_SetWindowSize(APP_ScreenWindow, windowW, windowH)
			) {
				APP_SetError("Could not set up window: %s", SDL_GetError());
				goto fail;
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
	if (!APP_ScreenRenderer) {
		APP_ScreenRenderer = SDL_CreateRenderer(APP_ScreenWindow, NULL);
		if (!APP_ScreenRenderer)
		{
			APP_SetError("Could not create renderer: %s", SDL_GetError());
			goto fail;
		}
	}
	if (
		!SDL_RenderClear(APP_ScreenRenderer) ||
		!SDL_RenderPresent(APP_ScreenRenderer)
	) {
		APP_SetError("Could not do initial render clear of screen: %s", SDL_GetError());
		goto fail;
	}

	// Unset the render target, if currently set, for making renderer setting
	// changes. The render target should only be set once all settings have been
	// set, with no setting changes made after the render target has been set;
	// various bugs have been observed when attempting setting changes while the
	// render target is non-NULL, such bugs disappearing when setting changes
	// are made only while the render target is NULL.
	if (APP_ScreenRenderTarget && !SDL_SetRenderTarget(APP_ScreenRenderer, NULL)) {
		APP_SetError("Could not unset render target: %s", SDL_GetError());
		goto fail;
	}

	/* Clear the whole screen, as the framebuffer might be uninitialized */
	if (!SDL_RenderClear(APP_ScreenRenderer)) {
		APP_SetError("Could not do initial render clear of screen: %s", SDL_GetError());
		goto fail;
	}

	if (!SDL_SetRenderVSync(APP_ScreenRenderer, (*screenMode & APP_SCREEN_MODE_VSYNC) ? 1 : SDL_RENDERER_VSYNC_DISABLED) ) {
		APP_SetError("Could not set render vsync mode: %s", SDL_GetError());
		goto fail;
	}

	if (*screenMode & APP_SCREEN_MODE_SCALE_MODE) {
		if (!SDL_SetRenderLogicalPresentation(APP_ScreenRenderer, logicalWidth, logicalHeight, SDL_LOGICAL_PRESENTATION_INTEGER_SCALE)) {
			APP_SetError("Could not set render dimensions: %s", SDL_GetError());
			goto fail;
		}
	}
	else {
		if (!SDL_SetRenderLogicalPresentation(APP_ScreenRenderer, logicalWidth, logicalHeight, SDL_LOGICAL_PRESENTATION_LETTERBOX)) {
			APP_SetError("Could not set render dimensions: %s", SDL_GetError());
			goto fail;
		}
	}

	/* Set up the render target, if required */
	if (!(*screenMode & APP_SCREEN_MODE_RENDER_LEVEL)) {
		// There's no need to create a render target texture if the
		// currently created render target texture is already the current
		// logicalWidth x logicalHeight.
		bool createNewRenderTarget = true;

		if (APP_ScreenRenderTarget) {
			float w, h;
			if (!SDL_GetTextureSize(APP_ScreenRenderTarget, &w, &h)) {
				APP_SetError("Could not get render target texture size: %s", SDL_GetError());
				goto fail;
			}

			if ((int)w == logicalWidth && (int)h == logicalHeight) {
				createNewRenderTarget = false;
			}
			else {
				SDL_DestroyTexture(APP_ScreenRenderTarget);
				APP_ScreenRenderTarget = NULL;
			}
		}

		if (createNewRenderTarget) {
			APP_ScreenRenderTarget = SDL_CreateTexture(APP_ScreenRenderer, SDL_PIXELFORMAT_RGBX8888, SDL_TEXTUREACCESS_TARGET, logicalWidth, logicalHeight);
			if (!APP_ScreenRenderTarget) {
				APP_SetError("Could not create render target texture: %s", SDL_GetError());
				goto fail;
			}
			if (!SDL_SetTextureScaleMode(APP_ScreenRenderTarget, SDL_SCALEMODE_NEAREST)) {
				APP_SetError("Could not set render target to nearest neighbor texture filtering: %s", SDL_GetError());
				goto fail;
			}
		}

		if (
			!SDL_SetRenderTarget(APP_ScreenRenderer, APP_ScreenRenderTarget) ||
			!SDL_RenderClear(APP_ScreenRenderer)
		) {
			APP_SetError("Could not set up render target: %s", SDL_GetError());
			goto fail;
		}
	}
	else if (APP_ScreenRenderTarget) {
		SDL_DestroyTexture(APP_ScreenRenderTarget);
		APP_ScreenRenderTarget = NULL;
	}

	// This should be after the renderer has been set up, as
	// APP_SCREEN_SUBPIXEL_OFFSET queries the renderer when a given platform
	// uses nonzero offsets.
	APP_ScreenSubpixelOffset = APP_SCREEN_SUBPIXEL_OFFSET;

	// WARNING: Make no changes to the renderer settings from here on down, as
	// the render target has been set, and bugs have been observed when
	// attempting renderer setting changes while a non-NULL render target is
	// set.

	APP_ScreenMode = *screenMode;
	APP_ScreenIndex = *screenIndex;

	/* Hide the mouse cursor at first */
	if (!SDL_HideCursor()) {
		APP_SetError("Could not hide mouse cursor: %s", SDL_GetError());
		goto fail;
	}

	while (!SDL_SyncWindow(APP_ScreenWindow)) {
		SDL_Delay(1);
	}

	/* Setup was successful, so return with success */
	return;

	/* A failure condition was encountered, so clean up and return with error */
	fail:
	if (APP_ScreenRenderTarget) {
		SDL_SetRenderTarget(APP_ScreenRenderer, NULL);
		SDL_DestroyTexture(APP_ScreenRenderTarget);
	}
	if (APP_ScreenRenderer) {
		SDL_DestroyRenderer(APP_ScreenRenderer);
	}
	if (APP_ScreenWindow) {
		SDL_DestroyWindow(APP_ScreenWindow);
	}
	SDL_free(displayModes);
	SDL_free(displays);
	APP_ScreenRenderTarget = NULL;
	APP_ScreenRenderer = NULL;
	APP_ScreenWindow = NULL;
	APP_Exit(EXIT_FAILURE);
}

void APP_RenderScreen(void)
{
	if (!APP_ScreenRenderer) {
		APP_SetError("Tried rendering when renderer isn't available");
		APP_Exit(EXIT_FAILURE);
	}
	if (!SDL_FlushRenderer(APP_ScreenRenderer)) {
		goto fail;
	}

	#ifndef NDEBUG
	APP_DrawWhileSkippingFrames = true;
	#endif
	if (APP_DrawWhileSkippingFrames) {
		/* バックサーフェスをフロントに転送 */
		if (APP_ScreenRenderTarget) {
			if (
				!SDL_SetRenderTarget(APP_ScreenRenderer, NULL) ||
				!SDL_RenderClear(APP_ScreenRenderer) ||
				!SDL_RenderTexture(APP_ScreenRenderer, APP_ScreenRenderTarget, NULL, NULL) ||
				!SDL_RenderPresent(APP_ScreenRenderer) ||
				!SDL_SetRenderTarget(APP_ScreenRenderer, APP_ScreenRenderTarget)
			) {
				goto fail;
			}
		}
		else if (!SDL_RenderPresent(APP_ScreenRenderer)) {
			goto fail;
		}

		/* フレームレート待ち || Frame rate waiting */
		APP_LastFrameSkipped = APP_FrameStep();

		/* 画面塗りつぶし || Fill screen */
		if (!SDL_RenderClear(APP_ScreenRenderer)) {
			goto fail;
		}
	}
	else {
		if (!APP_LastFrameSkipped) {
			/* バックサーフェスをフロントに転送 */
			if (APP_ScreenRenderTarget) {
				if (
					!SDL_SetRenderTarget(APP_ScreenRenderer, NULL) ||
					!SDL_RenderClear(APP_ScreenRenderer) ||
					!SDL_RenderTexture(APP_ScreenRenderer, APP_ScreenRenderTarget, NULL, NULL) ||
					!SDL_RenderPresent(APP_ScreenRenderer) ||
					!SDL_SetRenderTarget(APP_ScreenRenderer, APP_ScreenRenderTarget)
				) {
					goto fail;
				}
			}
			else if(!SDL_RenderPresent(APP_ScreenRenderer)) {
				goto fail;
			}

			/* 画面塗りつぶし */
			if (!SDL_RenderClear(APP_ScreenRenderer)) {
				goto fail;
			}
		}

		/* フレームレート待ち || Frame rate waiting */
		APP_LastFrameSkipped = APP_FrameStep();
	}

	/* 画面ずらし量の反映 */
	APP_PlaneDrawOffsetX = APP_NewPlaneDrawOffsetX;
	APP_PlaneDrawOffsetY = APP_NewPlaneDrawOffsetY;
	return;

	fail:
	APP_SetError("Failed rendering: %s", SDL_GetError());
	APP_Exit(EXIT_FAILURE);
}

int APP_GetMaxDisplayIndex(void)
{
	int displayCount;
	SDL_DisplayID* displays = SDL_GetDisplays(&displayCount);
	if (!displays) {
		APP_SetError("Failed getting list of displays: %s", SDL_GetError());
		return -1;
	}
	SDL_free(displays);
	return displayCount;
}

int APP_GetMaxDisplayMode(int displayIndex)
{
	if (displayIndex < 0) {
		APP_SetError("displayIndex invalid, must be at least 0");
		return -1;
	}
	int displayCount;
	SDL_DisplayID* displays = SDL_GetDisplays(&displayCount);
	if (!displays) {
		APP_SetError("Failed getting list of displays: %s", SDL_GetError());
		return -1;
	}
	if (displayIndex >= displayCount) {
		SDL_free(displays);
		APP_SetError("Display index is too high");
		return -1;
	}
	int modeCount;
	SDL_DisplayMode** modes = SDL_GetFullscreenDisplayModes(displays[displayIndex], &modeCount);
	if (!modes) {
		SDL_free(displays);
		APP_SetError("Could not get list of fullscreen display modes");
		return -1;
	}
	SDL_free(displays);
	SDL_free(modes);
	return modeCount;
}

void APP_GetDisplayMode(int displayIndex, int modeIndex, SDL_DisplayMode* mode)
{
	if (displayIndex < 0) {
		APP_SetError("%s", SDL_InvalidParamError("displayIndex"));
		APP_Exit(EXIT_FAILURE);
	}
	else if (modeIndex < 0) {
		APP_SetError("%s", SDL_InvalidParamError("modeIndex"));
		APP_Exit(EXIT_FAILURE);
	}
	else if (!mode) {
		APP_SetError("%s", SDL_InvalidParamError("mode"));
		APP_Exit(EXIT_FAILURE);
	}
	int displayCount;
	SDL_DisplayID* displays = SDL_GetDisplays(&displayCount);
	if (!displays) {
		APP_SetError("Could not get list of displays: %s", SDL_GetError());
		APP_Exit(EXIT_FAILURE);
	}
	if (displayIndex >= displayCount) {
		SDL_free(displays);
		APP_SetError("Number of displays is %d but requested display index is too high at %d", displayCount, displayIndex);
		APP_Exit(EXIT_FAILURE);
	}
	int modeCount;
	SDL_DisplayMode** modes = SDL_GetFullscreenDisplayModes(displays[displayIndex], &modeCount);
	SDL_free(displays);
	if (modeIndex >= modeCount) {
		SDL_free(modes);
		APP_SetError("Number of display modes is %d but requested display mode index is too high at %d", modeCount, modeIndex);
		APP_Exit(EXIT_FAILURE);
	}
	*mode = *modes[modeIndex];
}

const SDL_DisplayMode* APP_GetDesktopDisplayMode(int displayIndex)
{
	if (displayIndex < 0) {
		APP_SetError("Invalid display index of %d, must be at least 0", displayIndex);
		APP_Exit(EXIT_FAILURE);
	}
	int displayCount;
	SDL_DisplayID* displays = SDL_GetDisplays(&displayCount);
	if (!displays) {
		APP_SetError("Error getting list of displays: %s", SDL_GetError());
		APP_Exit(EXIT_FAILURE);
	}
	if (displayIndex >= displayCount) {
		SDL_free(displays);
		APP_SetError("Display index is too high");
		APP_Exit(EXIT_FAILURE);
	}
	const SDL_DisplayMode* mode = SDL_GetDesktopDisplayMode(displays[displayIndex]);
	SDL_free(displays);
	if (!mode) {
		APP_SetError("Error getting desktop display mode: %s", SDL_GetError());
		APP_Exit(EXIT_FAILURE);
	}
	return mode;
}

void APP_EnableTextLayer(int layer, int x, int y)
{
	if (layer < 0 || layer >= APP_TEXT_LAYER_COUNT) {
		APP_SetError("Invalid layer number requested to enable, must be in range 0 to %d", APP_TEXT_LAYER_COUNT);
		APP_Exit(EXIT_FAILURE);
	}
	APP_TextLayers[layer].enable = true;
	APP_TextLayers[layer].x = x;
	APP_TextLayers[layer].y = y;
}

void APP_SetTextLayerDrawPosition(int layer, int x, int y)
{
	if (layer < 0 || layer >= APP_TEXT_LAYER_COUNT) {
		APP_SetError("Invalid layer number requested to set draw position, must be in range 0 to %d", APP_TEXT_LAYER_COUNT);
		APP_Exit(EXIT_FAILURE);
	}
	APP_TextLayers[layer].x = x;
	APP_TextLayers[layer].y = y;
}

void APP_SetTextLayerColor(int layer, int r, int g, int b)
{
	if (layer < 0 || layer >= APP_TEXT_LAYER_COUNT) {
		APP_SetError("Invalid layer number requested to set text color, must be in range 0 to %d", APP_TEXT_LAYER_COUNT);
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

void APP_SetTextLayerSize(int layer, int size)
{
	if (layer < 0 || layer >= APP_TEXT_LAYER_COUNT) {
		APP_SetError("Invalid layer number requested to set text size, must be in range 0 to %d", APP_TEXT_LAYER_COUNT);
		APP_Exit(EXIT_FAILURE);
	}
	if (APP_TextLayers[layer].size != size) {
		APP_TextLayers[layer].size = size;
		APP_TextLayers[layer].updateTexture = true;
	}
}

void APP_PutTextLayerString(int layer, const char* string)
{
	if (layer < 0 || layer >= APP_TEXT_LAYER_COUNT) {
		APP_SetError("Invalid layer number requested to put text into, must be in range 0 to %d", APP_TEXT_LAYER_COUNT);
		APP_Exit(EXIT_FAILURE);
	}
	if (SDL_strcmp(string, APP_TextLayers[layer].string) != 0) {
		SDL_strlcpy(APP_TextLayers[layer].string, string, APP_TEXT_LAYER_STRING_LENGTH - 1);
		APP_TextLayers[layer].updateTexture = true;
	}
}

void APP_DrawTextLayer(int layer)
{
	if (layer < 0 || layer >= APP_TEXT_LAYER_COUNT) {
		APP_SetError("Invalid layer number requested to draw, must be in range 0 to %d", APP_TEXT_LAYER_COUNT);
		APP_Exit(EXIT_FAILURE);
	}
	if (!APP_ScreenRenderer || !APP_TextLayers[layer].enable) {
		return;
	}

	int font = 0;

	if (APP_TextLayers[layer].size >= 12) {
		font++;
	}
	if (APP_TextLayers[layer].size >= 16) {
		font++;
	}

	if (!APP_BDFFonts[font]) {
		return;
	}

	if (APP_TextLayers[layer].updateTexture) {
		if (APP_TextLayers[layer].texture) {
			SDL_DestroyTexture(APP_TextLayers[layer].texture);
		}
		APP_TextLayers[layer].texture = APP_CreateBDFTextTexture(APP_BDFFonts[font], APP_ScreenRenderer, APP_TextLayers[layer].string, APP_TextLayers[layer].color, 32, SDL_SCALEMODE_NEAREST);
		if (!APP_TextLayers[layer].texture) {
			APP_SetError("Error creating texture to blit text: %s", SDL_GetError());
			APP_Exit(EXIT_FAILURE);
		}
		if (!SDL_GetTextureSize(APP_TextLayers[layer].texture, &APP_TextLayers[layer].textureW, &APP_TextLayers[layer].textureH)) {
			APP_SetError("Error getting size of a text layer texture: %s", SDL_GetError());
			APP_Exit(EXIT_FAILURE);
		}
		APP_TextLayers[layer].updateTexture = false;
	}

	if (APP_TextLayers[layer].texture) {
		SDL_FRect dstRect = {
			APP_TextLayers[layer].x + APP_PlaneDrawOffsetX,
			APP_TextLayers[layer].y + APP_PlaneDrawOffsetY,
			APP_TextLayers[layer].textureW,
			APP_TextLayers[layer].textureH
		};
		if (!APP_ScreenRenderTarget) {
			dstRect.x += APP_ScreenSubpixelOffset;
			dstRect.y += APP_ScreenSubpixelOffset;
		}
		if (!SDL_RenderTexture(APP_ScreenRenderer, APP_TextLayers[layer].texture, NULL, &dstRect)) {
			APP_SetError("Error rendering text layer: %s", SDL_GetError());
			APP_Exit(EXIT_FAILURE);
		}
	}
}

void APP_DisableTextLayer(int layer) {
	if (layer < 0 || layer >= APP_TEXT_LAYER_COUNT) {
		APP_SetError("Invalid layer number requested to disable, must be in range 0 to %d", APP_TEXT_LAYER_COUNT);
		APP_Exit(EXIT_FAILURE);
	}
	APP_TextLayers[layer].enable = false;
}

void APP_LoadPlane(int plane, const char* filename)
{
	if (!APP_ScreenRenderer || !filename || !*filename || plane < 0 || plane >= APP_PLANE_COUNT) {
		return;
	}

	const char* const types[] = {
		"png",
		"gif",
		"bmp",
		"tga",
		"webp",
		"qoi",
		"svg",
		"jpg",
		"avif",
		"jxl",
		"tif",
		"xcf",
		"pcx",
		"ico",
		"cur",
		"ani",
		"lbm",
		"pnm",
		"xpm",
		"xv",
	};
	SDL_IOStream* file = NULL;
	char* filenameExt;
	const char* type;
	for (size_t i = 0; i < SDL_arraysize(types); i++) {
		if (SDL_asprintf(&filenameExt, "%s.%s", filename, types[i]) < 0) {
			APP_SetError("Error loading image");
			APP_Exit(EXIT_FAILURE);
		}
		if (!APP_FileExists(filenameExt)) {
			SDL_free(filenameExt);
			continue;
		}
		file = APP_OpenRead(filenameExt);
		if (!file) {
			SDL_free(filenameExt);
			APP_SetError("Error loading image");
			APP_Exit(EXIT_FAILURE);
		}
		type = types[i];
		break;
	}
	if (!file) {
		return;
	}

	if (APP_Planes[plane]) {
		SDL_DestroyTexture(APP_Planes[plane]);
	}
	APP_Planes[plane] = IMG_LoadTextureTyped_IO(APP_ScreenRenderer, file, true, type);
	if (!APP_Planes[plane]) {
		char* message;
		if (SDL_asprintf(&message,
			"Image file \"%s\" failed to load.\n"
			"It might be corrupt, so you would need to replace it.\n"
			"Or the build of SDL_image in use might not support its format, so try another format.",
			filenameExt
		) < 0) {
			APP_SetError("Error loading image");
			APP_Exit(EXIT_FAILURE);
		}
		APP_SetError("%s", message);
		APP_Exit(EXIT_FAILURE);
	}
	if (
		!SDL_SetTextureScaleMode(APP_Planes[plane], SDL_SCALEMODE_NEAREST) ||
		!SDL_SetTextureBlendMode(APP_Planes[plane], SDL_BLENDMODE_BLEND)
	) {
		APP_SetError("Error loading image: %s", SDL_GetError());
		APP_Exit(EXIT_FAILURE);
	}
}

void APP_DrawPlane(int plane, int dstX, int dstY) {
	if (plane < 0 || plane >= APP_PLANE_COUNT || !APP_Planes[plane]) {
		return;
	}
	float w, h;
	if (!SDL_GetTextureSize(APP_Planes[plane], &w, &h)) {
		APP_SetError("Error rendering graphics: %s", SDL_GetError());
		APP_Exit(EXIT_FAILURE);
	}
	APP_DrawPlaneRect(plane, dstX, dstY, 0, 0, (int)w, (int)h);
}

void APP_DrawPlaneRect(int plane, int dstX, int dstY, int srcX, int srcY, int w, int h)
{
	if (plane < 0 || plane >= APP_PLANE_COUNT || !APP_Planes[plane] || w == 0 || h == 0) {
		return;
	}

	const SDL_FRect src = { srcX, srcY, w, h };
	SDL_FRect dst = {
		dstX + APP_PlaneDrawOffsetX,
		dstY + APP_PlaneDrawOffsetY,
		w,
		h
	};
	if (!APP_ScreenRenderTarget) {
		dst.x += APP_ScreenSubpixelOffset;
		dst.y += APP_ScreenSubpixelOffset;
	}

	if (!SDL_RenderTexture(APP_ScreenRenderer, APP_Planes[plane], &src, &dst)) {
		APP_SetError("Error rendering graphics: %s", SDL_GetError());
		APP_Exit(EXIT_FAILURE);
	}
}

void APP_DrawPlaneTransparent(int plane, int dstX, int dstY, int a)
{
	if (plane < 0 || plane >= APP_PLANE_COUNT || !APP_Planes[plane]) {
		return;
	}

	if (!SDL_SetTextureAlphaMod(APP_Planes[plane], a)) {
		APP_SetError("Error rendering graphics: %s", SDL_GetError());
		APP_Exit(EXIT_FAILURE);
	}
	APP_DrawPlane(plane, dstX, dstY);
	if (!SDL_SetTextureAlphaMod(APP_Planes[plane], SDL_ALPHA_OPAQUE)) {
		APP_SetError("Error rendering graphics: %s", SDL_GetError());
		APP_Exit(EXIT_FAILURE);
	}
}

void APP_DrawPlaneRectTransparent(int plane, int dstX, int dstY, int srcX, int srcY, int w, int h, int a)
{
	if (plane < 0 || plane >= APP_PLANE_COUNT || !APP_Planes[plane] || w == 0 || h == 0) {
		return;
	}

	if (!SDL_SetTextureAlphaMod(APP_Planes[plane], a)) {
		APP_SetError("Error rendering graphics: %s", SDL_GetError());
		APP_Exit(EXIT_FAILURE);
	}
	APP_DrawPlaneRect(plane, dstX, dstY, srcX, srcY, w, h);
	if (!SDL_SetTextureAlphaMod(APP_Planes[plane], SDL_ALPHA_OPAQUE)) {
		APP_SetError("Error rendering graphics: %s", SDL_GetError());
		APP_Exit(EXIT_FAILURE);
	}
}

void APP_DrawPlaneScaled(int plane, int dstX, int dstY, int scaleW, int scaleH)
{
	APP_DrawPlaneRectScaled(plane, dstX, dstY, 0, 0, APP_LogicalWidth, APP_LogicalHeight, scaleW, scaleH);
}

void APP_DrawPlaneRectScaled(int plane, int dstX, int dstY, int srcX, int srcY, int w, int h, int scaleW, int scaleH)
{
	if (plane < 0 || plane >= APP_PLANE_COUNT || !APP_Planes[plane] || w == 0 || h == 0) {
		return;
	}

	// ちゃんと拡大して描画する
	const SDL_FRect src = { srcX, srcY, w, h };
	SDL_FRect dst = {
		dstX + APP_PlaneDrawOffsetX,
		dstY + APP_PlaneDrawOffsetY,
		(int)(w * (scaleW / 65536.0f)),
		(int)(h * (scaleH / 65536.0f))
	};
	if (!APP_ScreenRenderTarget) {
		dst.x += APP_ScreenSubpixelOffset;
		dst.y += APP_ScreenSubpixelOffset;
	}
	if (!SDL_RenderTexture(APP_ScreenRenderer, APP_Planes[plane], &src, &dst)) {
		APP_SetError("Error rendering graphics: %s", SDL_GetError());
		APP_Exit(EXIT_FAILURE);
	}
}

void APP_DrawPlaneTransparentScaled(int plane, int dstX, int dstY, int a, int scaleW, int scaleH)
{
	APP_DrawPlaneRectTransparentScaled(plane, dstX, dstY, 0, 0, APP_LogicalWidth, APP_LogicalHeight, a, scaleW, scaleH);
}

void APP_DrawPlaneRectTransparentScaled(int plane, int dstX, int dstY, int srcX, int srcY, int w, int h, int a, int scaleW, int scaleH)
{
	if (plane < 0 || plane >= APP_PLANE_COUNT || !APP_Planes[plane] || w == 0 || h == 0) {
		return;
	}

	// ちゃんと拡大して描画する
	const SDL_FRect src = { srcX, srcY, w, h };
	SDL_FRect dst = {
		dstX + APP_PlaneDrawOffsetX,
		dstY + APP_PlaneDrawOffsetX,
		w * (scaleW / 65536.0f),
		h * (scaleH / 65536.0f)
	};
	if (!APP_ScreenRenderTarget) {
		dst.x += APP_ScreenSubpixelOffset;
		dst.y += APP_ScreenSubpixelOffset;
	}
	if (
		!SDL_SetTextureAlphaMod(APP_Planes[plane], a) ||
		!SDL_RenderTexture(APP_ScreenRenderer, APP_Planes[plane], &src, &dst) ||
		!SDL_SetTextureAlphaMod(APP_Planes[plane], SDL_ALPHA_OPAQUE)
	) {
		APP_SetError("Error rendering graphics: %s", SDL_GetError());
		APP_Exit(EXIT_FAILURE);
	}
}

void APP_DrawPlaneText(int plane, const char* text, char firstChar, int charW, int charH, int dstX, int dstY, int srcX, int srcY, int srcW)
{
	if (
		plane < 0 || plane >= APP_PLANE_COUNT || !APP_Planes[plane] ||
		!text || !*text || charW <= 0 || charH <= 0 || dstX >= APP_LogicalWidth || dstY >= APP_LogicalHeight || dstY + charH <= 0
	) {
		return;
	}
	const size_t length = SDL_strlen(text);
	if (dstX + length * charW <= 0) {
		return;
	}

	if (length > APP_TextDataLength) {
		SDL_Vertex* const vertices = SDL_realloc(APP_TextVertices, length * 4 * sizeof(SDL_Vertex));
		if (!vertices) {
			APP_SetError("Could not allocate vertex array");
			goto fail;
		}
		APP_TextVertices = vertices;
		for (
			SDL_Vertex* vertex = APP_TextVertices + APP_TextDataLength * 4, * const verticesEnd = APP_TextVertices + length * 4;
			vertex < verticesEnd;
			vertex++
		) {
			vertex->color = (SDL_FColor) { 1.0f, 1.0f, 1.0f, SDL_ALPHA_OPAQUE_FLOAT };
		}

		int* const indices = SDL_realloc(APP_TextIndices, length * 6 * sizeof(int));
		if (!indices) {
			APP_SetError("Could not allocate index array");
			goto fail;
		}
		APP_TextIndices = indices;
		int* index = APP_TextIndices + APP_TextDataLength * 6;
		int* const indicesEnd = APP_TextIndices + length * 6;
		int i = APP_TextDataLength * 4;
		while (index < indicesEnd) {
			index[0] = i + 0;
			index[1] = i + 1;
			index[2] = i + 2;

			index[3] = i + 1;
			index[4] = i + 2;
			index[5] = i + 3;

			index += 6;
			i += 4;
		}

		APP_TextDataLength = length;
	}

	SDL_Vertex* vertex = APP_TextVertices;
	const char* c = text;
	SDL_Vertex* const verticesEnd = APP_TextVertices + length * 4;
	float x = dstX + APP_PlaneDrawOffsetX;
	float y = dstY + APP_PlaneDrawOffsetY;
	if (!APP_ScreenRenderTarget) {
		x += APP_ScreenSubpixelOffset;
		y += APP_ScreenSubpixelOffset;
	}
	float texW, texH;
	if (!SDL_GetTextureSize(APP_Planes[plane], &texW, &texH)) {
		goto fail;
	}
	const float texCharW = (float)charW / texW;
	const float texCharH = (float)charH / texH;
	while (vertex < verticesEnd) {
		float texX = (srcX + ((*c - firstChar) * charW) % srcW) / texW;
		float texY = (srcY + (((*c - firstChar) * charW) / srcW) * charH) / texH;

		vertex[0].position = (SDL_FPoint) { x, y };
		vertex[0].tex_coord = (SDL_FPoint) { texX, texY };

		vertex[1].position = (SDL_FPoint) { x + charW, y };
		vertex[1].tex_coord = (SDL_FPoint) { texX + texCharW, texY };

		vertex[2].position = (SDL_FPoint) { x, y + charH };
		vertex[2].tex_coord = (SDL_FPoint) { texX, texY + texCharH };

		vertex[3].position = (SDL_FPoint) { x + charW, y + charH };
		vertex[3].tex_coord = (SDL_FPoint) { texX + texCharW, texY + texCharH };

		vertex += 4;
		c++;
		x += charW;
	}

	if (!SDL_RenderGeometry(APP_ScreenRenderer, APP_Planes[plane], APP_TextVertices, length * 4, APP_TextIndices, length * 6)) {
		APP_SetError("Failed rendering: %s", SDL_GetError());
		goto fail;
	}
	return;

	fail:
	APP_Exit(EXIT_FAILURE);
}

void APP_SetPlaneDrawOffset(int x, int y)
{
	APP_NewPlaneDrawOffsetX = x;
	APP_NewPlaneDrawOffsetY = y;
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
		SDL_FRect rect;
		if (!SDL_GetRenderLogicalPresentationRect(APP_ScreenRenderer, &rect)) {
			APP_SetError("Graphics error: %s", SDL_GetError());
			APP_Exit(EXIT_FAILURE);
		}
		const float scale = rect.w / APP_LogicalWidth;
		return 0.49f / scale;
	}
	else {
		return 0.0f;
	}
}

void APP_SetDrawWhileSkippingFrames(bool draw)
{
	APP_DrawWhileSkippingFrames = draw;
}
