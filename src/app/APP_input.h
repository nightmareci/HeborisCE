#ifndef APP_input_h_
#define APP_input_h_

#include "APP_stdinc.h"

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

#define		APP_DEADZONE_MIN	(SDL_JOYSTICK_AXIS_MIN / 4)
#define		APP_DEADZONE_MAX	(SDL_JOYSTICK_AXIS_MAX / 4)

typedef enum APP_InputType
{
	// NULL must be zero.
	APP_INPUT_NULL = 0u
	#ifdef APP_ENABLE_JOYSTICK_INPUT
	,APP_INPUT_JOYSTICK
	#endif
	#ifdef APP_ENABLE_GAME_CONTROLLER_INPUT
	,APP_INPUT_FIRSTGAMECONTROLLERTYPE
	,APP_INPUT_XBOX = APP_INPUT_FIRSTGAMECONTROLLERTYPE
	,APP_INPUT_PLAYSTATION
	,APP_INPUT_NINTENDO
	,APP_INPUT_LASTGAMECONTROLLERTYPE = APP_INPUT_NINTENDO
	#endif
	#ifdef APP_ENABLE_KEYBOARD_INPUT
	,APP_INPUT_KEYBOARD
	#endif
	#ifdef APP_ENABLE_LINUX_GPIO_INPUT
	,APP_INPUT_LINUXGPIO
	#endif
} APP_InputType;

void APP_OpenInputs(void);
void APP_CloseInputs(void);

APP_InputType APP_GetLastInputType();

void APP_InputsUpdate(void);

#define APP_INPUTREPEAT_MAX INT_MAX

#ifdef APP_ENABLE_LINUX_GPIO_INPUT
bool APP_IsPushGPIO(int key);
bool APP_IsPressGPIO(int key);
int APP_GetGPIORepeat(int key);
#endif

#ifdef APP_ENABLE_KEYBOARD_INPUT
#define APP_KEY_MAX SDL_SCANCODE_COUNT

bool APP_IsPushKey(int key);
bool APP_IsPressKey(int key);
int APP_GetKeyRepeat(int key);
int APP_GetMaxKey(void);
#endif

#if defined(APP_ENABLE_JOYSTICK_INPUT) || defined(APP_ENABLE_GAME_CONTROLLER_INPUT)
typedef enum APP_PlayerSlotType
{
	APP_PLAYERSLOT_NULL,
	APP_PLAYERSLOT_JOY,
	APP_PLAYERSLOT_CON
} APP_PlayerSlotType;

int APP_GetNumPlayerSlots(void);
APP_PlayerSlotType APP_GetPlayerSlotType(int player);
bool APP_UpdatePlayerSlots(void);
#endif

#ifdef APP_ENABLE_JOYSTICK_INPUT
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

int APP_GetNumJoys(void);
APP_JoyGUID APP_GetJoyGUID(int player);
int APP_GetMaxJoyAxis(int player);
int APP_GetMaxJoyHat(int player);
int APP_GetMaxJoyButton(int player);
#endif

#ifdef APP_ENABLE_GAME_CONTROLLER_INPUT
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
#define APP_CONBUTTON_MAX SDL_GAMEPAD_BUTTON_COUNT

bool APP_IsPushConKey(const int player, const APP_ConKey* const key);
bool APP_IsPressConKey(const int player, const APP_ConKey* const key);
int APP_GetConKeyRepeat(const int player, const APP_ConKey* const key);

int APP_GetNumCons(void);
void APP_ResetLastActiveCon(void);
int APP_GetLastActiveCon(void);
APP_InputType APP_GetConType(const int player);
bool APP_GetConKeyDesc(const int player, const APP_ConKey* const key, const char** text, APP_Button* button);
#endif

#endif
