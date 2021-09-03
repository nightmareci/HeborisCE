#include "include.h"
#include "prototypes.h"
#include "gamedef.h"
#include "SDL_kanji.h"
#include "ygs2kfunc.h"

#include <iostream>
#include <cstdint>

#define		YGS_TEXTURE_MAX		100
#define		YGS_SOUND_MAX		100
#define		YGS_MUSIC_MAX		100
#define		YGS_KEY_MAX		10
#define		YGS_KEYREPEAT_MAX	SDL_NUM_SCANCODES
#define		YGS_TEXTLAYER_MAX	16

#define		GAME_CAPTION		"HEBORIS C7-EX SDL2"

struct STextLayer
{
	bool	enable;
	int	x;
	int	y;
	int	r, g, b;
	int	size;
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

static int			s_iKeyRepeat[YGS_KEYREPEAT_MAX];
static int			s_iJoyPadMax;
static SDL_Joystick		**s_pJoyPads;
static int			*s_pJoyAxisMax;
static int			*s_pJoyHatMax;
static int			*s_pJoyButtonMax;
static int			**s_pJoyAxisRepeat;
static int			**s_pJoyHatRepeat;
static int			**s_pJoyButtonRepeat;
static int			s_iYGSSoundType[YGS_SOUND_MAX];
static Mix_Chunk		*s_pYGSSound[YGS_SOUND_MAX];
static int			s_iYGSSoundVolume[YGS_SOUND_MAX];
static Mix_Music		*s_pYGSExMusic[YGS_SOUND_MAX];
static Mix_Music		*s_pYGSMusic;

#define		YGS_KANJIFONTFILE_MAX	3
static Kanji_Font		*s_pKanjiFont[YGS_KANJIFONTFILE_MAX];

static int			s_iLogicalWidth;
static int			s_iLogicalHeight;

static Uint64			s_uTimeCount;
static Uint64			s_uTimeAccumulatorCount;
static Uint64			s_uFPSCount;
static unsigned int		s_uNowFrame;
static unsigned int		s_uFPSCnting;
static unsigned int		s_uFPS;
static unsigned int		s_uNowFPS = 60;

static STextLayer		s_TextLayer[YGS_TEXTLAYER_MAX];
extern int32_t			screenMode;
extern int32_t			displayIndex;

static int			s_iNewOffsetX = 0, s_iNewOffsetY = 0;
static int			s_iOffsetX = 0, s_iOffsetY = 0;

bool YGS2kInit()
{
	int		logicalWidth, logicalHeight;

	s_iNewOffsetX = 0;	s_iNewOffsetY = 0;
	s_iOffsetX = 0;		s_iOffsetY = 0;

	/* CONFIG.SAVより設定をロード */
	if ( LoadConfig() )
	{
		readdef::readdef();
		LoadConfig();
	}

	Uint32		windowFlags = SDL_WINDOW_HIDDEN;
	int		windowX, windowY;

	/* 画面の設定 */
	int32_t windowType = screenMode & SCREEN_WINDOWTYPE_MASK;
	if (
			displayIndex >= SDL_GetNumVideoDisplays() ||
			(windowType == SCREEN_FULLSCREEN && SCREEN_GETDISPLAYMODE(screenMode) >= SDL_GetNumDisplayModes(displayIndex))
	) {
		screenMode = SCREEN_WINDOW | SCREEN_DETAIL_MASK;
		displayIndex = 0;
		windowFlags |= SDL_WINDOW_RESIZABLE;
		windowX = SDL_WINDOWPOS_CENTERED_DISPLAY(displayIndex);
		windowY = SDL_WINDOWPOS_CENTERED_DISPLAY(displayIndex);
	}
	else
	{
		windowX = SDL_WINDOWPOS_CENTERED_DISPLAY(displayIndex);
		windowY = SDL_WINDOWPOS_CENTERED_DISPLAY(displayIndex);
		switch ( windowType )
		{
		case SCREEN_WINDOW_MAXIMIZED:
			windowFlags |= SDL_WINDOW_MAXIMIZED;
		case SCREEN_WINDOW:
			windowFlags |= SDL_WINDOW_RESIZABLE;
			break;

		case SCREEN_FULLSCREEN_DESKTOP:
			windowFlags |= SDL_WINDOW_FULLSCREEN_DESKTOP;
			break;

		case SCREEN_FULLSCREEN: {
			windowFlags |= SDL_WINDOW_FULLSCREEN;
			break;
		}
		}
	}

	if ( screenMode & SCREEN_DETAIL_MASK )
	{
		logicalWidth  = 640;
		logicalHeight = 480;
	}
	else
	{
		logicalWidth  = 320;
		logicalHeight = 240;
	}

	s_iLogicalWidth  = logicalWidth;
	s_iLogicalHeight = logicalHeight;


	/* ウィンドウの作成 */
	if ( windowType == SCREEN_FULLSCREEN )
	{
		SDL_DisplayMode displayMode;
		if ( SDL_GetDisplayMode(displayIndex, SCREEN_GETDISPLAYMODE(screenMode), &displayMode) < 0 ) {
			screenMode &= SCREEN_DETAIL_MASK;
			screenMode |= SCREEN_WINDOW;
			displayIndex = 0;
			SaveConfig();
			s_pScreenWindow = SDL_CreateWindow(GAME_CAPTION, SDL_WINDOWPOS_CENTERED_DISPLAY(displayIndex), SDL_WINDOWPOS_CENTERED_DISPLAY(displayIndex), logicalWidth, logicalHeight, SDL_WINDOW_HIDDEN | SDL_WINDOW_RESIZABLE);
		}
		else {
			s_pScreenWindow = SDL_CreateWindow(GAME_CAPTION, windowX, windowY, displayMode.w, displayMode.h, windowFlags);
			SDL_SetWindowDisplayMode(s_pScreenWindow, &displayMode);
		}
	}
	else if ( windowType == SCREEN_WINDOW )
	{
		SDL_DisplayMode displayMode;
		SDL_GetDesktopDisplayMode(displayIndex, &displayMode);
		int maxDisplayMode;
		if(displayMode.w <= logicalWidth || displayMode.h <= logicalHeight)
		{
			maxDisplayMode = 1;
		}
		else if(displayMode.w > displayMode.h)
		{
			maxDisplayMode = (displayMode.h / logicalHeight) - (displayMode.h % logicalHeight == 0);
		}
		else
		{
			maxDisplayMode = (displayMode.w / logicalWidth) - (displayMode.w % logicalWidth == 0);
		}
		int maxWidth = maxDisplayMode * logicalWidth;
		int maxHeight = maxDisplayMode * logicalHeight;
		int displayModeIndex = SCREEN_GETDISPLAYMODE(screenMode) + 1;
		int windowW = displayModeIndex * logicalWidth;
		int windowH = displayModeIndex * logicalHeight;
		if ( displayModeIndex > maxDisplayMode) {
			windowW = logicalWidth;
			windowH = logicalHeight;
			screenMode &= SCREEN_NOTDISPLAYMODE_MASK;
			SaveConfig();
		}
		s_pScreenWindow = SDL_CreateWindow(GAME_CAPTION, windowX, windowY, windowW, windowH, windowFlags);
	}
	else
	{
		s_pScreenWindow = SDL_CreateWindow(GAME_CAPTION, windowX, windowY, logicalWidth, logicalHeight, windowFlags);
	}
	s_pScreenRenderer = SDL_CreateRenderer(s_pScreenWindow, -1, screenMode & SCREEN_VSYNC_MASK ? SDL_RENDERER_PRESENTVSYNC : 0);
	SDL_RenderSetLogicalSize(s_pScreenRenderer, logicalWidth, logicalHeight);
	SDL_ShowWindow(s_pScreenWindow);
	SDL_RenderClear(s_pScreenRenderer);

	/* マウスカーソルを消す場合は */
	SDL_ShowCursor( !(windowFlags & SDL_WINDOW_FULLSCREEN) );

	/* キーリピートバッファ初期化 */
	for ( int i = 0 ; i < YGS_KEYREPEAT_MAX ; i ++ )
	{
		s_iKeyRepeat[i] = 0;
	}

	/* パッドの初期化 */
	s_iJoyPadMax = SDL_NumJoysticks();
	s_pJoyPads = (SDL_Joystick**)malloc(sizeof(SDL_Joystick*) * s_iJoyPadMax);

	s_pJoyAxisMax = (int*)malloc(sizeof(int) * s_iJoyPadMax);
	s_pJoyHatMax = (int*)malloc(sizeof(int) * s_iJoyPadMax);
	s_pJoyButtonMax = (int*)malloc(sizeof(int) * s_iJoyPadMax);

	s_pJoyAxisRepeat = (int**)malloc(sizeof(int*) * s_iJoyPadMax);
	s_pJoyHatRepeat = (int**)malloc(sizeof(int*) * s_iJoyPadMax);
	s_pJoyButtonRepeat = (int**)malloc(sizeof(int*) * s_iJoyPadMax);
	for (int i = 0; i < s_iJoyPadMax; i++) {
		s_pJoyPads[i] = SDL_JoystickOpen(i);

		s_pJoyAxisMax[i] = SDL_JoystickNumAxes(s_pJoyPads[i]);
		s_pJoyHatMax[i] = SDL_JoystickNumHats(s_pJoyPads[i]);
		s_pJoyButtonMax[i] = SDL_JoystickNumButtons(s_pJoyPads[i]);

		s_pJoyAxisRepeat[i] = (int*)calloc(s_pJoyAxisMax[i] * 2, sizeof(int));
		s_pJoyHatRepeat[i] = (int*)calloc(s_pJoyHatMax[i] * 4, sizeof(int));
		s_pJoyButtonRepeat[i] = (int*)calloc(s_pJoyButtonMax[i], sizeof(int));
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

	/* テキストレイヤーの初期化 */
	for ( int i = 0 ; i < YGS_TEXTLAYER_MAX ; i ++ )
	{
		memset(&s_TextLayer[i], 0, sizeof(STextLayer));
		s_TextLayer[i].r = s_TextLayer[i].g = s_TextLayer[i].b = 255;
		s_TextLayer[i].size = 16;
	}
	YGS2kKanjiFontInitialize();

	s_uTimeCount = SDL_GetPerformanceCounter();
	s_uTimeAccumulatorCount = 0;
	s_uFPSCount  = SDL_GetPerformanceCounter();
	s_uNowFrame  = 0;
	s_uFPSCnting = 0;
	s_uFPS       = 0;

	srand(time(NULL));

	return true;
}

void YGS2kExit()
{
	/* パッドのクローズ */
	for (int i = 0; i < s_iJoyPadMax; i++) {
		SDL_JoystickClose(s_pJoyPads[i]);
		free(s_pJoyAxisRepeat[i]);
		free(s_pJoyHatRepeat[i]);
		free(s_pJoyButtonRepeat[i]);
	}
	free(s_pJoyPads);
	s_pJoyPads = NULL;
	free(s_pJoyAxisMax);
	free(s_pJoyHatMax);
	free(s_pJoyButtonMax);
	s_pJoyAxisMax = NULL;
	s_pJoyHatMax = NULL;
	s_pJoyButtonMax = NULL;
	free(s_pJoyAxisRepeat);
	free(s_pJoyHatRepeat);
	free(s_pJoyButtonRepeat);
	s_pJoyAxisRepeat = NULL;
	s_pJoyHatRepeat = NULL;
	s_pJoyButtonRepeat = NULL;
	
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
	const Uint64 frameTimeCount = SDL_GetPerformanceFrequency() / s_uNowFPS;
	bool renderClear = false;

	if ( s_uTimeAccumulatorCount < frameTimeCount )
	{
		/* テキストレイヤーの描画 */
		for ( int i = 0 ; i < YGS_TEXTLAYER_MAX ; i ++ )
		{
			TextBlt(i);
		}

		/* バックサーフェスをフロントに転送 */
		SDL_RenderPresent( s_pScreenRenderer );

		/* フレームレート待ち */
		while ( s_uTimeCount + frameTimeCount >= SDL_GetPerformanceCounter() )
		{
			SDL_Delay( 1 );
		}

		s_uTimeAccumulatorCount += SDL_GetPerformanceCounter() - s_uTimeCount;

		renderClear = true;
	}
	s_uTimeAccumulatorCount -= frameTimeCount;

	/* フレームレート計算 */
	s_uFPSCnting ++;
	s_uNowFrame ++;

	if ( s_uFPSCount + SDL_GetPerformanceFrequency() <= SDL_GetPerformanceCounter() )
	{
		s_uFPS = s_uFPSCnting;
		s_uFPSCnting = 0;
		s_uFPSCount = SDL_GetPerformanceCounter();
	}

	s_uTimeCount = SDL_GetPerformanceCounter();

	if ( renderClear )
	{
		/* 画面塗りつぶし */
		SDL_RenderClear( s_pScreenRenderer );
	}

	/* イベント処理 */
	while(SDL_PollEvent(&ev))
	{
		switch(ev.type){
			case SDL_QUIT:						// ウィンドウの×ボタンが押された時など
				return false;
				break;
			case SDL_KEYDOWN:					// キーボードからの入力があった時
				{
					SDL_Keycode	key = ev.key.keysym.sym; // どのキーが押されたかを取得
					if ( key == SDLK_ESCAPE )
					{
						return false;
					}
				}
				break;
			default:
				break;
		}
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

int IsPushKey ( int key )
{
	return s_iKeyRepeat[key] == 1 ? 1 : 0;
}

int IsPressKey ( int key )
{
	return s_iKeyRepeat[key] != 0 ? 1 : 0;
}

int IsPushJoyKey ( const JoyKey* const key )
{
	JoyPadGUID guid = GetJoyPadGUID(key->device);
	JoyPadGUID zeroGUID = { 0 };
	if (memcmp(&guid, &zeroGUID, sizeof(JoyPadGUID)) == 0 || memcmp(&key->guid, &guid, sizeof(JoyPadGUID)) != 0) return 0;
	switch(key->type)
	{
	case JOYKEY_AXIS:
		if(key->setting.value == -YGS_DEADZONE_MAX)
		{
			return s_pJoyAxisRepeat[key->device][key->setting.index * 2 + 0] == 1 ? 1 : 0;
		}
		else if(key->setting.value == +YGS_DEADZONE_MAX)
		{
			return s_pJoyAxisRepeat[key->device][key->setting.index * 2 + 1] == 1 ? 1 : 0;
		}
		break;
	case JOYKEY_HAT:
		switch(key->setting.value)
		{
		case SDL_HAT_LEFT:
			return s_pJoyHatRepeat[key->device][key->setting.index * 4 + 0] == 1 ? 1 : 0;
		case SDL_HAT_RIGHT:
			return s_pJoyHatRepeat[key->device][key->setting.index * 4 + 1] == 1 ? 1 : 0;
		case SDL_HAT_UP:
			return s_pJoyHatRepeat[key->device][key->setting.index * 4 + 2] == 1 ? 1 : 0;
		case SDL_HAT_DOWN:
			return s_pJoyHatRepeat[key->device][key->setting.index * 4 + 3] == 1 ? 1 : 0;
		default:
			break;
		}
		break;
	case JOYKEY_BUTTON:
		return s_pJoyButtonRepeat[key->device][key->setting.button] == 1 ? 1 : 0;
	default:
		break;
	}
	return 0;
}

int IsPressJoyKey ( const JoyKey* const key )
{
	if (key == NULL || key->device < 0 || key->device >= s_iJoyPadMax) return 0;

	int pressed = 0;
	JoyPadGUID checkGUID = GetJoyPadGUID(key->device);
	if (memcmp(&key->guid, &checkGUID, sizeof(JoyPadGUID)) == 0) {
		SDL_Joystick* const joy = s_pJoyPads[key->device];
		if (SDL_JoystickGetAttached(joy)) {
			switch (key->type) {
			case JOYKEY_AXIS:
				pressed =
					key->setting.value != 0 && SDL_JoystickNumAxes(joy) > key->setting.index &&
					(
						(key->setting.value > 0 && SDL_JoystickGetAxis(joy, key->setting.index) > key->setting.value) ||
						(key->setting.value < 0 && SDL_JoystickGetAxis(joy, key->setting.index) < key->setting.value)
					);
				break;

			case JOYKEY_HAT:
				pressed =
					key->setting.value != SDL_HAT_CENTERED &&
					SDL_JoystickNumHats(joy) > key->setting.index &&
					SDL_JoystickGetHat(joy, key->setting.index) == key->setting.value;
				break;

			case JOYKEY_BUTTON:
				pressed = SDL_JoystickGetButton(joy, key->setting.button);
				break;
			}
		}
	}

	return pressed;
}

int IsPushReturnKey()
{
	return IsPushKey(SDL_GetScancodeFromKey(SDLK_RETURN));
}

int IsPushDeleteKey()
{
	return IsPushKey(SDL_GetScancodeFromKey(SDLK_DELETE));
}

int IsPushBSKey()
{
	return IsPushKey(SDL_GetScancodeFromKey(SDLK_BACKSPACE));
}

int IsPushEscKey()
{
	return IsPushKey(SDL_GetScancodeFromKey(SDLK_ESCAPE));
}

int IsPushEndKey()
{
	return IsPushKey(SDL_GetScancodeFromKey(SDLK_END));
}

JoyPadGUID GetJoyPadGUID( int device ) {
	JoyPadGUID zeroGUID = { 0 };
	if (device >= s_iJoyPadMax) return zeroGUID;
	JoyPadGUID joyPadGUID;
	SDL_JoystickGUID sdlGUID = SDL_JoystickGetGUID( s_pJoyPads[device] );
	for (int32_t i = 0; i < 4; i++) {
		joyPadGUID.data[i] = 0;
		for (int32_t j = 0; j < 4; j++) {
			joyPadGUID.data[i] |= (int32_t)((uint32_t)sdlGUID.data[i * 4 + j] << (j * 8));
		}
	}
	return joyPadGUID;
}

int GetMaxKey()
{
	return YGS_KEYREPEAT_MAX;
}

int GetMaxJoyPad()
{
	return s_iJoyPadMax;
}

int GetMaxJoyAxis( int device ) {
	return s_pJoyAxisMax[device];
}

int GetMaxJoyHat( int device ) {
	return s_pJoyHatMax[device];
}

int GetMaxJoyButton( int device )
{
	return s_pJoyButtonMax[device];
}

int GetMaxDisplayIndex()
{
	return SDL_GetNumVideoDisplays();
}

int GetMaxDisplayMode( int displayIndex )
{
	return SDL_GetNumDisplayModes(displayIndex);
}

void SetConstParam ( const char *param, int value )
{

}

void KeyInput()
{
	int		keynum = 0;
	const Uint8	*KeyInp = SDL_GetKeyboardState(&keynum);

	for ( int i = 0 ; i < YGS_KEYREPEAT_MAX ; i ++ )
	{
		if ( i < keynum && KeyInp[i] == SDL_PRESSED )
		{
			s_iKeyRepeat[i] ++;
		}
		else
		{
			s_iKeyRepeat[i] = 0;
		}
	}

	JoyKey key;
	for ( int i = 0 ; i < s_iJoyPadMax ; i ++ )
	{
		key.device = i;
		key.guid = GetJoyPadGUID(i);

		key.type = JOYKEY_AXIS;
		for ( int j = 0 ; j < s_pJoyAxisMax[i] ; j ++ )
		{
			key.setting.index = j;

			key.setting.value = -YGS_DEADZONE_MAX;
			if ( IsPressJoyKey(&key) )
			{
				s_pJoyAxisRepeat[i][j*2+0] ++;
			}
			else
			{
				s_pJoyAxisRepeat[i][j*2+0] = 0;
			}

			key.setting.value = +YGS_DEADZONE_MAX;
			if ( IsPressJoyKey(&key) )
			{
				s_pJoyAxisRepeat[i][j*2+1] ++;
			}
			else
			{
				s_pJoyAxisRepeat[i][j*2+1] = 0;
			}
		}

		key.type = JOYKEY_HAT;
		for ( int j = 0 ; j < s_pJoyHatMax[i] ; j ++ )
		{
			key.setting.index = j;

			key.setting.value = SDL_HAT_LEFT;
			if ( IsPressJoyKey(&key) )
			{
				s_pJoyHatRepeat[i][j*4+0] ++;
			}
			else
			{
				s_pJoyHatRepeat[i][j*4+0] = 0;
			}

			key.setting.value = SDL_HAT_RIGHT;
			if ( IsPressJoyKey(&key) )
			{
				s_pJoyHatRepeat[i][j*4+1] ++;
			}
			else
			{
				s_pJoyHatRepeat[i][j*4+1] = 0;
			}

			key.setting.value = SDL_HAT_UP;
			if ( IsPressJoyKey(&key) )
			{
				s_pJoyHatRepeat[i][j*4+2] ++;
			}
			else
			{
				s_pJoyHatRepeat[i][j*4+2] = 0;
			}

			key.setting.value = SDL_HAT_DOWN;
			if ( IsPressJoyKey(&key) )
			{
				s_pJoyHatRepeat[i][j*4+3] ++;
			}
			else
			{
				s_pJoyHatRepeat[i][j*4+3] = 0;
			}
		}

		key.type = JOYKEY_BUTTON;
		for ( int j = 0 ; j < s_pJoyButtonMax[i] ; j ++ )
		{
			key.setting.button = j;
			if ( IsPressJoyKey(&key) )
			{
				s_pJoyButtonRepeat[i][j] ++;
			}
			else
			{
				s_pJoyButtonRepeat[i][j] = 0;
			}
		}
	}
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
	BltRectR(pno, dx, dy, 0, 0, s_iLogicalWidth, s_iLogicalHeight, scx, scy);
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
	BlendBltRectR(pno, dx, dy, 0, 0, s_iLogicalWidth, s_iLogicalHeight, ar, ag, ab, br, bg, bb, scx, scy);
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
