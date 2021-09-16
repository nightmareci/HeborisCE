#pragma once

#include <stdint.h>

typedef enum ScaleMode {
	SCALEMODE_FASTSTRETCH,
	SCALEMODE_INTEGER,
	SCALEMODE_PERFECTSTRETCH,
	MAXSCALEMODE
} ScaleMode;

typedef enum ScreenModeFlag {
	SCREENMODE_WINDOW,
	SCREENMODE_WINDOW_MAXIMIZED,
	SCREENMODE_FULLSCREEN_DESKTOP,
	SCREENMODE_FULLSCREEN,
	SCREENMODE_NUMWINDOWTYPES,
	SCREENMODE_WINDOWTYPE = 0x3,
	SCREENMODE_DETAILLEVEL = 0x4,
	SCREENMODE_VSYNC = 0x8,
	SCREENMODE_SCALEMODE = 0x30
} ScreenModeFlag;

typedef enum ScreenIndexMask {
	SCREENINDEX_DISPLAY = 0x0000FFFF,
	SCREENINDEX_MODE = 0xFFFF0000
} ScreenIndexMask;

#define SCALEMODE_TOVALUE(setting) ((ScaleMode)(((setting) & SCREENMODE_SCALEMODE) >> 4))
#define SCALEMODE_TOSETTING(value) ((int32_t)(((value) << 4) & SCREENMODE_SCALEMODE))

#define SCREENINDEX_DISPLAY_TOVALUE(setting) (((int)(setting)) & 0xFFFF)
#define SCREENINDEX_DISPLAY_TOSETTING(value) (((int32_t)(value)) & SCREENINDEX_DISPLAY)

#define SCREENINDEX_MODE_TOVALUE(setting) ((int)((((uint32_t)(setting)) & SCREENINDEX_MODE) >> 16))
#define SCREENINDEX_MODE_TOSETTING(setting) ((int32_t)((((uint32_t)(setting)) << 16) & SCREENINDEX_MODE))

#define SCREENINDEX_TOSETTING(display, mode) (((int32_t)((mode) & 0xFFFF) << 16) | (int32_t)((display) & 0xFFFF))