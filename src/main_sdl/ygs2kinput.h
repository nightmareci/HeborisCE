#pragma once

#include "defs.h"
#include "game/button.h"
#include <stdint.h>
#include <stdbool.h>

typedef enum EControllerType
{
	// NULL must be zero.
	CONTROLLER_NULL = 0u
	#ifdef ENABLE_JOYSTICK
	,CONTROLLER_JOYSTICK
	#endif
	#ifdef ENABLE_GAME_CONTROLLER
	,CONTROLLER_GAMECONTROLLER
	,CONTROLLER_FIRSTGAMECONTROLLERTYPE
	,CONTROLLER_XBOX = CONTROLLER_FIRSTGAMECONTROLLERTYPE
	,CONTROLLER_PLAYSTATION
	,CONTROLLER_NINTENDO
	#endif
	#ifdef ENABLE_KEYBOARD	
	,CONTROLLER_KEYBOARD
	#endif
	#ifdef ENABLE_LINUX_GPIO
	,CONTROLLER_LINUXGPIO
	#endif
} EControllerType;

EControllerType GetLastControllerType();

void Input();

#ifdef ENABLE_LINUX_GPIO
int IsPushGPIO(int key);
int IsPressGPIO(int key);
#endif

#ifdef ENABLE_KEYBOARD
int IsPushKey(int key);
int IsPressKey(int key);
int GetMaxKey();
#endif

#ifdef ENABLE_JOYSTICK
typedef struct {
	int32_t data[4];
} SJoyGUID;

typedef enum {
	JOYKEY_ANY,
	JOYKEY_AXIS,
	JOYKEY_HAT,
	JOYKEY_BUTTON
} EJoyKeyType;

typedef union {
	struct {
		int index;
		int value;
	};
	int button;
} SJoyKeySetting;

typedef struct {
	int index;
	SJoyGUID guid;
	EJoyKeyType type;
	SJoyKeySetting setting;
} SJoyKey;

int IsPushJoyKey(const SJoyKey* const key);
int IsPressJoyKey(const SJoyKey* const key);

int GetMaxJoys();
int GetNumJoys();

// If game controller support is enabled, a joystick index can possibly
// correspond to a game controller, in which case these functions return -1 if
// the joystick index refers to a game controller. Also, GetJoyGUID returns the
// zero GUID for a joystick index that's actually a game controller.

SJoyGUID GetJoyGUID(int index);
int GetMaxJoyAxis(int index);
int GetMaxJoyHat(int index);
int GetMaxJoyButton(int index);
#endif

#ifdef ENABLE_GAME_CONTROLLER
typedef enum {
	CONKEY_ANY,
	CONKEY_AXIS,
	CONKEY_BUTTON // The dpad is considered to be four buttons.
} EConKeyType;

typedef struct {
	EConKeyType type;
	int index;
} SConKey;

 /* Two for each stick, one for each trigger:
0,1: Left stick right,left
2,3: Left stick down,up
4,5: Right stick right,left
6,7: Right stick down,up
8: Left trigger (LT, L2, ZL)
9: Right trigger (RT, R2, ZR)
*/
#define CONAXIS_MAX 10

// Button numbers are the same as the SDL_CONTROLLER_BUTTON_* constants.
#define CONBUTTON_MAX SDL_CONTROLLER_BUTTON_MAX

int IsPushConKey(const int index, const SConKey* const key);
int IsPressConKey(const int index, const SConKey* const key);

void ResetLastConIndex();
int GetLastConIndex();
int GetNumCons();
EControllerType GetConType(const int index);
bool GetConKeyDesc(const int index, const SConKey* const key, const char** text, EButton* button);
#endif
