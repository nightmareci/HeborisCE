#ifndef APP_h_
#define APP_h_

#include "APP_common.h"

typedef enum APP_Button
{
	APP_BUTTON_NULL = -1,
	APP_BUTTON_UP = 0,
	APP_BUTTON_DOWN,
	APP_BUTTON_LEFT,
	APP_BUTTON_RIGHT,
	APP_BUTTON_A,
	APP_BUTTON_B,
	APP_BUTTON_C,
	APP_BUTTON_D,
	APP_BUTTON_GAME_COUNT,
	APP_BUTTON_GIVE_UP = APP_BUTTON_GAME_COUNT,
	APP_BUTTON_PAUSE,
	APP_BUTTON_COUNT
} APP_Button;

#define APP_BUTTON_IS_DIR(button) ((button) >= APP_BUTTON_UP && (button) <= APP_BUTTON_RIGHT)

typedef enum APP_Prompt
{
	APP_PROMPT_OK,
	APP_PROMPT_CANCEL,
	APP_PROMPT_RETRY
} APP_Prompt;

#define		APP_DEADZONE_MAX	(32767 / 4)

void APP_Init(int soundBufferSize);
void APP_Quit(void);
void APP_Exit(int exitStatus);
bool APP_Update(void);

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

typedef enum APP_WaveFormat
{
	APP_WAVE_MASK   = 0xFF,

	// Flag BGM type as "simple", only one bgm.ext for everything.
	APP_WAVE_SIMPLE = 0x80,

	APP_WAVE_FORMAT = 0x7F,
	APP_WAVE_MID    =  1,
	APP_WAVE_WAV    =  2,
	APP_WAVE_OGG    =  3,
	APP_WAVE_MP3    =  4,
	APP_WAVE_FLAC   =  5,
	APP_WAVE_OPUS   =  6,
	APP_WAVE_MOD    =  7,
	APP_WAVE_IT     =  8,
	APP_WAVE_XM     =  9,
	APP_WAVE_S3M    = 10,
	APP_WAVE_MAXFORMAT
} APP_WaveFormat;

bool APP_SetScreen(APP_ScreenModeFlag *screenMode, int32_t *screenIndex);
int APP_GetMaxDisplayIndex();
int APP_GetMaxDisplayMode( int displayIndex );
bool APP_RenderLevelLowSupported();

int APP_Rand ( int max );

void APP_PlayWave ( int num );
void APP_ReplayWave ( int num );
void APP_StopWave ( int num );
void APP_PauseWave ( int num );
void APP_SetVolumeWave( int num, int vol );
int APP_IsPlayWave( int num );
void APP_LoadWave( const char* filename, int num );
void APP_SetLoopModeWave( int num, int mode );

int APP_IsPlayMusic();
void APP_PauseMusic();
void APP_ReplayMusic();
void APP_LoadMusic( const char* filename );
void APP_PlayMusic();
void APP_StopMusic();
void APP_SetVolumeMusic(int vol);
bool APP_WaveFormatSupported(APP_WaveFormat format);

void APP_LoadBitmap( const char* filename, int plane, int value );
void APP_SetColorKeyPos(int plane, int x, int y);
void APP_EnableBlendColorKey(int plane, int key);
void APP_CreateSurface(int surf, int w, int h);
void APP_ClearSecondary();
void APP_SetFillColor(int col);
void APP_LoadFile( const char* filename, void* buf, size_t size );
void APP_ReadFile( const char* filename, void* buf, size_t size, size_t offset );
void APP_SaveFile( const char* filename, void* buf, size_t size );
void APP_AppendFile( const char* filename, void* buf, size_t size );
void APP_TextLayerOn ( int layer, int x, int y );
void APP_TextMove ( int layer, int x, int y );
void APP_TextColor ( int layer, int r, int g, int b );
void APP_TextBackColorDisable ( int layer );
void APP_TextSize ( int layer, int size );
void APP_TextHeight ( int layer, int height );
void APP_TextOut ( int layer, const char* text );
void APP_TextBlt ( int layer );
void APP_TextLayerOff ( int layer );
void APP_BltAlways(bool always);
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

void APP_ResetFrameStep();
void APP_SetFPS(unsigned fps);
int APP_GetFPS();
int APP_GetRealFPS();

void APP_StrCpy(char *dest, const char *src);
void APP_StrCat(char *str1, const char *str2);
int APP_StrLen(const char *stri);
void APP_MidStr(const char *src, int start, int len, char *dest);
void APP_LeftStr(const char *src, int len, char *dest);
char APP_CharAt(const char *stri, int pos);
int APP_ValLong(const char *stri);
void APP_FillMemory(void* buf, int size, int val);

typedef enum APP_InputType
{
	// NULL must be zero.
	APP_INPUT_NULL = 0u
	#ifdef APP_ENABLE_JOYSTICK
	,APP_INPUT_JOYSTICK
	#endif
	#ifdef APP_ENABLE_GAME_CONTROLLER
	,APP_INPUT_GAMECONTROLLER
	,APP_INPUT_FIRSTGAMECONTROLLERTYPE
	,APP_INPUT_XBOX = APP_INPUT_FIRSTGAMECONTROLLERTYPE
	,APP_INPUT_PLAYSTATION
	,APP_INPUT_NINTENDO
	#endif
	#ifdef APP_ENABLE_KEYBOARD
	,APP_INPUT_KEYBOARD
	#endif
	#ifdef APP_ENABLE_LINUX_GPIO
	,APP_INPUT_LINUXGPIO
	#endif
} APP_InputType;

APP_InputType APP_GetLastInputType();

void APP_InputsUpdate();

#define APP_INPUTREPEAT_MAX INT_MAX

#ifdef APP_ENABLE_LINUX_GPIO
bool APP_IsPushGPIO(int key);
bool APP_IsPressGPIO(int key);
int APP_GetGPIORepeat(int key);
#endif

#ifdef APP_ENABLE_KEYBOARD
#define APP_KEY_MAX SDL_NUM_SCANCODES

bool APP_IsPushKey(int key);
bool APP_IsPressKey(int key);
int APP_GetKeyRepeat(int key);
int APP_GetMaxKey();
#endif

#if defined(APP_ENABLE_JOYSTICK) || defined(APP_ENABLE_GAME_CONTROLLER)
typedef enum APP_PlayerSlotType
{
	APP_PLAYERSLOT_NULL,
	APP_PLAYERSLOT_JOY,
	APP_PLAYERSLOT_CON
} APP_PlayerSlotType;

int APP_GetNumPlayerSlots();
APP_PlayerSlotType APP_GetPlayerSlotType(int player);
#endif

#ifdef APP_ENABLE_JOYSTICK
typedef struct APP_JoyGUID {
	int32_t data[4];
} APP_JoyGUID;

typedef enum APP_JoyKeyType {
	APP_JOYKEY_ANY,
	APP_JOYKEY_AXIS,
	APP_JOYKEY_HAT,
	APP_JOYKEY_BUTTON
} APP_JoyKeyType;

typedef union APP_JoyKeySetting {
	struct {
		int index;
		int value;
	};
	int button;
} APP_JoyKeySetting;

typedef struct APP_JoyKey {
	int player;
	APP_JoyGUID guid;
	APP_JoyKeyType type;
	APP_JoyKeySetting setting;
} APP_JoyKey;

bool APP_IsPushJoyKey(const APP_JoyKey* const key);
bool APP_IsPressJoyKey(const APP_JoyKey* const key);
int APP_GetJoyKeyRepeat(const APP_JoyKey* const key); // If key->type == APP_JOYKEY_ANY, checks all inputs of the key's joystick and returns the max repeat value.

// If game controller support is enabled, a joystick player can possibly
// correspond to a game controller, in which case these functions return -1 if
// the joystick player refers to a game controller. Also, APP_GetJoyGUID
// returns the zero GUID for a joystick player that's actually a game
// controller.

int APP_GetNumJoys();
APP_JoyGUID APP_GetJoyGUID(int player);
int APP_GetMaxJoyAxis(int player);
int APP_GetMaxJoyHat(int player);
int APP_GetMaxJoyButton(int player);
#endif

#ifdef APP_ENABLE_GAME_CONTROLLER
typedef enum APP_ConKeyType {
	APP_CONKEY_ANY,
	APP_CONKEY_AXIS,
	APP_CONKEY_BUTTON // The dpad is considered to be four buttons.
} APP_ConKeyType;

typedef struct APP_ConKey {
	APP_ConKeyType type;
	int index;
} APP_ConKey;

/* Two for each stick, one for each trigger:
0,1: Left stick right,left
2,3: Left stick down,up
4,5: Right stick right,left
6,7: Right stick down,up
8: Left trigger (LT, L2, ZL)
9: Right trigger (RT, R2, ZR)
*/
#define APP_CONAXIS_MAX 10

// Button numbers are the same as the SDL_CONTROLLER_BUTTON_* constants.
#define APP_CONBUTTON_MAX SDL_CONTROLLER_BUTTON_MAX

bool APP_IsPushConKey(const int player, const APP_ConKey* const key);
bool APP_IsPressConKey(const int player, const APP_ConKey* const key);
int APP_GetConKeyRepeat(const int player, const APP_ConKey* const key);

int APP_GetNumCons();
void APP_ResetLastActiveCon();
int APP_GetLastActiveCon();
APP_InputType APP_GetConType(const int player);
bool APP_GetConKeyDesc(const int player, const APP_ConKey* const key, const char** text, APP_Button* button);
#endif

#endif
