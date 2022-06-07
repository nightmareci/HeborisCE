#pragma once

#ifdef ENABLE_LINUX_GPIO
int IsPushGPIO ( int key );
int IsPressGPIO ( int key );
#endif

#ifdef ENABLE_KEYBOARD
int IsPushKey ( int key );
int IsPressKey ( int key );
int GetMaxKey();
#endif

#ifdef ENABLE_JOYSTICK
typedef struct {
	int32_t data[4];
} SJoyGUID;

typedef enum {
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

int IsPushJoyKey ( const SJoyKey* const key );
int IsPressJoyKey ( const SJoyKey* const key );

int GetMaxJoys();
int GetNumJoys();

// If game controller support is enabled, a joystick index can possibly
// correspond to a game controller, in which case these functions return -1 if
// the joystick index refers to a game controller. Also, GetJoyGUID returns the
// zero GUID for a joystick index that's actually a game controller.

SJoyGUID GetJoyGUID( int index );
int GetMaxJoyAxis( int index );
int GetMaxJoyHat( int index );
int GetMaxJoyButton( int index );
#endif

#ifdef ENABLE_GAME_CONTROLLER
typedef enum {
	CONKEY_AXIS,
	CONKEY_BUTTON // The dpad is considered to be four buttons.
} EConKeyType;

typedef union {
	struct {
		int axis;
		int value;
	};
	int button;
} SConKeySetting;

typedef struct {
	EConKeyType type;
	SConKeySetting setting;
} SConKey;

int IsPushConKey ( const int index, const SConKey* const key );
int IsPressConKey ( const int index, const SConKey* const key );

int GetNumCons();
#endif

typedef enum EControllerType
{
	// NULL must be zero.
	CONTROLLER_NULL = 0u
	#ifdef ENABLE_JOYSTICK
	,CONTROLLER_JOYSTICK
	#endif
	#ifdef ENABLE_GAME_CONTROLLER
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

typedef enum EMenuInput
{
	MENUINPUT_OK,
	MENUINPUT_CANCEL,
	MENUINPUT_RETRY,
	MENUINPUT_QUIT,
	MENUINPUT_UP,
	MENUINPUT_DOWN,
	MENUINPUT_LEFT,
	MENUINPUT_RIGHT
} EMenuInput;
int IsPushMenu(EMenuInput input);

void Input();