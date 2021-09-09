#pragma once

#include "SDL.h"

#ifdef		TextOut
#undef		TextOut
#endif
#ifdef		LoadBitmap
#undef		LoadBitmap
#endif

#define		YGS_DEADZONE_MAX	(32767 / 4)

bool YGS2kInit();
void YGS2kExit();
bool YGS2kHalt();

void YGS2kTextOut(int x, int y, const char* text, int r, int g, int b, int size);

int IsPlayMIDI();

typedef struct {
	int32_t data[4];
} SJoyPadGUID;

typedef enum {
	JOYKEY_AXIS,
	JOYKEY_HAT,
	JOYKEY_BUTTON
} EJoyKeyType;

typedef union {
	struct { int index, value; };
	int button;
} SJoyKeySetting;

typedef struct {
	int device;
        SJoyPadGUID guid;
	EJoyKeyType type;
	SJoyKeySetting setting;
} SJoyKey;

int IsPushKey ( int key );
int IsPressKey ( int key );
int IsPushJoyKey ( const SJoyKey* const key );
int IsPressJoyKey ( const SJoyKey* const key );
int IsPushReturnKey();
int IsPushDeleteKey();
int IsPushBSKey();
int IsPushEscKey();
int IsPushEndKey();
int GetMaxKey();
int GetMaxJoyPad();
SJoyPadGUID GetJoyPadGUID( int device );
int GetMaxJoyAxis( int device );
int GetMaxJoyHat( int device );
int GetMaxJoyButton( int device );

int GetMaxDisplayIndex();
int GetMaxDisplayMode( int displayIndex );

void SetConstParam ( const char *param, int value );
void KeyInput();
int Rand ( int max );
void PauseMIDI();
void ReplayMIDI();
void PlayWave ( int no );
void ReplayWave ( int no );
void StopWave ( int no );
void PauseWave ( int no );
void SetVolumeWave( int no, int vol );
int IsPlayWave( int no );
void LoadWave( const char* filename, int no );
void SetLoopModeWave( int no, int mode );

void LoadMIDI( const char* filename );
void LoadBitmap( const char* filename, int plane, int value );
void PlayMIDI();
void StopMIDI();
void SetColorKeyPos(int plane, int x, int y);
void EnableBlendColorKey(int plane, int key);
void CreateSurface(int surf, int w, int h);
void ClearSecondary();
void SetFillColor(int col);
void LoadFile( const char* filename, void* buf, int size );
void SaveFile( const char* filename, void* buf, int size );
void TextLayerOn ( int layer, int x, int y );
void TextMove ( int layer, int x, int y );
void TextColor ( int layer, int r, int g, int b );
void TextBackColorDisable ( int layer );
void TextSize ( int layer, int size );
void TextHeight ( int layer, int height );
void TextOut ( int layer, const char* text );
void TextBlt ( int layer );
void TextLayerOff ( int layer );
void BltAlways(bool always);
void Blt(int pno, int dx, int dy);
void BltRect(int pno, int dx, int dy, int sx, int sy, int hx, int hy);
void BltFast(int pno, int dx, int dy);
void BltFastRect(int pno, int dx, int dy, int sx, int sy, int hx, int hy);
void BlendBlt(int pno, int dx, int dy, int ar, int ag, int ab, int br, int bg, int bb);
void BlendBltRect(int pno, int dx, int dy, int sx, int sy, int hx, int hy, int ar, int ag, int ab, int br, int bg, int bb);
void BltR(int pno, int dx, int dy, int scx, int scy);
void BltRectR(int pno, int dx, int dy, int sx, int sy, int hx, int hy, int scx, int scy);
void BltFastR(int pno, int dx, int dy, int scx, int scy);
void BltFastRectR(int pno, int dx, int dy, int sx, int sy, int hx, int hy, int scx, int scy);
void BltTrans(int pno, int dx, int dy);
void BlendBltR(int pno, int dx, int dy, int ar, int ag, int ab, int br, int bg, int bb, int scx, int scy);
void BlendBltRectR(int pno, int dx, int dy, int sx, int sy, int hx, int hy, int ar, int ag, int ab, int br, int bg, int bb, int scx, int scy);

void SetSecondaryOffset(int x, int y);
void SetColorKeyRGB(int pno, int r, int g, int b);
void SwapToSecondary(int pno);

void SetFPS(int fps);
int GetFPS();
int GetRealFPS();

void StrCpy(char *dest, const char *src);
void StrCat(char *str1, const char *str2);
int StrLen(const char *stri);
void MidStr(const char *src, int start, int len, char *dest);
void LeftStr(const char *src, int len, char *dest);
char CharAt(const char *stri, int pos);
int ValLong(const char *stri);
void FillMemory(void* buf, int size, int val);

void YGS2kKanjiFontInitialize();
void YGS2kKanjiFontFinalize();
void YGS2kKanjiDrawSub(int x, int y, int kx, int ky);
void YGS2kKanjiDraw(int x, int y, int r, int g, int b, int size, const char *str);
