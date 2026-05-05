#include <VIDEO.h>
#include <SYS_build_config.h>
#include <FILESYSTEM.h>
#include <MAIN.h>
#include "GLOBAL.h"
#include "BDF.h"
#include <ERROR.h>
#include <SDL3_image/SDL_image.h>

typedef struct VIDEO_TextLayer
{
	bool enable;
	int x;
	int y;
	SDL_Color color;
	int fontSize;
	size_t stringSize;
	char* string;
	bool updateTexture;
	SDL_Texture* texture;
	float textureW;
	float textureH;
} VIDEO_TextLayer;

typedef struct VIDEO_LoadPlaneData
{
	char* filename;
	SDL_Surface* surface;
	WORKER_JobID job;
} VIDEO_LoadPlaneData;

static int VIDEO_PlaneCount = 0;
static SDL_Texture** VIDEO_Planes = NULL;
static VIDEO_LoadPlaneData* VIDEO_PlanesLoadData = NULL;

#define VIDEO_BDF_FONT_FILE_COUNT	3
static BDF_Font* VIDEO_BDFFonts[VIDEO_BDF_FONT_FILE_COUNT] = { 0 };

static int VIDEO_LogicalWidth;
static int VIDEO_LogicalHeight;

static int VIDEO_TextLayerCount = 0;
static VIDEO_TextLayer* VIDEO_TextLayers = NULL;

static int32_t VIDEO_ScreenMode;
static int32_t VIDEO_ScreenIndex;

static size_t VIDEO_TextDataLength = 0;
static SDL_Vertex* VIDEO_TextVertices = NULL;
static int* VIDEO_TextIndices = NULL;

static int VIDEO_PlaneDrawOffsetX = 0;
static int VIDEO_PlaneDrawOffsetY = 0;

static SDL_IOStream* VIDEO_OpenImage(const char* filename, const char** type);

static void VIDEO_InitBDFFonts(void)
{
	const char* const filenames[VIDEO_BDF_FONT_FILE_COUNT] = {
		"res/font/font10.bdf",
		"res/font/font12.bdf",
		"res/font/font16.bdf"
	};

	for (int i = 0; i < VIDEO_BDF_FONT_FILE_COUNT; i++) {
		if (VIDEO_BDFFonts[i]) {
			continue;
		}
		if (!FILESYSTEM_FileExists(filenames[i])) {
			SDL_Log("Missing font file %s", filenames[i]);
			continue;
		}
		SDL_IOStream* const src = FILESYSTEM_OpenRead(filenames[i]);
		if (!src) {
			ERROR_Set("Failed to open font file \"%s\": %s", SDL_GetError());
			MAIN_Exit(SDL_APP_FAILURE);
		}
		VIDEO_BDFFonts[i] = BDF_OpenFont(src);
		if (!VIDEO_BDFFonts[i]) {
			SDL_CloseIO(src);
			ERROR_Set("Failed to load font file \"%s\": %s", filenames[i], SDL_GetError());
			MAIN_Exit(SDL_APP_FAILURE);
		}
		SDL_CloseIO(src);
	}
}

static void VIDEO_CloseBDFFonts(void)
{
	for (int i = 0; i < VIDEO_BDF_FONT_FILE_COUNT; i++) {
		if (VIDEO_BDFFonts[i]) {
			BDF_CloseFont(VIDEO_BDFFonts[i]);
		}
		VIDEO_BDFFonts[i] = NULL;
	}
}

void VIDEO_Init(int planeCount, int textLayerCount)
{
	if (planeCount > 0) {
		VIDEO_Planes = SDL_calloc(planeCount, sizeof(SDL_Texture*));
		if (!VIDEO_Planes) {
			ERROR_Set("Failed to allocate memory for planes");
			MAIN_Exit(SDL_APP_FAILURE);
		}
		VIDEO_PlanesLoadData = SDL_calloc(planeCount, sizeof(VIDEO_LoadPlaneData));
		if (!VIDEO_PlanesLoadData) {
			ERROR_Set("Failed to allocate memory for plane loading data");
			MAIN_Exit(SDL_APP_FAILURE);
		}
		VIDEO_PlaneCount = planeCount;
	}
	else {
		VIDEO_Planes = NULL;
		VIDEO_PlanesLoadData = NULL;
		VIDEO_PlaneCount = 0;
	}

	if (textLayerCount > 0) {
		VIDEO_TextLayers = SDL_calloc(textLayerCount, sizeof(VIDEO_TextLayer));
		if (!VIDEO_TextLayers) {
			ERROR_Set("Failed to allocate memory for text layers");
			MAIN_Exit(SDL_APP_FAILURE);
		}
		for (int i = 0; i < textLayerCount; i++) {
			VIDEO_TextLayers[i].color.r = VIDEO_TextLayers[i].color.g = VIDEO_TextLayers[i].color.b = 255;
			VIDEO_TextLayers[i].color.a = SDL_ALPHA_OPAQUE;
			VIDEO_TextLayers[i].fontSize = 16;
		}
		VIDEO_TextLayerCount = textLayerCount;
	}
	else {
		VIDEO_TextLayers = NULL;
		VIDEO_TextLayerCount = 0;
	}

	VIDEO_InitBDFFonts();
}

void VIDEO_Quit(void)
{
	if (GLOBAL_ScreenRenderer) {
		for (int i = 0; i < VIDEO_TextLayerCount; i++) {
			SDL_DestroyTexture(VIDEO_TextLayers[i].texture);
			SDL_free(VIDEO_TextLayers[i].string);
		}
		SDL_free(VIDEO_TextLayers);
		VIDEO_TextLayers = NULL;

		for (int i = 0; i < VIDEO_PlaneCount; i++) {
			if (VIDEO_PlanesLoadData[i].job) {
				WORKER_WaitForJobCompletion(GLOBAL_LoadingWorker, VIDEO_PlanesLoadData[i].job);
				SDL_DestroySurface(VIDEO_PlanesLoadData[i].surface);
			}
			SDL_DestroyTexture(VIDEO_Planes[i]);
		}
		SDL_free(VIDEO_PlanesLoadData);
		VIDEO_PlanesLoadData = NULL;
		SDL_free(VIDEO_Planes);
		VIDEO_Planes = NULL;

		if (GLOBAL_ScreenRenderTarget) {
			SDL_SetRenderTarget(GLOBAL_ScreenRenderer, NULL);
			SDL_DestroyTexture(GLOBAL_ScreenRenderTarget);
			GLOBAL_ScreenRenderTarget = NULL;
		}
		SDL_DestroyRenderer(GLOBAL_ScreenRenderer);
		GLOBAL_ScreenRenderer = NULL;
	}

	if (GLOBAL_ScreenWindow) {
		SDL_DestroyWindow(GLOBAL_ScreenWindow);
		GLOBAL_ScreenWindow = NULL;
	}

	SDL_free(VIDEO_TextVertices);
	VIDEO_TextVertices = NULL;
	SDL_free(VIDEO_TextIndices);
	VIDEO_TextIndices = NULL;
	VIDEO_TextDataLength = 0;

	VIDEO_CloseBDFFonts();
}

void VIDEO_SetScreen(VIDEO_ScreenModeFlag* screenMode, int32_t* screenIndex)
{
	int windowX, windowY;
	int logicalWidth, logicalHeight;
	SDL_DisplayID* displays = NULL;
	SDL_DisplayMode** displayModes = NULL;

	VIDEO_ScreenModeFlag windowType = *screenMode & VIDEO_SCREEN_MODE_WINDOW_TYPE;
	if (windowType >= VIDEO_SCREEN_MODE_WINDOW_TYPES_COUNT) {
		ERROR_Set("Invalid window type value of %d", (int)windowType);
		goto fail;
	}

	int displayIndex = VIDEO_SCREEN_INDEX_DISPLAY_TO_VALUE(*screenIndex);
	int displayModeIndex = VIDEO_SCREEN_INDEX_MODE_TO_VALUE(*screenIndex);
	int displayCount;
	displays = SDL_GetDisplays(&displayCount);
	if (!displays) {
		ERROR_Set("Could not get list of displays: %s", SDL_GetError());
		goto fail;
	}
	const SDL_DisplayID display = displays[displayIndex];
	SDL_free(displays);
	displays = NULL;
	// TODO: There's a bug in the Vita port of SDL 3.2.24 where displayModeCount is set to 0. An issue/PR should be submitted to libsdl-org/SDL. We avoid needing to deal with the bug by using VIDEO_SCREEN_MODE_FULLSCREEN_DESKTOP on Vita.
	int displayModeCount;
	displayModes = SDL_GetFullscreenDisplayModes(display, &displayModeCount);
	if (!displayModes) {
		ERROR_Set("Could not get list of fullscreen display modes: %s", SDL_GetError());
		goto fail;
	}

	windowX = SDL_WINDOWPOS_CENTERED_DISPLAY(display);
	windowY = SDL_WINDOWPOS_CENTERED_DISPLAY(display);
	if (
		displayIndex >= displayCount ||
		(windowType == VIDEO_SCREEN_MODE_FULLSCREEN && displayModeCount > 0 && displayModeIndex >= displayModeCount)
	) {
		*screenMode = VIDEO_SCREEN_MODE_DEFAULT;
		*screenIndex = 0;
		displayModeIndex = 0;
		displayIndex = 0;
	}
	const bool maximized = windowType == VIDEO_SCREEN_MODE_WINDOW_MAXIMIZED;

	if (*screenMode & VIDEO_SCREEN_MODE_DETAIL_LEVEL) {
		logicalWidth  = 640;
		logicalHeight = 480;
	}
	else {
		logicalWidth  = 320;
		logicalHeight = 240;
	}

	VIDEO_LogicalWidth  = logicalWidth;
	VIDEO_LogicalHeight = logicalHeight;

#ifdef VIDEO_ENABLE_WINDOW_ICON_FILE
	bool createdWindow = false;
#endif
	if (
		windowType == VIDEO_SCREEN_MODE_FULLSCREEN ||
		windowType == VIDEO_SCREEN_MODE_FULLSCREEN_DESKTOP
	) {
		if (!GLOBAL_ScreenWindow) {
			SDL_PropertiesID props = SDL_CreateProperties();
			if (!props) {
				ERROR_Set("Could not create properties object: %s", SDL_GetError());
				goto fail;
			}
			if (
				!SDL_SetStringProperty(props, SDL_PROP_WINDOW_CREATE_TITLE_STRING, PROJECT_TITLE) ||
				!SDL_SetBooleanProperty(props, SDL_PROP_WINDOW_CREATE_FULLSCREEN_BOOLEAN, true) ||
				!SDL_SetNumberProperty(props, SDL_PROP_WINDOW_CREATE_X_NUMBER, windowX) ||
				!SDL_SetNumberProperty(props, SDL_PROP_WINDOW_CREATE_Y_NUMBER, windowY) ||
				!SDL_SetNumberProperty(props, SDL_PROP_WINDOW_CREATE_WIDTH_NUMBER, logicalWidth) ||
				!SDL_SetNumberProperty(props, SDL_PROP_WINDOW_CREATE_HEIGHT_NUMBER, logicalHeight) ||
				!SDL_SetBooleanProperty(props, SDL_PROP_WINDOW_CREATE_RESIZABLE_BOOLEAN, true)
			) {
				ERROR_Set("Could not set up window properties object: %s", SDL_GetError());
				goto fail;
			}
			GLOBAL_ScreenWindow = SDL_CreateWindowWithProperties(props);
			SDL_DestroyProperties(props);
			if (!GLOBAL_ScreenWindow) {
				ERROR_Set("Could not create window: %s", SDL_GetError());
				goto fail;
			}
			if (windowType == VIDEO_SCREEN_MODE_FULLSCREEN && !SDL_SetWindowFullscreenMode(GLOBAL_ScreenWindow, displayModes[displayModeIndex])) {
				ERROR_Set("Could not set window fullscreen mode: %s", SDL_GetError());
				goto fail;
			}
#ifdef VIDEO_ENABLE_WINDOW_ICON_FILE
			createdWindow = true;
#endif
		}
		else {
			if (!SDL_SetWindowFullscreen(GLOBAL_ScreenWindow, true)) {
				ERROR_Set("Could not set window fullscreen: %s", SDL_GetError());
				goto fail;
			}
			if (!SDL_SetWindowSize(GLOBAL_ScreenWindow, logicalWidth, logicalHeight)) {
				ERROR_Set("Could not set window size: %s", SDL_GetError());
				goto fail;
			}
			if (!SDL_SetWindowFullscreenMode(GLOBAL_ScreenWindow, (windowType == VIDEO_SCREEN_MODE_FULLSCREEN) ? displayModes[displayModeIndex] : NULL)) {
				ERROR_Set("Could not set window fullscreen mode: %s", SDL_GetError());
				goto fail;
			}
			// NOTE: This can fail on some platforms due to not being supported (Wayland), so don't check the return status
			SDL_SetWindowPosition(GLOBAL_ScreenWindow, windowX, windowY);
		}
	}
	else if (
		windowType == VIDEO_SCREEN_MODE_WINDOW ||
		windowType == VIDEO_SCREEN_MODE_WINDOW_MAXIMIZED
	) {
		const SDL_DisplayMode* const displayMode = SDL_GetDesktopDisplayMode(display);
		if (!displayMode) {
			ERROR_Set("Could not get desktop display mode: %s", SDL_GetError());
			goto fail;
		}
		int maxScale;
		if (displayMode->w <= logicalWidth * 2 || displayMode->h <= logicalHeight * 2) {
			maxScale = 1;
		}
		else if (displayMode->w > displayMode->h * VIDEO_SCREEN_WIDE) {
			maxScale = (displayMode->h / logicalHeight) - (displayMode->h % logicalHeight == 0);
		}
		else {
			maxScale = (displayMode->w / logicalWidth) - (displayMode->w % logicalWidth == 0);
		}
		int scale = displayModeIndex + 1;
		int windowW = scale * logicalWidth;
		int windowH = scale * logicalHeight;
		if (scale > maxScale) {
			windowW = logicalWidth;
			windowH = logicalHeight;
			*screenIndex = VIDEO_SCREEN_INDEX_TO_SETTING(displayIndex, 0);
		}
		if (!GLOBAL_ScreenWindow) {
			SDL_PropertiesID props = SDL_CreateProperties();
			if (!props) {
				ERROR_Set("Could not create properties object: %s", SDL_GetError());
				goto fail;
			}
			if (
				!SDL_SetStringProperty(props, SDL_PROP_WINDOW_CREATE_TITLE_STRING, PROJECT_TITLE) ||
				!SDL_SetNumberProperty(props, SDL_PROP_WINDOW_CREATE_X_NUMBER, windowX) ||
				!SDL_SetNumberProperty(props, SDL_PROP_WINDOW_CREATE_Y_NUMBER, windowY) ||
				!SDL_SetNumberProperty(props, SDL_PROP_WINDOW_CREATE_WIDTH_NUMBER, windowW) ||
				!SDL_SetNumberProperty(props, SDL_PROP_WINDOW_CREATE_HEIGHT_NUMBER, windowH) ||
				!SDL_SetBooleanProperty(props, SDL_PROP_WINDOW_CREATE_RESIZABLE_BOOLEAN, true) ||
				!SDL_SetBooleanProperty(props, SDL_PROP_WINDOW_CREATE_MAXIMIZED_BOOLEAN, maximized)
			) {
				ERROR_Set("Could not set up window properties object: %s", SDL_GetError());
				goto fail;
			}
			GLOBAL_ScreenWindow = SDL_CreateWindowWithProperties(props);
			SDL_DestroyProperties(props);
			if (!GLOBAL_ScreenWindow) {
				ERROR_Set("Could not create window: %s", SDL_GetError());
				goto fail;
			}
#ifdef VIDEO_ENABLE_WINDOW_ICON_FILE
			createdWindow = true;
#endif
		}
		else {
			if (!SDL_SetWindowFullscreen(GLOBAL_ScreenWindow, false)) {
				ERROR_Set("Could not set window windowed: %s", SDL_GetError());
				goto fail;
			}
			if (!SDL_SetWindowPosition(GLOBAL_ScreenWindow, windowX, windowY)) {
				// NOTE: Position setting fails on some platforms due to not being supported (Wayland), so don't fail if it can't be set.
				SDL_Log("Could not set window position, but continuing anyways. SDL error message: %s", SDL_GetError());
			}
			if (maximized) {
				if (!SDL_MaximizeWindow(GLOBAL_ScreenWindow)) {
					ERROR_Set("Could not maximize window: %s", SDL_GetError());
					goto fail;
				}
			}
			else if (!SDL_RestoreWindow(GLOBAL_ScreenWindow)) {
				ERROR_Set("Could not restore window: %s", SDL_GetError());
				goto fail;
			}
			else if (!SDL_SetWindowSize(GLOBAL_ScreenWindow, windowW, windowH)) {
				ERROR_Set("Could not set window size: %s", SDL_GetError());
				goto fail;
			}
		}
	}
	SDL_free(displayModes);
	displayModes = NULL;
#ifdef VIDEO_ENABLE_WINDOW_ICON_FILE
	if (createdWindow) {
		const char* type;
		SDL_IOStream* const file = VIDEO_OpenImage(PROJECT_NAME, &type);
		if (file) {
			SDL_Surface* const surface = IMG_LoadTyped_IO(file, true, type);
			if (!surface) {
				ERROR_Set("Could not load icon image: %s", SDL_GetError());
				goto fail;
			}
			if (!SDL_SetWindowIcon(GLOBAL_ScreenWindow, surface)) {
				SDL_DestroySurface(surface);
				ERROR_Set("Could not set window icon: %s", SDL_GetError());
				goto fail;
			}
			SDL_DestroySurface(surface);
		}
	}
#endif

	// Create the renderer, if not already created. It's important to not
	// recreate the renderer if it's already created, so restarting without
	// changing the detail level doesn't require reloading graphics. If the
	// renderer were destroyed/created anew every restart, it would be required
	// to reload the graphics every restart, even when detail level isn't
	// changed.
	if (!GLOBAL_ScreenRenderer) {
		GLOBAL_ScreenRenderer = SDL_CreateRenderer(GLOBAL_ScreenWindow, NULL);
		if (!GLOBAL_ScreenRenderer) {
			ERROR_Set("Could not create renderer: %s", SDL_GetError());
			goto fail;
		}
	}
	if (
		!SDL_RenderClear(GLOBAL_ScreenRenderer) ||
		!SDL_RenderPresent(GLOBAL_ScreenRenderer)
	) {
		ERROR_Set("Could not do initial render clear of screen: %s", SDL_GetError());
		goto fail;
	}

	// Unset the render target, if currently set, for making renderer setting
	// changes. The render target should only be set once all settings have been
	// set, with no setting changes made after the render target has been set;
	// various bugs have been observed when attempting setting changes while the
	// render target is non-NULL, such bugs disappearing when setting changes
	// are made only while the render target is NULL.
	if (GLOBAL_ScreenRenderTarget && !SDL_SetRenderTarget(GLOBAL_ScreenRenderer, NULL)) {
		ERROR_Set("Could not unset render target: %s", SDL_GetError());
		goto fail;
	}

	if (!SDL_RenderClear(GLOBAL_ScreenRenderer)) {
		ERROR_Set("Could not do initial render clear of screen: %s", SDL_GetError());
		goto fail;
	}

	if (!SDL_SetRenderVSync(GLOBAL_ScreenRenderer, (*screenMode & VIDEO_SCREEN_MODE_VSYNC) ? 1 : SDL_RENDERER_VSYNC_DISABLED) ) {
		ERROR_Set("Could not set render vsync mode: %s", SDL_GetError());
		goto fail;
	}

	if (*screenMode & VIDEO_SCREEN_MODE_SCALE_MODE) {
		if (!SDL_SetRenderLogicalPresentation(GLOBAL_ScreenRenderer, logicalWidth, logicalHeight, SDL_LOGICAL_PRESENTATION_INTEGER_SCALE)) {
			ERROR_Set("Could not set render dimensions: %s", SDL_GetError());
			goto fail;
		}
	}
	else {
		if (!SDL_SetRenderLogicalPresentation(GLOBAL_ScreenRenderer, logicalWidth, logicalHeight, SDL_LOGICAL_PRESENTATION_LETTERBOX)) {
			ERROR_Set("Could not set render dimensions: %s", SDL_GetError());
			goto fail;
		}
	}

	if (!(*screenMode & VIDEO_SCREEN_MODE_RENDER_LEVEL)) {
		// There's no need to create a render target texture if the
		// currently created render target texture is already the current
		// logicalWidth x logicalHeight.
		bool createNewRenderTarget = true;

		if (GLOBAL_ScreenRenderTarget) {
			float w, h;
			if (!SDL_GetTextureSize(GLOBAL_ScreenRenderTarget, &w, &h)) {
				ERROR_Set("Could not get render target texture size: %s", SDL_GetError());
				goto fail;
			}

			if ((int)w == logicalWidth && (int)h == logicalHeight) {
				createNewRenderTarget = false;
			}
			else {
				SDL_DestroyTexture(GLOBAL_ScreenRenderTarget);
				GLOBAL_ScreenRenderTarget = NULL;
			}
		}

		if (createNewRenderTarget) {
			GLOBAL_ScreenRenderTarget = SDL_CreateTexture(GLOBAL_ScreenRenderer, SDL_PIXELFORMAT_RGB24, SDL_TEXTUREACCESS_TARGET, logicalWidth, logicalHeight);
			if (!GLOBAL_ScreenRenderTarget) {
				ERROR_Set("Could not create render target texture: %s", SDL_GetError());
				goto fail;
			}
			if (!SDL_SetTextureScaleMode(GLOBAL_ScreenRenderTarget, SDL_SCALEMODE_NEAREST)) {
				ERROR_Set("Could not set render target to nearest neighbor texture filtering: %s", SDL_GetError());
				goto fail;
			}
		}

		if (
			!SDL_SetRenderTarget(GLOBAL_ScreenRenderer, GLOBAL_ScreenRenderTarget) ||
			!SDL_RenderClear(GLOBAL_ScreenRenderer)
		) {
			ERROR_Set("Could not set up render target: %s", SDL_GetError());
			goto fail;
		}
	}
	else if (GLOBAL_ScreenRenderTarget) {
		SDL_DestroyTexture(GLOBAL_ScreenRenderTarget);
		GLOBAL_ScreenRenderTarget = NULL;
	}

	GLOBAL_ScreenSubpixelOffset = VIDEO_GetScreenSubpixelOffset();

	// WARNING: Make no changes to the renderer settings from here on down, as
	// the render target has been set, and bugs have been observed when
	// attempting renderer setting changes while a non-NULL render target is
	// set.

	VIDEO_ScreenMode = *screenMode;
	VIDEO_ScreenIndex = *screenIndex;

	if (!SDL_HideCursor()) {
		ERROR_Set("Could not hide mouse cursor: %s", SDL_GetError());
		goto fail;
	}

	while (!SDL_SyncWindow(GLOBAL_ScreenWindow)) {
		SDL_Delay(1);
	}

	return;

	fail:
	if (GLOBAL_ScreenRenderTarget) {
		SDL_SetRenderTarget(GLOBAL_ScreenRenderer, NULL);
		SDL_DestroyTexture(GLOBAL_ScreenRenderTarget);
	}
	if (GLOBAL_ScreenRenderer) {
		SDL_DestroyRenderer(GLOBAL_ScreenRenderer);
	}
	if (GLOBAL_ScreenWindow) {
		SDL_DestroyWindow(GLOBAL_ScreenWindow);
	}
	SDL_free(displayModes);
	SDL_free(displays);
	GLOBAL_ScreenRenderTarget = NULL;
	GLOBAL_ScreenRenderer = NULL;
	GLOBAL_ScreenWindow = NULL;
	MAIN_Exit(SDL_APP_FAILURE);
}

int VIDEO_GetMaxDisplayIndex(void)
{
	int displayCount;
	SDL_DisplayID* displays = SDL_GetDisplays(&displayCount);
	if (!displays) {
		ERROR_Set("Failed getting list of displays: %s", SDL_GetError());
		return -1;
	}
	SDL_free(displays);
	return displayCount;
}

int VIDEO_GetMaxDisplayMode(int displayIndex)
{
	if (displayIndex < 0) {
		ERROR_Set("displayIndex invalid, must be at least 0");
		return -1;
	}
	int displayCount;
	SDL_DisplayID* displays = SDL_GetDisplays(&displayCount);
	if (!displays) {
		ERROR_Set("Failed getting list of displays: %s", SDL_GetError());
		return -1;
	}
	if (displayIndex >= displayCount) {
		SDL_free(displays);
		ERROR_Set("Display index is too high");
		return -1;
	}
	int modeCount;
	SDL_DisplayMode** modes = SDL_GetFullscreenDisplayModes(displays[displayIndex], &modeCount);
	if (!modes) {
		SDL_free(displays);
		ERROR_Set("Could not get list of fullscreen display modes");
		return -1;
	}
	SDL_free(displays);
	SDL_free(modes);
	return modeCount;
}

void VIDEO_GetDisplayMode(int displayIndex, int modeIndex, SDL_DisplayMode* mode)
{
	if (displayIndex < 0) {
		ERROR_Set("%s", SDL_InvalidParamError("displayIndex"));
		MAIN_Exit(SDL_APP_FAILURE);
	}
	else if (modeIndex < 0) {
		ERROR_Set("%s", SDL_InvalidParamError("modeIndex"));
		MAIN_Exit(SDL_APP_FAILURE);
	}
	else if (!mode) {
		ERROR_Set("%s", SDL_InvalidParamError("mode"));
		MAIN_Exit(SDL_APP_FAILURE);
	}
	int displayCount;
	SDL_DisplayID* displays = SDL_GetDisplays(&displayCount);
	if (!displays) {
		ERROR_Set("Could not get list of displays: %s", SDL_GetError());
		MAIN_Exit(SDL_APP_FAILURE);
	}
	if (displayIndex >= displayCount) {
		SDL_free(displays);
		ERROR_Set("Number of displays is %d but requested display index is too high at %d", displayCount, displayIndex);
		MAIN_Exit(SDL_APP_FAILURE);
	}
	int modeCount;
	SDL_DisplayMode** modes = SDL_GetFullscreenDisplayModes(displays[displayIndex], &modeCount);
	SDL_free(displays);
	if (modeIndex >= modeCount) {
		SDL_free(modes);
		ERROR_Set("Number of display modes is %d but requested display mode index is too high at %d", modeCount, modeIndex);
		MAIN_Exit(SDL_APP_FAILURE);
	}
	*mode = *modes[modeIndex];
}

const SDL_DisplayMode* VIDEO_GetDesktopDisplayMode(int displayIndex)
{
	if (displayIndex < 0) {
		ERROR_Set("Invalid display index of %d, must be at least 0", displayIndex);
		MAIN_Exit(SDL_APP_FAILURE);
	}
	int displayCount;
	SDL_DisplayID* displays = SDL_GetDisplays(&displayCount);
	if (!displays) {
		ERROR_Set("Error getting list of displays: %s", SDL_GetError());
		MAIN_Exit(SDL_APP_FAILURE);
	}
	if (displayIndex >= displayCount) {
		SDL_free(displays);
		ERROR_Set("Display index is too high");
		MAIN_Exit(SDL_APP_FAILURE);
	}
	const SDL_DisplayMode* mode = SDL_GetDesktopDisplayMode(displays[displayIndex]);
	SDL_free(displays);
	if (!mode) {
		ERROR_Set("Error getting desktop display mode: %s", SDL_GetError());
		MAIN_Exit(SDL_APP_FAILURE);
	}
	return mode;
}

void VIDEO_EnableTextLayer(int layer, int x, int y)
{
	if (layer < 0 || layer >= VIDEO_TextLayerCount) {
		if (VIDEO_TextLayerCount == 0) {
			ERROR_Set("Zero text layers are available");
		}
		else {
			ERROR_Set("Invalid layer number requested to enable, must be in range 0 to %d", VIDEO_TextLayerCount - 1);
		}
		MAIN_Exit(SDL_APP_FAILURE);
	}
	VIDEO_TextLayers[layer].enable = true;
	VIDEO_TextLayers[layer].x = x;
	VIDEO_TextLayers[layer].y = y;
}

void VIDEO_SetTextLayerDrawPosition(int layer, int x, int y)
{
	if (layer < 0 || layer >= VIDEO_TextLayerCount) {
		if (VIDEO_TextLayerCount == 0) {
			ERROR_Set("Zero text layers are available");
		}
		else {
			ERROR_Set("Invalid layer number requested to set draw position, must be in range 0 to %d", VIDEO_TextLayerCount - 1);
		}
		MAIN_Exit(SDL_APP_FAILURE);
	}
	VIDEO_TextLayers[layer].x = x;
	VIDEO_TextLayers[layer].y = y;
}

void VIDEO_SetTextLayerColor(int layer, int r, int g, int b)
{
	if (layer < 0 || layer >= VIDEO_TextLayerCount) {
		if (VIDEO_TextLayerCount == 0) {
			ERROR_Set("Zero text layers are available");
		}
		else {
			ERROR_Set("Invalid layer number requested to set text color, must be in range 0 to %d", VIDEO_TextLayerCount - 1);
		}
		MAIN_Exit(SDL_APP_FAILURE);
	}
	if (
		VIDEO_TextLayers[layer].color.r != r ||
		VIDEO_TextLayers[layer].color.g != g ||
		VIDEO_TextLayers[layer].color.b != b
	) {
		VIDEO_TextLayers[layer].color.r = r;
		VIDEO_TextLayers[layer].color.g = g;
		VIDEO_TextLayers[layer].color.b = b;
		VIDEO_TextLayers[layer].updateTexture = true;
	}
}

void VIDEO_SetTextLayerSize(int layer, int size)
{
	if (layer < 0 || layer >= VIDEO_TextLayerCount) {
		if (VIDEO_TextLayerCount == 0) {
			ERROR_Set("Zero text layers are available");
		}
		else {
			ERROR_Set("Invalid layer number requested to set text size, must be in range 0 to %d", VIDEO_TextLayerCount - 1);
		}
		MAIN_Exit(SDL_APP_FAILURE);
	}
	if (VIDEO_TextLayers[layer].fontSize != size) {
		VIDEO_TextLayers[layer].fontSize = size;
		VIDEO_TextLayers[layer].updateTexture = true;
	}
}

void VIDEO_PutTextLayerString(int layer, const char* string)
{
	if (layer < 0 || layer >= VIDEO_TextLayerCount) {
		if (VIDEO_TextLayerCount == 0) {
			ERROR_Set("Zero text layers are available");
		}
		else {
			ERROR_Set("Invalid layer number requested to put text into, must be in range 0 to %d", VIDEO_TextLayerCount - 1);
		}
		MAIN_Exit(SDL_APP_FAILURE);
	}
	if (!VIDEO_TextLayers[layer].string || SDL_strcmp(string, VIDEO_TextLayers[layer].string) != 0) {
		const size_t newStringSize = SDL_strlen(string) + 1;
		if (newStringSize > VIDEO_TextLayers[layer].stringSize) {
			char* const newString = SDL_realloc(VIDEO_TextLayers[layer].string, newStringSize);
			if (!newString) {
				ERROR_Set("Failed allocating memory for putting a string into a text layer");
				MAIN_Exit(SDL_APP_FAILURE);
			}
			VIDEO_TextLayers[layer].stringSize = newStringSize;
			VIDEO_TextLayers[layer].string = newString;
		}
		SDL_memcpy(VIDEO_TextLayers[layer].string, string, newStringSize);
		VIDEO_TextLayers[layer].updateTexture = true;
	}
}

void VIDEO_DrawTextLayer(int layer)
{
	if (layer < 0 || layer >= VIDEO_TextLayerCount) {
		if (VIDEO_TextLayerCount == 0) {
			ERROR_Set("Zero text layers are available");
		}
		else {
			ERROR_Set("Invalid layer number requested to draw, must be in range 0 to %d", VIDEO_TextLayerCount - 1);
		}
		MAIN_Exit(SDL_APP_FAILURE);
	}
	if (!GLOBAL_ScreenRenderer || !VIDEO_TextLayers[layer].enable || !MAIN_RenderThisFrame()) {
		return;
	}

	int font = 0;

	if (VIDEO_TextLayers[layer].fontSize >= 12) {
		font++;
	}
	if (VIDEO_TextLayers[layer].fontSize >= 16) {
		font++;
	}

	if (!VIDEO_BDFFonts[font]) {
		return;
	}

	if (VIDEO_TextLayers[layer].updateTexture) {
		if (VIDEO_TextLayers[layer].texture) {
			SDL_DestroyTexture(VIDEO_TextLayers[layer].texture);
		}
		VIDEO_TextLayers[layer].texture = BDF_CreateTextTexture(VIDEO_BDFFonts[font], GLOBAL_ScreenRenderer, VIDEO_TextLayers[layer].string, VIDEO_TextLayers[layer].color, 32, SDL_SCALEMODE_NEAREST);
		if (!VIDEO_TextLayers[layer].texture) {
			ERROR_Set("Error creating texture to render text: %s", SDL_GetError());
			MAIN_Exit(SDL_APP_FAILURE);
		}
		if (!SDL_GetTextureSize(VIDEO_TextLayers[layer].texture, &VIDEO_TextLayers[layer].textureW, &VIDEO_TextLayers[layer].textureH)) {
			ERROR_Set("Error getting size of a text layer texture: %s", SDL_GetError());
			MAIN_Exit(SDL_APP_FAILURE);
		}
		VIDEO_TextLayers[layer].updateTexture = false;
	}

	if (VIDEO_TextLayers[layer].texture) {
		SDL_FRect dstRect = {
			VIDEO_TextLayers[layer].x + VIDEO_PlaneDrawOffsetX,
			VIDEO_TextLayers[layer].y + VIDEO_PlaneDrawOffsetY,
			VIDEO_TextLayers[layer].textureW,
			VIDEO_TextLayers[layer].textureH
		};
		if (!GLOBAL_ScreenRenderTarget) {
			dstRect.x += GLOBAL_ScreenSubpixelOffset;
			dstRect.y += GLOBAL_ScreenSubpixelOffset;
		}
		if (!SDL_RenderTexture(GLOBAL_ScreenRenderer, VIDEO_TextLayers[layer].texture, NULL, &dstRect)) {
			ERROR_Set("Error rendering text layer: %s", SDL_GetError());
			MAIN_Exit(SDL_APP_FAILURE);
		}
	}
}

void VIDEO_DisableTextLayer(int layer) {
	if (layer < 0 || layer >= VIDEO_TextLayerCount) {
		if (VIDEO_TextLayerCount == 0) {
			ERROR_Set("Zero text layers are available");
		}
		else {
			ERROR_Set("Invalid layer number requested to disable, must be in range 0 to %d", VIDEO_TextLayerCount - 1);
		}
		MAIN_Exit(SDL_APP_FAILURE);
	}
	VIDEO_TextLayers[layer].enable = false;
}

static const char* const VIDEO_ImageTypes[] = {
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

static SDL_IOStream* VIDEO_OpenImage(const char* filename, const char** type)
{
	SDL_IOStream* file = NULL;
	char* filenameExt;
	for (size_t i = 0; i < SDL_arraysize(VIDEO_ImageTypes); i++) {
		*type = VIDEO_ImageTypes[i];
		if (SDL_asprintf(&filenameExt, "%s.%s", filename, *type) < 0) {
			ERROR_Set("Error opening image file");
			MAIN_Exit(SDL_APP_FAILURE);
		}
		if (!FILESYSTEM_FileExists(filenameExt)) {
			SDL_free(filenameExt);
			continue;
		}
		file = FILESYSTEM_OpenRead(filenameExt);
		if (!file) {
			SDL_free(filenameExt);
			ERROR_Set("Error opening image file: %s", SDL_GetError());
			MAIN_Exit(SDL_APP_FAILURE);
		}
		SDL_free(filenameExt);
		break;
	}
	if (!file) {
		SDL_Log("Missing image file %s.*", filename);
	}
	return file;
}

static bool VIDEO_LoadPlaneCallback(void* userdata)
{
	VIDEO_LoadPlaneData* const data = userdata;
	const char* type;
	SDL_IOStream* const file = VIDEO_OpenImage(data->filename, &type);
	SDL_free(data->filename);
	data->filename = NULL;
	if (!file) {
		data->surface = NULL;
		return true;
	}
	data->surface = IMG_LoadTyped_IO(file, true, type);
	return data->surface != NULL;
}

void VIDEO_LoadPlane(int plane, const char* filename)
{
	if (!GLOBAL_ScreenRenderer || !filename || !*filename || plane < 0 || plane >= VIDEO_PlaneCount) {
		return;
	}

	if (VIDEO_Planes[plane]) {
		SDL_DestroyTexture(VIDEO_Planes[plane]);
	}
	VIDEO_Planes[plane] = NULL;
	VIDEO_PlanesLoadData[plane].filename = SDL_strdup(filename);
	if (!VIDEO_PlanesLoadData[plane].filename) {
		ERROR_Set("Failed loading a plane");
		MAIN_Exit(SDL_APP_FAILURE);
	}
	VIDEO_PlanesLoadData[plane].job = WORKER_SubmitJob(GLOBAL_LoadingWorker, VIDEO_LoadPlaneCallback, &VIDEO_PlanesLoadData[plane]);
	if (!VIDEO_PlanesLoadData[plane].job) {
		SDL_free(VIDEO_PlanesLoadData[plane].filename);
		VIDEO_PlanesLoadData[plane].filename = NULL;
		ERROR_Set("Failed loading a plane: %s", SDL_GetError());
		MAIN_Exit(SDL_APP_FAILURE);
	}
}

static bool VIDEO_HaveSinglePlane(int plane)
{
	if (!VIDEO_PlanesLoadData[plane].job) {
		return VIDEO_Planes[plane] != NULL;
	}
	else if (!WORKER_WaitForJobCompletion(GLOBAL_LoadingWorker, VIDEO_PlanesLoadData[plane].job)) {
		ERROR_Set("Failed to finish loading a plane: %s", SDL_GetError());
		MAIN_Exit(SDL_APP_FAILURE);
	}
	if (!VIDEO_PlanesLoadData[plane].surface) {
		return false;
	}
	VIDEO_Planes[plane] = SDL_CreateTextureFromSurface(GLOBAL_ScreenRenderer, VIDEO_PlanesLoadData[plane].surface);
	SDL_DestroySurface(VIDEO_PlanesLoadData[plane].surface);
	VIDEO_PlanesLoadData[plane].surface = NULL;
	VIDEO_PlanesLoadData[plane].job = 0;
	if (!VIDEO_Planes[plane]) {
		ERROR_Set("Failed to create texture for a plane: %s", SDL_GetError());
		MAIN_Exit(SDL_APP_FAILURE);
	}
	else if (
		!SDL_SetTextureScaleMode(VIDEO_Planes[plane], SDL_SCALEMODE_NEAREST) ||
		!SDL_SetTextureBlendMode(VIDEO_Planes[plane], SDL_BLENDMODE_BLEND)
	) {
		ERROR_Set("Error loading image: %s", SDL_GetError());
		MAIN_Exit(SDL_APP_FAILURE);
	}
	return true;
}

bool VIDEO_HavePlane(int plane)
{
	if (plane < 0) {
		bool havePlane = false;
		for (int i = 0; i < VIDEO_PlaneCount; i++) {
			if (VIDEO_HaveSinglePlane(i)) {
				havePlane = true;
			}
		}
		return havePlane;
	}
	else if (plane < VIDEO_PlaneCount) {
		return VIDEO_HaveSinglePlane(plane);
	}
	else {
		return false;
	}
}

void VIDEO_DrawPlane(int plane, int dstX, int dstY)
{
	if (plane < 0 || plane >= VIDEO_PlaneCount || !VIDEO_HavePlane(plane) || !MAIN_RenderThisFrame()) {
		return;
	}
	float w, h;
	if (!SDL_GetTextureSize(VIDEO_Planes[plane], &w, &h)) {
		ERROR_Set("Error rendering graphics: %s", SDL_GetError());
		MAIN_Exit(SDL_APP_FAILURE);
	}
	VIDEO_DrawPlaneRect(plane, dstX, dstY, 0, 0, (int)w, (int)h);
}

void VIDEO_DrawPlaneRect(int plane, int dstX, int dstY, int srcX, int srcY, int w, int h)
{
	if (plane < 0 || plane >= VIDEO_PlaneCount || !VIDEO_HavePlane(plane) || w == 0 || h == 0 || !MAIN_RenderThisFrame()) {
		return;
	}

	const SDL_FRect src = { srcX, srcY, w, h };
	SDL_FRect dst = {
		dstX + VIDEO_PlaneDrawOffsetX,
		dstY + VIDEO_PlaneDrawOffsetY,
		w,
		h
	};
	if (!GLOBAL_ScreenRenderTarget) {
		dst.x += GLOBAL_ScreenSubpixelOffset;
		dst.y += GLOBAL_ScreenSubpixelOffset;
	}

	if (!SDL_RenderTexture(GLOBAL_ScreenRenderer, VIDEO_Planes[plane], &src, &dst)) {
		MAIN_Exit(SDL_APP_FAILURE);
	}
}

void VIDEO_DrawPlaneTransparent(int plane, int dstX, int dstY, uint8_t a)
{
	if (plane < 0 || plane >= VIDEO_PlaneCount || !VIDEO_HavePlane(plane) || !MAIN_RenderThisFrame()) {
		return;
	}

	if (!SDL_SetTextureAlphaMod(VIDEO_Planes[plane], a)) {
		ERROR_Set("Error rendering graphics: %s", SDL_GetError());
		MAIN_Exit(SDL_APP_FAILURE);
	}
	VIDEO_DrawPlane(plane, dstX, dstY);
	if (!SDL_SetTextureAlphaMod(VIDEO_Planes[plane], SDL_ALPHA_OPAQUE)) {
		ERROR_Set("Error rendering graphics: %s", SDL_GetError());
		MAIN_Exit(SDL_APP_FAILURE);
	}
}

void VIDEO_DrawPlaneRectTransparent(int plane, int dstX, int dstY, int srcX, int srcY, int w, int h, Uint8 a)
{
	if (plane < 0 || plane >= VIDEO_PlaneCount || !VIDEO_HavePlane(plane) || w == 0 || h == 0 || !MAIN_RenderThisFrame()) {
		return;
	}

	if (!SDL_SetTextureAlphaMod(VIDEO_Planes[plane], a)) {
		ERROR_Set("Error rendering graphics: %s", SDL_GetError());
		MAIN_Exit(SDL_APP_FAILURE);
	}
	VIDEO_DrawPlaneRect(plane, dstX, dstY, srcX, srcY, w, h);
	if (!SDL_SetTextureAlphaMod(VIDEO_Planes[plane], SDL_ALPHA_OPAQUE)) {
		ERROR_Set("Error rendering graphics: %s", SDL_GetError());
		MAIN_Exit(SDL_APP_FAILURE);
	}
}

void VIDEO_DrawPlaneScaled(int plane, int dstX, int dstY, int scaleW, int scaleH)
{
	VIDEO_DrawPlaneRectScaled(plane, dstX, dstY, 0, 0, VIDEO_LogicalWidth, VIDEO_LogicalHeight, scaleW, scaleH);
}

void VIDEO_DrawPlaneRectScaled(int plane, int dstX, int dstY, int srcX, int srcY, int w, int h, int scaleW, int scaleH)
{
	if (plane < 0 || plane >= VIDEO_PlaneCount || !VIDEO_HavePlane(plane) || w == 0 || h == 0 || !MAIN_RenderThisFrame()) {
		return;
	}

	const SDL_FRect src = { srcX, srcY, w, h };
	SDL_FRect dst = {
		dstX + VIDEO_PlaneDrawOffsetX,
		dstY + VIDEO_PlaneDrawOffsetY,
		(int)(w * (scaleW / 65536.0f)),
		(int)(h * (scaleH / 65536.0f))
	};
	if (!GLOBAL_ScreenRenderTarget) {
		dst.x += GLOBAL_ScreenSubpixelOffset;
		dst.y += GLOBAL_ScreenSubpixelOffset;
	}
	if (!SDL_RenderTexture(GLOBAL_ScreenRenderer, VIDEO_Planes[plane], &src, &dst)) {
		ERROR_Set("Error rendering graphics: %s", SDL_GetError());
		MAIN_Exit(SDL_APP_FAILURE);
	}
}

void VIDEO_DrawPlaneTransparentScaled(int plane, int dstX, int dstY, uint8_t a, int scaleW, int scaleH)
{
	VIDEO_DrawPlaneRectTransparentScaled(plane, dstX, dstY, 0, 0, VIDEO_LogicalWidth, VIDEO_LogicalHeight, a, scaleW, scaleH);
}

void VIDEO_DrawPlaneRectTransparentScaled(int plane, int dstX, int dstY, int srcX, int srcY, int w, int h, uint8_t a, int scaleW, int scaleH)
{
	if (plane < 0 || plane >= VIDEO_PlaneCount || !VIDEO_HavePlane(plane) || w == 0 || h == 0 || !MAIN_RenderThisFrame()) {
		return;
	}

	const SDL_FRect src = { srcX, srcY, w, h };
	SDL_FRect dst = {
		dstX + VIDEO_PlaneDrawOffsetX,
		dstY + VIDEO_PlaneDrawOffsetX,
		w * (scaleW / 65536.0f),
		h * (scaleH / 65536.0f)
	};
	if (!GLOBAL_ScreenRenderTarget) {
		dst.x += GLOBAL_ScreenSubpixelOffset;
		dst.y += GLOBAL_ScreenSubpixelOffset;
	}
	if (
		!SDL_SetTextureAlphaMod(VIDEO_Planes[plane], a) ||
		!SDL_RenderTexture(GLOBAL_ScreenRenderer, VIDEO_Planes[plane], &src, &dst) ||
		!SDL_SetTextureAlphaMod(VIDEO_Planes[plane], SDL_ALPHA_OPAQUE)
	) {
		ERROR_Set("Error rendering graphics: %s", SDL_GetError());
		MAIN_Exit(SDL_APP_FAILURE);
	}
}

void VIDEO_DrawPlaneText(int plane, const char* text, char firstChar, int charW, int charH, int dstX, int dstY, int sheetX, int sheetY, int sheetW)
{
	if (
		plane < 0 || plane >= VIDEO_PlaneCount || !VIDEO_HavePlane(plane) ||
		!text || !*text || charW <= 0 || charH <= 0 || dstX >= VIDEO_LogicalWidth || dstY >= VIDEO_LogicalHeight || dstY + charH <= 0 ||
		!MAIN_RenderThisFrame()
	) {
		return;
	}
	const size_t length = SDL_strlen(text);
	if (length > INT_MAX / 6) {
		ERROR_Set("Length of text to draw is too long, %d is max supported", INT_MAX / 6);
		goto fail;
	}
	if (dstX + length * charW <= 0) {
		return;
	}

	if (length > VIDEO_TextDataLength) {
		SDL_Vertex* const vertices = SDL_realloc(VIDEO_TextVertices, length * 4 * sizeof(SDL_Vertex));
		if (!vertices) {
			ERROR_Set("Could not allocate vertex array");
			goto fail;
		}
		VIDEO_TextVertices = vertices;
		for (
			SDL_Vertex* vertex = VIDEO_TextVertices + VIDEO_TextDataLength * 4, * const verticesEnd = VIDEO_TextVertices + length * 4;
			vertex < verticesEnd;
			vertex++
		) {
			vertex->color = (SDL_FColor) { 1.0f, 1.0f, 1.0f, SDL_ALPHA_OPAQUE_FLOAT };
		}

		int* const indices = SDL_realloc(VIDEO_TextIndices, length * 6 * sizeof(int));
		if (!indices) {
			ERROR_Set("Could not allocate index array");
			goto fail;
		}
		VIDEO_TextIndices = indices;
		int* index = VIDEO_TextIndices + VIDEO_TextDataLength * 6;
		int* const indicesEnd = VIDEO_TextIndices + length * 6;
		int i = (int)VIDEO_TextDataLength * 4;
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

		VIDEO_TextDataLength = length;
	}

	SDL_Vertex* vertex = VIDEO_TextVertices;
	const char* c = text;
	SDL_Vertex* const verticesEnd = VIDEO_TextVertices + length * 4;
	float x = dstX + VIDEO_PlaneDrawOffsetX;
	float y = dstY + VIDEO_PlaneDrawOffsetY;
	if (!GLOBAL_ScreenRenderTarget) {
		x += GLOBAL_ScreenSubpixelOffset;
		y += GLOBAL_ScreenSubpixelOffset;
	}
	float texW, texH;
	if (!SDL_GetTextureSize(VIDEO_Planes[plane], &texW, &texH)) {
		goto fail;
	}
	const float texCharW = (float)charW / texW;
	const float texCharH = (float)charH / texH;
	while (vertex < verticesEnd) {
		const int charPos = (*c - firstChar) * charW;
		const int charX = sheetX + charPos % sheetW;
		const int charY = sheetY + (charPos / sheetW) * charH;
		const float texX = charX / texW;
		const float texY = charY / texH;

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

	if (!SDL_RenderGeometry(GLOBAL_ScreenRenderer, VIDEO_Planes[plane], VIDEO_TextVertices, (int)length * 4, VIDEO_TextIndices, (int)length * 6)) {
		ERROR_Set("Failed rendering: %s", SDL_GetError());
		goto fail;
	}
	return;

	fail:
	MAIN_Exit(SDL_APP_FAILURE);
}

void VIDEO_SetPlaneDrawOffset(int x, int y)
{
	VIDEO_PlaneDrawOffsetX = x;
	VIDEO_PlaneDrawOffsetY = y;
}

float VIDEO_GetScreenSubpixelOffset(void)
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
	if (GLOBAL_ScreenRenderer && !GLOBAL_ScreenRenderTarget) {
		SDL_FRect rect;
		if (!SDL_GetRenderLogicalPresentationRect(GLOBAL_ScreenRenderer, &rect)) {
			ERROR_Set("Graphics error: %s", SDL_GetError());
			MAIN_Exit(SDL_APP_FAILURE);
		}
		const float scale = rect.w / VIDEO_LogicalWidth;
		float y;
		if (SDL_modff(scale, &y) != 0.0f) {
			return 0.49f / scale;
		}
		else {
			return 0.0f;
		}
	}
	else {
		return 0.0f;
	}
}
