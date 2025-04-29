#pragma once

#include "include.h"
#include "ygs2kinput.h"

#define		YGS_DEADZONE_MAX	(32767 / 4)

void YGS2kInit(int soundBufferSize);
void YGS2kDeinit();
void YGS2kExit(int exitStatus);
bool YGS2kHalt();

typedef enum YGS2kEScreenModeFlag {
	YGS_SCREENMODE_WINDOW,
	YGS_SCREENMODE_WINDOW_MAXIMIZED,
	YGS_SCREENMODE_FULLSCREEN_DESKTOP,
	YGS_SCREENMODE_FULLSCREEN,
	YGS_SCREENMODE_NUMWINDOWTYPES,
	YGS_SCREENMODE_WINDOWTYPE = 0x3,
	YGS_SCREENMODE_DETAILLEVEL = 0x4,
	YGS_SCREENMODE_VSYNC = 0x8,
	YGS_SCREENMODE_SCALEMODE = 0x10,
	YGS_SCREENMODE_RENDERLEVEL = 0x20
} YGS2kEScreenModeFlag;

typedef enum YGS2kEScreenIndexMask {
	YGS_SCREENINDEX_DISPLAY = 0x0000FFFF,
	YGS_SCREENINDEX_MODE = 0xFFFF0000
} YGS2kEScreenIndexMask;

#define YGS_SCREENINDEX_DISPLAY_TOVALUE(setting) (((int)(setting)) & 0xFFFF)
#define YGS_SCREENINDEX_DISPLAY_TOSETTING(value) (((int32_t)(value)) & YGS_SCREENINDEX_DISPLAY)

#define YGS_SCREENINDEX_MODE_TOVALUE(setting) ((int)((((uint32_t)(setting)) & YGS_SCREENINDEX_MODE) >> 16))
#define YGS_SCREENINDEX_MODE_TOSETTING(setting) ((int32_t)((((uint32_t)(setting)) << 16) & YGS_SCREENINDEX_MODE))

#define YGS_SCREENINDEX_TOSETTING(display, mode) (((int32_t)((mode) & 0xFFFF) << 16) | (int32_t)((display) & 0xFFFF))

typedef enum YGS2kEWaveFormat
{
	YGS_WAVE_MASK   = 0xFF,

	// Flag BGM type as "simple", only one bgm.ext for everything.
	YGS_WAVE_SIMPLE = 0x80,

	YGS_WAVE_FORMAT = 0x7F,
	YGS_WAVE_MID    =  1,
	YGS_WAVE_WAV    =  2,
	YGS_WAVE_OGG    =  3,
	YGS_WAVE_MP3    =  4,
	YGS_WAVE_FLAC   =  5,
	YGS_WAVE_OPUS   =  6,
	YGS_WAVE_MOD    =  7,
	YGS_WAVE_IT     =  8,
	YGS_WAVE_XM     =  9,
	YGS_WAVE_S3M    = 10,
	YGS_WAVE_MAXFORMAT
} YGS2kEWaveFormat;

bool YGS2kSetScreen(YGS2kEScreenModeFlag *screenMode, int32_t *screenIndex);
int YGS2kGetMaxDisplayIndex();
int YGS2kGetMaxDisplayMode( int displayIndex );
bool YGS2kRenderLevelLowSupported();

void YGS2kSetConstParam ( const char *param, int value );

int YGS2kRand ( int max );

void YGS2kPlayWave ( int num );
void YGS2kReplayWave ( int num );
void YGS2kStopWave ( int num );
void YGS2kPauseWave ( int num );
void YGS2kSetVolumeWave( int num, int vol );
int YGS2kIsPlayWave( int num );
void YGS2kLoadWave( const char* filename, int num );
void YGS2kSetLoopModeWave( int num, int mode );

int YGS2kIsPlayMusic();
void YGS2kPauseMusic();
void YGS2kReplayMusic();
void YGS2kLoadMusic( const char* filename );
void YGS2kPlayMusic();
void YGS2kStopMusic();
void YGS2kSetVolumeMusic(int vol);
bool YGS2kWaveFormatSupported(YGS2kEWaveFormat format);

void YGS2kLoadBitmap( const char* filename, int plane, int value );
void YGS2kSetColorKeyPos(int plane, int x, int y);
void YGS2kEnableBlendColorKey(int plane, int key);
void YGS2kCreateSurface(int surf, int w, int h);
void YGS2kClearSecondary();
void YGS2kSetFillColor(int col);
void YGS2kLoadFile( const char* filename, void* buf, size_t size );
void YGS2kReadFile( const char* filename, void* buf, size_t size, size_t offset );
void YGS2kSaveFile( const char* filename, void* buf, size_t size );
void YGS2kAppendFile( const char* filename, void* buf, size_t size );
void YGS2kTextLayerOn ( int layer, int x, int y );
void YGS2kTextMove ( int layer, int x, int y );
void YGS2kTextColor ( int layer, int r, int g, int b );
void YGS2kTextBackColorDisable ( int layer );
void YGS2kTextSize ( int layer, int size );
void YGS2kTextHeight ( int layer, int height );
void YGS2kTextOut ( int layer, const char* text );
void YGS2kTextBlt ( int layer );
void YGS2kTextLayerOff ( int layer );
void YGS2kBltAlways(bool always);
void YGS2kBlt(int pno, int dx, int dy);
void YGS2kBltRect(int pno, int dx, int dy, int sx, int sy, int hx, int hy);
void YGS2kBltFast(int pno, int dx, int dy);
void YGS2kBltFastRect(int pno, int dx, int dy, int sx, int sy, int hx, int hy);
void YGS2kBlendBlt(int pno, int dx, int dy, int ar, int ag, int ab, int br, int bg, int bb);
void YGS2kBlendBltRect(int pno, int dx, int dy, int sx, int sy, int hx, int hy, int ar, int ag, int ab, int br, int bg, int bb);
void YGS2kBltR(int pno, int dx, int dy, int scx, int scy);
void YGS2kBltRectR(int pno, int dx, int dy, int sx, int sy, int hx, int hy, int scx, int scy);
void YGS2kBltFastR(int pno, int dx, int dy, int scx, int scy);
void YGS2kBltFastRectR(int pno, int dx, int dy, int sx, int sy, int hx, int hy, int scx, int scy);
void YGS2kBltTrans(int pno, int dx, int dy);
void YGS2kBlendBltR(int pno, int dx, int dy, int ar, int ag, int ab, int br, int bg, int bb, int scx, int scy);
void YGS2kBlendBltRectR(int pno, int dx, int dy, int sx, int sy, int hx, int hy, int ar, int ag, int ab, int br, int bg, int bb, int scx, int scy);

void YGS2kSetSecondaryOffset(int x, int y);
void YGS2kSetColorKeyRGB(int pno, int r, int g, int b);
void YGS2kSwapToSecondary(int pno);

void YGS2kResetFrameStep();
void YGS2kSetFPS(unsigned fps);
int YGS2kGetFPS();
int YGS2kGetRealFPS();

void YGS2kStrCpy(char *dest, const char *src);
void YGS2kStrCat(char *str1, const char *str2);
int YGS2kStrLen(const char *stri);
void YGS2kMidStr(const char *src, int start, int len, char *dest);
void YGS2kLeftStr(const char *src, int len, char *dest);
char YGS2kCharAt(const char *stri, int pos);
int YGS2kValLong(const char *stri);
void YGS2kFillMemory(void* buf, int size, int val);

bool IsRenderToTargetSupported();
extern int32_t getDrawRate();