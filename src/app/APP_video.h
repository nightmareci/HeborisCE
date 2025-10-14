#ifndef APP_video_h_
#define APP_video_h_

#include "APP_stdinc.h"

typedef enum APP_ScreenModeFlag {
	APP_SCREENMODE_WINDOW,
	APP_SCREENMODE_WINDOW_MAXIMIZED,
	APP_SCREENMODE_FULLSCREEN_DESKTOP,
	APP_SCREENMODE_FULLSCREEN,
	APP_SCREENMODE_NUMWINDOWTYPES,
	APP_SCREENMODE_WINDOWTYPE = 0x3,
	APP_SCREENMODE_DETAILLEVEL = 0x4,
	APP_SCREENMODE_VSYNC = 0x8,
	APP_SCREENMODE_SCALEMODE = 0x10,
	APP_SCREENMODE_RENDERLEVEL = 0x20
} APP_ScreenModeFlag;

typedef enum APP_ScreenIndexMask {
	APP_SCREENINDEX_DISPLAY = 0x0000FFFF,
	APP_SCREENINDEX_MODE = 0xFFFF0000
} APP_ScreenIndexMask;

#define APP_SCREENINDEX_DISPLAY_TOVALUE(setting) (((int)(setting)) & 0xFFFF)
#define APP_SCREENINDEX_DISPLAY_TOSETTING(value) (((int32_t)(value)) & APP_SCREENINDEX_DISPLAY)

#define APP_SCREENINDEX_MODE_TOVALUE(setting) ((int)((((uint32_t)(setting)) & APP_SCREENINDEX_MODE) >> 16))
#define APP_SCREENINDEX_MODE_TOSETTING(setting) ((int32_t)((((uint32_t)(setting)) << 16) & APP_SCREENINDEX_MODE))

#define APP_SCREENINDEX_TOSETTING(display, mode) (((int32_t)((mode) & 0xFFFF) << 16) | (int32_t)((display) & 0xFFFF))

void APP_InitVideo(void);
void APP_QuitVideo(void);

bool APP_SetScreen(APP_ScreenModeFlag *screenMode, int32_t *screenIndex);
bool APP_RenderScreen(void);
int APP_GetMaxDisplayIndex();
int APP_GetMaxDisplayMode( int displayIndex );
bool APP_GetDisplayMode( int displayIndex, int modeIndex, SDL_DisplayMode *mode );
const SDL_DisplayMode* APP_GetDesktopDisplayMode(unsigned displayIndex);

void APP_LoadBitmap( const char* filename, unsigned plane );
void APP_SetColorKeyPos(int plane, int x, int y);
void APP_EnableBlendColorKey(int plane, int key);
void APP_CreateSurface(int surf, int w, int h);
void APP_ClearSecondary();
void APP_SetFillColor(int col);
void APP_TextLayerOn ( int layer, int x, int y );
void APP_TextMove ( int layer, int x, int y );
void APP_TextColor ( int layer, int r, int g, int b );
void APP_TextBackColorDisable ( int layer );
void APP_TextSize ( int layer, int size );
void APP_TextHeight ( int layer, int height );
void APP_TextOut ( int layer, const char* text );
void APP_TextBlt ( int layer );
void APP_TextLayerOff ( int layer );
void APP_Blt(int pno, int dx, int dy);
void APP_BltRect(int pno, int dx, int dy, int sx, int sy, int hx, int hy);
void APP_BltFast(int pno, int dx, int dy);
void APP_BltFastRect(int pno, int dx, int dy, int sx, int sy, int hx, int hy);
void APP_BlendBlt(int pno, int dx, int dy, int ar, int ag, int ab, int br, int bg, int bb);
void APP_BlendBltRect(int pno, int dx, int dy, int sx, int sy, int hx, int hy, int ar, int ag, int ab, int br, int bg, int bb);
void APP_BltR(int pno, int dx, int dy, int scx, int scy);
void APP_BltRectR(int pno, int dx, int dy, int sx, int sy, int hx, int hy, int scx, int scy);
void APP_BltFastR(int pno, int dx, int dy, int scx, int scy);
void APP_BltFastRectR(int pno, int dx, int dy, int sx, int sy, int hx, int hy, int scx, int scy);
void APP_BltTrans(int pno, int dx, int dy);
void APP_BlendBltR(int pno, int dx, int dy, int ar, int ag, int ab, int br, int bg, int bb, int scx, int scy);
void APP_BlendBltRectR(int pno, int dx, int dy, int sx, int sy, int hx, int hy, int ar, int ag, int ab, int br, int bg, int bb, int scx, int scy);

void APP_SetSecondaryOffset(int x, int y);
void APP_SetColorKeyRGB(int pno, int r, int g, int b);

float APP_GetScreenSubpixelOffset(void);

void APP_BltAlways(bool always);

#endif
