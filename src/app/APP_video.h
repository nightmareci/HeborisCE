#ifndef APP_video_h_
#define APP_video_h_

#include "APP_stdinc.h"

typedef enum APP_ScreenModeFlag {
	APP_SCREEN_MODE_WINDOW,
	APP_SCREEN_MODE_WINDOW_MAXIMIZED,
	APP_SCREEN_MODE_FULLSCREEN_DESKTOP,
	APP_SCREEN_MODE_FULLSCREEN,
	APP_SCREEN_MODE_WINDOW_TYPES_COUNT,
	APP_SCREEN_MODE_WINDOW_TYPE = 0x3,
	APP_SCREEN_MODE_DETAIL_LEVEL = 0x4,
	APP_SCREEN_MODE_VSYNC = 0x8,
	APP_SCREEN_MODE_SCALE_MODE = 0x10,
	APP_SCREEN_MODE_RENDER_LEVEL = 0x20
} APP_ScreenModeFlag;

typedef enum APP_ScreenIndexMask {
	APP_SCREEN_INDEX_DISPLAY = 0x0000FFFF,
	APP_SCREEN_INDEX_MODE = 0xFFFF0000
} APP_ScreenIndexMask;

#define APP_SCREEN_INDEX_DISPLAY_TO_VALUE(setting) (((int)(setting)) & 0xFFFF)
#define APP_SCREEN_INDEX_DISPLAY_TO_SETTING(value) (((int32_t)(value)) & APP_SCREEN_INDEX_DISPLAY)

#define APP_SCREEN_INDEX_MODE_TO_VALUE(setting) ((int)((((uint32_t)(setting)) & APP_SCREEN_INDEX_MODE) >> 16))
#define APP_SCREEN_INDEX_MODE_TO_SETTING(setting) ((int32_t)((((uint32_t)(setting)) << 16) & APP_SCREEN_INDEX_MODE))

#define APP_SCREEN_INDEX_TO_SETTING(display, mode) (((int32_t)((mode) & 0xFFFF) << 16) | (int32_t)((display) & 0xFFFF))

void APP_InitVideo(void);
void APP_QuitVideo(void);

void APP_ResetFrameStep(void);
bool APP_SetScreen(APP_ScreenModeFlag* screenMode, int32_t* screenIndex);
bool APP_RenderScreen(void);
int APP_GetMaxDisplayIndex();
int APP_GetMaxDisplayMode(int displayIndex);
void APP_GetDisplayMode(int displayIndex, int modeIndex, SDL_DisplayMode* mode);
const SDL_DisplayMode* APP_GetDesktopDisplayMode(int displayIndex);

void APP_EnableTextLayer(int layer, int x, int y);
void APP_SetTextLayerDrawPosition(int layer, int x, int y);
void APP_SetTextLayerColor(int layer, int r, int g, int b);
void APP_SetTextLayerSize(int layer, int size);
void APP_PutTextLayerString(int layer, const char* string);
void APP_DrawTextLayer(int layer);
void APP_DisableTextLayer(int layer);

// Load an image file into a drawing plane.
// filename must not have a file extension. All supported file types/extensions
// are tried until a file is found.
void APP_LoadPlane(int plane, const char* filename);
void APP_DrawPlane(int plane, int dstX, int dstY);
void APP_DrawPlaneRect(int plane, int dstX, int dstY, int srcX, int srcY, int w, int h);
void APP_DrawPlaneTransparent(int plane, int dstX, int dstY, int a);
void APP_DrawPlaneRectTransparent(int plane, int dstX, int dstY, int srcX, int srcY, int w, int h, int a);
void APP_DrawPlaneScaled(int plane, int dstX, int dstY, int scaleW, int scaleH);
void APP_DrawPlaneRectScaled(int plane, int dstX, int dstY, int srcX, int srcY, int w, int h, int scaleW, int scaleH);
void APP_DrawPlaneTransparentScaled(int plane, int dstX, int dstY, int a, int scaleW, int scaleH);
void APP_DrawPlaneRectTransparentScaled(int plane, int dstX, int dstY, int srcX, int srcY, int w, int h, int a, int scaleW, int scaleH);
void APP_SetPlaneDrawOffset(int x, int y);

float APP_GetScreenSubpixelOffset(void);

void APP_SetDrawWhileSkippingFrames(bool draw);

#endif
