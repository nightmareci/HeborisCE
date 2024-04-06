#include "main_sdl/include.h"
#include "ygs2kfunc.h"
#include "ygs2kprivate.h"

#ifdef ENABLE_LINUX_GPIO
static int			s_iGPIORepeat[10];
static struct gpiod_chip	*s_pGPIOChip;
static struct gpiod_line	*s_pGPIOLines[10];
#endif

#ifdef ENABLE_KEYBOARD
static int			s_iKeyRepeat[YGS_KEY_MAX];
#endif

#ifdef ENABLE_JOYSTICK
typedef struct YGS2kSJoystick
{
	SDL_Joystick* device;
	int numAxes;
	int numHats;
	int numButtons;
	int* axesRepeat;
	int* hatsRepeat;
	int* buttonsRepeat;
} YGS2kSJoystick;
static YGS2kSJoystick* s_aJoysticks = NULL;
static int s_iNumJoysticks = -1;
#endif

#ifdef ENABLE_GAME_CONTROLLER
#include "main_sdl/gamecontrollerdb.h"
typedef struct YGS2kSGameController
{
	SDL_GameController* device;
	int axesRepeat[YGS_CONAXIS_MAX];
	int buttonsRepeat[YGS_CONBUTTON_MAX];
} YGS2kSGameController;
static YGS2kSGameController* s_aGameControllers = NULL;
static int s_iNumGameControllers = -1;
static int s_iLastGameControllerIndex = -1;
#endif

YGS2kEControllerType YGS2kGetLastControllerType ()
{
	#ifdef ONLY_CONTROLLER_TYPE
	return ONLY_CONTROLLER_TYPE;
	#else
	return YGS2kLastControllerType;
	#endif
}

#ifdef ENABLE_LINUX_GPIO
int YGS2kIsPushGPIO ( int key )
{
	return key >= 0 && key < NUMBTNS && s_iGPIORepeat[key] == 1 ? 1 : 0;
}

int YGS2kIsPressGPIO ( int key )
{
	return key >= 0 && key < NUMBTNS && s_iGPIORepeat[key] != 0 ? 1 : 0;
}

int YGS2kGetGPIORepeat( int key )
{
	return key >= 0 && key < NUMBTNS ? s_iGPIORepeat[key] : -1;
}
#endif

#ifdef ENABLE_KEYBOARD
int YGS2kIsPushKey (int key)
{
	return key >= 0 && key < YGS_KEY_MAX && s_iKeyRepeat[key] == 1 ? 1 : 0;
}

int YGS2kIsPressKey (int key)
{
	return key >= 0 && key < YGS_KEY_MAX && s_iKeyRepeat[key] != 0 ? 1 : 0;
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
void YGS2kJoyClose ()
{
	if (s_aJoysticks)
	{
		for (int joyIndex = 0; joyIndex < s_iNumJoysticks; joyIndex++)
		{
			if (s_aJoysticks[joyIndex].device && SDL_JoystickGetAttached(s_aJoysticks[joyIndex].device)) SDL_JoystickClose(s_aJoysticks[joyIndex].device);
			if (s_aJoysticks[joyIndex].axesRepeat) free(s_aJoysticks[joyIndex].axesRepeat);
			if (s_aJoysticks[joyIndex].hatsRepeat) free(s_aJoysticks[joyIndex].hatsRepeat);
			if (s_aJoysticks[joyIndex].buttonsRepeat) free(s_aJoysticks[joyIndex].buttonsRepeat);
		}
		free(s_aJoysticks);
	}
	s_aJoysticks = NULL;
	s_iNumJoysticks = -1;
}

int YGS2kJoyOpen ()
{
	if ((s_iNumJoysticks = SDL_NumJoysticks()) > 0)
	{
		if (!(s_aJoysticks = (YGS2kSJoystick*)calloc((size_t)s_iNumJoysticks, sizeof(YGS2kSJoystick))))
		{
			s_iNumJoysticks = -1;
			return 0;
		}

		for (int joyIndex = 0; joyIndex < s_iNumJoysticks; joyIndex++)
		{
			#ifdef ENABLE_GAME_CONTROLLER
			if (!YGS2kIsGameController(joyIndex))
			{
			#endif
				bool fail = false;
				if (!(s_aJoysticks[joyIndex].device = SDL_JoystickOpen(joyIndex))) fail = true;
				else if (
					(s_aJoysticks[joyIndex].numAxes = SDL_JoystickNumAxes(s_aJoysticks[joyIndex].device)) >= 0 &&
					(s_aJoysticks[joyIndex].numHats = SDL_JoystickNumHats(s_aJoysticks[joyIndex].device)) >= 0 &&
					(s_aJoysticks[joyIndex].numButtons = SDL_JoystickNumButtons(s_aJoysticks[joyIndex].device)) >= 0
				)
				{
					if (s_aJoysticks[joyIndex].numAxes > 0 && !(s_aJoysticks[joyIndex].axesRepeat = (int*)calloc((size_t)s_aJoysticks[joyIndex].numAxes * 2, sizeof(int)))) fail = true;
					if (s_aJoysticks[joyIndex].numHats > 0 && !(s_aJoysticks[joyIndex].hatsRepeat = (int*)calloc((size_t)s_aJoysticks[joyIndex].numHats * 4, sizeof(int)))) fail = true;
					if (s_aJoysticks[joyIndex].numButtons > 0 && !(s_aJoysticks[joyIndex].buttonsRepeat = (int*)calloc((size_t)s_aJoysticks[joyIndex].numButtons, sizeof(int)))) fail = true;
				}
				if (fail)
				{
					s_iNumJoysticks = joyIndex + !!s_aJoysticks[joyIndex].device;
					YGS2kJoyClose();
					s_iNumJoysticks = -1;
					return 0;
				}
			#ifdef ENABLE_GAME_CONTROLLER
			}
			#endif
		}
		return 1;
	}
	else if (s_iNumJoysticks == 0) return 1;
	else return 0;
}

void YGS2kJoyInput ()
{
	if (!s_aJoysticks) return;

	for (int joyIndex = 0; joyIndex < s_iNumJoysticks; joyIndex++)
	{
		#ifdef ENABLE_GAME_CONTROLLER
		if (YGS2kIsGameController(joyIndex)) continue;
		#endif
		if (!SDL_JoystickGetAttached(s_aJoysticks[joyIndex].device))
		{
			continue;
		}

		const YGS2kSJoyGUID checkGUID = YGS2kGetJoyGUID(joyIndex);
		const YGS2kSJoyGUID zeroGUID = { 0 };
		if (memcmp(checkGUID.data, zeroGUID.data, sizeof(checkGUID.data)) == 0) continue;

		for (int axis = 0; axis < s_aJoysticks[joyIndex].numAxes; axis++)
		{
			const int value = SDL_JoystickGetAxis(s_aJoysticks[joyIndex].device, axis);

			if (value < -YGS_DEADZONE_MAX)
			{
				if (++s_aJoysticks[joyIndex].axesRepeat[axis*2 + 0] == 1) YGS2kLastControllerType = YGS_CONTROLLER_JOYSTICK;
			}
			else
			{
				s_aJoysticks[joyIndex].axesRepeat[axis*2 + 0] = 0;
			}

			if (value > +YGS_DEADZONE_MAX)
			{
				if (++s_aJoysticks[joyIndex].axesRepeat[axis*2 + 1] == 1) YGS2kLastControllerType = YGS_CONTROLLER_JOYSTICK;
			}
			else
			{
				s_aJoysticks[joyIndex].axesRepeat[axis*2 + 1] = 0;
			}
		}

		for (int hat = 0; hat < s_aJoysticks[joyIndex].numHats; hat++)
		{
			Uint8 value = SDL_JoystickGetHat(s_aJoysticks[joyIndex].device, hat);
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
					if (++s_aJoysticks[joyIndex].hatsRepeat[hat*4 + valueIndex] == 1) YGS2kLastControllerType = YGS_CONTROLLER_JOYSTICK;
				}
				else
				{
					s_aJoysticks[joyIndex].hatsRepeat[hat*4 + valueIndex] = 0;
				}
			}
		}

		for (int button = 0; button < s_aJoysticks[joyIndex].numButtons; button++)
		{
			if (SDL_JoystickGetButton(s_aJoysticks[joyIndex].device, button))
			{
				if (++s_aJoysticks[joyIndex].buttonsRepeat[button] == 1) YGS2kLastControllerType = YGS_CONTROLLER_JOYSTICK;
			}
			else
			{
				s_aJoysticks[joyIndex].buttonsRepeat[button] = 0;
			}
		}
	}
}

int YGS2kIsPushJoyKey ( const YGS2kSJoyKey* const key )
{
	if (!s_aJoysticks || s_iNumJoysticks <= 0 || key == NULL || key->index >= s_iNumJoysticks) return 0;

	int joyIndex = 0;
	int joyIndexMax = 0;
	if (key->index >= 0)
	{
		YGS2kSJoyGUID checkGUID = YGS2kGetJoyGUID(key->index);
		YGS2kSJoyGUID zeroGUID = { 0 };
		if (memcmp(checkGUID.data, zeroGUID.data, sizeof(checkGUID.data)) != 0 && memcmp(key->guid.data, checkGUID.data, sizeof(checkGUID.data)) == 0)
		{
			joyIndex = key->index;
			joyIndexMax = key->index + 1;
		}
	}
	else
	{
		joyIndex = 0;
		joyIndexMax = s_iNumJoysticks;
	}
	for (; joyIndex < joyIndexMax; joyIndex++)
	{
		switch (key->type)
		{
		case YGS_JOYKEY_ANY:
			for (int axis = 0; axis < s_aJoysticks[joyIndex].numAxes; axis++)
			{
				if (s_aJoysticks[joyIndex].axesRepeat[axis*2 + 0] == 1) return 1;
				if (s_aJoysticks[joyIndex].axesRepeat[axis*2 + 1] == 1) return 1;
			}
			for (int hat = 0; hat < s_aJoysticks[joyIndex].numHats; hat++)
			{
				if (s_aJoysticks[joyIndex].hatsRepeat[hat] == 1) return 1;
			}
			for (int button = 0; button < s_aJoysticks[joyIndex].numButtons; button++)
			{
				if (s_aJoysticks[joyIndex].buttonsRepeat[button] == 1) return 1;
			}
			break;
		case YGS_JOYKEY_AXIS:
			if (key->setting.index < s_aJoysticks[joyIndex].numAxes)
			{
				if (key->setting.value == -YGS_DEADZONE_MAX)
				{
					return s_aJoysticks[joyIndex].axesRepeat[key->setting.index * 2 + 0] == 1 ? 1 : 0;
				}
				else if (key->setting.value == +YGS_DEADZONE_MAX)
				{
					return s_aJoysticks[joyIndex].axesRepeat[key->setting.index * 2 + 1] == 1 ? 1 : 0;
				}
			}
			break;
		case YGS_JOYKEY_HAT:
			if (key->setting.index < s_aJoysticks[joyIndex].numHats)
			{
				switch (key->setting.value)
				{
				case SDL_HAT_LEFT:
					return s_aJoysticks[joyIndex].hatsRepeat[key->setting.index * 4 + 0] == 1 ? 1 : 0;
				case SDL_HAT_RIGHT:
					return s_aJoysticks[joyIndex].hatsRepeat[key->setting.index * 4 + 1] == 1 ? 1 : 0;
				case SDL_HAT_UP:
					return s_aJoysticks[joyIndex].hatsRepeat[key->setting.index * 4 + 2] == 1 ? 1 : 0;
				case SDL_HAT_DOWN:
					return s_aJoysticks[joyIndex].hatsRepeat[key->setting.index * 4 + 3] == 1 ? 1 : 0;
				default:
					break;
				}
			}
			break;
		case YGS_JOYKEY_BUTTON:
			if (key->setting.button < s_aJoysticks[joyIndex].numButtons) return s_aJoysticks[joyIndex].buttonsRepeat[key->setting.button] == 1 ? 1 : 0;
			break;
		default:
			break;
		}
	}
	return 0;
}

int YGS2kIsPressJoyKey ( const YGS2kSJoyKey* const key )
{
	if (!s_aJoysticks || s_iNumJoysticks <= 0 || key == NULL || key->index >= s_iNumJoysticks) return 0;

	int joyIndex = 0;
	int joyIndexMax = 0;
	if (key->index >= 0)
	{
		YGS2kSJoyGUID checkGUID = YGS2kGetJoyGUID(key->index);
		YGS2kSJoyGUID zeroGUID = { 0 };
		if (memcmp(checkGUID.data, zeroGUID.data, sizeof(checkGUID.data)) != 0 && memcmp(key->guid.data, checkGUID.data, sizeof(checkGUID.data)) == 0)
		{
			joyIndex = key->index;
			joyIndexMax = key->index + 1;
		}
	}
	else
	{
		joyIndex = 0;
		joyIndexMax = s_iNumJoysticks;
	}
	for (; joyIndex < joyIndexMax; joyIndex++)
	{
		switch (key->type)
		{
		case YGS_JOYKEY_ANY:
			for (int axis = 0; axis < s_aJoysticks[joyIndex].numAxes; axis++)
			{
				if (s_aJoysticks[joyIndex].axesRepeat[axis*2 + 0] != 0) return 1;
				if (s_aJoysticks[joyIndex].axesRepeat[axis*2 + 1] != 0) return 1;
			}
			for (int hat = 0; hat < s_aJoysticks[joyIndex].numHats; hat++)
			{
				if (s_aJoysticks[joyIndex].hatsRepeat[hat] != 0) return 1;
			}
			for (int button = 0; button < s_aJoysticks[joyIndex].numButtons; button++)
			{
				if (s_aJoysticks[joyIndex].buttonsRepeat[button] != 0) return 1;
			}
			break;
		case YGS_JOYKEY_AXIS:
			if (key->setting.index < s_aJoysticks[joyIndex].numAxes)
			{
				if (key->setting.value == -YGS_DEADZONE_MAX)
				{
					return s_aJoysticks[joyIndex].axesRepeat[key->setting.index * 2 + 0] != 0 ? 1 : 0;
				}
				else if (key->setting.value == +YGS_DEADZONE_MAX)
				{
					return s_aJoysticks[joyIndex].axesRepeat[key->setting.index * 2 + 1] != 0 ? 1 : 0;
				}
			}
			break;
		case YGS_JOYKEY_HAT:
			if (key->setting.index < s_aJoysticks[joyIndex].numHats)
			{
				switch (key->setting.value)
				{
				case SDL_HAT_LEFT:
					return s_aJoysticks[joyIndex].hatsRepeat[key->setting.index * 4 + 0] != 0 ? 1 : 0;
				case SDL_HAT_RIGHT:
					return s_aJoysticks[joyIndex].hatsRepeat[key->setting.index * 4 + 1] != 0 ? 1 : 0;
				case SDL_HAT_UP:
					return s_aJoysticks[joyIndex].hatsRepeat[key->setting.index * 4 + 2] != 0 ? 1 : 0;
				case SDL_HAT_DOWN:
					return s_aJoysticks[joyIndex].hatsRepeat[key->setting.index * 4 + 3] != 0 ? 1 : 0;
				default:
					break;
				}
			}
			break;
		case YGS_JOYKEY_BUTTON:
			if (key->setting.button < s_aJoysticks[joyIndex].numButtons) return s_aJoysticks[joyIndex].buttonsRepeat[key->setting.button] != 0 ? 1 : 0;
			break;
		default:
			break;
		}
	}
	return 0;
}

int YGS2kGetJoyKeyRepeat ( const YGS2kSJoyKey* const key )
{
	if (!s_aJoysticks || s_iNumJoysticks <= 0 || key == NULL || key->index >= s_iNumJoysticks) return 0;

	bool multi;
	int joyIndex = 0;
	int joyIndexMax = 0;
	int maxRepeat = 0;
	if (key->index >= 0)
	{
		YGS2kSJoyGUID checkGUID = YGS2kGetJoyGUID(key->index);
		YGS2kSJoyGUID zeroGUID = { 0 };
		if (memcmp(checkGUID.data, zeroGUID.data, sizeof(checkGUID.data)) != 0 && memcmp(key->guid.data, checkGUID.data, sizeof(checkGUID.data)) == 0)
		{
			joyIndex = key->index;
			joyIndexMax = key->index + 1;
			multi = false;
		}
	}
	else
	{
		joyIndex = 0;
		joyIndexMax = s_iNumJoysticks;
		multi = true;
	}
	for (; joyIndex < joyIndexMax; joyIndex++)
	{
		switch (key->type)
		{
		case YGS_JOYKEY_ANY:
			for (int axis = 0; axis < s_aJoysticks[joyIndex].numAxes; axis++)
			{
				if (s_aJoysticks[joyIndex].axesRepeat[axis*2 + 0] > maxRepeat) maxRepeat = s_aJoysticks[joyIndex].axesRepeat[axis*2 + 0];
				if (s_aJoysticks[joyIndex].axesRepeat[axis*2 + 1] > maxRepeat) maxRepeat = s_aJoysticks[joyIndex].axesRepeat[axis*2 + 1];
			}
			for (int hat = 0; hat < s_aJoysticks[joyIndex].numHats; hat++)
			{
				if (s_aJoysticks[joyIndex].hatsRepeat[hat] > maxRepeat) maxRepeat = s_aJoysticks[joyIndex].hatsRepeat[hat];
			}
			for (int button = 0; button < s_aJoysticks[joyIndex].numButtons; button++)
			{
				if (s_aJoysticks[joyIndex].buttonsRepeat[button] > maxRepeat) maxRepeat = s_aJoysticks[joyIndex].buttonsRepeat[button];
			}
			break;
		case YGS_JOYKEY_AXIS:
			if (key->setting.index < s_aJoysticks[joyIndex].numAxes)
			{
				if (key->setting.value == -YGS_DEADZONE_MAX)
				{
					if (multi)
					{
						if (s_aJoysticks[joyIndex].axesRepeat[key->setting.index * 2 + 0] > maxRepeat) maxRepeat = s_aJoysticks[joyIndex].axesRepeat[key->setting.index * 2 + 0];
					}
					else return s_aJoysticks[joyIndex].axesRepeat[key->setting.index * 2 + 0];
				}
				else if (key->setting.value == +YGS_DEADZONE_MAX)
				{
					if (multi)
					{
						if (s_aJoysticks[joyIndex].axesRepeat[key->setting.index * 2 + 1] > maxRepeat) maxRepeat = s_aJoysticks[joyIndex].axesRepeat[key->setting.index * 2 + 1];
					}
					else return s_aJoysticks[joyIndex].axesRepeat[key->setting.index * 2 + 1];
				}
			}
			break;
		case YGS_JOYKEY_HAT:
			if (key->setting.index < s_aJoysticks[joyIndex].numHats)
			{
				switch (key->setting.value)
				{
				case SDL_HAT_LEFT:
					if (multi)
					{
						if (s_aJoysticks[joyIndex].hatsRepeat[key->setting.index * 4 + 0] > maxRepeat) maxRepeat = s_aJoysticks[joyIndex].hatsRepeat[key->setting.index * 4 + 0];
					}
					else return s_aJoysticks[joyIndex].hatsRepeat[key->setting.index * 4 + 0];
				case SDL_HAT_RIGHT:
					if (multi)
					{
						if (s_aJoysticks[joyIndex].hatsRepeat[key->setting.index * 4 + 1] > maxRepeat) maxRepeat = s_aJoysticks[joyIndex].hatsRepeat[key->setting.index * 4 + 1];
					}
					else return s_aJoysticks[joyIndex].hatsRepeat[key->setting.index * 4 + 1];
				case SDL_HAT_UP:
					if (multi)
					{
						if (s_aJoysticks[joyIndex].hatsRepeat[key->setting.index * 4 + 2] > maxRepeat) maxRepeat = s_aJoysticks[joyIndex].hatsRepeat[key->setting.index * 4 + 2];
					}
					else return s_aJoysticks[joyIndex].hatsRepeat[key->setting.index * 4 + 2];
				case SDL_HAT_DOWN:
					if (multi)
					{
						if (s_aJoysticks[joyIndex].hatsRepeat[key->setting.index * 4 + 3] > maxRepeat) maxRepeat = s_aJoysticks[joyIndex].hatsRepeat[key->setting.index * 4 + 3];
					}
					else return s_aJoysticks[joyIndex].hatsRepeat[key->setting.index * 4 + 3];
				default:
					break;
				}
			}
			break;
		case YGS_JOYKEY_BUTTON:
			if (key->setting.button < s_aJoysticks[joyIndex].numButtons)
			{
				if (multi)
				{
					if (s_aJoysticks[joyIndex].buttonsRepeat[key->setting.button] > maxRepeat) maxRepeat = s_aJoysticks[joyIndex].buttonsRepeat[key->setting.button];
				}
				else return s_aJoysticks[joyIndex].buttonsRepeat[key->setting.button];
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
	return s_iNumJoysticks;
}

int YGS2kGetNumJoys()
{
	#ifdef ENABLE_GAME_CONTROLLER
	int numJoys = 0;
	for (int joyIndex = 0; joyIndex < s_iNumJoysticks; joyIndex++)
	{
		if (!YGS2kIsGameController(joyIndex)) numJoys++;
	}
	return numJoys;
	#else
	return s_iNumJoysticks;
	#endif
}

YGS2kSJoyGUID YGS2kGetJoyGUID(int joyIndex)
{
	if (s_iNumJoysticks <= 0 || joyIndex >= s_iNumJoysticks
	#ifdef ENABLE_GAME_CONTROLLER
		|| YGS2kIsGameController(joyIndex)
	#endif
	) return (YGS2kSJoyGUID) { 0 };

	SDL_JoystickGUID sdlGUID;
	sdlGUID = SDL_JoystickGetGUID(s_aJoysticks[joyIndex].device);

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

int YGS2kGetMaxJoyAxis(int joyIndex)
{
	if (s_iNumJoysticks > 0 && joyIndex < s_iNumJoysticks)
	{
		#ifdef ENABLE_GAME_CONTROLLER
		if (!YGS2kIsGameController(joyIndex))
		{
			return s_aJoysticks[joyIndex].numAxes;
		}
		else
		{
			return -1;
		}
		#else
		return s_aJoysticks[joyIndex].numAxes;
		#endif
	}
	else
	{
		return -1;
	}
}

int YGS2kGetMaxJoyHat(int joyIndex)
{
	if (s_iNumJoysticks > 0 && joyIndex < s_iNumJoysticks)
	{
		#ifdef ENABLE_GAME_CONTROLLER
		if (!YGS2kIsGameController(joyIndex))
		{
			return s_aJoysticks[joyIndex].numHats;
		}
		else
		{
			return -1;
		}
		#else
		return s_aJoysticks[joyIndex].numHats;
		#endif
	}
	else
	{
		return -1;
	}
}

int YGS2kGetMaxJoyButton(int joyIndex)
{
	if (s_iNumJoysticks > 0 && joyIndex < s_iNumJoysticks)
	{
		#ifdef ENABLE_GAME_CONTROLLER
		if (!YGS2kIsGameController(joyIndex))
		{
			return s_aJoysticks[joyIndex].numButtons;
		}
		else
		{
			return -1;
		}
		#else
		return s_aJoysticks[joyIndex].numButtons;
		#endif
	}
	else
	{
		return -1;
	}
}
#endif

#ifdef ENABLE_GAME_CONTROLLER
void YGS2kConClose()
{
	for (int conIndex = 0; conIndex < s_iNumGameControllers; conIndex++)
	{
		if (SDL_GameControllerGetAttached(s_aGameControllers[conIndex].device))
		{
			SDL_GameControllerClose(s_aGameControllers[conIndex].device);
		}
	}
	free(s_aGameControllers);
	s_aGameControllers = NULL;
	s_iNumGameControllers = -1;
}

int YGS2kConOpen()
{
	const int numJoysticks = SDL_NumJoysticks();
	if (numJoysticks < 0)
	{
		s_iNumGameControllers = -1;
		return -1;
	}

	s_iNumGameControllers = 0;
	for (int joyIndex = 0; joyIndex < numJoysticks; joyIndex++)
	{
		if (YGS2kIsGameController(joyIndex)) s_iNumGameControllers++;
	}
	if (s_iNumGameControllers == 0) return 1;

	if ((s_aGameControllers = calloc((size_t)s_iNumGameControllers, sizeof(YGS2kSGameController))) == NULL)
	{
		s_iNumGameControllers = -1;
		return -1;
	}

	for (int joyIndex = 0, conIndex = 0; joyIndex < numJoysticks; joyIndex++)
	{
		if (YGS2kIsGameController(joyIndex))
		{
			if (!(s_aGameControllers[conIndex].device = SDL_GameControllerOpen(joyIndex)))
			{
				s_iNumGameControllers = conIndex;
				YGS2kConClose();
				s_iNumGameControllers = -1;
				return -1;
			}
			else
			{
				const char* name = SDL_GameControllerName(s_aGameControllers[conIndex].device);
				if (name == NULL) name = "???";
				printf("Controller %d name: %s\n", conIndex, name);
				conIndex++;
			}
		}
	}
	return 1;
}

static SDL_GameControllerType YGS2kGetSDLConType(SDL_GameController* const device)
{
	#ifdef ONLY_SDL_CONTROLLER_TYPE
	return ONLY_SDL_CONTROLLER_TYPE;
	#else
	return SDL_GameControllerGetType(device);
	#endif
}

void YGS2kConInput()
{
	if (!s_aGameControllers) return;

	for (int conIndex = 0; conIndex < s_iNumGameControllers; conIndex++)
	{
		if (!SDL_GameControllerGetAttached(s_aGameControllers[conIndex].device)) continue;

		YGS2kEControllerType controllerType;
		switch (YGS2kGetSDLConType(s_aGameControllers[conIndex].device))
		{
		default:
		case SDL_CONTROLLER_TYPE_XBOX360:
		case SDL_CONTROLLER_TYPE_XBOXONE:
		case SDL_CONTROLLER_TYPE_AMAZON_LUNA:
		case SDL_CONTROLLER_TYPE_GOOGLE_STADIA:
			controllerType = YGS_CONTROLLER_XBOX;
			break;

		case SDL_CONTROLLER_TYPE_PS3:
		case SDL_CONTROLLER_TYPE_PS4:
		case SDL_CONTROLLER_TYPE_PS5:
			controllerType = YGS_CONTROLLER_PLAYSTATION;
			break;

		case SDL_CONTROLLER_TYPE_NINTENDO_SWITCH_PRO:
			controllerType = YGS_CONTROLLER_NINTENDO;
			break;
		}
		Sint16 axisValue;

		axisValue = SDL_GameControllerGetAxis(s_aGameControllers[conIndex].device, SDL_CONTROLLER_AXIS_LEFTX);
		if (axisValue > 0 && axisValue > +YGS_DEADZONE_MAX)
		{
			if (++s_aGameControllers[conIndex].axesRepeat[0] == 1)
			{
				YGS2kLastControllerType = controllerType;
				s_iLastGameControllerIndex = conIndex;
			}
			s_aGameControllers[conIndex].axesRepeat[1] = 0;
		}
		else if (axisValue < 0 && axisValue < -YGS_DEADZONE_MAX)
		{
			s_aGameControllers[conIndex].axesRepeat[0] = 0;
			if (++s_aGameControllers[conIndex].axesRepeat[1] == 1)
			{
				YGS2kLastControllerType = controllerType;
				s_iLastGameControllerIndex = conIndex;
			}
		}
		else
		{
			s_aGameControllers[conIndex].axesRepeat[0] = 0;
			s_aGameControllers[conIndex].axesRepeat[1] = 0;
		}

		axisValue = SDL_GameControllerGetAxis(s_aGameControllers[conIndex].device, SDL_CONTROLLER_AXIS_LEFTY);
		if (axisValue > 0 && axisValue > +YGS_DEADZONE_MAX)
		{
			if (++s_aGameControllers[conIndex].axesRepeat[2] == 1)
			{
				YGS2kLastControllerType = controllerType;
				s_iLastGameControllerIndex = conIndex;
			}
			s_aGameControllers[conIndex].axesRepeat[3] = 0;
		}
		else if (axisValue < 0 && axisValue < -YGS_DEADZONE_MAX)
		{
			s_aGameControllers[conIndex].axesRepeat[2] = 0;
			if (++s_aGameControllers[conIndex].axesRepeat[3] == 1)
			{
				YGS2kLastControllerType = controllerType;
				s_iLastGameControllerIndex = conIndex;
			}
		}
		else
		{
			s_aGameControllers[conIndex].axesRepeat[2] = 0;
			s_aGameControllers[conIndex].axesRepeat[3] = 0;
		}

		axisValue = SDL_GameControllerGetAxis(s_aGameControllers[conIndex].device, SDL_CONTROLLER_AXIS_RIGHTX);
		if (axisValue > 0 && axisValue > +YGS_DEADZONE_MAX)
		{
			if (++s_aGameControllers[conIndex].axesRepeat[4] == 1)
			{
				YGS2kLastControllerType = controllerType;
				s_iLastGameControllerIndex = conIndex;
			}
			s_aGameControllers[conIndex].axesRepeat[5] = 0;
		}
		else if (axisValue < 0 && axisValue < -YGS_DEADZONE_MAX)
		{
			s_aGameControllers[conIndex].axesRepeat[4] = 0;
			if (++s_aGameControllers[conIndex].axesRepeat[5] == 1)
			{
				YGS2kLastControllerType = controllerType;
				s_iLastGameControllerIndex = conIndex;
			}
		}
		else
		{
			s_aGameControllers[conIndex].axesRepeat[4] = 0;
			s_aGameControllers[conIndex].axesRepeat[5] = 0;
		}

		axisValue = SDL_GameControllerGetAxis(s_aGameControllers[conIndex].device, SDL_CONTROLLER_AXIS_RIGHTY);
		if (axisValue > 0 && axisValue > +YGS_DEADZONE_MAX)
		{
			if (++s_aGameControllers[conIndex].axesRepeat[6] == 1)
			{
				YGS2kLastControllerType = controllerType;
				s_iLastGameControllerIndex = conIndex;
			}
			s_aGameControllers[conIndex].axesRepeat[7] = 0;
		}
		else if (axisValue < 0 && axisValue < -YGS_DEADZONE_MAX)
		{
			s_aGameControllers[conIndex].axesRepeat[6] = 0;
			if (++s_aGameControllers[conIndex].axesRepeat[7] == 1)
			{
				YGS2kLastControllerType = controllerType;
				s_iLastGameControllerIndex = conIndex;
			}
		}
		else
		{
			s_aGameControllers[conIndex].axesRepeat[6] = 0;
			s_aGameControllers[conIndex].axesRepeat[7] = 0;
		}

		axisValue = SDL_GameControllerGetAxis(s_aGameControllers[conIndex].device, SDL_CONTROLLER_AXIS_TRIGGERLEFT);
		if (axisValue > YGS_DEADZONE_MAX)
		{
			if (++s_aGameControllers[conIndex].axesRepeat[8] == 1)
			{
				YGS2kLastControllerType = controllerType;
				s_iLastGameControllerIndex = conIndex;
			}
		}
		else
		{
			s_aGameControllers[conIndex].axesRepeat[8] = 0;
		}

		axisValue = SDL_GameControllerGetAxis(s_aGameControllers[conIndex].device, SDL_CONTROLLER_AXIS_TRIGGERRIGHT);
		if (axisValue > YGS_DEADZONE_MAX)
		{
			if (++s_aGameControllers[conIndex].axesRepeat[9] == 1)
			{
				YGS2kLastControllerType = controllerType;
				s_iLastGameControllerIndex = conIndex;
			}
		}
		else
		{
			s_aGameControllers[conIndex].axesRepeat[9] = 0;
		}

		for (SDL_GameControllerButton button = 0; button < SDL_CONTROLLER_BUTTON_MAX; button++)
		{
			if (SDL_GameControllerGetButton(s_aGameControllers[conIndex].device, button))
			{
				if (++s_aGameControllers[conIndex].buttonsRepeat[button] == 1)
				{
					YGS2kLastControllerType = controllerType;
					s_iLastGameControllerIndex = conIndex;
				}
			}
			else
			{
				s_aGameControllers[conIndex].buttonsRepeat[button] = 0;
			}
		}
	}
}

int YGS2kIsPushConKey ( const int conIndex, const YGS2kSConKey* const key )
{
	if (!s_aGameControllers || s_iNumGameControllers <= 0 || key == NULL || conIndex >= s_iNumGameControllers) return 0;

	int conIndexStart = 0;
	int conIndexMax = 0;
	if (conIndex >= 0)
	{
		conIndexStart = conIndex;
		conIndexMax = conIndex + 1;
	}
	else
	{
		conIndexStart = 0;
		conIndexMax = s_iNumGameControllers;
	}
	for (; conIndexStart < conIndexMax; conIndexStart++)
	{
		switch (key->type)
		{
		case YGS_CONKEY_ANY:
			for (int axis = 0; axis < YGS_CONAXIS_MAX; axis++)
			{
				if (s_aGameControllers[conIndexStart].axesRepeat[axis] == 1) return 1;
			}
			for (int button = 0; button < YGS_CONBUTTON_MAX; button++)
			{
				if (s_aGameControllers[conIndexStart].buttonsRepeat[button] == 1) return 1;
			}
			break;
		case YGS_CONKEY_AXIS:
			if (key->index < YGS_CONAXIS_MAX)
			{
				if (s_aGameControllers[conIndexStart].axesRepeat[key->index] == 1) return 1;
			}
			break;
		case YGS_CONKEY_BUTTON:
			if (key->index < YGS_CONBUTTON_MAX)
			{
				if (s_aGameControllers[conIndexStart].buttonsRepeat[key->index] == 1) return 1;
			}
			break;
		default:
			break;
		}
	}
	return 0;
}

int YGS2kIsPressConKey ( const int conIndex, const YGS2kSConKey* const key )
{
	if (!s_aGameControllers || s_iNumGameControllers <= 0 || key == NULL || conIndex >= s_iNumGameControllers) return 0;

	int conIndexStart = 0;
	int conIndexMax = 0;
	if (conIndex >= 0)
	{
		conIndexStart = conIndex;
		conIndexMax = conIndex + 1;
	}
	else
	{
		conIndexStart = 0;
		conIndexMax = s_iNumGameControllers;
	}
	for (; conIndexStart < conIndexMax; conIndexStart++)
	{
		switch (key->type)
		{
		case YGS_CONKEY_ANY:
			for (int axis = 0; axis < YGS_CONAXIS_MAX; axis++)
			{
				if (s_aGameControllers[conIndexStart].axesRepeat[axis] != 0) return 1;
			}
			for (int button = 0; button < YGS_CONBUTTON_MAX; button++)
			{
				if (s_aGameControllers[conIndexStart].buttonsRepeat[button] != 0) return 1;
			}
			break;
		case YGS_CONKEY_AXIS:
			if (key->index < YGS_CONAXIS_MAX)
			{
				if (s_aGameControllers[conIndexStart].axesRepeat[key->index] != 0) return 1;
			}
			break;
		case YGS_CONKEY_BUTTON:
			if (key->index < YGS_CONBUTTON_MAX)
			{
				if (s_aGameControllers[conIndexStart].buttonsRepeat[key->index] != 0) return 1;
			}
			break;
		default:
			break;
		}
	}
	return 0;
}

int YGS2kGetConKeyRepeat ( const int conIndex, YGS2kSConKey* const key )
{
	if (!s_aGameControllers || s_iNumGameControllers <= 0 || key == NULL || key->index >= s_iNumGameControllers) return 0;

	bool multi;
	int conIndexStart = 0;
	int conIndexMax = 0;
	int maxRepeat = 0;
	if (conIndex >= 0)
	{
		conIndexStart = conIndex;
		conIndexMax = conIndex + 1;
		multi = false;
	}
	else
	{
		conIndexStart = 0;
		conIndexMax = s_iNumGameControllers;
		multi = true;
	}
	for (; conIndexStart < conIndexMax; conIndexStart++)
	{
		switch (key->type)
		{
		case YGS_CONKEY_ANY:
			for (int axis = 0; axis < YGS_CONAXIS_MAX; axis++)
			{
				if (s_aGameControllers[conIndexStart].axesRepeat[axis] > maxRepeat) maxRepeat = s_aGameControllers[conIndexStart].axesRepeat[axis];
			}
			for (int button = 0; button < YGS_CONBUTTON_MAX; button++)
			{
				if (s_aGameControllers[conIndexStart].buttonsRepeat[button] > maxRepeat) maxRepeat = s_aGameControllers[conIndexStart].buttonsRepeat[button];
			}
			break;
		case YGS_CONKEY_AXIS:
			if (key->index < YGS_CONAXIS_MAX)
			{
					if (multi)
					{
						if (s_aGameControllers[conIndexStart].axesRepeat[key->index] > maxRepeat) maxRepeat = s_aGameControllers[conIndexStart].axesRepeat[key->index];
					}
					else return s_aGameControllers[conIndexStart].axesRepeat[key->index];
			}
			break;
		case YGS_CONKEY_BUTTON:
			if (key->index < YGS_CONBUTTON_MAX)
			{
				if (multi)
				{
					if (s_aGameControllers[conIndexStart].buttonsRepeat[key->index] > maxRepeat) maxRepeat = s_aGameControllers[conIndexStart].buttonsRepeat[key->index];
				}
				else return s_aGameControllers[conIndexStart].buttonsRepeat[key->index];
			}
			break;
		default:
			break;
		}
	}
	return maxRepeat;
}

bool YGS2kIsGameController ( int joyIndex )
{
	return
		(SDL_IsGameController(joyIndex) && SDL_GameControllerTypeForIndex(joyIndex) != SDL_CONTROLLER_TYPE_UNKNOWN) ||
		SDL_GameControllerMappingForGUID(SDL_JoystickGetDeviceGUID(joyIndex)) != NULL;
}

void YGS2kResetLastConIndex()
{
	s_iLastGameControllerIndex = -1;
}

int YGS2kGetLastConIndex()
{
	return s_iLastGameControllerIndex;
}

int YGS2kGetNumCons()
{
	return s_iNumGameControllers;
}

YGS2kEControllerType YGS2kGetConType(const int conIndex)
{
	if (!s_aGameControllers || s_iNumGameControllers <= 0 || conIndex < 0 || conIndex >= s_iNumGameControllers) return YGS_CONTROLLER_NULL;

	#ifdef ONLY_CONTROLLER_TYPE
	return ONLY_CONTROLLER_TYPE;
	#else
	switch(YGS2kGetSDLConType(s_aGameControllers[conIndex].device))
	{
	default:
	case SDL_CONTROLLER_TYPE_XBOX360:
	case SDL_CONTROLLER_TYPE_XBOXONE:
	case SDL_CONTROLLER_TYPE_AMAZON_LUNA:
	case SDL_CONTROLLER_TYPE_GOOGLE_STADIA:
		return YGS_CONTROLLER_XBOX;

	case SDL_CONTROLLER_TYPE_PS3:
	case SDL_CONTROLLER_TYPE_PS4:
	case SDL_CONTROLLER_TYPE_PS5:
		return YGS_CONTROLLER_PLAYSTATION;

	case SDL_CONTROLLER_TYPE_NINTENDO_SWITCH_PRO:
		return YGS_CONTROLLER_NINTENDO;
	}
	#endif
}

bool YGS2kGetConKeyDesc(const int conIndex, const YGS2kSConKey* const key, const char** text, EButton* button)
{
	if (YGS2kGetNumCons() <= 0 || conIndex < 0 || conIndex >= YGS2kGetNumCons() || key == NULL || text == NULL || button == NULL) return false;

	*text = NULL;
	*button = BTN_NULL;
	const SDL_GameControllerType conType = YGS2kGetSDLConType(s_aGameControllers[conIndex].device);
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
			switch(conType)
			{
			default:
			case SDL_CONTROLLER_TYPE_XBOX360:
			case SDL_CONTROLLER_TYPE_XBOXONE:
			case SDL_CONTROLLER_TYPE_AMAZON_LUNA:
				*text = "LT";
				break;

			case SDL_CONTROLLER_TYPE_GOOGLE_STADIA:
			case SDL_CONTROLLER_TYPE_PS3:
			case SDL_CONTROLLER_TYPE_PS4:
			case SDL_CONTROLLER_TYPE_PS5:
				*text = "L2";
				break;

			case SDL_CONTROLLER_TYPE_NINTENDO_SWITCH_PRO:
				*text = "ZL";
				break;
			}
			break;
		case 9:
			switch(conType)
			{
			default:
			case SDL_CONTROLLER_TYPE_XBOX360:
			case SDL_CONTROLLER_TYPE_XBOXONE:
			case SDL_CONTROLLER_TYPE_AMAZON_LUNA:
				*text = "RT";
				break;

			case SDL_CONTROLLER_TYPE_GOOGLE_STADIA:
			case SDL_CONTROLLER_TYPE_PS3:
			case SDL_CONTROLLER_TYPE_PS4:
			case SDL_CONTROLLER_TYPE_PS5:
				*text = "R2";
				break;

			case SDL_CONTROLLER_TYPE_NINTENDO_SWITCH_PRO:
				*text = "ZR";
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
			*text = "LOGO";
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
			switch(conType)
			{
			case SDL_CONTROLLER_TYPE_XBOX360:
			case SDL_CONTROLLER_TYPE_XBOXONE:
				*text = "SHARE";
				break;
			case SDL_CONTROLLER_TYPE_NINTENDO_SWITCH_PRO:
				*text = "CAPTURE";
				break;
			case SDL_CONTROLLER_TYPE_PS5:
			case SDL_CONTROLLER_TYPE_AMAZON_LUNA:
			case SDL_CONTROLLER_TYPE_GOOGLE_STADIA:
				*text = "MICROPHONE";
				break;
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
			switch(conType)
			{
			default:
			case SDL_CONTROLLER_TYPE_XBOX360:
			case SDL_CONTROLLER_TYPE_XBOXONE:
			case SDL_CONTROLLER_TYPE_NINTENDO_SWITCH_PRO:
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
			switch(conType)
			{
			default:
			case SDL_CONTROLLER_TYPE_XBOX360:
			case SDL_CONTROLLER_TYPE_XBOXONE:
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
			switch(conType)
			{
			default:
			case SDL_CONTROLLER_TYPE_XBOX360:
			case SDL_CONTROLLER_TYPE_XBOXONE:
			case SDL_CONTROLLER_TYPE_AMAZON_LUNA:
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
			switch(conType)
			{
			default:
			case SDL_CONTROLLER_TYPE_XBOX360:
			case SDL_CONTROLLER_TYPE_XBOXONE:
			case SDL_CONTROLLER_TYPE_AMAZON_LUNA:
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

int YGS2kInputOpen()
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
		memset(s_pGPIOLines, 0, sizeof(s_pGPIOLines));
		if (
			!(s_pGPIOLines[0] = gpiod_chip_get_line(s_pGPIOChip,  5)) ||
			!(s_pGPIOLines[1] = gpiod_chip_get_line(s_pGPIOChip, 13)) ||
			!(s_pGPIOLines[2] = gpiod_chip_get_line(s_pGPIOChip,  6)) ||
			!(s_pGPIOLines[3] = gpiod_chip_get_line(s_pGPIOChip, 12)) ||
			!(s_pGPIOLines[4] = gpiod_chip_get_line(s_pGPIOChip, 19)) ||
			!(s_pGPIOLines[5] = gpiod_chip_get_line(s_pGPIOChip, 16)) ||
			!(s_pGPIOLines[6] = gpiod_chip_get_line(s_pGPIOChip, 26)) ||
			!(s_pGPIOLines[7] = gpiod_chip_get_line(s_pGPIOChip, 20)) ||
			!(s_pGPIOLines[8] = gpiod_chip_get_line(s_pGPIOChip, 21)) ||
			!(s_pGPIOLines[9] = gpiod_chip_get_line(s_pGPIOChip,  4))
		)
		{
			for (int line = 0; line < 10; line++)
			{
				if (s_pGPIOLines[line])
				{
					gpiod_line_release(s_pGPIOLines[line]);
					s_pGPIOLines[line] = NULL;
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
				if (gpiod_line_request_input(s_pGPIOLines[line], "input") < 0)
				{
					for (line = 0; line < 10; line++)
					{
						gpiod_line_release(s_pGPIOLines[line]);
						s_pGPIOLines[line] = NULL;
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
	YGS2kJoyOpen();
	#endif

	#ifdef ENABLE_GAME_CONTROLLER
	OpenGameControllerDB();
	SDL_SetHint(SDL_HINT_GAMECONTROLLER_USE_BUTTON_LABELS, "0");
	YGS2kConOpen();
	#endif

	return 1;
}

void YGS2kInputClose()
{
	#ifdef ENABLE_LINUX_GPIO
	if ( s_pGPIOChip )
	{
		for ( int i = 0 ; i < 10 ; i ++ )
		{
			gpiod_line_release(s_pGPIOLines[i]);
		}
		gpiod_chip_close(s_pGPIOChip);

		s_pGPIOChip = NULL;
		memset(s_pGPIOLines, 0, sizeof(s_pGPIOLines));
	}
	#endif

	#ifdef ENABLE_KEYBOARD
	memset(s_iKeyRepeat, 0, sizeof(s_iKeyRepeat));
	#endif

	#ifdef ENABLE_JOYSTICK
	YGS2kJoyClose();
	#endif

	#ifdef ENABLE_GAME_CONTROLLER
	YGS2kConClose();
	#endif
}

void YGS2kInput()
{
	#ifdef ENABLE_LINUX_GPIO
	for (int line = 0; line < 10; line++)
	{
		if (gpiod_line_get_value(s_pGPIOLines[line]) == 1)
		{
			if (++s_iGPIORepeat[line] == 1) LastControllerType = YGS_CONTROLLER_LINUXGPIO;
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
			if (++s_iKeyRepeat[i] == 1) YGS2kLastControllerType = YGS_CONTROLLER_KEYBOARD;
		}
		else
		{
			s_iKeyRepeat[i] = 0;
		}
	}
	#endif

	#ifdef ENABLE_JOYSTICK
	YGS2kJoyInput();
	#endif
	
	#ifdef ENABLE_GAME_CONTROLLER
	YGS2kConInput();
	#endif
}
