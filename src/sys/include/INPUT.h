#ifndef INPUT_h_
#define INPUT_h_

#include <STD.h>

typedef enum INPUT_Button
{
	INPUT_BUTTON_NULL = -1,
	INPUT_BUTTON_UP = 0,
	INPUT_BUTTON_DOWN,
	INPUT_BUTTON_LEFT,
	INPUT_BUTTON_RIGHT,
	INPUT_BUTTON_A,
	INPUT_BUTTON_B,
	INPUT_BUTTON_C,
	INPUT_BUTTON_D,
	INPUT_BUTTON_GAME_COUNT,
	INPUT_BUTTON_GIVE_UP = INPUT_BUTTON_GAME_COUNT,
	INPUT_BUTTON_PAUSE,
	INPUT_BUTTON_COUNT
} INPUT_Button;

#define INPUT_BUTTON_IS_DIR(button) ((button) >= INPUT_BUTTON_UP && (button) <= INPUT_BUTTON_RIGHT)

typedef enum INPUT_Prompt
{
	INPUT_PROMPT_OK,
	INPUT_PROMPT_CANCEL,
	INPUT_PROMPT_RETRY
} INPUT_Prompt;

#define INPUT_DEADZONE_MIN (SDL_JOYSTICK_AXIS_MIN / 4)
#define INPUT_DEADZONE_MAX (SDL_JOYSTICK_AXIS_MAX / 4)

typedef enum INPUT_DeviceType
{
	// NULL must be zero.
	INPUT_DEVICE_TYPE_NULL = 0u
	#ifdef INPUT_ENABLE_JOYSTICK
	,INPUT_DEVICE_TYPE_JOYSTICK
	#endif
	#ifdef INPUT_ENABLE_GAME_CONTROLLER
	,INPUT_DEVICE_TYPE_FIRST_GAME_CONTROLLER_TYPE
	,INPUT_DEVICE_TYPE_XBOX = INPUT_DEVICE_TYPE_FIRST_GAME_CONTROLLER_TYPE
	,INPUT_DEVICE_TYPE_PLAYSTATION
	,INPUT_DEVICE_TYPE_NINTENDO
	// TODO: Rename INPUT_DEVICE_TYPE_NINTENDO => INPUT_DEVICE_TYPE_SNES and add INPUT_DEVICE_TYPE_GAMECUBE
	,INPUT_DEVICE_TYPE_LAST_GAME_CONTROLLER_TYPE = INPUT_DEVICE_TYPE_NINTENDO
	#endif
	#ifdef INPUT_ENABLE_KEYBOARD
	,INPUT_DEVICE_TYPE_KEYBOARD
	#endif
	#ifdef INPUT_ENABLE_LINUX_GPIO
	,INPUT_DEVICE_TYPE_LINUX_GPIO
	#endif
} INPUT_DeviceType;

void INPUT_Open(void);
void INPUT_Close(void);

INPUT_DeviceType INPUT_GetLastDeviceType();

void INPUT_Update(void);

#define INPUT_REPEAT_MAX INT_MAX

#ifdef INPUT_ENABLE_LINUX_GPIO
bool INPUT_IsPushGPIO(int key);
bool INPUT_IsPressGPIO(int key);
int INPUT_GetGPIORepeat(int key);
#endif

#ifdef INPUT_ENABLE_KEYBOARD
#define INPUT_KEY_MAX SDL_SCANCODE_COUNT

bool INPUT_IsPushKey(int key);
bool INPUT_IsPressKey(int key);
int INPUT_GetKeyRepeat(int key);
int INPUT_GetMaxKey(void);
#endif

#if defined(INPUT_ENABLE_JOYSTICK) || defined(INPUT_ENABLE_GAME_CONTROLLER)
typedef enum INPUT_PlayerSlotType
{
	INPUT_PLAYER_SLOT_NULL,
	INPUT_PLAYER_SLOT_JOY,
	INPUT_PLAYER_SLOT_CON
} INPUT_PlayerSlotType;

int INPUT_GetNumPlayerSlots(void);
INPUT_PlayerSlotType INPUT_GetPlayerSlotType(int player);
bool INPUT_UpdatePlayerSlots(void);
#endif

#ifdef INPUT_ENABLE_JOYSTICK
typedef struct INPUT_JoyGUID
{
	int32_t data[4];
} INPUT_JoyGUID;

typedef enum INPUT_JoyKeyType
{
	INPUT_JOY_KEY_ANY,
	INPUT_JOY_KEY_AXIS,
	INPUT_JOY_KEY_HAT,
	INPUT_JOY_KEY_BUTTON
} INPUT_JoyKeyType;

typedef union INPUT_JoyKeySetting
{
	struct {
		int index;
		int value;
	};
	int button;
} INPUT_JoyKeySetting;

typedef struct INPUT_JoyKey
{
	int player;
	INPUT_JoyGUID guid;
	INPUT_JoyKeyType type;
	INPUT_JoyKeySetting setting;
} INPUT_JoyKey;

bool INPUT_IsPushJoyKey(const INPUT_JoyKey* key);
bool INPUT_IsPressJoyKey(const INPUT_JoyKey* key);
int INPUT_GetJoyKeyRepeat(const INPUT_JoyKey* key); // If key->type == INPUT_JOY_KEY_ANY, checks all inputs of the key's joystick and returns the max repeat value.

// If game controller support is enabled, a joystick player can possibly
// correspond to a game controller, in which case these functions return -1 if
// the joystick player refers to a game controller. Also, INPUT_GetJoyGUID
// returns the zero GUID for a joystick player that's actually a game
// controller.

int INPUT_GetNumJoys(void);
INPUT_JoyGUID INPUT_GetJoyGUID(int player);
int INPUT_GetMaxJoyAxis(int player);
int INPUT_GetMaxJoyHat(int player);
int INPUT_GetMaxJoyButton(int player);
#endif

#ifdef INPUT_ENABLE_GAME_CONTROLLER
typedef enum INPUT_ConKeyType
{
	INPUT_CON_KEY_ANY,
	INPUT_CON_KEY_AXIS,
	INPUT_CON_KEY_BUTTON // The dpad is considered to be four buttons.
} INPUT_ConKeyType;

typedef struct INPUT_ConKey
{
	INPUT_ConKeyType type;
	int index;
} INPUT_ConKey;

/* Two for each stick, one for each trigger:
0,1: Left stick right,left
2,3: Left stick down,up
4,5: Right stick right,left
6,7: Right stick down,up
8: Left trigger (LT, L2, ZL)
9: Right trigger (RT, R2, ZR)
*/
#define INPUT_CON_AXIS_MAX 10

// Button numbers are the same as the SDL_GAMEPAD_BUTTON_* constants.
#define INPUT_CON_BUTTON_MAX SDL_GAMEPAD_BUTTON_COUNT

bool INPUT_IsPushConKey(int player, const INPUT_ConKey* key);
bool INPUT_IsPressConKey(int player, const INPUT_ConKey* key);
int INPUT_GetConKeyRepeat(int player, const INPUT_ConKey* key);

int INPUT_GetNumCons(void);
void INPUT_ResetLastActiveCon(void);
int INPUT_GetLastActiveCon(void);
INPUT_DeviceType INPUT_GetConType(const int player);
bool INPUT_GetConKeyDesc(int player, const INPUT_ConKey* key, const char** text, INPUT_Button* button);
#endif

#endif
