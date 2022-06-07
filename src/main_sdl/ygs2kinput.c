#include "main_sdl/include.h"
#include "ygs2kfunc.h"
#include "ygs2kprivate.h"

#ifdef ENABLE_LINUX_GPIO
static int			s_iGPIORepeat[10];
static struct gpiod_chip	*s_pGPIOChip;
static struct gpiod_line	*s_pGPIOLines[10];
#endif

#ifdef ENABLE_KEYBOARD
static int			s_iKeyRepeat[YGS_KEYREPEAT_MAX];
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
	int axesRepeat[10]; // Two for each stick, one for each trigger.
	int hatRepeat[4];
	int buttonRepeat[SDL_CONTROLLER_BUTTON_MAX];
} SGameController;
static SGameController* s_aGameControllers = NULL;
static int s_iNumGameControllers = -1;
#endif

EControllerType GetLastControllerType()
{
	return LastControllerType;
}

int IsPushMenu(EMenuInput input)
{
	switch (LastControllerType)
	{
	#ifdef ENABLE_LINUXGPIO
	case CONTROLLER_LINUXGPIO:
		switch (input)
		{
		case MENUINPUT_OK: return IsPushGPIO(4);
		case MENUINPUT_CANCEL: return IsPushGPIO(5);
		case MENUINPUT_RETRY: return IsPushGPIO(6);
		default: return 0;
		}
	#endif

	#ifdef ENABLE_KEYBOARD
	#ifdef ENABLE_JOYSTICK
	case CONTROLLER_JOYSTICK:
		// TODO: Something better for joysticks than requiring a keyboard.
	#endif
	case CONTROLLER_KEYBOARD:
		switch (input)
		{
		case MENUINPUT_OK: return IsPushKey(SDL_GetScancodeFromKey(SDLK_RETURN));
		case MENUINPUT_CANCEL: return IsPushKey(SDL_GetScancodeFromKey(SDLK_BACKSPACE));
		case MENUINPUT_RETRY: return IsPushKey(SDL_GetScancodeFromKey(SDLK_DELETE));
		case MENUINPUT_UP: return IsPushKey(SDL_GetScancodeFromKey(SDLK_UP));
		case MENUINPUT_DOWN: return IsPushKey(SDL_GetScancodeFromKey(SDLK_DOWN));
		case MENUINPUT_LEFT: return IsPushKey(SDL_GetScancodeFromKey(SDLK_LEFT));
		case MENUINPUT_RIGHT: return IsPushKey(SDL_GetScancodeFromKey(SDLK_RIGHT));
		default: return 0;
		}
	#endif

	#ifdef ENABLE_GAME_CONTROLLER
	case CONTROLLER_XBOX:
	case CONTROLLER_PLAYSTATION:
	case CONTROLLER_NINTENDO:
		for (int index = 0; index < s_iNumGameControllers; index++) {
			switch (SDL_GameControllerGetType(s_aGameControllers[index].device))
			{
			case SDL_CONTROLLER_TYPE_XBOX360:
			case SDL_CONTROLLER_TYPE_XBOXONE:
			case SDL_CONTROLLER_TYPE_AMAZON_LUNA:
			case SDL_CONTROLLER_TYPE_GOOGLE_STADIA:
			case SDL_CONTROLLER_TYPE_PS3:
			case SDL_CONTROLLER_TYPE_PS4:
			case SDL_CONTROLLER_TYPE_PS5:
				switch (input)
				{
				case MENUINPUT_OK: return SDL_GameControllerGetButton(s_aGameControllers[index].device, SDL_CONTROLLER_BUTTON_A);
				case MENUINPUT_CANCEL: return SDL_GameControllerGetButton(s_aGameControllers[index].device, SDL_CONTROLLER_BUTTON_B);
				case MENUINPUT_RETRY: return SDL_GameControllerGetButton(s_aGameControllers[index].device, SDL_CONTROLLER_BUTTON_X);
				default: break;
				}

			case SDL_CONTROLLER_TYPE_NINTENDO_SWITCH_PRO:
				switch (input)
				{
				case MENUINPUT_OK: return SDL_GameControllerGetButton(s_aGameControllers[index].device, SDL_CONTROLLER_BUTTON_B);
				case MENUINPUT_CANCEL: return SDL_GameControllerGetButton(s_aGameControllers[index].device, SDL_CONTROLLER_BUTTON_A);
				case MENUINPUT_RETRY: return SDL_GameControllerGetButton(s_aGameControllers[index].device, SDL_CONTROLLER_BUTTON_X);
				default: break;
				}

			default: continue;
			}

			switch (input) {
			case MENUINPUT_UP:
				return
					SDL_GameControllerGetAxis(s_aGameControllers[index].device, SDL_CONTROLLER_AXIS_LEFTY) > +YGS_DEADZONE_MAX ||
					SDL_GameControllerGetButton(s_aGameControllers[index].device, SDL_CONTROLLER_BUTTON_DPAD_UP);
			case MENUINPUT_DOWN:
				return
					SDL_GameControllerGetAxis(s_aGameControllers[index].device, SDL_CONTROLLER_AXIS_LEFTY) < -YGS_DEADZONE_MAX ||
					SDL_GameControllerGetButton(s_aGameControllers[index].device, SDL_CONTROLLER_BUTTON_DPAD_DOWN);
			case MENUINPUT_LEFT:
				return
					SDL_GameControllerGetAxis(s_aGameControllers[index].device, SDL_CONTROLLER_AXIS_LEFTX) < -YGS_DEADZONE_MAX ||
					SDL_GameControllerGetButton(s_aGameControllers[index].device, SDL_CONTROLLER_BUTTON_DPAD_LEFT);
			case MENUINPUT_RIGHT:
				return
					SDL_GameControllerGetAxis(s_aGameControllers[index].device, SDL_CONTROLLER_AXIS_LEFTX) > +YGS_DEADZONE_MAX ||
					SDL_GameControllerGetButton(s_aGameControllers[index].device, SDL_CONTROLLER_BUTTON_DPAD_RIGHT);
			default: break;
			}
		}
		return 0;
	#endif

	default:
		return 0;
	}
}

int IsPressPrompt(EMenuInput input)
{
	switch (LastControllerType)
	{
	#ifdef ENABLE_KEYBOARD
	case CONTROLLER_KEYBOARD:
		switch (input)
		{
		case MENUINPUT_OK: return IsPressKey(SDL_GetScancodeFromKey(SDLK_RETURN));
		case MENUINPUT_CANCEL: return IsPressKey(SDL_GetScancodeFromKey(SDLK_BACKSPACE));
		case MENUINPUT_RETRY: return IsPressKey(SDL_GetScancodeFromKey(SDLK_DELETE));
		default: return 0;
		}
	#endif
		
	// TODO: Remaining controller types.

	default:
		return 0;
	}
}

#ifdef ENABLE_LINUX_GPIO
int IsPushGPIO ( int key )
{
	return s_iGPIORepeat[key] == 1 ? 1 : 0;
}

int IsPressGPIO ( int key )
{
	return s_iGPIORepeat[key] != 0 ? 1 : 0;
}
#endif

#ifdef ENABLE_KEYBOARD
int IsPushKey (int key)
{
	return s_iKeyRepeat[key] == 1 ? 1 : 0;
}

int IsPressKey (int key)
{
	return s_iKeyRepeat[key] != 0 ? 1 : 0;
}

int GetMaxKey()
{
	return YGS_KEYREPEAT_MAX;
}
#endif

#ifdef ENABLE_JOYSTICK
void JoyClose()
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

int JoyOpen()
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
			if (!SDL_IsGameController(index))
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

void JoyInput()
{
	if (!s_aJoysticks) return;

	SJoyKey key = { 0 };
	for (int index = 0; index < s_iNumJoysticks; index++)
	{
		#ifdef ENABLE_GAME_CONTROLLER
		if (SDL_IsGameController(index)) continue;
		#endif
		if (!SDL_JoystickGetAttached(s_aJoysticks[index].device))
		{
			continue;
		}

		key.index = index;
		key.guid = GetJoyGUID(index);
		const SJoyGUID zeroGUID = { 0 };
		if (memcmp(key.guid.data, zeroGUID.data, sizeof(key.guid.data) == 0)) continue;

		key.type = JOYKEY_AXIS;
		for (int axis = 0; axis < s_aJoysticks[index].numAxes; axis++)
		{
			key.setting.index = axis;

			const int axisValues[2] = {-YGS_DEADZONE_MAX, +YGS_DEADZONE_MAX};
			for (int valueIndex = 0; valueIndex < 2; valueIndex++)
			{
				key.setting.value = axisValues[valueIndex];
				if (IsPressJoyKey(&key))
				{
					s_aJoysticks[index].axesRepeat[axis*2 + valueIndex]++;
					LastControllerType = CONTROLLER_JOYSTICK;
				}
				else
				{
					s_aJoysticks[index].axesRepeat[axis*2 + valueIndex] = 0;
				}
			}
		}

		key.type = JOYKEY_HAT;
		for (int hat = 0; hat < s_aJoysticks[index].numHats; hat++)
		{
			key.setting.index = hat;

			const Uint8 hatValues[4] =
			{
				SDL_HAT_LEFT,
				SDL_HAT_RIGHT,
				SDL_HAT_UP,
				SDL_HAT_DOWN
			};

			for (int valueIndex = 0; valueIndex < 4; valueIndex++)
			{
				key.setting.value = hatValues[valueIndex];
				if (IsPressJoyKey(&key))
				{
					s_aJoysticks[index].hatsRepeat[hat*4 + valueIndex]++;
					LastControllerType = CONTROLLER_JOYSTICK;
				}
				else
				{
					s_aJoysticks[index].hatsRepeat[hat*4 + valueIndex] = 0;
				}
			}
		}

		key.type = JOYKEY_BUTTON;
		for (int button = 0; button < s_aJoysticks[index].numButtons; button++)
		{
			key.setting.button = button;
			if (IsPressJoyKey(&key))
			{
				s_aJoysticks[index].buttonsRepeat[button]++;
				LastControllerType = CONTROLLER_JOYSTICK;
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
	if (!s_aJoysticks || s_iNumJoysticks <= 0 || key == NULL || key->index < 0 || key->index >= s_iNumJoysticks) return 0;

	SJoyGUID checkGUID = GetJoyGUID(key->index);
	SJoyGUID zeroGUID = { 0 };
	if (memcmp(checkGUID.data, zeroGUID.data, sizeof(checkGUID.data)) != 0 && memcmp(key->guid.data, checkGUID.data, sizeof(checkGUID.data)) == 0)
	{
		switch (key->type)
		{
		case JOYKEY_AXIS:
			if (key->setting.index < s_aJoysticks[key->index].numAxes)
			{
				if (key->setting.value == -YGS_DEADZONE_MAX)
				{
					return s_aJoysticks[key->index].axesRepeat[key->setting.index * 2 + 0] == 1 ? 1 : 0;
				}
				else if (key->setting.value == +YGS_DEADZONE_MAX)
				{
					return s_aJoysticks[key->index].axesRepeat[key->setting.index * 2 + 1] == 1 ? 1 : 0;
				}
			}
			break;
		case JOYKEY_HAT:
			if (key->setting.index < s_aJoysticks[key->index].numHats)
			{
				switch (key->setting.value)
				{
				case SDL_HAT_LEFT:
					return s_aJoysticks[key->index].hatsRepeat[key->setting.index * 4 + 0] == 1 ? 1 : 0;
				case SDL_HAT_RIGHT:
					return s_aJoysticks[key->index].hatsRepeat[key->setting.index * 4 + 1] == 1 ? 1 : 0;
				case SDL_HAT_UP:
					return s_aJoysticks[key->index].hatsRepeat[key->setting.index * 4 + 2] == 1 ? 1 : 0;
				case SDL_HAT_DOWN:
					return s_aJoysticks[key->index].hatsRepeat[key->setting.index * 4 + 3] == 1 ? 1 : 0;
				default:
					break;
				}
			}
			break;
		case JOYKEY_BUTTON:
			if (key->setting.button < s_aJoysticks[key->index].numButtons) return s_aJoysticks[key->index].buttonsRepeat[key->setting.button] == 1 ? 1 : 0;
			break;
		default:
			break;
		}
	}
	return 0;
}

int IsPressJoyKey ( const SJoyKey* const key )
{
	if (!s_aJoysticks || s_iNumJoysticks <= 0 || key == NULL || key->index < 0 || key->index >= s_iNumJoysticks) return 0;

	SJoyGUID checkGUID = GetJoyGUID(key->index);
	SJoyGUID zeroGUID = { 0 };
	if (memcmp(checkGUID.data, zeroGUID.data, sizeof(checkGUID.data)) != 0 && memcmp(key->guid.data, checkGUID.data, sizeof(checkGUID.data)) == 0) {
		SDL_Joystick* const joy = s_aJoysticks[key->index].device;
		if (SDL_JoystickGetAttached(joy)) {
			switch (key->type) {
			case JOYKEY_AXIS:
				return
					key->setting.value != 0 && key->setting.index < SDL_JoystickNumAxes(joy) &&
					(
						(key->setting.value > 0 && SDL_JoystickGetAxis(joy, key->setting.index) > key->setting.value) ||
						(key->setting.value < 0 && SDL_JoystickGetAxis(joy, key->setting.index) < key->setting.value)
					);
			case JOYKEY_HAT:
				return
					key->setting.value != SDL_HAT_CENTERED &&
					key->setting.index < SDL_JoystickNumHats(joy) &&
					SDL_JoystickGetHat(joy, key->setting.index) == key->setting.value;
			case JOYKEY_BUTTON:
				return key->setting.button < SDL_JoystickNumButtons(joy) && SDL_JoystickGetButton(joy, key->setting.button);
			default:
				break;
			}
		}
	}
	return 0;
}

int GetMaxJoys()
{
	return s_iNumJoysticks;
}

int GetNumJoys()
{
	int numJoys = 0;
	for (int index = 0; index < s_iNumJoysticks; index++) {
		if (!SDL_IsGameController(index)) numJoys++;
	}
	return numJoys;
}

SJoyGUID GetJoyGUID(int index)
{
	if (s_iNumJoysticks <= 0 || index >= s_iNumJoysticks
	#ifdef ENABLE_GAME_CONTROLLER
		|| SDL_IsGameController(index)
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
		if (!SDL_IsGameController(index))
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
		if (!SDL_IsGameController(index))
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
		if (!SDL_IsGameController(index))
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
void ConClose() {
	for (int index = 0; index < s_iNumGameControllers; index++) {
		if (SDL_GameControllerGetAttached(s_aGameControllers[index].device)) {
			SDL_GameControllerClose(s_aGameControllers[index].device);
		}
	}
	free(s_aGameControllers);
	s_aGameControllers = NULL;
	s_iNumGameControllers = -1;
}

int ConOpen() {
	const int numJoysticks = SDL_NumJoysticks();
	if (numJoysticks < 0) {
		s_iNumGameControllers = -1;
		return -1;
	}

	s_iNumGameControllers = 0;
	for (int joystick_index = 0; joystick_index < numJoysticks; joystick_index++) {
		if (SDL_IsGameController(joystick_index)) s_iNumGameControllers++;
	}
	if (s_iNumGameControllers == 0) return 1;

	if ((s_aGameControllers = calloc((size_t)s_iNumGameControllers, sizeof(SGameController))) == NULL) {
		s_iNumGameControllers = -1;
		return -1;
	}

	for (int joystick_index = 0, controller_index = 0; joystick_index < numJoysticks; joystick_index++)
	{
		if (SDL_IsGameController(joystick_index)) {
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

void ConInput() {
	if (!s_aGameControllers) return;

	for (int index = 0; index < s_iNumGameControllers; index++)
	{
		if (!SDL_GameControllerGetAttached(s_aGameControllers[index].device)) continue;

		EControllerType controllerType;
		switch (SDL_GameControllerGetType(s_aGameControllers[index].device)) {
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

		default:
			continue;
		}
		Sint16 axisValue;

		axisValue = SDL_GameControllerGetAxis(s_aGameControllers[index].device, SDL_CONTROLLER_AXIS_LEFTX);
		if (axisValue > 0 && axisValue > +YGS_DEADZONE_MAX)
		{
			s_aGameControllers[index].axesRepeat[0]++;
			s_aGameControllers[index].axesRepeat[1] = 0;
			LastControllerType = controllerType;
		}
		else if (axisValue < 0 && axisValue < -YGS_DEADZONE_MAX)
		{
			s_aGameControllers[index].axesRepeat[0] = 0;
			s_aGameControllers[index].axesRepeat[1]++;
			LastControllerType = controllerType;
		}
		else
		{
			s_aGameControllers[index].axesRepeat[0] = 0;
			s_aGameControllers[index].axesRepeat[1] = 0;
		}

		axisValue = SDL_GameControllerGetAxis(s_aGameControllers[index].device, SDL_CONTROLLER_AXIS_LEFTY);
		if (axisValue > 0 && axisValue > +YGS_DEADZONE_MAX)
		{
			s_aGameControllers[index].axesRepeat[2]++;
			s_aGameControllers[index].axesRepeat[3] = 0;
			LastControllerType = controllerType;
		}
		else if (axisValue < 0 && axisValue < -YGS_DEADZONE_MAX)
		{
			s_aGameControllers[index].axesRepeat[2] = 0;
			s_aGameControllers[index].axesRepeat[3]++;
			LastControllerType = controllerType;
		}
		else
		{
			s_aGameControllers[index].axesRepeat[2] = 0;
			s_aGameControllers[index].axesRepeat[3] = 0;
		}

		axisValue = SDL_GameControllerGetAxis(s_aGameControllers[index].device, SDL_CONTROLLER_AXIS_RIGHTX);
		if (axisValue > 0 && axisValue > +YGS_DEADZONE_MAX)
		{
			s_aGameControllers[index].axesRepeat[4]++;
			s_aGameControllers[index].axesRepeat[5] = 0;
			LastControllerType = controllerType;
		}
		else if (axisValue < 0 && axisValue < -YGS_DEADZONE_MAX)
		{
			s_aGameControllers[index].axesRepeat[4] = 0;
			s_aGameControllers[index].axesRepeat[5]++;
			LastControllerType = controllerType;
		}
		else
		{
			s_aGameControllers[index].axesRepeat[4] = 0;
			s_aGameControllers[index].axesRepeat[5] = 0;
		}

		axisValue = SDL_GameControllerGetAxis(s_aGameControllers[index].device, SDL_CONTROLLER_AXIS_RIGHTY);
		if (axisValue > 0 && axisValue > +YGS_DEADZONE_MAX)
		{
			s_aGameControllers[index].axesRepeat[6]++;
			s_aGameControllers[index].axesRepeat[7] = 0;
			LastControllerType = controllerType;
		}
		else if (axisValue < 0 && axisValue < -YGS_DEADZONE_MAX)
		{
			s_aGameControllers[index].axesRepeat[6] = 0;
			s_aGameControllers[index].axesRepeat[7]++;
			LastControllerType = controllerType;
		}
		else
		{
			s_aGameControllers[index].axesRepeat[6] = 0;
			s_aGameControllers[index].axesRepeat[7] = 0;
		}


		axisValue = SDL_GameControllerGetAxis(s_aGameControllers[index].device, SDL_CONTROLLER_AXIS_TRIGGERLEFT);
		if (axisValue > YGS_DEADZONE_MAX)
		{
			s_aGameControllers[index].axesRepeat[8]++;
			LastControllerType = controllerType;
		}
		else
		{
			s_aGameControllers[index].axesRepeat[8] = 0;
		}

		axisValue = SDL_GameControllerGetAxis(s_aGameControllers[index].device, SDL_CONTROLLER_AXIS_TRIGGERRIGHT);
		if (axisValue > YGS_DEADZONE_MAX)
		{
			s_aGameControllers[index].axesRepeat[9]++;
			LastControllerType = controllerType;
		}
		else
		{
			s_aGameControllers[index].axesRepeat[9] = 0;
		}

		for (SDL_GameControllerButton button = 0; button < SDL_CONTROLLER_BUTTON_MAX; button++) {
			if (SDL_GameControllerGetButton(s_aGameControllers[index].device, button)) {
				s_aGameControllers[index].buttonRepeat[button]++;
				LastControllerType = controllerType;
			}
			else {
				s_aGameControllers[index].buttonRepeat[button] = 0;
			}
		}
	}
}

int IsPushConKey ( const int index, const SConKey* const key )
{
	if (!s_aGameControllers || s_iNumGameControllers <= 0 || key == NULL || index < 0 || index >= s_iNumGameControllers) return 0;

	switch (key->type)
	{
	case CONKEY_AXIS:
		return s_aGameControllers[index].axesRepeat[key->setting.axis] == 1 ? 1 : 0;
	case CONKEY_BUTTON:
		return s_aGameControllers[index].buttonRepeat[key->setting.button] == 1 ? 1 : 0;
	default:
		break;
	}
	return 0;
}

int IsPressConKey ( const int index, const SConKey* const key ) {
	if (!s_aGameControllers || s_iNumGameControllers <= 0 || key == NULL || index < 0 || index >= s_iNumGameControllers) return 0;

	switch (key->type)
	{
	case CONKEY_AXIS:
		return s_aGameControllers[index].axesRepeat[key->setting.axis] != 0 ? 1 : 0;
	case CONKEY_BUTTON:
		return s_aGameControllers[index].buttonRepeat[key->setting.button] != 0 ? 1 : 0;
	default:
		break;
	}
	return 0;
}

int GetNumCons() {
	return s_iNumGameControllers;
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
	else {
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
				if (s_pGPIOLines[line]) {
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
			s_iGPIORepeat[line]++;
			LastControllerType = CONTROLLER_LINUXGPIO;
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

	for (int i = 0; i < YGS_KEYREPEAT_MAX; i++)
	{
		if (i < numKeys && keyStates[i] == SDL_PRESSED)
		{
			s_iKeyRepeat[i]++;
			LastControllerType = CONTROLLER_KEYBOARD;
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