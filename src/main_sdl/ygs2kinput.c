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
typedef struct SJoystick
{
	SDL_Joystick* device;
	int numAxes;
	int numHats;
	int numButtons;
	int* axesRepeat;
	int* hatsRepeat;
	int* buttonsRepeat;
} SJoystick;
static SJoystick* s_aJoysticks = NULL;
static int s_iNumJoysticks = -1;
#endif

#ifdef ENABLE_GAME_CONTROLLER
typedef struct SGameController
{
	SDL_GameController* device;
	int axesRepeat[CONAXIS_MAX];
	int buttonsRepeat[CONBUTTON_MAX];
} SGameController;
static SGameController* s_aGameControllers = NULL;
static int s_iNumGameControllers = -1;
static int s_iLastGameControllerIndex = -1;
#endif

EControllerType GetLastControllerType ()
{
	#ifdef ONLY_CONTROLLER_TYPE
	return ONLY_CONTROLLER_TYPE;
	#else
	return LastControllerType;
	#endif
}

#ifdef ENABLE_LINUX_GPIO
int IsPushGPIO ( int key )
{
	return key >= 0 && key < NUMBTNS && s_iGPIORepeat[key] == 1 ? 1 : 0;
}

int IsPressGPIO ( int key )
{
	return key >= 0 && key < NUMBTNS && s_iGPIORepeat[key] != 0 ? 1 : 0;
}

int GetGPIORepeat( int key )
{
	return key >= 0 && key < NUMBTNS ? s_iGPIORepeat[key] : -1;
}
#endif

#ifdef ENABLE_KEYBOARD
int IsPushKey (int key)
{
	return key >= 0 && key < YGS_KEY_MAX && s_iKeyRepeat[key] == 1 ? 1 : 0;
}

int IsPressKey (int key)
{
	return key >= 0 && key < YGS_KEY_MAX && s_iKeyRepeat[key] != 0 ? 1 : 0;
}

int GetKeyRepeat( int key )
{
	return key >= 0 && key < YGS_KEY_MAX ? s_iKeyRepeat[key] : 0;
}

int GetMaxKey ()
{
	return YGS_KEY_MAX;
}
#endif

#ifdef ENABLE_JOYSTICK
void JoyClose ()
{
	if (s_aJoysticks)
	{
		for (int index = 0; index < s_iNumJoysticks; index++)
		{
			if (s_aJoysticks[index].device && SDL_JoystickGetAttached(s_aJoysticks[index].device)) SDL_JoystickClose(s_aJoysticks[index].device);
			if (s_aJoysticks[index].axesRepeat) free(s_aJoysticks[index].axesRepeat);
			if (s_aJoysticks[index].hatsRepeat) free(s_aJoysticks[index].hatsRepeat);
			if (s_aJoysticks[index].buttonsRepeat) free(s_aJoysticks[index].buttonsRepeat);
		}
		free(s_aJoysticks);
	}
	s_aJoysticks = NULL;
	s_iNumJoysticks = -1;
}

int JoyOpen ()
{
	if ((s_iNumJoysticks = SDL_NumJoysticks()) > 0)
	{
		if (!(s_aJoysticks = (SJoystick*)calloc((size_t)s_iNumJoysticks, sizeof(SJoystick))))
		{
			s_iNumJoysticks = -1;
			return 0;
		}

		for (int index = 0; index < s_iNumJoysticks; index++)
		{
			#ifdef ENABLE_GAME_CONTROLLER
			if (!IsGameController(index))
			{
			#endif
				bool fail = false;
				if (!(s_aJoysticks[index].device = SDL_JoystickOpen(index))) fail = true;
				else if (
					(s_aJoysticks[index].numAxes = SDL_JoystickNumAxes(s_aJoysticks[index].device)) >= 0 &&
					(s_aJoysticks[index].numHats = SDL_JoystickNumHats(s_aJoysticks[index].device)) >= 0 &&
					(s_aJoysticks[index].numButtons = SDL_JoystickNumButtons(s_aJoysticks[index].device)) >= 0
				)
				{
					if (s_aJoysticks[index].numAxes > 0 && !(s_aJoysticks[index].axesRepeat = (int*)calloc((size_t)s_aJoysticks[index].numAxes * 2, sizeof(int)))) fail = true;
					if (s_aJoysticks[index].numHats > 0 && !(s_aJoysticks[index].hatsRepeat = (int*)calloc((size_t)s_aJoysticks[index].numHats * 4, sizeof(int)))) fail = true;
					if (s_aJoysticks[index].numButtons > 0 && !(s_aJoysticks[index].buttonsRepeat = (int*)calloc((size_t)s_aJoysticks[index].numButtons, sizeof(int)))) fail = true;
				}
				if (fail)
				{
					s_iNumJoysticks = index + !!s_aJoysticks[index].device;
					JoyClose();
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

void JoyInput ()
{
	if (!s_aJoysticks) return;

	for (int index = 0; index < s_iNumJoysticks; index++)
	{
		#ifdef ENABLE_GAME_CONTROLLER
		if (IsGameController(index)) continue;
		#endif
		if (!SDL_JoystickGetAttached(s_aJoysticks[index].device))
		{
			continue;
		}

		const SJoyGUID checkGUID = GetJoyGUID(index);
		const SJoyGUID zeroGUID = { 0 };
		if (memcmp(checkGUID.data, zeroGUID.data, sizeof(checkGUID.data)) == 0) continue;

		for (int axis = 0; axis < s_aJoysticks[index].numAxes; axis++)
		{
			const int value = SDL_JoystickGetAxis(s_aJoysticks[index].device, axis);

			if (value < -YGS_DEADZONE_MAX)
			{
				if (++s_aJoysticks[index].axesRepeat[axis*2 + 0] == 1) LastControllerType = CONTROLLER_JOYSTICK;
			}
			else
			{
				s_aJoysticks[index].axesRepeat[axis*2 + 0] = 0;
			}

			if (value > +YGS_DEADZONE_MAX)
			{
				if (++s_aJoysticks[index].axesRepeat[axis*2 + 1] == 1) LastControllerType = CONTROLLER_JOYSTICK;
			}
			else
			{
				s_aJoysticks[index].axesRepeat[axis*2 + 1] = 0;
			}
		}

		for (int hat = 0; hat < s_aJoysticks[index].numHats; hat++)
		{
			Uint8 value = SDL_JoystickGetHat(s_aJoysticks[index].device, hat);
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
					if (++s_aJoysticks[index].hatsRepeat[hat*4 + valueIndex] == 1) LastControllerType = CONTROLLER_JOYSTICK;
				}
				else
				{
					s_aJoysticks[index].hatsRepeat[hat*4 + valueIndex] = 0;
				}
			}
		}

		for (int button = 0; button < s_aJoysticks[index].numButtons; button++)
		{
			if (SDL_JoystickGetButton(s_aJoysticks[index].device, button))
			{
				if (++s_aJoysticks[index].buttonsRepeat[button] == 1) LastControllerType = CONTROLLER_JOYSTICK;
			}
			else
			{
				s_aJoysticks[index].buttonsRepeat[button] = 0;
			}
		}
	}
}

int IsPushJoyKey ( const SJoyKey* const key )
{
	if (!s_aJoysticks || s_iNumJoysticks <= 0 || key == NULL || key->index >= s_iNumJoysticks) return 0;

	int index = 0;
	int indexMax = 0;
	if (key->index >= 0)
	{
		SJoyGUID checkGUID = GetJoyGUID(key->index);
		SJoyGUID zeroGUID = { 0 };
		if (memcmp(checkGUID.data, zeroGUID.data, sizeof(checkGUID.data)) != 0 && memcmp(key->guid.data, checkGUID.data, sizeof(checkGUID.data)) == 0)
		{
			index = key->index;
			indexMax = key->index + 1;
		}
	}
	else
	{
		index = 0;
		indexMax = s_iNumJoysticks;
	}
	for (; index < indexMax; index++)
	{
		switch (key->type)
		{
		case JOYKEY_ANY:
			for (int axis = 0; axis < s_aJoysticks[index].numAxes; axis++)
			{
				if (s_aJoysticks[index].axesRepeat[axis*2 + 0] == 1) return 1;
				if (s_aJoysticks[index].axesRepeat[axis*2 + 1] == 1) return 1;
			}
			for (int hat = 0; hat < s_aJoysticks[index].numHats; hat++)
			{
				if (s_aJoysticks[index].hatsRepeat[hat] == 1) return 1;
			}
			for (int button = 0; button < s_aJoysticks[index].numButtons; button++)
			{
				if (s_aJoysticks[index].buttonsRepeat[button] == 1) return 1;
			}
			break;
		case JOYKEY_AXIS:
			if (key->setting.index < s_aJoysticks[index].numAxes)
			{
				if (key->setting.value == -YGS_DEADZONE_MAX)
				{
					return s_aJoysticks[index].axesRepeat[key->setting.index * 2 + 0] == 1 ? 1 : 0;
				}
				else if (key->setting.value == +YGS_DEADZONE_MAX)
				{
					return s_aJoysticks[index].axesRepeat[key->setting.index * 2 + 1] == 1 ? 1 : 0;
				}
			}
			break;
		case JOYKEY_HAT:
			if (key->setting.index < s_aJoysticks[index].numHats)
			{
				switch (key->setting.value)
				{
				case SDL_HAT_LEFT:
					return s_aJoysticks[index].hatsRepeat[key->setting.index * 4 + 0] == 1 ? 1 : 0;
				case SDL_HAT_RIGHT:
					return s_aJoysticks[index].hatsRepeat[key->setting.index * 4 + 1] == 1 ? 1 : 0;
				case SDL_HAT_UP:
					return s_aJoysticks[index].hatsRepeat[key->setting.index * 4 + 2] == 1 ? 1 : 0;
				case SDL_HAT_DOWN:
					return s_aJoysticks[index].hatsRepeat[key->setting.index * 4 + 3] == 1 ? 1 : 0;
				default:
					break;
				}
			}
			break;
		case JOYKEY_BUTTON:
			if (key->setting.button < s_aJoysticks[index].numButtons) return s_aJoysticks[index].buttonsRepeat[key->setting.button] == 1 ? 1 : 0;
			break;
		default:
			break;
		}
	}
	return 0;
}

int IsPressJoyKey ( const SJoyKey* const key )
{
	if (!s_aJoysticks || s_iNumJoysticks <= 0 || key == NULL || key->index >= s_iNumJoysticks) return 0;

	int index = 0;
	int indexMax = 0;
	if (key->index >= 0)
	{
		SJoyGUID checkGUID = GetJoyGUID(key->index);
		SJoyGUID zeroGUID = { 0 };
		if (memcmp(checkGUID.data, zeroGUID.data, sizeof(checkGUID.data)) != 0 && memcmp(key->guid.data, checkGUID.data, sizeof(checkGUID.data)) == 0)
		{
			index = key->index;
			indexMax = key->index + 1;
		}
	}
	else
	{
		index = 0;
		indexMax = s_iNumJoysticks;
	}
	for (; index < indexMax; index++)
	{
		switch (key->type)
		{
		case JOYKEY_ANY:
			for (int axis = 0; axis < s_aJoysticks[index].numAxes; axis++)
			{
				if (s_aJoysticks[index].axesRepeat[axis*2 + 0] != 0) return 1;
				if (s_aJoysticks[index].axesRepeat[axis*2 + 1] != 0) return 1;
			}
			for (int hat = 0; hat < s_aJoysticks[index].numHats; hat++)
			{
				if (s_aJoysticks[index].hatsRepeat[hat] != 0) return 1;
			}
			for (int button = 0; button < s_aJoysticks[index].numButtons; button++)
			{
				if (s_aJoysticks[index].buttonsRepeat[button] != 0) return 1;
			}
			break;
		case JOYKEY_AXIS:
			if (key->setting.index < s_aJoysticks[index].numAxes)
			{
				if (key->setting.value == -YGS_DEADZONE_MAX)
				{
					return s_aJoysticks[index].axesRepeat[key->setting.index * 2 + 0] != 0 ? 1 : 0;
				}
				else if (key->setting.value == +YGS_DEADZONE_MAX)
				{
					return s_aJoysticks[index].axesRepeat[key->setting.index * 2 + 1] != 0 ? 1 : 0;
				}
			}
			break;
		case JOYKEY_HAT:
			if (key->setting.index < s_aJoysticks[index].numHats)
			{
				switch (key->setting.value)
				{
				case SDL_HAT_LEFT:
					return s_aJoysticks[index].hatsRepeat[key->setting.index * 4 + 0] != 0 ? 1 : 0;
				case SDL_HAT_RIGHT:
					return s_aJoysticks[index].hatsRepeat[key->setting.index * 4 + 1] != 0 ? 1 : 0;
				case SDL_HAT_UP:
					return s_aJoysticks[index].hatsRepeat[key->setting.index * 4 + 2] != 0 ? 1 : 0;
				case SDL_HAT_DOWN:
					return s_aJoysticks[index].hatsRepeat[key->setting.index * 4 + 3] != 0 ? 1 : 0;
				default:
					break;
				}
			}
			break;
		case JOYKEY_BUTTON:
			if (key->setting.button < s_aJoysticks[index].numButtons) return s_aJoysticks[index].buttonsRepeat[key->setting.button] != 0 ? 1 : 0;
			break;
		default:
			break;
		}
	}
	return 0;
}

int GetJoyKeyRepeat ( const SJoyKey* const key )
{
	if (!s_aJoysticks || s_iNumJoysticks <= 0 || key == NULL || key->index >= s_iNumJoysticks) return 0;

	bool multi;
	int index = 0;
	int indexMax = 0;
	int maxRepeat = 0;
	if (key->index >= 0)
	{
		SJoyGUID checkGUID = GetJoyGUID(key->index);
		SJoyGUID zeroGUID = { 0 };
		if (memcmp(checkGUID.data, zeroGUID.data, sizeof(checkGUID.data)) != 0 && memcmp(key->guid.data, checkGUID.data, sizeof(checkGUID.data)) == 0)
		{
			index = key->index;
			indexMax = key->index + 1;
			multi = false;
		}
	}
	else
	{
		index = 0;
		indexMax = s_iNumJoysticks;
		multi = true;
	}
	for (; index < indexMax; index++)
	{
		switch (key->type)
		{
		case JOYKEY_ANY:
			for (int axis = 0; axis < s_aJoysticks[index].numAxes; axis++)
			{
				if (s_aJoysticks[index].axesRepeat[axis*2 + 0] > maxRepeat) maxRepeat = s_aJoysticks[index].axesRepeat[axis*2 + 0];
				if (s_aJoysticks[index].axesRepeat[axis*2 + 1] > maxRepeat) maxRepeat = s_aJoysticks[index].axesRepeat[axis*2 + 1];
			}
			for (int hat = 0; hat < s_aJoysticks[index].numHats; hat++)
			{
				if (s_aJoysticks[index].hatsRepeat[hat] > maxRepeat) maxRepeat = s_aJoysticks[index].hatsRepeat[hat];
			}
			for (int button = 0; button < s_aJoysticks[index].numButtons; button++)
			{
				if (s_aJoysticks[index].buttonsRepeat[button] > maxRepeat) maxRepeat = s_aJoysticks[index].buttonsRepeat[button];
			}
			break;
		case JOYKEY_AXIS:
			if (key->setting.index < s_aJoysticks[index].numAxes)
			{
				if (key->setting.value == -YGS_DEADZONE_MAX)
				{
					if (multi)
					{
						if (s_aJoysticks[index].axesRepeat[key->setting.index * 2 + 0] > maxRepeat) maxRepeat = s_aJoysticks[index].axesRepeat[key->setting.index * 2 + 0];
					}
					else return s_aJoysticks[index].axesRepeat[key->setting.index * 2 + 0];
				}
				else if (key->setting.value == +YGS_DEADZONE_MAX)
				{
					if (multi)
					{
						if (s_aJoysticks[index].axesRepeat[key->setting.index * 2 + 1] > maxRepeat) maxRepeat = s_aJoysticks[index].axesRepeat[key->setting.index * 2 + 1];
					}
					else return s_aJoysticks[index].axesRepeat[key->setting.index * 2 + 1];
				}
			}
			break;
		case JOYKEY_HAT:
			if (key->setting.index < s_aJoysticks[index].numHats)
			{
				switch (key->setting.value)
				{
				case SDL_HAT_LEFT:
					if (multi)
					{
						if (s_aJoysticks[index].hatsRepeat[key->setting.index * 4 + 0] > maxRepeat) maxRepeat = s_aJoysticks[index].hatsRepeat[key->setting.index * 4 + 0];
					}
					else return s_aJoysticks[index].hatsRepeat[key->setting.index * 4 + 0];
				case SDL_HAT_RIGHT:
					if (multi)
					{
						if (s_aJoysticks[index].hatsRepeat[key->setting.index * 4 + 1] > maxRepeat) maxRepeat = s_aJoysticks[index].hatsRepeat[key->setting.index * 4 + 1];
					}
					else return s_aJoysticks[index].hatsRepeat[key->setting.index * 4 + 1];
				case SDL_HAT_UP:
					if (multi)
					{
						if (s_aJoysticks[index].hatsRepeat[key->setting.index * 4 + 2] > maxRepeat) maxRepeat = s_aJoysticks[index].hatsRepeat[key->setting.index * 4 + 2];
					}
					else return s_aJoysticks[index].hatsRepeat[key->setting.index * 4 + 2];
				case SDL_HAT_DOWN:
					if (multi)
					{
						if (s_aJoysticks[index].hatsRepeat[key->setting.index * 4 + 3] > maxRepeat) maxRepeat = s_aJoysticks[index].hatsRepeat[key->setting.index * 4 + 3];
					}
					else return s_aJoysticks[index].hatsRepeat[key->setting.index * 4 + 3];
				default:
					break;
				}
			}
			break;
		case JOYKEY_BUTTON:
			if (key->setting.button < s_aJoysticks[index].numButtons)
			{
				if (multi)
				{
					if (s_aJoysticks[index].buttonsRepeat[key->setting.button] > maxRepeat) maxRepeat = s_aJoysticks[index].buttonsRepeat[key->setting.button];
				}
				else return s_aJoysticks[index].buttonsRepeat[key->setting.button];
			}
			break;
		default:
			break;
		}
	}
	return maxRepeat;
}

int GetMaxJoys()
{
	return s_iNumJoysticks;
}

int GetNumJoys()
{
	#ifdef ENABLE_GAME_CONTROLLER
	int numJoys = 0;
	for (int index = 0; index < s_iNumJoysticks; index++)
	{
		if (!IsGameController(index)) numJoys++;
	}
	return numJoys;
	#else
	return s_iNumJoysticks;
	#endif
}

SJoyGUID GetJoyGUID(int index)
{
	if (s_iNumJoysticks <= 0 || index >= s_iNumJoysticks
	#ifdef ENABLE_GAME_CONTROLLER
		|| IsGameController(index)
	#endif
	) return (SJoyGUID) { 0 };

	SDL_JoystickGUID sdlGUID;
	sdlGUID = SDL_JoystickGetGUID(s_aJoysticks[index].device);

	SJoyGUID joyGUID = { 0 };
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

int GetMaxJoyAxis(int index)
{
	if (s_iNumJoysticks > 0 && index < s_iNumJoysticks)
	{
		#ifdef ENABLE_GAME_CONTROLLER
		if (!IsGameController(index))
		{
			return s_aJoysticks[index].numAxes;
		}
		else
		{
			return -1;
		}
		#else
		return s_aJoysticks[index].numAxes;
		#endif
	}
	else
	{
		return -1;
	}
}

int GetMaxJoyHat(int index)
{
	if (s_iNumJoysticks > 0 && index < s_iNumJoysticks)
	{
		#ifdef ENABLE_GAME_CONTROLLER
		if (!IsGameController(index))
		{
			return s_aJoysticks[index].numHats;
		}
		else
		{
			return -1;
		}
		#else
		return s_aJoysticks[index].numHats;
		#endif
	}
	else
	{
		return -1;
	}
}

int GetMaxJoyButton(int index)
{
	if (s_iNumJoysticks > 0 && index < s_iNumJoysticks)
	{
		#ifdef ENABLE_GAME_CONTROLLER
		if (!IsGameController(index))
		{
			return s_aJoysticks[index].numButtons;
		}
		else
		{
			return -1;
		}
		#else
		return s_aJoysticks[index].numButtons;
		#endif
	}
	else
	{
		return -1;
	}
}
#endif

#ifdef ENABLE_GAME_CONTROLLER
void ConClose()
{
	for (int index = 0; index < s_iNumGameControllers; index++)
	{
		if (SDL_GameControllerGetAttached(s_aGameControllers[index].device))
		{
			SDL_GameControllerClose(s_aGameControllers[index].device);
		}
	}
	free(s_aGameControllers);
	s_aGameControllers = NULL;
	s_iNumGameControllers = -1;
}

int ConOpen()
{
	const int numJoysticks = SDL_NumJoysticks();
	if (numJoysticks < 0)
	{
		s_iNumGameControllers = -1;
		return -1;
	}

	s_iNumGameControllers = 0;
	for (int joystick_index = 0; joystick_index < numJoysticks; joystick_index++)
	{
		if (IsGameController(joystick_index)) s_iNumGameControllers++;
	}
	if (s_iNumGameControllers == 0) return 1;

	if ((s_aGameControllers = calloc((size_t)s_iNumGameControllers, sizeof(SGameController))) == NULL)
	{
		s_iNumGameControllers = -1;
		return -1;
	}

	for (int joystick_index = 0, controller_index = 0; joystick_index < numJoysticks; joystick_index++)
	{
		if (IsGameController(joystick_index))
		{
			if (!(s_aGameControllers[controller_index].device = SDL_GameControllerOpen(joystick_index)))
			{
				s_iNumGameControllers = controller_index;
				ConClose();
				s_iNumGameControllers = -1;
				return -1;
			}
			else
			{
				controller_index++;
			}
		}
	}
	return 1;
}

static SDL_GameControllerType GetSDLConType(SDL_GameController* const device)
{
	#ifdef ONLY_SDL_CONTROLLER_TYPE
	return ONLY_SDL_CONTROLLER_TYPE;
	#else
	return SDL_GameControllerGetType(device);
	#endif
}

void ConInput()
{
	if (!s_aGameControllers) return;

	for (int index = 0; index < s_iNumGameControllers; index++)
	{
		if (!SDL_GameControllerGetAttached(s_aGameControllers[index].device)) continue;

		EControllerType controllerType;
		switch (GetSDLConType(s_aGameControllers[index].device))
		{
		default:
		case SDL_CONTROLLER_TYPE_XBOX360:
		case SDL_CONTROLLER_TYPE_XBOXONE:
		case SDL_CONTROLLER_TYPE_AMAZON_LUNA:
		case SDL_CONTROLLER_TYPE_GOOGLE_STADIA:
			controllerType = CONTROLLER_XBOX;
			break;

		case SDL_CONTROLLER_TYPE_PS3:
		case SDL_CONTROLLER_TYPE_PS4:
		case SDL_CONTROLLER_TYPE_PS5:
			controllerType = CONTROLLER_PLAYSTATION;
			break;

		case SDL_CONTROLLER_TYPE_NINTENDO_SWITCH_PRO:
			controllerType = CONTROLLER_NINTENDO;
			break;
		}
		Sint16 axisValue;

		axisValue = SDL_GameControllerGetAxis(s_aGameControllers[index].device, SDL_CONTROLLER_AXIS_LEFTX);
		if (axisValue > 0 && axisValue > +YGS_DEADZONE_MAX)
		{
			if (++s_aGameControllers[index].axesRepeat[0] == 1)
			{
				LastControllerType = controllerType;
				s_iLastGameControllerIndex = index;
			}
			s_aGameControllers[index].axesRepeat[1] = 0;
		}
		else if (axisValue < 0 && axisValue < -YGS_DEADZONE_MAX)
		{
			s_aGameControllers[index].axesRepeat[0] = 0;
			if (++s_aGameControllers[index].axesRepeat[1] == 1)
			{
				LastControllerType = controllerType;
				s_iLastGameControllerIndex = index;
			}
		}
		else
		{
			s_aGameControllers[index].axesRepeat[0] = 0;
			s_aGameControllers[index].axesRepeat[1] = 0;
		}

		axisValue = SDL_GameControllerGetAxis(s_aGameControllers[index].device, SDL_CONTROLLER_AXIS_LEFTY);
		if (axisValue > 0 && axisValue > +YGS_DEADZONE_MAX)
		{
			if (++s_aGameControllers[index].axesRepeat[2] == 1)
			{
				LastControllerType = controllerType;
				s_iLastGameControllerIndex = index;
			}
			s_aGameControllers[index].axesRepeat[3] = 0;
		}
		else if (axisValue < 0 && axisValue < -YGS_DEADZONE_MAX)
		{
			s_aGameControllers[index].axesRepeat[2] = 0;
			if (++s_aGameControllers[index].axesRepeat[3] == 1)
			{
				LastControllerType = controllerType;
				s_iLastGameControllerIndex = index;
			}
		}
		else
		{
			s_aGameControllers[index].axesRepeat[2] = 0;
			s_aGameControllers[index].axesRepeat[3] = 0;
		}

		axisValue = SDL_GameControllerGetAxis(s_aGameControllers[index].device, SDL_CONTROLLER_AXIS_RIGHTX);
		if (axisValue > 0 && axisValue > +YGS_DEADZONE_MAX)
		{
			if (++s_aGameControllers[index].axesRepeat[4] == 1)
			{
				LastControllerType = controllerType;
				s_iLastGameControllerIndex = index;
			}
			s_aGameControllers[index].axesRepeat[5] = 0;
		}
		else if (axisValue < 0 && axisValue < -YGS_DEADZONE_MAX)
		{
			s_aGameControllers[index].axesRepeat[4] = 0;
			if (++s_aGameControllers[index].axesRepeat[5] == 1)
			{
				LastControllerType = controllerType;
				s_iLastGameControllerIndex = index;
			}
		}
		else
		{
			s_aGameControllers[index].axesRepeat[4] = 0;
			s_aGameControllers[index].axesRepeat[5] = 0;
		}

		axisValue = SDL_GameControllerGetAxis(s_aGameControllers[index].device, SDL_CONTROLLER_AXIS_RIGHTY);
		if (axisValue > 0 && axisValue > +YGS_DEADZONE_MAX)
		{
			if (++s_aGameControllers[index].axesRepeat[6] == 1)
			{
				LastControllerType = controllerType;
				s_iLastGameControllerIndex = index;
			}
			s_aGameControllers[index].axesRepeat[7] = 0;
		}
		else if (axisValue < 0 && axisValue < -YGS_DEADZONE_MAX)
		{
			s_aGameControllers[index].axesRepeat[6] = 0;
			if (++s_aGameControllers[index].axesRepeat[7] == 1)
			{
				LastControllerType = controllerType;
				s_iLastGameControllerIndex = index;
			}
		}
		else
		{
			s_aGameControllers[index].axesRepeat[6] = 0;
			s_aGameControllers[index].axesRepeat[7] = 0;
		}


		axisValue = SDL_GameControllerGetAxis(s_aGameControllers[index].device, SDL_CONTROLLER_AXIS_TRIGGERLEFT);
		if (axisValue > YGS_DEADZONE_MAX)
		{
			if (++s_aGameControllers[index].axesRepeat[8] == 1)
			{
				LastControllerType = controllerType;
				s_iLastGameControllerIndex = index;
			}
		}
		else
		{
			s_aGameControllers[index].axesRepeat[8] = 0;
		}

		axisValue = SDL_GameControllerGetAxis(s_aGameControllers[index].device, SDL_CONTROLLER_AXIS_TRIGGERRIGHT);
		if (axisValue > YGS_DEADZONE_MAX)
		{
			if (++s_aGameControllers[index].axesRepeat[9] == 1)
			{
				LastControllerType = controllerType;
				s_iLastGameControllerIndex = index;
			}
		}
		else
		{
			s_aGameControllers[index].axesRepeat[9] = 0;
		}

		for (SDL_GameControllerButton button = 0; button < SDL_CONTROLLER_BUTTON_MAX; button++)
		{
			if (SDL_GameControllerGetButton(s_aGameControllers[index].device, button))
			{
				if (++s_aGameControllers[index].buttonsRepeat[button] == 1)
				{
					LastControllerType = controllerType;
					s_iLastGameControllerIndex = index;
				}
			}
			else
			{
				s_aGameControllers[index].buttonsRepeat[button] = 0;
			}
		}
	}
}

int IsPushConKey ( const int conIndex, const SConKey* const key )
{
	if (!s_aGameControllers || s_iNumGameControllers <= 0 || key == NULL || conIndex >= s_iNumGameControllers) return 0;

	int index = 0;
	int indexMax = 0;
	if (conIndex >= 0)
	{
		index = conIndex;
		indexMax = conIndex + 1;
	}
	else
	{
		index = 0;
		indexMax = s_iNumGameControllers;
	}
	for (; index < indexMax; index++)
	{
		switch (key->type)
		{
		case CONKEY_ANY:
			for (int axis = 0; axis < CONAXIS_MAX; axis++)
			{
				if (s_aGameControllers[index].axesRepeat[axis] == 1) return 1;
			}
			for (int button = 0; button < CONBUTTON_MAX; button++)
			{
				if (s_aGameControllers[index].buttonsRepeat[button] == 1) return 1;
			}
			break;
		case CONKEY_AXIS:
			if (key->index < CONAXIS_MAX)
			{
				if (s_aGameControllers[index].axesRepeat[key->index] == 1) return 1;
			}
			break;
		case CONKEY_BUTTON:
			if (key->index < CONBUTTON_MAX)
			{
				if (s_aGameControllers[index].buttonsRepeat[key->index] == 1) return 1;
			}
			break;
		default:
			break;
		}
	}
	return 0;
}

int IsPressConKey ( const int conIndex, const SConKey* const key )
{
	if (!s_aGameControllers || s_iNumGameControllers <= 0 || key == NULL || conIndex >= s_iNumGameControllers) return 0;

	int index = 0;
	int indexMax = 0;
	if (conIndex >= 0)
	{
		index = conIndex;
		indexMax = conIndex + 1;
	}
	else
	{
		index = 0;
		indexMax = s_iNumGameControllers;
	}
	for (; index < indexMax; index++)
	{
		switch (key->type)
		{
		case CONKEY_ANY:
			for (int axis = 0; axis < CONAXIS_MAX; axis++)
			{
				if (s_aGameControllers[index].axesRepeat[axis] != 0) return 1;
			}
			for (int button = 0; button < CONBUTTON_MAX; button++)
			{
				if (s_aGameControllers[index].buttonsRepeat[button] != 0) return 1;
			}
			break;
		case CONKEY_AXIS:
			if (key->index < CONAXIS_MAX)
			{
				if (s_aGameControllers[index].axesRepeat[key->index] != 0) return 1;
			}
			break;
		case CONKEY_BUTTON:
			if (key->index < CONBUTTON_MAX)
			{
				if (s_aGameControllers[index].buttonsRepeat[key->index] != 0) return 1;
			}
			break;
		default:
			break;
		}
	}
	return 0;
}

int GetConKeyRepeat ( const int conIndex, SConKey* const key )
{
	if (!s_aGameControllers || s_iNumGameControllers <= 0 || key == NULL || key->index >= s_iNumGameControllers) return 0;

	bool multi;
	int index = 0;
	int indexMax = 0;
	int maxRepeat = 0;
	if (conIndex >= 0)
	{
		index = conIndex;
		indexMax = conIndex + 1;
		multi = false;
	}
	else
	{
		index = 0;
		indexMax = s_iNumGameControllers;
		multi = true;
	}
	for (; index < indexMax; index++)
	{
		switch (key->type)
		{
		case CONKEY_ANY:
			for (int axis = 0; axis < CONAXIS_MAX; axis++)
			{
				if (s_aGameControllers[index].axesRepeat[axis] > maxRepeat) maxRepeat = s_aGameControllers[index].axesRepeat[axis];
			}
			for (int button = 0; button < CONBUTTON_MAX; button++)
			{
				if (s_aGameControllers[index].buttonsRepeat[button] > maxRepeat) maxRepeat = s_aGameControllers[index].buttonsRepeat[button];
			}
			break;
		case CONKEY_AXIS:
			if (key->index < CONAXIS_MAX)
			{
					if (multi)
					{
						if (s_aGameControllers[index].axesRepeat[key->index] > maxRepeat) maxRepeat = s_aGameControllers[index].axesRepeat[key->index];
					}
					else return s_aGameControllers[index].axesRepeat[key->index];
			}
			break;
		case CONKEY_BUTTON:
			if (key->index < CONBUTTON_MAX)
			{
				if (multi)
				{
					if (s_aGameControllers[index].buttonsRepeat[key->index] > maxRepeat) maxRepeat = s_aGameControllers[index].buttonsRepeat[key->index];
				}
				else return s_aGameControllers[index].buttonsRepeat[key->index];
			}
			break;
		default:
			break;
		}
	}
	return maxRepeat;
}

bool IsGameController ( int index )
{
	return
		(SDL_IsGameController(index) && SDL_GameControllerTypeForIndex(index) != SDL_CONTROLLER_TYPE_UNKNOWN) ||
		SDL_GameControllerMappingForGUID(SDL_JoystickGetDeviceGUID(index)) != NULL;
}

void ResetLastConIndex()
{
	s_iLastGameControllerIndex = -1;
}

int GetLastConIndex()
{
	return s_iLastGameControllerIndex;
}

int GetNumCons()
{
	return s_iNumGameControllers;
}

EControllerType GetConType(const int index)
{
	if (!s_aGameControllers || s_iNumGameControllers <= 0 || index < 0 || index >= s_iNumGameControllers) return CONTROLLER_NULL;

	#ifdef ONLY_CONTROLLER_TYPE
	return ONLY_CONTROLLER_TYPE;
	#else
	switch(GetSDLConType(s_aGameControllers[index].device))
	{
	default:
	case SDL_CONTROLLER_TYPE_XBOX360:
	case SDL_CONTROLLER_TYPE_XBOXONE:
	case SDL_CONTROLLER_TYPE_AMAZON_LUNA:
	case SDL_CONTROLLER_TYPE_GOOGLE_STADIA:
		return CONTROLLER_XBOX;

	case SDL_CONTROLLER_TYPE_PS3:
	case SDL_CONTROLLER_TYPE_PS4:
	case SDL_CONTROLLER_TYPE_PS5:
		return CONTROLLER_PLAYSTATION;

	case SDL_CONTROLLER_TYPE_NINTENDO_SWITCH_PRO:
		return CONTROLLER_NINTENDO;
	}
	#endif
}

bool GetConKeyDesc(const int index, const SConKey* const key, const char** text, EButton* button)
{
	if (GetNumCons() <= 0 || index < 0 || index >= GetNumCons() || key == NULL || text == NULL || button == NULL) return false;

	*text = NULL;
	*button = BTN_NULL;
	switch (key->type)
	{
	case CONKEY_AXIS:
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
			switch(GetSDLConType(s_aGameControllers[index].device))
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
			switch(GetSDLConType(s_aGameControllers[index].device))
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
	case CONKEY_BUTTON:
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
			switch(GetSDLConType(s_aGameControllers[index].device))
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
			switch(GetSDLConType(s_aGameControllers[index].device))
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
			switch(GetSDLConType(s_aGameControllers[index].device))
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
			switch(GetSDLConType(s_aGameControllers[index].device))
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
			switch(GetSDLConType(s_aGameControllers[index].device))
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

int InputOpen()
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
	JoyOpen();
	#endif

	#ifdef ENABLE_GAME_CONTROLLER
	ConOpen();
	#endif

	return 1;
}

void InputClose()
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
	JoyClose();
	#endif

	#ifdef ENABLE_GAME_CONTROLLER
	ConClose();
	#endif
}

void Input()
{
	#ifdef ENABLE_LINUX_GPIO
	for (int line = 0; line < 10; line++)
	{
		if (gpiod_line_get_value(s_pGPIOLines[line]) == 1)
		{
			if (++s_iGPIORepeat[line] == 1) LastControllerType = CONTROLLER_LINUXGPIO;
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
			if (++s_iKeyRepeat[i] == 1) LastControllerType = CONTROLLER_KEYBOARD;
		}
		else
		{
			s_iKeyRepeat[i] = 0;
		}
	}
	#endif

	#ifdef ENABLE_JOYSTICK
	JoyInput();
	#endif
	
	#ifdef ENABLE_GAME_CONTROLLER
	ConInput();
	#endif
}
