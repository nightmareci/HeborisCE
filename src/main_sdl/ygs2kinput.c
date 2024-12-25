#include "main_sdl/include.h"
#include "ygs2kfunc.h"
#include "ygs2kprivate.h"

#ifdef ENABLE_LINUX_GPIO
static int			s_iGPIORepeat[10];
static struct gpiod_chip	*s_pGPIOChip;
static struct gpiod_line	*s_apGPIOLines[10];
#endif

#ifdef ENABLE_KEYBOARD
static int			s_iKeyRepeat[YGS_KEY_MAX];
#endif

#ifdef ENABLE_JOYSTICK
typedef struct YGS2kSJoy
{
	SDL_Joystick* joystick;
	int numAxes;
	int numHats;
	int numButtons;
	int* axesRepeat;
	int* hatsRepeat;
	int* buttonsRepeat;
} YGS2kSJoy;
static YGS2kSJoy* s_aJoys = NULL;
static int s_iNumJoys = -1;
#endif

#ifdef ENABLE_GAME_CONTROLLER
#include "main_sdl/gamecontrollerdb.h"
typedef struct YGS2kSCon
{
	SDL_GameController* controller;
	SDL_JoystickID joyid;
	int axesRepeat[YGS_CONAXIS_MAX];
	int buttonsRepeat[YGS_CONBUTTON_MAX];
} YGS2kSCon;
static YGS2kSCon* s_aConPlayerSlots = NULL;
static int s_iNumConPlayerSlots = 0;
static int s_iLastActiveConPlayer = -1;

bool YGS2kIsCon ( int device )
{
	return
		(SDL_IsGameController(device) && SDL_GameControllerTypeForIndex(device) != SDL_CONTROLLER_TYPE_UNKNOWN) ||
		SDL_GameControllerMappingForGUID(SDL_JoystickGetDeviceGUID(device)) != NULL;
}

#endif

YGS2kEInputType YGS2kGetLastInputType ()
{
	#ifdef ONLY_INPUT_TYPE
	return ONLY_INPUT_TYPE;
	#else
	return YGS2kLastInputType;
	#endif
}

#ifdef ENABLE_LINUX_GPIO
bool YGS2kIsPushGPIO ( int key )
{
	return key >= 0 && key < NUMBTNS && s_iGPIORepeat[key] == 1;
}

bool YGS2kIsPressGPIO ( int key )
{
	return key >= 0 && key < NUMBTNS && s_iGPIORepeat[key] != 0;
}

int YGS2kGetGPIORepeat( int key )
{
	return key >= 0 && key < NUMBTNS ? s_iGPIORepeat[key] : 0;
}
#endif

#ifdef ENABLE_KEYBOARD
bool YGS2kIsPushKey (int key)
{
	return key >= 0 && key < YGS_KEY_MAX && s_iKeyRepeat[key] == 1;
}

bool YGS2kIsPressKey (int key)
{
	return key >= 0 && key < YGS_KEY_MAX && s_iKeyRepeat[key] != 0;
}

int YGS2kGetKeyRepeat( int key )
{
	return key >= 0 && key < YGS_KEY_MAX ? s_iKeyRepeat[key] : 0;
}

int YGS2kGetMaxKey ()
{
	return YGS_KEY_MAX;
}
#endif

#ifdef ENABLE_JOYSTICK
// TODO: Change joysticks to use SDL player index APIs instead of SDL device
// index APIs, similar to game controllers.

void YGS2kJoysClose ()
{
	if (s_aJoys)
	{
		for (int device = 0; device < s_iNumJoys; device++)
		{
			if (s_aJoys[device].joystick && SDL_JoystickGetAttached(s_aJoys[device].joystick)) SDL_JoystickClose(s_aJoys[device].joystick);
			if (s_aJoys[device].axesRepeat) free(s_aJoys[device].axesRepeat);
			if (s_aJoys[device].hatsRepeat) free(s_aJoys[device].hatsRepeat);
			if (s_aJoys[device].buttonsRepeat) free(s_aJoys[device].buttonsRepeat);
		}
		free(s_aJoys);
	}
	s_aJoys = NULL;
	s_iNumJoys = -1;
}

bool YGS2kJoysOpen ()
{
	if ((s_iNumJoys = SDL_NumJoysticks()) > 0)
	{
		if (!(s_aJoys = (YGS2kSJoy*)calloc((size_t)s_iNumJoys, sizeof(YGS2kSJoy))))
		{
			s_iNumJoys = -1;
			return false;
		}

		for (int device = 0; device < s_iNumJoys; device++)
		{
			#ifdef ENABLE_GAME_CONTROLLER
			if (!YGS2kIsCon(device))
			{
			#endif
				bool fail = false;
				if (!(s_aJoys[device].joystick = SDL_JoystickOpen(device))) fail = true;
				else if (
					(s_aJoys[device].numAxes = SDL_JoystickNumAxes(s_aJoys[device].joystick)) >= 0 &&
					(s_aJoys[device].numHats = SDL_JoystickNumHats(s_aJoys[device].joystick)) >= 0 &&
					(s_aJoys[device].numButtons = SDL_JoystickNumButtons(s_aJoys[device].joystick)) >= 0
				)
				{
					if (s_aJoys[device].numAxes > 0 && !(s_aJoys[device].axesRepeat = (int*)calloc((size_t)s_aJoys[device].numAxes * 2, sizeof(int)))) fail = true;
					if (s_aJoys[device].numHats > 0 && !(s_aJoys[device].hatsRepeat = (int*)calloc((size_t)s_aJoys[device].numHats * 4, sizeof(int)))) fail = true;
					if (s_aJoys[device].numButtons > 0 && !(s_aJoys[device].buttonsRepeat = (int*)calloc((size_t)s_aJoys[device].numButtons, sizeof(int)))) fail = true;
				}
				if (fail)
				{
					s_iNumJoys = device + !!s_aJoys[device].joystick;
					YGS2kJoysClose();
					s_iNumJoys = -1;
					return false;
				}
			#ifdef ENABLE_GAME_CONTROLLER
			}
			#endif
		}
		return true;
	}
	else if (s_iNumJoys == 0) return true;
	else return false;
}

static void YGS2kJoyInputsUpdate ()
{
	if (!s_aJoys) return;

	for (int device = 0; device < s_iNumJoys; device++)
	{
		#ifdef ENABLE_GAME_CONTROLLER
		if (YGS2kIsCon(device)) continue;
		#endif
		if (!SDL_JoystickGetAttached(s_aJoys[device].joystick))
		{
			continue;
		}

		const YGS2kSJoyGUID checkGUID = YGS2kGetJoyGUID(device);
		const YGS2kSJoyGUID zeroGUID = { 0 };
		if (memcmp(checkGUID.data, zeroGUID.data, sizeof(checkGUID.data)) == 0) continue;

		for (int axis = 0; axis < s_aJoys[device].numAxes; axis++)
		{
			const int value = SDL_JoystickGetAxis(s_aJoys[device].joystick, axis);

			if (value < -YGS_DEADZONE_MAX)
			{
				if (++s_aJoys[device].axesRepeat[axis*2 + 0] == 1) YGS2kLastInputType = YGS_INPUT_JOYSTICK;
			}
			else
			{
				s_aJoys[device].axesRepeat[axis*2 + 0] = 0;
			}

			if (value > +YGS_DEADZONE_MAX)
			{
				if (++s_aJoys[device].axesRepeat[axis*2 + 1] == 1) YGS2kLastInputType = YGS_INPUT_JOYSTICK;
			}
			else
			{
				s_aJoys[device].axesRepeat[axis*2 + 1] = 0;
			}
		}

		for (int hat = 0; hat < s_aJoys[device].numHats; hat++)
		{
			Uint8 value = SDL_JoystickGetHat(s_aJoys[device].joystick, hat);
			const Uint8 hatValues[4] =
			{
				SDL_HAT_LEFT,
				SDL_HAT_RIGHT,
				SDL_HAT_UP,
				SDL_HAT_DOWN
			};
			for (int valueIndex = 0; valueIndex < 4; valueIndex++)
			{
				if (value & hatValues[valueIndex])
				{
					if (++s_aJoys[device].hatsRepeat[hat*4 + valueIndex] == 1) YGS2kLastInputType = YGS_INPUT_JOYSTICK;
				}
				else
				{
					s_aJoys[device].hatsRepeat[hat*4 + valueIndex] = 0;
				}
			}
		}

		for (int button = 0; button < s_aJoys[device].numButtons; button++)
		{
			if (SDL_JoystickGetButton(s_aJoys[device].joystick, button))
			{
				if (++s_aJoys[device].buttonsRepeat[button] == 1) YGS2kLastInputType = YGS_INPUT_JOYSTICK;
			}
			else
			{
				s_aJoys[device].buttonsRepeat[button] = 0;
			}
		}
	}
}

bool YGS2kIsPushJoyKey ( const YGS2kSJoyKey* const key )
{
	if (!s_aJoys || s_iNumJoys <= 0 || key == NULL || key->device >= s_iNumJoys) return false;

	int device = 0;
	int deviceMax = 0;
	if (key->device >= 0)
	{
		YGS2kSJoyGUID checkGUID = YGS2kGetJoyGUID(key->device);
		YGS2kSJoyGUID zeroGUID = { 0 };
		if (memcmp(checkGUID.data, zeroGUID.data, sizeof(checkGUID.data)) != 0 && memcmp(key->guid.data, checkGUID.data, sizeof(checkGUID.data)) == 0)
		{
			device = key->device;
			deviceMax = key->device + 1;
		}
	}
	else
	{
		device = 0;
		deviceMax = s_iNumJoys;
	}
	for (; device < deviceMax; device++)
	{
		switch (key->type)
		{
		case YGS_JOYKEY_ANY:
			for (int axis = 0; axis < s_aJoys[device].numAxes; axis++)
			{
				if (s_aJoys[device].axesRepeat[axis*2 + 0] == 1) return true;
				if (s_aJoys[device].axesRepeat[axis*2 + 1] == 1) return true;
			}
			for (int hat = 0; hat < s_aJoys[device].numHats; hat++)
			{
				if (s_aJoys[device].hatsRepeat[hat] == 1) return true;
			}
			for (int button = 0; button < s_aJoys[device].numButtons; button++)
			{
				if (s_aJoys[device].buttonsRepeat[button] == 1) return true;
			}
			break;
		case YGS_JOYKEY_AXIS:
			if (key->setting.index < s_aJoys[device].numAxes)
			{
				if (key->setting.value == -YGS_DEADZONE_MAX)
				{
					return s_aJoys[device].axesRepeat[key->setting.index * 2 + 0] == 1;
				}
				else if (key->setting.value == +YGS_DEADZONE_MAX)
				{
					return s_aJoys[device].axesRepeat[key->setting.index * 2 + 1] == 1;
				}
			}
			break;
		case YGS_JOYKEY_HAT:
			if (key->setting.index < s_aJoys[device].numHats)
			{
				switch (key->setting.value)
				{
				case SDL_HAT_LEFT:
					return s_aJoys[device].hatsRepeat[key->setting.index * 4 + 0] == 1;
				case SDL_HAT_RIGHT:
					return s_aJoys[device].hatsRepeat[key->setting.index * 4 + 1] == 1;
				case SDL_HAT_UP:
					return s_aJoys[device].hatsRepeat[key->setting.index * 4 + 2] == 1;
				case SDL_HAT_DOWN:
					return s_aJoys[device].hatsRepeat[key->setting.index * 4 + 3] == 1;
				default:
					break;
				}
			}
			break;
		case YGS_JOYKEY_BUTTON:
			if (key->setting.button < s_aJoys[device].numButtons) return s_aJoys[device].buttonsRepeat[key->setting.button] == 1;
			break;
		default:
			break;
		}
	}
	return false;
}

bool YGS2kIsPressJoyKey ( const YGS2kSJoyKey* const key )
{
	if (!s_aJoys || s_iNumJoys <= 0 || key == NULL || key->device >= s_iNumJoys) return false;

	int device = 0;
	int deviceMax = 0;
	if (key->device >= 0)
	{
		YGS2kSJoyGUID checkGUID = YGS2kGetJoyGUID(key->device);
		YGS2kSJoyGUID zeroGUID = { 0 };
		if (memcmp(checkGUID.data, zeroGUID.data, sizeof(checkGUID.data)) != 0 && memcmp(key->guid.data, checkGUID.data, sizeof(checkGUID.data)) == 0)
		{
			device = key->device;
			deviceMax = key->device + 1;
		}
	}
	else
	{
		device = 0;
		deviceMax = s_iNumJoys;
	}
	for (; device < deviceMax; device++)
	{
		switch (key->type)
		{
		case YGS_JOYKEY_ANY:
			for (int axis = 0; axis < s_aJoys[device].numAxes; axis++)
			{
				if (s_aJoys[device].axesRepeat[axis*2 + 0] != 0) return true;
				if (s_aJoys[device].axesRepeat[axis*2 + 1] != 0) return true;
			}
			for (int hat = 0; hat < s_aJoys[device].numHats; hat++)
			{
				if (s_aJoys[device].hatsRepeat[hat] != 0) return true;
			}
			for (int button = 0; button < s_aJoys[device].numButtons; button++)
			{
				if (s_aJoys[device].buttonsRepeat[button] != 0) return true;
			}
			break;
		case YGS_JOYKEY_AXIS:
			if (key->setting.index < s_aJoys[device].numAxes)
			{
				if (key->setting.value == -YGS_DEADZONE_MAX)
				{
					return s_aJoys[device].axesRepeat[key->setting.index * 2 + 0] != 0;
				}
				else if (key->setting.value == +YGS_DEADZONE_MAX)
				{
					return s_aJoys[device].axesRepeat[key->setting.index * 2 + 1] != 0;
				}
			}
			break;
		case YGS_JOYKEY_HAT:
			if (key->setting.index < s_aJoys[device].numHats)
			{
				switch (key->setting.value)
				{
				case SDL_HAT_LEFT:
					return s_aJoys[device].hatsRepeat[key->setting.index * 4 + 0] != 0;
				case SDL_HAT_RIGHT:
					return s_aJoys[device].hatsRepeat[key->setting.index * 4 + 1] != 0;
				case SDL_HAT_UP:
					return s_aJoys[device].hatsRepeat[key->setting.index * 4 + 2] != 0;
				case SDL_HAT_DOWN:
					return s_aJoys[device].hatsRepeat[key->setting.index * 4 + 3] != 0;
				default:
					break;
				}
			}
			break;
		case YGS_JOYKEY_BUTTON:
			if (key->setting.button < s_aJoys[device].numButtons) return s_aJoys[device].buttonsRepeat[key->setting.button] != 0 ? 1 : 0;
			break;
		default:
			break;
		}
	}
	return false;
}

int YGS2kGetJoyKeyRepeat ( const YGS2kSJoyKey* const key )
{
	if (!s_aJoys || s_iNumJoys <= 0 || key == NULL || key->device >= s_iNumJoys) return 0;

	bool multi;
	int device = 0;
	int deviceMax = 0;
	int maxRepeat = 0;
	if (key->device >= 0)
	{
		YGS2kSJoyGUID checkGUID = YGS2kGetJoyGUID(key->device);
		YGS2kSJoyGUID zeroGUID = { 0 };
		if (memcmp(checkGUID.data, zeroGUID.data, sizeof(checkGUID.data)) != 0 && memcmp(key->guid.data, checkGUID.data, sizeof(checkGUID.data)) == 0)
		{
			device = key->device;
			deviceMax = key->device + 1;
			multi = false;
		}
	}
	else
	{
		device = 0;
		deviceMax = s_iNumJoys;
		multi = true;
	}
	for (; device < deviceMax; device++)
	{
		switch (key->type)
		{
		case YGS_JOYKEY_ANY:
			for (int axis = 0; axis < s_aJoys[device].numAxes; axis++)
			{
				if (s_aJoys[device].axesRepeat[axis*2 + 0] > maxRepeat) maxRepeat = s_aJoys[device].axesRepeat[axis*2 + 0];
				if (s_aJoys[device].axesRepeat[axis*2 + 1] > maxRepeat) maxRepeat = s_aJoys[device].axesRepeat[axis*2 + 1];
			}
			for (int hat = 0; hat < s_aJoys[device].numHats; hat++)
			{
				if (s_aJoys[device].hatsRepeat[hat] > maxRepeat) maxRepeat = s_aJoys[device].hatsRepeat[hat];
			}
			for (int button = 0; button < s_aJoys[device].numButtons; button++)
			{
				if (s_aJoys[device].buttonsRepeat[button] > maxRepeat) maxRepeat = s_aJoys[device].buttonsRepeat[button];
			}
			break;
		case YGS_JOYKEY_AXIS:
			if (key->setting.index < s_aJoys[device].numAxes)
			{
				if (key->setting.value == -YGS_DEADZONE_MAX)
				{
					if (multi)
					{
						if (s_aJoys[device].axesRepeat[key->setting.index * 2 + 0] > maxRepeat) maxRepeat = s_aJoys[device].axesRepeat[key->setting.index * 2 + 0];
					}
					else return s_aJoys[device].axesRepeat[key->setting.index * 2 + 0];
				}
				else if (key->setting.value == +YGS_DEADZONE_MAX)
				{
					if (multi)
					{
						if (s_aJoys[device].axesRepeat[key->setting.index * 2 + 1] > maxRepeat) maxRepeat = s_aJoys[device].axesRepeat[key->setting.index * 2 + 1];
					}
					else return s_aJoys[device].axesRepeat[key->setting.index * 2 + 1];
				}
			}
			break;
		case YGS_JOYKEY_HAT:
			if (key->setting.index < s_aJoys[device].numHats)
			{
				switch (key->setting.value)
				{
				case SDL_HAT_LEFT:
					if (multi)
					{
						if (s_aJoys[device].hatsRepeat[key->setting.index * 4 + 0] > maxRepeat) maxRepeat = s_aJoys[device].hatsRepeat[key->setting.index * 4 + 0];
					}
					else return s_aJoys[device].hatsRepeat[key->setting.index * 4 + 0];
				case SDL_HAT_RIGHT:
					if (multi)
					{
						if (s_aJoys[device].hatsRepeat[key->setting.index * 4 + 1] > maxRepeat) maxRepeat = s_aJoys[device].hatsRepeat[key->setting.index * 4 + 1];
					}
					else return s_aJoys[device].hatsRepeat[key->setting.index * 4 + 1];
				case SDL_HAT_UP:
					if (multi)
					{
						if (s_aJoys[device].hatsRepeat[key->setting.index * 4 + 2] > maxRepeat) maxRepeat = s_aJoys[device].hatsRepeat[key->setting.index * 4 + 2];
					}
					else return s_aJoys[device].hatsRepeat[key->setting.index * 4 + 2];
				case SDL_HAT_DOWN:
					if (multi)
					{
						if (s_aJoys[device].hatsRepeat[key->setting.index * 4 + 3] > maxRepeat) maxRepeat = s_aJoys[device].hatsRepeat[key->setting.index * 4 + 3];
					}
					else return s_aJoys[device].hatsRepeat[key->setting.index * 4 + 3];
				default:
					break;
				}
			}
			break;
		case YGS_JOYKEY_BUTTON:
			if (key->setting.button < s_aJoys[device].numButtons)
			{
				if (multi)
				{
					if (s_aJoys[device].buttonsRepeat[key->setting.button] > maxRepeat) maxRepeat = s_aJoys[device].buttonsRepeat[key->setting.button];
				}
				else return s_aJoys[device].buttonsRepeat[key->setting.button];
			}
			break;
		default:
			break;
		}
	}
	return maxRepeat;
}

int YGS2kGetMaxJoys()
{
	return s_iNumJoys;
}

int YGS2kGetNumJoys()
{
	#ifdef ENABLE_GAME_CONTROLLER
	int numJoys = 0;
	for (int device = 0; device < s_iNumJoys; device++)
	{
		if (!YGS2kIsCon(device)) numJoys++;
	}
	return numJoys;
	#else
	return s_iNumJoys;
	#endif
}

YGS2kSJoyGUID YGS2kGetJoyGUID(int device)
{
	if (s_iNumJoys <= 0 || device >= s_iNumJoys
	#ifdef ENABLE_GAME_CONTROLLER
		|| YGS2kIsCon(device)
	#endif
	) return (YGS2kSJoyGUID) { 0 };

	SDL_JoystickGUID sdlGUID;
	sdlGUID = SDL_JoystickGetGUID(s_aJoys[device].joystick);

	YGS2kSJoyGUID joyGUID = { 0 };
	for (int32_t i = 0; i < 4; i++)
	{
		joyGUID.data[i] = 0;
		for (int32_t j = 0; j < 4; j++)
		{
			joyGUID.data[i] |= (int32_t)((uint32_t)sdlGUID.data[i * 4 + j] << (j * 8));
		}
	}
	return joyGUID;
}

int YGS2kGetMaxJoyAxis(int device)
{
	if (s_iNumJoys > 0 && device < s_iNumJoys)
	{
		#ifdef ENABLE_GAME_CONTROLLER
		if (!YGS2kIsCon(device))
		{
			return s_aJoys[device].numAxes;
		}
		else
		{
			return -1;
		}
		#else
		return s_aJoys[device].numAxes;
		#endif
	}
	else
	{
		return -1;
	}
}

int YGS2kGetMaxJoyHat(int device)
{
	if (s_iNumJoys > 0 && device < s_iNumJoys)
	{
		#ifdef ENABLE_GAME_CONTROLLER
		if (!YGS2kIsCon(device))
		{
			return s_aJoys[device].numHats;
		}
		else
		{
			return -1;
		}
		#else
		return s_aJoys[device].numHats;
		#endif
	}
	else
	{
		return -1;
	}
}

int YGS2kGetMaxJoyButton(int device)
{
	if (s_iNumJoys > 0 && device < s_iNumJoys)
	{
		#ifdef ENABLE_GAME_CONTROLLER
		if (!YGS2kIsCon(device))
		{
			return s_aJoys[device].numButtons;
		}
		else
		{
			return -1;
		}
		#else
		return s_aJoys[device].numButtons;
		#endif
	}
	else
	{
		return -1;
	}
}
#endif

#ifdef ENABLE_GAME_CONTROLLER
void YGS2kConsClose()
{
	for (int player = 0; player < s_iNumConPlayerSlots; player++)
	{
		if (s_aConPlayerSlots[player].controller && SDL_GameControllerGetAttached(s_aConPlayerSlots[player].controller))
		{
			SDL_GameControllerClose(s_aConPlayerSlots[player].controller);
		}
	}
	if (s_aConPlayerSlots) {
		free(s_aConPlayerSlots);
		s_aConPlayerSlots = NULL;
	}
	s_iNumConPlayerSlots = 0;
}

static bool YGS2kConPlayerSlotsResize(int numSlots) {
	if (numSlots == s_iNumConPlayerSlots) {
		return true;
	}
	else if (numSlots == 0) {
		free(s_aConPlayerSlots);
		s_aConPlayerSlots = NULL;
		s_iNumConPlayerSlots = 0;
		return true;
	}

	YGS2kSCon* const oldSlots = s_aConPlayerSlots;
	const int oldNumSlots = s_iNumConPlayerSlots;
	if (s_aConPlayerSlots) {
		s_aConPlayerSlots = realloc(s_aConPlayerSlots, sizeof(YGS2kSCon) * numSlots);
	}
	else {
		s_aConPlayerSlots = malloc(sizeof(YGS2kSCon) * numSlots);
	}
	if (!s_aConPlayerSlots) {
		printf("Failed to resize controllers array\n");
		s_aConPlayerSlots = oldSlots;
		return false;
	}
	for (int player = s_iNumConPlayerSlots; player < numSlots; player++) {
		s_aConPlayerSlots[player] = (YGS2kSCon) { 0 };
	}
	s_iNumConPlayerSlots = numSlots;
	return true;
}

bool YGS2kConsChanged() {
	for (int player = 0; player < s_iNumConPlayerSlots; player++) {
		if (!SDL_GameControllerGetAttached(s_aConPlayerSlots[player].controller)) {
			s_aConPlayerSlots[player] = (YGS2kSCon) { 0 };
		}
	}

	const int numJoys = SDL_NumJoysticks();
	for (int device = 0; device < numJoys; device++) {
		if (YGS2kIsCon(device)) {
			// SDL internally calls SDL_JoystickOpen() in SDL_GameControllerOpen(), and
			// SDL_JoystickFromInstanceID() only returns a joystick if it's been opened, so
			// we can use SDL_JoystickFromInstanceID() to determine if a device index is
			// already open.
			const SDL_JoystickID id = SDL_JoystickGetDeviceInstanceID(device);
			if (id >= 0 && !SDL_JoystickFromInstanceID(id)) {
				SDL_GameController* const controller = SDL_GameControllerOpen(device);
				if (!controller) {
					printf("Failed to open controller\n");
					return false;
				}
				const int player = SDL_GameControllerGetPlayerIndex(controller);
				if (player < 0) {
					printf("Failed to get controller player index\n");
					SDL_GameControllerClose(controller);
					return false;
				}
				if (player >= s_iNumConPlayerSlots && !YGS2kConPlayerSlotsResize(player + 1)) {
					SDL_GameControllerClose(controller);
					return false;
				}
				s_aConPlayerSlots[player].controller = controller;
			}
		}
	}

	int numRequiredSlots = 0;
	for (int player = 0; player < s_iNumConPlayerSlots; player++) {
		if (s_aConPlayerSlots[player].controller) {
			numRequiredSlots = player + 1;
		}
	}
	return YGS2kConPlayerSlotsResize(numRequiredSlots);
}

static SDL_GameControllerType YGS2kGetSDLGameControllerType(SDL_GameController* const controller)
{
	#ifdef ONLY_SDL_CONTROLLER_TYPE
	return ONLY_SDL_CONTROLLER_TYPE;
	#else
	return SDL_GameControllerGetType(controller);
	#endif
}

static void YGS2kConInputsUpdate()
{
	if (!s_aConPlayerSlots) return;

	for (int player = 0; player < s_iNumConPlayerSlots; player++)
	{
		if (!SDL_GameControllerGetAttached(s_aConPlayerSlots[player].controller)) continue;

		YGS2kEInputType const inputType = YGS2kGetConPlayerType(player);
		Sint16 axisValue;

		axisValue = SDL_GameControllerGetAxis(s_aConPlayerSlots[player].controller, SDL_CONTROLLER_AXIS_LEFTX);
		if (axisValue > 0 && axisValue > +YGS_DEADZONE_MAX)
		{
			if (++s_aConPlayerSlots[player].axesRepeat[0] == 1)
			{
				YGS2kLastInputType = inputType;
				s_iLastActiveConPlayer = player;
			}
			s_aConPlayerSlots[player].axesRepeat[1] = 0;
		}
		else if (axisValue < 0 && axisValue < -YGS_DEADZONE_MAX)
		{
			s_aConPlayerSlots[player].axesRepeat[0] = 0;
			if (++s_aConPlayerSlots[player].axesRepeat[1] == 1)
			{
				YGS2kLastInputType = inputType;
				s_iLastActiveConPlayer = player;
			}
		}
		else
		{
			s_aConPlayerSlots[player].axesRepeat[0] = 0;
			s_aConPlayerSlots[player].axesRepeat[1] = 0;
		}

		axisValue = SDL_GameControllerGetAxis(s_aConPlayerSlots[player].controller, SDL_CONTROLLER_AXIS_LEFTY);
		if (axisValue > 0 && axisValue > +YGS_DEADZONE_MAX)
		{
			if (++s_aConPlayerSlots[player].axesRepeat[2] == 1)
			{
				YGS2kLastInputType = inputType;
				s_iLastActiveConPlayer = player;
			}
			s_aConPlayerSlots[player].axesRepeat[3] = 0;
		}
		else if (axisValue < 0 && axisValue < -YGS_DEADZONE_MAX)
		{
			s_aConPlayerSlots[player].axesRepeat[2] = 0;
			if (++s_aConPlayerSlots[player].axesRepeat[3] == 1)
			{
				YGS2kLastInputType = inputType;
				s_iLastActiveConPlayer = player;
			}
		}
		else
		{
			s_aConPlayerSlots[player].axesRepeat[2] = 0;
			s_aConPlayerSlots[player].axesRepeat[3] = 0;
		}

		axisValue = SDL_GameControllerGetAxis(s_aConPlayerSlots[player].controller, SDL_CONTROLLER_AXIS_RIGHTX);
		if (axisValue > 0 && axisValue > +YGS_DEADZONE_MAX)
		{
			if (++s_aConPlayerSlots[player].axesRepeat[4] == 1)
			{
				YGS2kLastInputType = inputType;
				s_iLastActiveConPlayer = player;
			}
			s_aConPlayerSlots[player].axesRepeat[5] = 0;
		}
		else if (axisValue < 0 && axisValue < -YGS_DEADZONE_MAX)
		{
			s_aConPlayerSlots[player].axesRepeat[4] = 0;
			if (++s_aConPlayerSlots[player].axesRepeat[5] == 1)
			{
				YGS2kLastInputType = inputType;
				s_iLastActiveConPlayer = player;
			}
		}
		else
		{
			s_aConPlayerSlots[player].axesRepeat[4] = 0;
			s_aConPlayerSlots[player].axesRepeat[5] = 0;
		}

		axisValue = SDL_GameControllerGetAxis(s_aConPlayerSlots[player].controller, SDL_CONTROLLER_AXIS_RIGHTY);
		if (axisValue > 0 && axisValue > +YGS_DEADZONE_MAX)
		{
			if (++s_aConPlayerSlots[player].axesRepeat[6] == 1)
			{
				YGS2kLastInputType = inputType;
				s_iLastActiveConPlayer = player;
			}
			s_aConPlayerSlots[player].axesRepeat[7] = 0;
		}
		else if (axisValue < 0 && axisValue < -YGS_DEADZONE_MAX)
		{
			s_aConPlayerSlots[player].axesRepeat[6] = 0;
			if (++s_aConPlayerSlots[player].axesRepeat[7] == 1)
			{
				YGS2kLastInputType = inputType;
				s_iLastActiveConPlayer = player;
			}
		}
		else
		{
			s_aConPlayerSlots[player].axesRepeat[6] = 0;
			s_aConPlayerSlots[player].axesRepeat[7] = 0;
		}

		axisValue = SDL_GameControllerGetAxis(s_aConPlayerSlots[player].controller, SDL_CONTROLLER_AXIS_TRIGGERLEFT);
		if (axisValue > YGS_DEADZONE_MAX)
		{
			if (++s_aConPlayerSlots[player].axesRepeat[8] == 1)
			{
				YGS2kLastInputType = inputType;
				s_iLastActiveConPlayer = player;
			}
		}
		else
		{
			s_aConPlayerSlots[player].axesRepeat[8] = 0;
		}

		axisValue = SDL_GameControllerGetAxis(s_aConPlayerSlots[player].controller, SDL_CONTROLLER_AXIS_TRIGGERRIGHT);
		if (axisValue > YGS_DEADZONE_MAX)
		{
			if (++s_aConPlayerSlots[player].axesRepeat[9] == 1)
			{
				YGS2kLastInputType = inputType;
				s_iLastActiveConPlayer = player;
			}
		}
		else
		{
			s_aConPlayerSlots[player].axesRepeat[9] = 0;
		}

		for (SDL_GameControllerButton button = 0; button < SDL_CONTROLLER_BUTTON_MAX; button++)
		{
			if (SDL_GameControllerGetButton(s_aConPlayerSlots[player].controller, button))
			{
				if (++s_aConPlayerSlots[player].buttonsRepeat[button] == 1)
				{
					YGS2kLastInputType = inputType;
					s_iLastActiveConPlayer = player;
				}
			}
			else
			{
				s_aConPlayerSlots[player].buttonsRepeat[button] = 0;
			}
		}
	}
}

bool YGS2kIsPushConPlayerKey ( const int player, const YGS2kSConKey* const key )
{
	if (!s_aConPlayerSlots || s_iNumConPlayerSlots == 0 || player >= s_iNumConPlayerSlots || !s_aConPlayerSlots[player].controller || !key) return false;

	int playerStart = 0;
	int playerMax = 0;
	if (player >= 0)
	{
		playerStart = player;
		playerMax = player + 1;
	}
	else
	{
		playerStart = 0;
		playerMax = s_iNumConPlayerSlots;
	}
	for (; playerStart < playerMax; playerStart++)
	{
		switch (key->type)
		{
		case YGS_CONKEY_ANY:
			for (int axis = 0; axis < YGS_CONAXIS_MAX; axis++)
			{
				if (s_aConPlayerSlots[playerStart].axesRepeat[axis] == 1) return true;
			}
			for (int button = 0; button < YGS_CONBUTTON_MAX; button++)
			{
				if (s_aConPlayerSlots[playerStart].buttonsRepeat[button] == 1) return true;
			}
			break;
		case YGS_CONKEY_AXIS:
			if (key->index < YGS_CONAXIS_MAX)
			{
				if (s_aConPlayerSlots[playerStart].axesRepeat[key->index] == 1) return true;
			}
			break;
		case YGS_CONKEY_BUTTON:
			if (key->index < YGS_CONBUTTON_MAX)
			{
				if (s_aConPlayerSlots[playerStart].buttonsRepeat[key->index] == 1) return true;
			}
			break;
		default:
			break;
		}
	}
	return false;
}

bool YGS2kIsPressConPlayerKey ( const int player, const YGS2kSConKey* const key )
{
	if (!s_aConPlayerSlots || s_iNumConPlayerSlots == 0 || player >= s_iNumConPlayerSlots || !s_aConPlayerSlots[player].controller || !key) return false;

	int playerStart = 0;
	int playerMax = 0;
	if (player >= 0)
	{
		playerStart = player;
		playerMax = player + 1;
	}
	else
	{
		playerStart = 0;
		playerMax = s_iNumConPlayerSlots;
	}
	for (; playerStart < playerMax; playerStart++)
	{
		switch (key->type)
		{
		case YGS_CONKEY_ANY:
			for (int axis = 0; axis < YGS_CONAXIS_MAX; axis++)
			{
				if (s_aConPlayerSlots[playerStart].axesRepeat[axis] != 0) return true;
			}
			for (int button = 0; button < YGS_CONBUTTON_MAX; button++)
			{
				if (s_aConPlayerSlots[playerStart].buttonsRepeat[button] != 0) return true;
			}
			break;
		case YGS_CONKEY_AXIS:
			if (key->index < YGS_CONAXIS_MAX)
			{
				if (s_aConPlayerSlots[playerStart].axesRepeat[key->index] != 0) return true;
			}
			break;
		case YGS_CONKEY_BUTTON:
			if (key->index < YGS_CONBUTTON_MAX)
			{
				if (s_aConPlayerSlots[playerStart].buttonsRepeat[key->index] != 0) return true;
			}
			break;
		default:
			break;
		}
	}
	return false;
}

int YGS2kGetConPlayerKeyRepeat ( const int player, YGS2kSConKey* const key )
{
	if (!s_aConPlayerSlots || s_iNumConPlayerSlots == 0 || player >= s_iNumConPlayerSlots || !s_aConPlayerSlots[player].controller || !key) return 0;

	bool multi;
	int playerStart = 0;
	int playerMax = 0;
	int maxRepeat = 0;
	if (player >= 0)
	{
		playerStart = player;
		playerMax = player + 1;
		multi = false;
	}
	else
	{
		playerStart = 0;
		playerMax = s_iNumConPlayerSlots;
		multi = true;
	}
	for (; playerStart < playerMax; playerStart++)
	{
		switch (key->type)
		{
		case YGS_CONKEY_ANY:
			for (int axis = 0; axis < YGS_CONAXIS_MAX; axis++)
			{
				if (s_aConPlayerSlots[playerStart].axesRepeat[axis] > maxRepeat) maxRepeat = s_aConPlayerSlots[playerStart].axesRepeat[axis];
			}
			for (int button = 0; button < YGS_CONBUTTON_MAX; button++)
			{
				if (s_aConPlayerSlots[playerStart].buttonsRepeat[button] > maxRepeat) maxRepeat = s_aConPlayerSlots[playerStart].buttonsRepeat[button];
			}
			break;
		case YGS_CONKEY_AXIS:
			if (key->index < YGS_CONAXIS_MAX)
			{
					if (multi)
					{
						if (s_aConPlayerSlots[playerStart].axesRepeat[key->index] > maxRepeat) maxRepeat = s_aConPlayerSlots[playerStart].axesRepeat[key->index];
					}
					else return s_aConPlayerSlots[playerStart].axesRepeat[key->index];
			}
			break;
		case YGS_CONKEY_BUTTON:
			if (key->index < YGS_CONBUTTON_MAX)
			{
				if (multi)
				{
					if (s_aConPlayerSlots[playerStart].buttonsRepeat[key->index] > maxRepeat) maxRepeat = s_aConPlayerSlots[playerStart].buttonsRepeat[key->index];
				}
				else return s_aConPlayerSlots[playerStart].buttonsRepeat[key->index];
			}
			break;
		default:
			break;
		}
	}
	return maxRepeat;
}

void YGS2kResetLastActiveConPlayer()
{
	s_iLastActiveConPlayer = -1;
}

int YGS2kGetLastActiveConPlayer()
{
	return s_iLastActiveConPlayer;
}

int YGS2kGetNumConPlayers()
{
	return s_iNumConPlayerSlots;
}

YGS2kEInputType YGS2kGetConPlayerType(const int player)
{
	if (!s_aConPlayerSlots || s_iNumConPlayerSlots == 0 || player < 0 || player >= s_iNumConPlayerSlots || !s_aConPlayerSlots[player].controller) return YGS_INPUT_NULL;

	#ifdef ONLY_INPUT_TYPE
	return ONLY_INPUT_TYPE;
	#else
	switch(YGS2kGetSDLGameControllerType(s_aConPlayerSlots[player].controller))
	{
	default:
	case SDL_CONTROLLER_TYPE_XBOX360:
	case SDL_CONTROLLER_TYPE_XBOXONE:
	case SDL_CONTROLLER_TYPE_VIRTUAL:
	case SDL_CONTROLLER_TYPE_AMAZON_LUNA:
	case SDL_CONTROLLER_TYPE_GOOGLE_STADIA:
	case SDL_CONTROLLER_TYPE_NVIDIA_SHIELD:
		return YGS_INPUT_XBOX;

	case SDL_CONTROLLER_TYPE_PS3:
	case SDL_CONTROLLER_TYPE_PS4:
	case SDL_CONTROLLER_TYPE_PS5:
		return YGS_INPUT_PLAYSTATION;

	case SDL_CONTROLLER_TYPE_NINTENDO_SWITCH_PRO:
	case SDL_CONTROLLER_TYPE_NINTENDO_SWITCH_JOYCON_LEFT:
	case SDL_CONTROLLER_TYPE_NINTENDO_SWITCH_JOYCON_RIGHT:
	case SDL_CONTROLLER_TYPE_NINTENDO_SWITCH_JOYCON_PAIR:
		return YGS_INPUT_NINTENDO;
	}
	#endif
}

bool YGS2kGetConPlayerKeyDesc(const int player, const YGS2kSConKey* const key, const char** text, EButton* button)
{
	if (
		!s_aConPlayerSlots ||
		s_iNumConPlayerSlots == 0 ||
		player < 0 ||
		player >= s_iNumConPlayerSlots ||
		!s_aConPlayerSlots[player].controller ||
		!key ||
		!text ||
		!button
	) {
		return false;
	}

	*text = NULL;
	*button = BTN_NULL;
	const SDL_GameControllerType sdlGameControllerType = YGS2kGetSDLGameControllerType(s_aConPlayerSlots[player].controller);
	switch (key->type)
	{
	case YGS_CONKEY_AXIS:
		switch (key->index)
		{
		case 0:
			*text = "LS";
			*button = BTN_RIGHT;
			break;
		case 1:
			*text = "LS";
			*button = BTN_LEFT;
			break;
		case 2:
			*text = "LS";
			*button = BTN_DOWN;
			break;
		case 3:
			*text = "LS";
			*button = BTN_UP;
			break;
		case 4:
			*text = "RS";
			*button = BTN_RIGHT;
			break;
		case 5:
			*text = "RS";
			*button = BTN_LEFT;
			break;
		case 6:
			*text = "RS";
			*button = BTN_DOWN;
			break;
		case 7:
			*text = "RS";
			*button = BTN_UP;
			break;
		case 8:
			switch(sdlGameControllerType)
			{
			default:
			case SDL_CONTROLLER_TYPE_XBOX360:
			case SDL_CONTROLLER_TYPE_XBOXONE:
			case SDL_CONTROLLER_TYPE_VIRTUAL:
			case SDL_CONTROLLER_TYPE_AMAZON_LUNA:
			case SDL_CONTROLLER_TYPE_NVIDIA_SHIELD:
				*text = "LT";
				break;

			case SDL_CONTROLLER_TYPE_GOOGLE_STADIA:
			case SDL_CONTROLLER_TYPE_PS3:
			case SDL_CONTROLLER_TYPE_PS4:
			case SDL_CONTROLLER_TYPE_PS5:
				*text = "L2";
				break;

			case SDL_CONTROLLER_TYPE_NINTENDO_SWITCH_PRO:
			case SDL_CONTROLLER_TYPE_NINTENDO_SWITCH_JOYCON_PAIR:
				*text = "ZL";
				break;

			case SDL_CONTROLLER_TYPE_NINTENDO_SWITCH_JOYCON_LEFT:
			case SDL_CONTROLLER_TYPE_NINTENDO_SWITCH_JOYCON_RIGHT:
				*text = "SL";
				break;
			}
			break;
		case 9:
			switch(sdlGameControllerType)
			{
			default:
			case SDL_CONTROLLER_TYPE_XBOX360:
			case SDL_CONTROLLER_TYPE_XBOXONE:
			case SDL_CONTROLLER_TYPE_VIRTUAL:
			case SDL_CONTROLLER_TYPE_AMAZON_LUNA:
			case SDL_CONTROLLER_TYPE_NVIDIA_SHIELD:
				*text = "RT";
				break;

			case SDL_CONTROLLER_TYPE_GOOGLE_STADIA:
			case SDL_CONTROLLER_TYPE_PS3:
			case SDL_CONTROLLER_TYPE_PS4:
			case SDL_CONTROLLER_TYPE_PS5:
				*text = "R2";
				break;

			case SDL_CONTROLLER_TYPE_NINTENDO_SWITCH_PRO:
			case SDL_CONTROLLER_TYPE_NINTENDO_SWITCH_JOYCON_PAIR:
				*text = "ZR";
				break;

			case SDL_CONTROLLER_TYPE_NINTENDO_SWITCH_JOYCON_LEFT:
			case SDL_CONTROLLER_TYPE_NINTENDO_SWITCH_JOYCON_RIGHT:
				*text = "SR";
				break;
			}
			break;
		default:
			break;
		}
		break;
	case YGS_CONKEY_BUTTON:
		switch (key->index)
		{
		case SDL_CONTROLLER_BUTTON_BACK:
		case SDL_CONTROLLER_BUTTON_START:
			// "Start" and "Back" are reserved for "Pause" and "Give Up", and may not be remapped.
			break;
		case SDL_CONTROLLER_BUTTON_GUIDE:
			*text = "HOME";
			break;
		case SDL_CONTROLLER_BUTTON_A:
		case SDL_CONTROLLER_BUTTON_B:
		case SDL_CONTROLLER_BUTTON_X:
		case SDL_CONTROLLER_BUTTON_Y:
			*button = BTN_A + key->index;
			break;
		case SDL_CONTROLLER_BUTTON_DPAD_UP:
		case SDL_CONTROLLER_BUTTON_DPAD_DOWN:
		case SDL_CONTROLLER_BUTTON_DPAD_LEFT:
		case SDL_CONTROLLER_BUTTON_DPAD_RIGHT:
			*text = "DPAD";
			*button = BTN_UP + key->index - SDL_CONTROLLER_BUTTON_DPAD_UP;
			break;
		case SDL_CONTROLLER_BUTTON_MISC1:
			switch(sdlGameControllerType)
			{
			case SDL_CONTROLLER_TYPE_XBOX360:
			case SDL_CONTROLLER_TYPE_XBOXONE:
				*text = "SHARE";
				break;
			case SDL_CONTROLLER_TYPE_NINTENDO_SWITCH_PRO:
			case SDL_CONTROLLER_TYPE_NINTENDO_SWITCH_JOYCON_LEFT:
				*text = "CAPTURE";
				break;
			case SDL_CONTROLLER_TYPE_PS5:
			case SDL_CONTROLLER_TYPE_AMAZON_LUNA:
			case SDL_CONTROLLER_TYPE_GOOGLE_STADIA:
			case SDL_CONTROLLER_TYPE_NVIDIA_SHIELD:
				*text = "MICROPHONE";
				break;
			case SDL_CONTROLLER_TYPE_VIRTUAL:
			default:
				*text = "MISC1";
				break;
			}
			break;
		case SDL_CONTROLLER_BUTTON_PADDLE1:
			*text = "P1";
			break;
		case SDL_CONTROLLER_BUTTON_PADDLE2:
			*text = "P2";
			break;
		case SDL_CONTROLLER_BUTTON_PADDLE3:
			*text = "P3";
			break;
		case SDL_CONTROLLER_BUTTON_PADDLE4:
			*text = "P4";
			break;
		case SDL_CONTROLLER_BUTTON_TOUCHPAD:
			*text = "TOUCHPAD";
			break;
		case SDL_CONTROLLER_BUTTON_LEFTSTICK:
			switch(sdlGameControllerType)
			{
			default:
			case SDL_CONTROLLER_TYPE_XBOX360:
			case SDL_CONTROLLER_TYPE_XBOXONE:
			case SDL_CONTROLLER_TYPE_VIRTUAL:
			case SDL_CONTROLLER_TYPE_NVIDIA_SHIELD:
			case SDL_CONTROLLER_TYPE_NINTENDO_SWITCH_PRO:
			case SDL_CONTROLLER_TYPE_NINTENDO_SWITCH_JOYCON_LEFT:
			case SDL_CONTROLLER_TYPE_NINTENDO_SWITCH_JOYCON_RIGHT:
				*text = "LS";
				break;

			case SDL_CONTROLLER_TYPE_AMAZON_LUNA:
			case SDL_CONTROLLER_TYPE_GOOGLE_STADIA:
			case SDL_CONTROLLER_TYPE_PS3:
			case SDL_CONTROLLER_TYPE_PS4:
			case SDL_CONTROLLER_TYPE_PS5:
				*text = "L3";
				break;
			}
			break;
		case SDL_CONTROLLER_BUTTON_RIGHTSTICK:
			switch(sdlGameControllerType)
			{
			default:
			case SDL_CONTROLLER_TYPE_XBOX360:
			case SDL_CONTROLLER_TYPE_XBOXONE:
			case SDL_CONTROLLER_TYPE_VIRTUAL:
			case SDL_CONTROLLER_TYPE_NVIDIA_SHIELD:
			case SDL_CONTROLLER_TYPE_NINTENDO_SWITCH_PRO:
				*text = "RS";
				break;

			case SDL_CONTROLLER_TYPE_AMAZON_LUNA:
			case SDL_CONTROLLER_TYPE_GOOGLE_STADIA:
			case SDL_CONTROLLER_TYPE_PS3:
			case SDL_CONTROLLER_TYPE_PS4:
			case SDL_CONTROLLER_TYPE_PS5:
				*text = "R3";
				break;
			}
			break;
		case SDL_CONTROLLER_BUTTON_LEFTSHOULDER:
			switch(sdlGameControllerType)
			{
			default:
			case SDL_CONTROLLER_TYPE_XBOX360:
			case SDL_CONTROLLER_TYPE_XBOXONE:
			case SDL_CONTROLLER_TYPE_VIRTUAL:
			case SDL_CONTROLLER_TYPE_AMAZON_LUNA:
			case SDL_CONTROLLER_TYPE_NVIDIA_SHIELD:
				*text = "LB";
				break;

			case SDL_CONTROLLER_TYPE_GOOGLE_STADIA:
			case SDL_CONTROLLER_TYPE_PS3:
			case SDL_CONTROLLER_TYPE_PS4:
			case SDL_CONTROLLER_TYPE_PS5:
				*text = "L1";
				break;
			case SDL_CONTROLLER_TYPE_NINTENDO_SWITCH_PRO:
				*text = "L";
				break;
			}
			break;
		case SDL_CONTROLLER_BUTTON_RIGHTSHOULDER:
			switch(sdlGameControllerType)
			{
			default:
			case SDL_CONTROLLER_TYPE_XBOX360:
			case SDL_CONTROLLER_TYPE_XBOXONE:
			case SDL_CONTROLLER_TYPE_VIRTUAL:
			case SDL_CONTROLLER_TYPE_AMAZON_LUNA:
			case SDL_CONTROLLER_TYPE_NVIDIA_SHIELD:
				*text = "RB";
				break;

			case SDL_CONTROLLER_TYPE_GOOGLE_STADIA:
			case SDL_CONTROLLER_TYPE_PS3:
			case SDL_CONTROLLER_TYPE_PS4:
			case SDL_CONTROLLER_TYPE_PS5:
				*text = "R1";
				break;
			case SDL_CONTROLLER_TYPE_NINTENDO_SWITCH_PRO:
				*text = "R";
				break;
			}
			break;
		default:
			break;
		}
		break;
	default:
		break;
	}
	return *text != NULL || *button != BTN_NULL;
}
#endif

bool YGS2kInputsOpen()
{
	#ifdef ENABLE_LINUXGPIO
	const char* chipName = "gpiochip0";

	s_pGPIOChip = gpiod_chip_open_by_name(chipName);
	if ( !s_pGPIOChip )
	{
		fprintf(stderr, "Failed opening GPIO chip \"%s\". Continuing without GPIO input support.\n", chipName);
	}
	else
	{
		memset(s_iGPIORepeat, 0, sizeof(s_iGPIORepeat));
		memset(s_apGPIOLines, 0, sizeof(s_apGPIOLines));
		if (
			!(s_apGPIOLines[0] = gpiod_chip_get_line(s_pGPIOChip,  5)) ||
			!(s_apGPIOLines[1] = gpiod_chip_get_line(s_pGPIOChip, 13)) ||
			!(s_apGPIOLines[2] = gpiod_chip_get_line(s_pGPIOChip,  6)) ||
			!(s_apGPIOLines[3] = gpiod_chip_get_line(s_pGPIOChip, 12)) ||
			!(s_apGPIOLines[4] = gpiod_chip_get_line(s_pGPIOChip, 19)) ||
			!(s_apGPIOLines[5] = gpiod_chip_get_line(s_pGPIOChip, 16)) ||
			!(s_apGPIOLines[6] = gpiod_chip_get_line(s_pGPIOChip, 26)) ||
			!(s_apGPIOLines[7] = gpiod_chip_get_line(s_pGPIOChip, 20)) ||
			!(s_apGPIOLines[8] = gpiod_chip_get_line(s_pGPIOChip, 21)) ||
			!(s_apGPIOLines[9] = gpiod_chip_get_line(s_pGPIOChip,  4))
		)
		{
			for (int line = 0; line < 10; line++)
			{
				if (s_apGPIOLines[line])
				{
					gpiod_line_release(s_apGPIOLines[line]);
					s_apGPIOLines[line] = NULL;
				}
			}
			gpiod_chip_close(s_pGPIOChip);
			s_pGPIOChip = NULL;
			fprintf(stderr, "Failed opening GPIO lines. Continuing without GPIO input support.\n");
		}
		else
		{
			for (int line = 0; line < 10; line++)
			{
				if (gpiod_line_request_input(s_apGPIOLines[line], "input") < 0)
				{
					for (line = 0; line < 10; line++)
					{
						gpiod_line_release(s_apGPIOLines[line]);
						s_apGPIOLines[line] = NULL;
					}
					gpiod_chip_close(s_pGPIOChip);
					s_pGPIOChip = NULL;
					fprintf(stderr, "Failed setting GPIO lines for input. Continuing without GPIO input support.\n");
					break;
				}
			}
		}
	}
	#endif

	#ifdef ENABLE_KEYBOARD
	/* キーリピートバッファ初期化 */
	memset(s_iKeyRepeat, 0, sizeof(s_iKeyRepeat));
	#endif

	/* パッドの初期化 */

	#ifdef ENABLE_JOYSTICK
	if (!YGS2kJoysOpen()) {
		return false;
	}
	#endif

	#ifdef ENABLE_GAME_CONTROLLER
	OpenGameControllerDB();
	SDL_SetHint(SDL_HINT_GAMECONTROLLER_USE_BUTTON_LABELS, "0");
	#endif

	return true;
}

void YGS2kInputsClose()
{
	#ifdef ENABLE_LINUX_GPIO
	if ( s_pGPIOChip )
	{
		for ( int i = 0 ; i < 10 ; i ++ )
		{
			gpiod_line_release(s_apGPIOLines[i]);
		}
		gpiod_chip_close(s_pGPIOChip);

		s_pGPIOChip = NULL;
		memset(s_apGPIOLines, 0, sizeof(s_apGPIOLines));
	}
	#endif

	#ifdef ENABLE_KEYBOARD
	memset(s_iKeyRepeat, 0, sizeof(s_iKeyRepeat));
	#endif

	#ifdef ENABLE_JOYSTICK
	YGS2kJoysClose();
	#endif

	#ifdef ENABLE_GAME_CONTROLLER
	YGS2kConsClose();
	#endif
}

void YGS2kInputsUpdate()
{
	#ifdef ENABLE_LINUX_GPIO
	for (int line = 0; line < 10; line++)
	{
		if (gpiod_line_get_value(s_apGPIOLines[line]) == 1)
		{
			if (++s_iGPIORepeat[line] == 1) LastControllerType = YGS_INPUT_LINUXGPIO;
		}
		else
		{
			s_iGPIORepeat[line] = 0;
		}
	}
	#endif

	#ifdef ENABLE_KEYBOARD
	int numKeys = 0;
	const Uint8* keyStates = SDL_GetKeyboardState(&numKeys);

	for (int i = 0; i < YGS_KEY_MAX; i++)
	{
		if (i < numKeys && keyStates[i] == SDL_PRESSED)
		{
			if (++s_iKeyRepeat[i] == 1) YGS2kLastInputType = YGS_INPUT_KEYBOARD;
		}
		else
		{
			s_iKeyRepeat[i] = 0;
		}
	}
	#endif

	#ifdef ENABLE_JOYSTICK
	YGS2kJoyInputsUpdate();
	#endif
	
	#ifdef ENABLE_GAME_CONTROLLER
	YGS2kConInputsUpdate();
	#endif
}
