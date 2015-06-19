#ifndef		__YGS2KFUNC_H__
#define		__YGS2KFUNC_H__

#ifdef		TextOut
#undef		TextOut
#endif
#ifdef		LoadBitmap
#undef		LoadBitmap
#endif

bool YGS2kInit();
void YGS2kExit();
bool YGS2kHalt();
void YGS2kTextOut(int x, int y, char* text, int r = 255, int g = 255, int b = 255, int size = 12);
void YGS2kPrimitiveFlush();
void YGS2kSetExtraBltColor(int r = 255, int g = 255, int b = 255, int a = 255);

int IsPlayMIDI();
void SelectJoyStick( int pl );
int IsPushKey ( int key );
int IsPressKey ( int key );
int IsPushJoyKey ( int key );
int IsPressJoyKey ( int key );
int IsPushReturnKey();
int IsPushDeleteKey();
int IsPushBSKey();
int IsPushEscKey();
int IsPushEndKey();
int getMaxKey();

void SetJoyButtonMax ( int max );
void SetConstParam ( char *param, int value );
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
void LoadWave( char* filename, int no );
void SetLoopModeWave( int no, int mode );

void LoadMIDI( char* filename );
void LoadBitmap( char* filename, int plane, int val );
void PlayMIDI();
void StopMIDI();
void SetColorKeyPos(int plane, int x, int y);
void EnableBlendColorKey(int plane, int key);
void CreateSurface(int surf, int w, int h);
void ClearSecondary();
void SetFillColor(int col);
void LoadFile( char* filename, void* buf, int size );
void SaveFile( char* filename, void* buf, int size );
void TextLayerOn ( int layer, int x, int y );
void TextMove ( int layer, int x, int y );
void TextColor ( int layer, int r, int g, int b );
void TextBackColorDisable ( int layer );
void TextSize ( int layer, int size );
void TextHeight ( int layer, int height );
void TextOut ( int layer, char* text );
void TextBlt ( int layer );
void TextLayerOff ( int layer );
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

void YGS2kExtraBlt(int pno, int dx, int dy, int rotate = 65536, int scx = 65536, int scy = 65536, int rotcx = 0, int rotcy = 0);
void YGS2kExtraBltRect(int pno, int dx, int dy, int sx, int sy, int hx, int hy, int rotate = 65536, int scx = 65536, int scy = 65536, int rotcx = 0, int rotcy = 0);

void SetSecondaryOffset(int x, int y);
void SetColorKeyRGB(int pno, int r, int g, int b);
void SwapToSecondary(int pno);

void SetFPS(int fps);
int GetFPS();
int GetRealFPS();

void StrCpy(char *dest, char *src);
void StrCpy(void *dest, char *src);
void StrCpy(char *dest, void *src);
void StrCat(char *str1, char *str2);
int StrLen(char *stri);
void MidStr(char *src, int start, int len, char *dest);
void LeftStr(char *src, int len, char *dest);
char CharAt(char *stri, int pos);
int ValLong(char *stri);

#endif
