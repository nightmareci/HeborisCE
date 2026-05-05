#ifndef VIDEO_h_
#define VIDEO_h_

#include <STD.h>

typedef enum VIDEO_ScreenModeFlag
{
	VIDEO_SCREEN_MODE_WINDOW,
	VIDEO_SCREEN_MODE_WINDOW_MAXIMIZED,
	VIDEO_SCREEN_MODE_FULLSCREEN_DESKTOP,
	VIDEO_SCREEN_MODE_FULLSCREEN,
	VIDEO_SCREEN_MODE_WINDOW_TYPES_COUNT,
	VIDEO_SCREEN_MODE_WINDOW_TYPE = 0x3,
	VIDEO_SCREEN_MODE_DETAIL_LEVEL = 0x4,
	VIDEO_SCREEN_MODE_VSYNC = 0x8,
	VIDEO_SCREEN_MODE_SCALE_MODE = 0x10,
	VIDEO_SCREEN_MODE_RENDER_LEVEL = 0x20
} VIDEO_ScreenModeFlag;

typedef enum VIDEO_ScreenIndexMask
{
	VIDEO_SCREEN_INDEX_DISPLAY = 0x0000FFFF,
	VIDEO_SCREEN_INDEX_MODE = 0xFFFF0000
} VIDEO_ScreenIndexMask;

#define VIDEO_SCREEN_INDEX_DISPLAY_TO_VALUE(setting) (((int)(setting)) & 0xFFFF)
#define VIDEO_SCREEN_INDEX_DISPLAY_TO_SETTING(value) (((int32_t)(value)) & VIDEO_SCREEN_INDEX_DISPLAY)

#define VIDEO_SCREEN_INDEX_MODE_TO_VALUE(setting) ((int)((((uint32_t)(setting)) & VIDEO_SCREEN_INDEX_MODE) >> 16))
#define VIDEO_SCREEN_INDEX_MODE_TO_SETTING(setting) ((int32_t)((((uint32_t)(setting)) << 16) & VIDEO_SCREEN_INDEX_MODE))

#define VIDEO_SCREEN_INDEX_TO_SETTING(display, mode) (((int32_t)((mode) & 0xFFFF) << 16) | (int32_t)((display) & 0xFFFF))

#define VIDEO_SCREEN_WIDTH 320
#define VIDEO_SCREEN_HEIGHT 240
#define VIDEO_SCREEN_WIDE ((float)VIDEO_SCREEN_WIDTH / VIDEO_SCREEN_HEIGHT)

void VIDEO_Init(int planeCount, int textLayerCount);
void VIDEO_Quit(void);

void VIDEO_SetScreen(VIDEO_ScreenModeFlag* screenMode, int32_t* screenIndex);
int VIDEO_GetMaxDisplayIndex();
int VIDEO_GetMaxDisplayMode(int displayIndex);
void VIDEO_GetDisplayMode(int displayIndex, int modeIndex, SDL_DisplayMode* mode);
const SDL_DisplayMode* VIDEO_GetDesktopDisplayMode(int displayIndex);

void VIDEO_EnableTextLayer(int layer, int x, int y);
void VIDEO_SetTextLayerDrawPosition(int layer, int x, int y);
void VIDEO_SetTextLayerColor(int layer, int r, int g, int b);
void VIDEO_SetTextLayerSize(int layer, int size);
void VIDEO_PutTextLayerString(int layer, const char* string);
void VIDEO_DrawTextLayer(int layer);
void VIDEO_DisableTextLayer(int layer);

// Load an image file into a drawing plane.
// filename must not have a file extension. All supported file types/extensions
// are tried until a file is found.
void VIDEO_LoadPlane(int plane, const char* filename);

// Check if a plane is loaded.
//
// If plane < 0, returns whether at least one plane is loaded.
bool VIDEO_HavePlane(int plane);

void VIDEO_DrawPlane(int plane, int dstX, int dstY);
void VIDEO_DrawPlaneRect(int plane, int dstX, int dstY, int srcX, int srcY, int w, int h);

void VIDEO_DrawPlaneTransparent(int plane, int dstX, int dstY, uint8_t a);
void VIDEO_DrawPlaneRectTransparent(int plane, int dstX, int dstY, int srcX, int srcY, int w, int h, uint8_t a);

void VIDEO_DrawPlaneScaled(int plane, int dstX, int dstY, int scaleW, int scaleH);
void VIDEO_DrawPlaneRectScaled(int plane, int dstX, int dstY, int srcX, int srcY, int w, int h, int scaleW, int scaleH);

void VIDEO_DrawPlaneTransparentScaled(int plane, int dstX, int dstY, uint8_t a, int scaleW, int scaleH);
void VIDEO_DrawPlaneRectTransparentScaled(int plane, int dstX, int dstY, int srcX, int srcY, int w, int h, uint8_t a, int scaleW, int scaleH);

void VIDEO_DrawPlaneText(int plane, const char* text, char firstChar, int charW, int charH, int dstX, int dstY, int sheetX, int sheetY, int sheetW);

void VIDEO_SetPlaneDrawOffset(int x, int y);

float VIDEO_GetScreenSubpixelOffset(void);

#endif
