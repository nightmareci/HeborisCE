#pragma once

#include "include.h"
#include "game/button.h"

typedef enum YGS2kEControllerType
{
	// NULL must be zero.
	YGS_CONTROLLER_NULL = 0u
	#ifdef ENABLE_JOYSTICK
	,YGS_CONTROLLER_JOYSTICK
	#endif
	#ifdef ENABLE_GAME_CONTROLLER
	,YGS_CONTROLLER_GAMECONTROLLER
	,YGS_CONTROLLER_FIRSTGAMECONTROLLERTYPE
	,YGS_CONTROLLER_XBOX = YGS_CONTROLLER_FIRSTGAMECONTROLLERTYPE
	,YGS_CONTROLLER_PLAYSTATION
	,YGS_CONTROLLER_NINTENDO
	#endif
	#ifdef ENABLE_KEYBOARD	
	,YGS_CONTROLLER_KEYBOARD
	#endif
	#ifdef ENABLE_LINUX_GPIO
	,YGS_CONTROLLER_LINUXGPIO
	#endif
} YGS2kEControllerType;

YGS2kEControllerType YGS2kGetLastControllerType();

void YGS2kInput();

#define YGS_INPUTREPEAT_MAX INT_MAX

#ifdef ENABLE_LINUX_GPIO
int YGS2kIsPushGPIO(int key);
int YGS2kIsPressGPIO(int key);
int YGS2kGetGPIORepeat(int key);
#endif

#ifdef ENABLE_KEYBOARD
#define YGS_KEY_MAX SDL_NUM_SCANCODES

int YGS2kIsPushKey(int key);
int YGS2kIsPressKey(int key);
int YGS2kGetKeyRepeat(int key);
int YGS2kGetMaxKey();
#endif

#ifdef ENABLE_JOYSTICK
typedef struct YGS2kSJoyGUID {
	int32_t data[4];
} YGS2kSJoyGUID;

typedef enum YGS2kEJoyKeyType {
	YGS_JOYKEY_ANY,
	YGS_JOYKEY_AXIS,
	YGS_JOYKEY_HAT,
	YGS_JOYKEY_BUTTON
} YGS2kEJoyKeyType;

typedef union YGS2kSJoyKeySetting {
	struct {
		int index;
		int value;
	};
	int button;
} YGS2kSJoyKeySetting;

typedef struct YGS2kSJoyKey {
	int index;
	YGS2kSJoyGUID guid;
	YGS2kEJoyKeyType type;
	YGS2kSJoyKeySetting setting;
} YGS2kSJoyKey;

int YGS2kIsPushJoyKey(const YGS2kSJoyKey* const key);
int YGS2kIsPressJoyKey(const YGS2kSJoyKey* const key);
int YGS2kGetJoyKeyRepeat(const YGS2kSJoyKey* const key); // If key->type == YGS_JOYKEY_ANY, checks all inputs of the key's joystick and returns the max repeat value.

int YGS2kGetMaxJoys();
int YGS2kGetNumJoys();

// If game controller support is enabled, a joystick index can possibly
// correspond to a game controller, in which case these functions return -1 if
// the joystick index refers to a game controller. Also, YGS2kGetJoyGUID returns the
// zero GUID for a joystick index that's actually a game controller.

YGS2kSJoyGUID YGS2kGetJoyGUID(int index);
int YGS2kGetMaxJoyAxis(int index);
int YGS2kGetMaxJoyHat(int index);
int YGS2kGetMaxJoyButton(int index);
#endif

#ifdef ENABLE_GAME_CONTROLLER
typedef enum YGS2kEConKeyType {
	YGS_CONKEY_ANY,
	YGS_CONKEY_AXIS,
	YGS_CONKEY_BUTTON // The dpad is considered to be four buttons.
} YGS2kEConKeyType;

typedef struct YGS2kSConKey {
	YGS2kEConKeyType type;
	int index;
} YGS2kSConKey;

 /* Two for each stick, one for each trigger:
0,1: Left stick right,left
2,3: Left stick down,up
4,5: Right stick right,left
6,7: Right stick down,up
8: Left trigger (LT, L2, ZL)
9: Right trigger (RT, R2, ZR)
*/
#define YGS_CONAXIS_MAX 10

// Button numbers are the same as the SDL_CONTROLLER_BUTTON_* constants.
#define YGS_CONBUTTON_MAX SDL_CONTROLLER_BUTTON_MAX

int YGS2kIsPushConKey(const int index, const YGS2kSConKey* const key);
int YGS2kIsPressConKey(const int index, const YGS2kSConKey* const key);
int YGS2kGetConKeyRepeat(const int index, YGS2kSConKey* const key);

bool YGS2kIsGameController(int index);
void YGS2kResetLastConIndex();
int YGS2kGetLastConIndex();
int YGS2kGetNumCons();
YGS2kEControllerType YGS2kGetConType(const int index);
bool YGS2kGetConKeyDesc(const int index, const YGS2kSConKey* const key, const char** text, EButton* button);
#endif
