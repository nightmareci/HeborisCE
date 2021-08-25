#include "include.h"
#include "prototypes.h"
#include "gamedef.h"
#include "SDL_kanji.h"
#include "ygs2kfunc.h"

#include <iostream>
#include <cstdint>

#define		SCREEN_BPP			0
#define		USE_SOFTSTRETCH		1

#define		YGS_TEXTURE_MAX		100
#define		YGS_SOUND_MAX		100
#define		YGS_MUSIC_MAX		100
#define		YGS_KEYREPEAT_MAX	SDL_NUM_SCANCODES
#define		YGS_JOYREPEAT_MAX	20
#define		YGS_JOYPAD_MAX		2
#define		YGS_TEXTLAYER_MAX	16

#define		GAME_CAPTION		"HEBORIS C7-EX SDL2"

struct SScreenInfo
{
	int		real_w;
	int		real_h;
	int		win_w;
	int		win_h;
	int		full_screen;
};

struct STextLayer
{
	bool	enable;
	int		x;
	int		y;
	int		r, g, b;
	int		size;
	char	string[256];
};

enum
{
	YGS_SOUNDTYPE_NONE,
	YGS_SOUNDTYPE_WAV,
	YGS_SOUNDTYPE_MUS,
};

static SDL_Window		*s_pScreenWindow = NULL;
static SDL_Renderer		*s_pScreenRenderer = NULL;

static SDL_Texture		*s_pYGSTexture[YGS_TEXTURE_MAX];

static int				s_iKeyRepeat[YGS_KEYREPEAT_MAX];
static int				s_iJoyRepeat[YGS_JOYPAD_MAX][YGS_JOYREPEAT_MAX];
static SDL_Joystick		*s_pJoyPads[YGS_JOYPAD_MAX];
static int				s_iYGSSoundType[YGS_SOUND_MAX];
static Mix_Chunk		*s_pYGSSound[YGS_SOUND_MAX];
static int				s_iYGSSoundVolume[YGS_SOUND_MAX];
static Mix_Music		*s_pYGSExMusic[YGS_SOUND_MAX];
static Mix_Music		*s_pYGSMusic;

#define		YGS_KANJIFONTFILE_MAX	3
static Kanji_Font		*s_pKanjiFont[YGS_KANJIFONTFILE_MAX];

static int				s_iActivePad;

static int				s_iWinWidth;
static int				s_iWinHeight;
static float			s_fDrawScale = 1.0f;

static unsigned int		s_uTimeCount;
static unsigned int		s_uFPSCount;
static unsigned int		s_uNowFrame;
static unsigned int		s_uFPSCnting;
static unsigned int		s_uFPS;
static unsigned int		s_uNowFPS = 60;

static STextLayer		s_TextLayer[YGS_TEXTLAYER_MAX];
extern int				screenMode;

static int				s_iNewOffsetX = 0, s_iNewOffsetY = 0;
static int				s_iOffsetX = 0, s_iOffsetY = 0;

static SScreenInfo	s_ScreenInfo[] =
{
	{ 320, 240,  320,  240, true },		// 0
	{ 320, 240,  320,  240, false },	// 1
	{ 640, 480,  640,  480, false },	// 2
	{ 640, 480,  640,  480, true },		// 3
	{ 640, 480,  800,  600, false },	// 4
	{ 640, 480, 1024,  768, false },	// 5
	{ 640, 480, 1280,  960, false },	// 6
	{ 320, 240,  640,  480, true },		// 7
	{ 320, 240,  480,  360, false },	// 8
	{ 320, 240,  640,  480, false },	// 9
	{ 320, 240,  800,  600, false },	// 10
	{ 320, 240, 1024,  768, false },	// 11
	{ 320, 240, 1280,  960, false },	// 12
};

bool YGS2kInit()
{
	int		winWidth, winHeight;

	s_iNewOffsetX = 0;	s_iNewOffsetY = 0;
	s_iOffsetX = 0;		s_iOffsetY = 0;

	/* CONFIG.SAVより設定をロード */
	if ( LoadConfig() )
	{
		readdef::readdef();
		LoadConfig();
	}

	Uint32		windowFlags = SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE;

#if		0
	/* 画面モードの変更 */
	if ( screenMode >= 0 && screenMode <= 12 )
	{
		SScreenInfo		*s = &s_ScreenInfo[screenMode];
		winWidth  = s->win_w;
		winHeight = s->win_h;
		windowFlags |= SDL_WINDOW_OPENGL | (s->full_screen ? SDL_WINDOW_FULLSCREEN : 0);
		s_fDrawScale = (float)s->win_w / (float)s->real_w;
		SDL_GL_SetDrawRate(s_fDrawScale);
	}
#else
	/* 画面の設定 */
	if ( screenMode > 3 ) { screenMode = 1; }

	switch ( screenMode )
	{
	case 0:
	case 1:
		winWidth  = 320;
		winHeight = 240;
		break;

	default:
		winWidth  = 640;
		winHeight = 480;
		break;
	}

	/* ウィンドウの初期化 */
	windowFlags |= (screenMode == 0 || screenMode == 3) ? SDL_WINDOW_FULLSCREEN : 0;
#endif

	s_iWinWidth  = winWidth;
	s_iWinHeight = winHeight;


	/* キャプションの設定 */
	s_pScreenWindow = SDL_CreateWindow(GAME_CAPTION, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, winWidth, winHeight, windowFlags);
	s_pScreenRenderer = SDL_CreateRenderer(s_pScreenWindow, -1, 0);
	SDL_RenderSetLogicalSize(s_pScreenRenderer, winWidth, winHeight);

	/* マウスカーソルを消す場合は */
	if ( windowFlags & SDL_WINDOW_FULLSCREEN )
	{
		SDL_ShowCursor(SDL_DISABLE);
	}

	/* キーリピートバッファ初期化 */
	for ( int i = 0 ; i < YGS_KEYREPEAT_MAX ; i ++ )
	{
		s_iKeyRepeat[i] = 0;
	}

	for ( int pl = 0 ; pl < YGS_JOYPAD_MAX ; pl ++ )
	{
		for ( int i = 0 ; i < YGS_JOYREPEAT_MAX ; i ++ )
		{
			s_iJoyRepeat[pl][i] = 0;
		}
	}

	/* テクスチャ領域の初期化 */
	memset(s_pYGSTexture, 0, sizeof(s_pYGSTexture));

	/* サウンドの初期化 */
	for ( int i = 0 ; i < YGS_SOUND_MAX ; i ++ )
	{
		s_iYGSSoundType[i] = YGS_SOUNDTYPE_NONE;
		s_iYGSSoundVolume[i] = 0;
		s_pYGSSound[i] = NULL;
		s_pYGSExMusic[i] = NULL;
	}

	s_pYGSMusic = NULL;

	/* パッドの初期化 */
	for ( int i = 0 ; i < YGS_JOYPAD_MAX ; i ++ )
	{
		s_pJoyPads[i] = SDL_JoystickOpen(i);
	}

	/* テキストレイヤーの初期化 */
	for ( int i = 0 ; i < YGS_TEXTLAYER_MAX ; i ++ )
	{
		memset(&s_TextLayer[i], 0, sizeof(STextLayer));
		s_TextLayer[i].r = s_TextLayer[i].g = s_TextLayer[i].b = 255;
		s_TextLayer[i].size = 16;
	}
	YGS2kKanjiFontInitialize();

	s_iActivePad = 0;

	s_uTimeCount = SDL_GetTicks();
	s_uFPSCount  = SDL_GetTicks();
	s_uNowFrame  = 0;
	s_uFPSCnting = 0;
	s_uFPS       = 0;

	srand(time(NULL));

	return true;
}

void YGS2kExit()
{
	/* テクスチャ領域の解放 */
	for ( int i = 0 ; i < YGS_TEXTURE_MAX ; i ++ )
	{
		if ( s_pYGSTexture[i] )
		{
			SDL_DestroyTexture(s_pYGSTexture[i]);
			s_pYGSTexture[i] = NULL;
		}
	}
	if (s_pScreenRenderer ) {
		SDL_DestroyRenderer( s_pScreenRenderer );
		s_pScreenRenderer = NULL;
	}
	if ( s_pScreenWindow ) {
		SDL_DestroyWindow( s_pScreenWindow );
		s_pScreenWindow = NULL;
	}

	/* パッドのクローズ */
	for ( int i = 0 ; i < YGS_JOYPAD_MAX ; i ++ )
	{
		if ( s_pJoyPads[i] )
		{
			SDL_JoystickClose(s_pJoyPads[i]);
			s_pJoyPads[i] = NULL;
		}
	}

	/* サウンドの解放 */
	for ( int i = 0 ; i < YGS_SOUND_MAX ; i ++ )
	{
		s_iYGSSoundType[i] = YGS_SOUNDTYPE_NONE;

		if ( s_pYGSSound[i] )
		{
			Mix_FreeChunk(s_pYGSSound[i]);
			s_pYGSSound[i] = NULL;
		}
		if ( s_pYGSExMusic[i] )
		{
			Mix_FreeMusic(s_pYGSExMusic[i]);
			s_pYGSExMusic[i] = NULL;
		}
	}

	YGS2kKanjiFontFinalize();

	if ( s_pYGSMusic )
	{
		Mix_FreeMusic(s_pYGSMusic);
		s_pYGSMusic = NULL;
	}
}

bool YGS2kHalt()
{
	SDL_Event	ev;
	SDL_Keycode	*key;

	/* テキストレイヤーの描画 */
	for ( int i = 0 ; i < YGS_TEXTLAYER_MAX ; i ++ )
	{
		TextBlt(i);
	}

	/* バックサーフェスをフロントに転送 */
	SDL_RenderPresent( s_pScreenRenderer );

	/* 画面塗りつぶし */
	SDL_RenderClear( s_pScreenRenderer );

	/* イベント処理 */
	while(SDL_PollEvent(&ev) )
	{
		switch(ev.type){
			case SDL_QUIT:						// ウィンドウの×ボタンが押された時など
				return false;
				break;
			case SDL_KEYDOWN:					// キーボードからの入力があった時
				{
					key = &(ev.key.keysym.sym); // どのキーが押されたかを取得
					if ( *key == 27 )			// ESCキー
					{
						return false;
					}
				}
				break;
		}
	}

	/* フレームレート待ち */
	while (s_uTimeCount + (1000 / s_uNowFPS) >= SDL_GetTicks())
	{
		SDL_Delay(1);
	};

	/* フレームレート計算 */
	s_uFPSCnting ++;
	s_uNowFrame ++;

	s_uTimeCount = SDL_GetTicks();

	if ( s_uFPSCount + 1000 <= SDL_GetTicks() )
	{
		s_uFPS = s_uFPSCnting;
		s_uFPSCnting = 0;
		s_uFPSCount = SDL_GetTicks();
	}

	/* 画面ずらし量の反映 */
	s_iOffsetX = s_iNewOffsetX;
	s_iOffsetY = s_iNewOffsetY;

	return true;
}

void YGS2kTextOut(int x, int y, const char* text, int r, int g, int b, int size)
{
	YGS2kKanjiDraw(x, y, r, g, b, size, text);
}

int IsPlayMIDI()
{
	return Mix_PlayingMusic();
}

void SelectJoyStick( int pl )
{
	s_iActivePad = pl;
}

int IsPushKey ( int key )
{
	SDL_Scancode scancode = SDL_GetScancodeFromKey(key);
	return s_iKeyRepeat[scancode] == 1 ? 1 : 0;
}

int IsPressKey ( int key )
{
	SDL_Scancode scancode = SDL_GetScancodeFromKey(key);
	return s_iKeyRepeat[scancode] != 0 ? 1 : 0;
}

int IsPushJoyKey ( int key )
{
	return s_iJoyRepeat[s_iActivePad][key] == 1 ? 1 : 0;
}

int IsPressJoyKey ( int key )
{
	if ( s_pJoyPads[s_iActivePad] )
	{
		Uint8 hat = SDL_JoystickGetHat(s_pJoyPads[s_iActivePad], 0);
		switch ( key )
		{
		case 0:		// 上
			if ( SDL_JoystickGetAxis(s_pJoyPads[s_iActivePad], 1) < -32767 / 4 ) return 1;
			if ( hat & SDL_HAT_UP) return 1;
			break;

		case 1:		// 下
			if ( SDL_JoystickGetAxis(s_pJoyPads[s_iActivePad], 1) > +32767 / 4 ) return 1;
			if ( hat & SDL_HAT_DOWN) return 1;
			break;

		case 2:		// 左
			if ( SDL_JoystickGetAxis(s_pJoyPads[s_iActivePad], 0) < -32767 / 4 ) return 1;
			if ( hat & SDL_HAT_LEFT) return 1;
			break;

		case 3:		// 右
			if ( SDL_JoystickGetAxis(s_pJoyPads[s_iActivePad], 0) > +32767 / 4 ) return 1;
			if ( hat & SDL_HAT_RIGHT) return 1;
			break;

		default:
			int		key2 = key - 4;
			if ( key2 >= 0 && key2 < 16 ) return SDL_JoystickGetButton(s_pJoyPads[s_iActivePad], key2);
			break;
		}
	}

	return 0;
}

int IsPushReturnKey()
{
	return IsPushKey(SDLK_RETURN);
}

int IsPushDeleteKey()
{
	return IsPushKey(SDLK_DELETE);
}

int IsPushBSKey()
{
	return IsPushKey(SDLK_BACKSPACE);
}

int IsPushEscKey()
{
	return IsPushKey(SDLK_ESCAPE);
}

int IsPushEndKey()
{
	return IsPushKey(SDLK_END);
}

int getMaxKey()
{
	return YGS_KEYREPEAT_MAX;
}

void SetJoyButtonMax ( int max )
{

}

void SetConstParam ( const char *param, int value )
{

}

void KeyInput()
{
	int		padbak = s_iActivePad;
	int		keynum = 0;
	const Uint8	*KeyInp = SDL_GetKeyboardState(&keynum);

	for ( int i = 0 ; i < YGS_KEYREPEAT_MAX ; i ++ )
	{
		if ( i != 300 && i < keynum && KeyInp[i] == SDL_PRESSED )
		{
			s_iKeyRepeat[i] ++;
		}
		else
		{
			s_iKeyRepeat[i] = 0;
		}
	}

	if ( SDL_NumJoysticks() > 0 )
	{
		SDL_JoystickUpdate();
	}

	for ( int pl = 0 ; pl < 2 ; pl ++ )
	{
		s_iActivePad = pl;

		for ( int i = 0 ; i < YGS_JOYREPEAT_MAX ; i ++ )
		{
			if ( IsPressJoyKey(i) )
			{
				s_iJoyRepeat[pl][i] ++;
			}
			else
			{
				s_iJoyRepeat[pl][i] = 0;
			}
		}
	}

	s_iActivePad = padbak;
}

int Rand ( int max )
{
	return rand() % max;
}

void PauseMIDI()
{
	Mix_PauseMusic();
}

void ReplayMIDI()
{
	Mix_ResumeMusic();
}

void PlayWave ( int no )
{
	switch ( s_iYGSSoundType[no] )
	{
	case YGS_SOUNDTYPE_WAV:
		Mix_PlayChannel(no, s_pYGSSound[no], 0);
		Mix_Volume(no, s_iYGSSoundVolume[no]);
		break;

	case YGS_SOUNDTYPE_MUS:
if ((no == 56) || (no == 57)) {
		Mix_PlayMusic(s_pYGSExMusic[no], 0);
}else{
		Mix_PlayMusic(s_pYGSExMusic[no], -1);
}
		Mix_VolumeMusic(s_iYGSSoundVolume[no]);
		break;
	}
}

void ReplayWave ( int no )
{
	switch ( s_iYGSSoundType[no] )
	{
	case YGS_SOUNDTYPE_WAV:
		Mix_Resume(no);
		break;

	case YGS_SOUNDTYPE_MUS:
		Mix_RewindMusic();
		break;
	}
}

void StopWave ( int no )
{
	switch ( s_iYGSSoundType[no] )
	{
	case YGS_SOUNDTYPE_WAV:
		Mix_HaltChannel(no);
		break;

	case YGS_SOUNDTYPE_MUS:
		Mix_HaltMusic();
		break;
	}
}

void PauseWave ( int no )
{
	switch ( s_iYGSSoundType[no] )
	{
	case YGS_SOUNDTYPE_WAV:
		Mix_Pause(no);
		break;

	case YGS_SOUNDTYPE_MUS:
		Mix_PauseMusic();
		break;
	}
}

void SetVolumeWave( int no, int vol )
{
	int		volume = (vol + 10000) * 127 / 10000;
	if ( volume > 127 ) { volume = 127; }
	if ( volume < 0 )   { volume = 0; }
	s_iYGSSoundVolume[no] = volume;

	switch ( s_iYGSSoundType[no] )
	{
	case YGS_SOUNDTYPE_WAV:
		Mix_Volume(no, volume);
		break;

	case YGS_SOUNDTYPE_MUS:
		Mix_VolumeMusic(volume);
		break;
	}
}

int IsPlayWave( int no )
{
	switch ( s_iYGSSoundType[no] )
	{
	case YGS_SOUNDTYPE_WAV:
		/* なぜかここを実行すると落ちる… */
		return Mix_Playing(no);
		break;

	case YGS_SOUNDTYPE_MUS:
		return Mix_PlayingMusic();
		break;
	}
	return 0;
}

void LoadWave( const char* filename, int no )
{
	int		len = strlen(filename);
	if ( len < 4 ) { return; }

	if ( s_pYGSSound[no] != NULL )
	{
		Mix_FreeChunk(s_pYGSSound[no]);
		s_pYGSSound[no] = NULL;
	}

	if ( s_pYGSExMusic[no] != NULL )
	{
		Mix_FreeMusic(s_pYGSExMusic[no]);
		s_pYGSExMusic[no] = NULL;
	}

	s_iYGSSoundType[no] = YGS_SOUNDTYPE_NONE;

	// 拡張子、または番号(50番以降がBGM)によって読み込み方法を変える
	if ( strcasecmp(&filename[len - 4], ".wav") || no >= 50 )
	{
		s_pYGSExMusic[no] = Mix_LoadMUS(filename);
		s_iYGSSoundType[no] = YGS_SOUNDTYPE_MUS;
		s_iYGSSoundVolume[no] = 127;
	}
	else
	{
		s_pYGSSound[no] = Mix_LoadWAV(filename);
		s_iYGSSoundType[no] = YGS_SOUNDTYPE_WAV;
		s_iYGSSoundVolume[no] = 127;
	}
}

void SetLoopModeWave( int no, int mode )
{

}

void LoadMIDI( const char* filename )
{
	if ( s_pYGSMusic != NULL )
	{
		Mix_FreeMusic(s_pYGSMusic);
		s_pYGSMusic = NULL;
	}

	s_pYGSMusic = Mix_LoadMUS(filename);
}

void LoadBitmap( const char* filename, int plane, int val )
{
	if ( s_pYGSTexture[plane] )
	{
		SDL_DestroyTexture(s_pYGSTexture[plane]);
		s_pYGSTexture[plane] = NULL;
	}

	SDL_Surface		*surface	= IMG_Load(filename);
	if ( surface != NULL )
	{
		s_pYGSTexture[plane] = SDL_CreateTextureFromSurface(s_pScreenRenderer, surface);
		SDL_SetTextureBlendMode(s_pYGSTexture[plane], SDL_BLENDMODE_BLEND);
		SDL_FreeSurface(surface);
	}
}

void PlayMIDI()
{
	Mix_PlayMusic(s_pYGSMusic, -1);
}

void StopMIDI()
{
	Mix_HaltMusic();
}

void SetColorKeyPos(int plane, int x, int y)
{

}

void EnableBlendColorKey(int plane, int key)
{

}

void CreateSurface(int surf, int w, int h)
{

}

void ClearSecondary()
{

}

void SetFillColor(int col)
{

}

void LoadFile( const char* filename, void* buf, int size )
{
	FILE	*file;
	file = fopen(filename, "rb");
	if ( file )
	{
		fread(buf, 1, size, file);
		fclose(file);

		int32_t		i, *buf2;

		/* エンディアン変換 */
		buf2 = (int32_t*)buf;
		for ( i = 0 ; i < size / sizeof(int32_t) ; i ++ )
		{
			buf2[i] = SWAP32(buf2[i]);
		}
	}
}

void SaveFile( const char* filename, void* buf, int size )
{
	FILE	*file;
	int32_t		i, *buf2;

	/* エンディアン変換 */
	buf2 = (int32_t*)buf;
	for ( i = 0 ; i < size / sizeof(int32_t) ; i ++ )
	{
		buf2[i] = SWAP32(buf2[i]);
	}

	file = fopen(filename, "wb");
	if ( file )
	{
		fwrite(buf, 1, size, file);
		fclose(file);
	}

	/* もどす */
	for ( i = 0 ; i < size / sizeof(int32_t) ; i ++ )
	{
		buf2[i] = SWAP32(buf2[i]);
	}
}

void TextLayerOn ( int layer, int x, int y )
{
	s_TextLayer[layer].enable = true;
	s_TextLayer[layer].x = x;
	s_TextLayer[layer].y = y;
}

void TextMove ( int layer, int x, int y )
{
	s_TextLayer[layer].x = x;
	s_TextLayer[layer].y = y;
}

void TextColor ( int layer, int r, int g, int b )
{
	s_TextLayer[layer].r = r;
	s_TextLayer[layer].g = g;
	s_TextLayer[layer].b = b;
}

void TextBackColorDisable ( int layer )
{

}

void TextSize ( int layer, int size )
{
	s_TextLayer[layer].size = size;
}

void TextHeight ( int layer, int height )
{

}

void TextOut ( int layer, const char* text )
{
	strcpy(s_TextLayer[layer].string, text);
}

void TextBlt ( int layer )
{
	if ( s_TextLayer[layer].enable )
	{
		YGS2kTextOut(s_TextLayer[layer].x + s_iOffsetX, s_TextLayer[layer].y + s_iOffsetY, s_TextLayer[layer].string, s_TextLayer[layer].r, s_TextLayer[layer].g, s_TextLayer[layer].b, s_TextLayer[layer].size);
	}
}

void TextLayerOff ( int layer )
{
	s_TextLayer[layer].enable = false;
}

void Blt(int pno, int dx, int dy)
{
	if ( s_pYGSTexture[pno] == NULL ) { return; }
	int w, h;
	SDL_QueryTexture(s_pYGSTexture[pno], NULL, NULL, &w, &h);
	BltRect(pno, dx, dy, 0, 0, w, h);
}

void BltRect(int pno, int dx, int dy, int sx, int sy, int hx, int hy)
{
	if ( s_pYGSTexture[pno] == NULL ) return;

	SDL_Rect	src;
	SDL_Rect	dst;

	src.x = sx;					src.y = sy;
	src.w = hx;					src.h = hy;
	dst.x = dx + s_iOffsetX;	dst.y = dy + s_iOffsetY;
	dst.w = hx;					dst.h = hy;

	SDL_RenderCopy(s_pScreenRenderer, s_pYGSTexture[pno], &src, &dst);
}

void BltFast(int pno, int dx, int dy)
{
	Blt(pno, dx, dy);
}

void BltFastRect(int pno, int dx, int dy, int sx, int sy, int hx, int hy)
{
	BltRect(pno, dx, dy, sx, sy, hx, hy);
}

void BlendBlt(int pno, int dx, int dy, int ar, int ag, int ab, int br, int bg, int bb)
{
	if ( s_pYGSTexture[pno] == NULL ) return;

	SDL_SetTextureAlphaMod(s_pYGSTexture[pno], ar);
	Blt(pno, dx, dy);
	SDL_SetTextureAlphaMod(s_pYGSTexture[pno], SDL_ALPHA_OPAQUE);
}

void BlendBltRect(int pno, int dx, int dy, int sx, int sy, int hx, int hy, int ar, int ag, int ab, int br, int bg, int bb)
{
	if ( s_pYGSTexture[pno] == NULL ) return;

	SDL_SetTextureAlphaMod(s_pYGSTexture[pno], ar);
	BltRect(pno, dx, dy, sx, sy, hx, hy);
	SDL_SetTextureAlphaMod(s_pYGSTexture[pno], SDL_ALPHA_OPAQUE);
}

void BltR(int pno, int dx, int dy, int scx, int scy)
{
	BltRectR(pno, dx, dy, 0, 0, s_iWinWidth, s_iWinHeight, scx, scy);
}

void BltRectR(int pno, int dx, int dy, int sx, int sy, int hx, int hy, int scx, int scy)
{
	if ( s_pYGSTexture[pno] == NULL ) return;

	// ちゃんと拡大して描画する
	SDL_Rect	src;
	SDL_Rect	dst;

	src.x = sx;					src.y = sy;
	src.w = hx;					src.h = hy;
	dst.x = dx + s_iOffsetX;	dst.y = dy + s_iOffsetY;
	dst.w = hx * ((float)scx / 65536.0f);
	dst.h = hy * ((float)scy / 65536.0f);

	if ( src.w == 0 || src.h == 0 || dst.w == 0 || dst.h == 0 ) { return; }

	SDL_RenderCopy( s_pScreenRenderer, s_pYGSTexture[pno], &src, &dst );
}

void BltFastR(int pno, int dx, int dy, int scx, int scy)
{
	BltR(pno, dx, dy, scx, scy);
}

void BltFastRectR(int pno, int dx, int dy, int sx, int sy, int hx, int hy, int scx, int scy)
{
	BltRectR(pno, dx, dy, sx, sy, hx, hy, scx, scy);
}

void BltTrans(int pno, int dx, int dy)
{

}

void BlendBltR(int pno, int dx, int dy, int ar, int ag, int ab, int br, int bg, int bb, int scx, int scy)
{
	BlendBltRectR(pno, dx, dy, 0, 0, s_iWinWidth, s_iWinHeight, ar, ag, ab, br, bg, bb, scx, scy);
}

void BlendBltRectR(int pno, int dx, int dy, int sx, int sy, int hx, int hy, int ar, int ag, int ab, int br, int bg, int bb, int scx, int scy)
{
	if ( s_pYGSTexture[pno] == NULL ) return;

	SDL_SetTextureAlphaMod(s_pYGSTexture[pno], ar);

	// ちゃんと拡大して描画する
	SDL_Rect	src;
	SDL_Rect	dst;

	src.x = sx;					src.y = sy;
	src.w = hx;					src.h = hy;
	dst.x = dx + s_iOffsetX;	dst.y = dy + s_iOffsetY;
	dst.w = hx * ((float)scx / 65536.0f);
	dst.h = hy * ((float)scy / 65536.0f);

	if ( src.w == 0 || src.h == 0 || dst.w == 0 || dst.h == 0 ) { return; }

	SDL_RenderCopy( s_pScreenRenderer, s_pYGSTexture[pno], &src, &dst );

	SDL_SetTextureAlphaMod(s_pYGSTexture[pno], SDL_ALPHA_OPAQUE);
}

void SetSecondaryOffset(int x, int y)
{
	s_iNewOffsetX = x;
	s_iNewOffsetY = y;
}

void SetColorKeyRGB(int pno, int r, int g, int b)
{

}

void SwapToSecondary(int pno)
{

}

void SetFPS(int fps)
{
	s_uNowFPS = fps;
}

int GetFPS()
{
	return s_uNowFPS;
}

int GetRealFPS()
{
	return s_uFPS;
}

void StrCpy(char *dest, const char *src)
{
	strcpy(dest, src);
}

void StrCpy(void *dest, const char *src)
{
	strcpy((char*)dest, src);
}

void StrCpy(char *dest, const void *src)
{
	strcpy(dest, (const char*)src);
}

void StrCat(char *str1, const char *str2)
{
	strcat(str1, str2);
}

int StrLen(const char *stri)
{
	return strlen(stri);
}

void MidStr(const char *src, int start, int len, char *dest)
{
	int		i;
	for ( i = 0 ; i < len ; i ++ )
	{
		dest[i] = src[start - 1 + i];
	}
	dest[len] = '\0';
}

void LeftStr(const char *src, int len, char *dest)
{
	MidStr(src, 1, len, dest);
}

char CharAt(const char *stri, int pos)
{
	return stri[pos];
}

int ValLong(const char *stri)
{
	return atoi(stri);
}

void FillMemory(void* buf, int size, int val)
{
	memset(buf, val, size);
}

////////////////////////////////////////////////////

void YGS2kKanjiFontInitialize()
{
	/* 10pxフォント読み込み */
	s_pKanjiFont[0] = Kanji_OpenFont("res/font/knj10.bdf", 10);
	if ( s_pKanjiFont[0] )
	{
		Kanji_AddFont(s_pKanjiFont[0], "res/font/5x10a.bdf");
	}
	else
	{
		/* フォントがない場合代替を使う */
		s_pKanjiFont[0] = Kanji_OpenFont("res/font/knj12.bdf", 10);
		Kanji_AddFont(s_pKanjiFont[0], "res/font/6x12a.bdf");
	}

	if ( s_pKanjiFont[0] )
	{
		Kanji_SetCodingSystem(s_pKanjiFont[0], KANJI_SJIS);
	}

	/* 12pxフォント読み込み */
	s_pKanjiFont[1] = Kanji_OpenFont("res/font/knj12.bdf", 12);
	if ( s_pKanjiFont[1] )
	{
		Kanji_AddFont(s_pKanjiFont[1], "res/font/6x12a.bdf");
		Kanji_SetCodingSystem(s_pKanjiFont[1], KANJI_SJIS);
	}

	/* 16pxフォント読み込み */
	s_pKanjiFont[2] = Kanji_OpenFont("res/font/knj16.bdf", 16);
	if ( s_pKanjiFont[2] )
	{
		Kanji_AddFont(s_pKanjiFont[2], "res/font/8x16a.bdf");
		Kanji_SetCodingSystem(s_pKanjiFont[2], KANJI_SJIS);
	}
}

void YGS2kKanjiFontFinalize()
{
	if ( s_pKanjiFont[0] ) { Kanji_CloseFont(s_pKanjiFont[0]); }
	if ( s_pKanjiFont[1] ) { Kanji_CloseFont(s_pKanjiFont[1]); }
	if ( s_pKanjiFont[2] ) { Kanji_CloseFont(s_pKanjiFont[2]); }
}

void YGS2kKanjiDraw(int x, int y, int r, int g, int b, int size, const char *str)
{
	SDL_Color col;
	int		font = 0;

	if ( size >= 12 )
	{
		font ++;
	}
	if ( size >= 16 )
	{
		font ++;
	}

	col.r = r;
	col.g = g;
	col.b = b;

	if ( s_pKanjiFont[font] )
	{
		Kanji_PutTextRenderer(s_pKanjiFont[font], x, y, s_pScreenRenderer, str, col);
	}
}
