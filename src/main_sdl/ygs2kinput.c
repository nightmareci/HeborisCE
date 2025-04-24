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
#endif

#ifdef ENABLE_GAME_CONTROLLER
#include "main_sdl/gamecontrollerdb.h"
typedef struct YGS2kSCon
{
	SDL_GameController* controller;
	int axesRepeat[YGS_CONAXIS_MAX];
	int buttonsRepeat[YGS_CONBUTTON_MAX];
} YGS2kSCon;
static int s_iLastActiveCon = -1;
#endif

#if defined(ENABLE_JOYSTICK) || defined(ENABLE_GAME_CONTROLLER)
typedef struct YGS2kSPlayerSlot
{
	YGS2kEPlayerSlotType type;
	union
	{
		#ifdef ENABLE_JOYSTICK
		YGS2kSJoy joy;
		#endif
		#ifdef ENABLE_GAME_CONTROLLER
		YGS2kSCon con;
		#endif
	};
} YGS2kSPlayerSlot;

static YGS2kSPlayerSlot* s_aPlayerSlots = NULL;
static int s_iNumPlayerSlots = 0;
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

#if defined(ENABLE_JOYSTICK) || defined(ENABLE_GAME_CONTROLLER)

static bool YGS2kIsCon ( int device )
{
	if (device < 0) {
		return false;
	}

	return
		(SDL_IsGameController(device) && SDL_GameControllerTypeForIndex(device) != SDL_CONTROLLER_TYPE_UNKNOWN) ||
		SDL_GameControllerMappingForGUID(SDL_JoystickGetDeviceGUID(device)) != NULL;
}

static bool YGS2kResizePlayerSlots(int numSlots) {
	if (numSlots == s_iNumPlayerSlots) {
		return true;
	}
	else if (numSlots == 0) {
		free(s_aPlayerSlots);
		s_aPlayerSlots = NULL;
		s_iNumPlayerSlots = 0;
		return true;
	}

	YGS2kSPlayerSlot* const oldSlots = s_aPlayerSlots;
	const int oldNumSlots = s_iNumPlayerSlots;
	if (s_aPlayerSlots) {
		s_aPlayerSlots = realloc(s_aPlayerSlots, sizeof(YGS2kSPlayerSlot) * numSlots);
	}
	else {
		s_aPlayerSlots = malloc(sizeof(YGS2kSPlayerSlot) * numSlots);
	}
	if (!s_aPlayerSlots) {
		fprintf(stderr, "Failed to resize joysticks/controllers array\n");
		s_aPlayerSlots = oldSlots;
		return false;
	}
	for (int player = s_iNumPlayerSlots; player < numSlots; player++) {
		s_aPlayerSlots[player] = (YGS2kSPlayerSlot) { 0 };
	}
	s_iNumPlayerSlots = numSlots;
	return true;
}

bool YGS2kPlayerSlotsChanged() {
	for (int player = 0; player < s_iNumPlayerSlots; player++) {
		YGS2kSPlayerSlot* const slot = &s_aPlayerSlots[player];
		switch (slot->type) {
		#ifdef ENABLE_JOYSTICK
		case YGS_PLAYERSLOT_JOY:
			if (!SDL_JoystickGetAttached(slot->joy.joystick)) {
				if (slot->joy.axesRepeat) free(slot->joy.axesRepeat);
				if (slot->joy.hatsRepeat) free(slot->joy.hatsRepeat);
				if (slot->joy.buttonsRepeat) free(slot->joy.buttonsRepeat);
				*slot = (YGS2kSPlayerSlot) { 0 };
			}
			break;
		#endif

		#ifdef ENABLE_GAME_CONTROLLER
		case YGS_PLAYERSLOT_CON:
			if (!SDL_GameControllerGetAttached(slot->con.controller)) {
				*slot = (YGS2kSPlayerSlot) { 0 };
			}
			break;
		#endif

		default:
		case YGS_PLAYERSLOT_NULL:
			break;
		}
	}

	const int numJoys = SDL_NumJoysticks();

	// Only opened joysticks will have a non-NULL return value from
	// SDL_JoystickFromInstanceID(), so we can use it to skip already opened
	// devices.
	//
	// Internally, SDL uses joystick APIs for game controllers, so it also skips
	// already opened game controllers.

	#ifdef ENABLE_GAME_CONTROLLER
	for (int device = 0; device < numJoys; device++) {
		const SDL_JoystickID id = SDL_JoystickGetDeviceInstanceID(device);
		if (id >= 0 && SDL_JoystickFromInstanceID(id)) {
			continue;
		}
		if (!YGS2kIsCon(device)) {
			continue;
		}

		SDL_GameController* const controller = SDL_GameControllerOpen(device);
		if (!controller) {
			fprintf(stderr, "Failed to open controller: %s\n", SDL_GetError());
			return false;
		}
		bool playerIndexFound = false;
		int player;
		for (player = 0; player < s_iNumPlayerSlots; player++) {
			if (s_aPlayerSlots[player].type == YGS_PLAYERSLOT_NULL) {
				playerIndexFound = true;
				break;
			}
		}
		if (!playerIndexFound) {
			if (!YGS2kResizePlayerSlots(s_iNumPlayerSlots + 1)) {
				fprintf(stderr, "Failed to open controller: %s\n", SDL_GetError());
				SDL_GameControllerClose(controller);
				return false;
			}
			player = s_iNumPlayerSlots - 1;
		}
		SDL_GameControllerSetPlayerIndex(controller, player);
		s_aPlayerSlots[player].con.controller = controller;
		s_aPlayerSlots[player].type = YGS_PLAYERSLOT_CON;
	}
	#endif

	#ifdef ENABLE_JOYSTICK
	for (int device = 0; device < numJoys; device++) {
		const SDL_JoystickID id = SDL_JoystickGetDeviceInstanceID(device);
		if (id >= 0 && SDL_JoystickFromInstanceID(id)) {
			continue;
		}
		if (YGS2kIsCon(device)) {
			continue;
		}

		SDL_Joystick* const joystick = SDL_JoystickOpen(device);
		if (!joystick) {
			fprintf(stderr, "Failed to open joystick: %s\n", SDL_GetError());
			goto fail;
		}
		bool playerIndexFound = false;
		int player;
		for (player = 0; player < s_iNumPlayerSlots; player++) {
			if (s_aPlayerSlots[player].type == YGS_PLAYERSLOT_NULL) {
				playerIndexFound = true;
				break;
			}
		}
		if (!playerIndexFound) {
			if (!YGS2kResizePlayerSlots(s_iNumPlayerSlots + 1)) {
				goto fail;
			}
			player = s_iNumPlayerSlots - 1;
		}
		SDL_JoystickSetPlayerIndex(joystick, player);

		if (
			(s_aPlayerSlots[player].joy.numAxes = SDL_JoystickNumAxes(joystick)) >= 0 &&
			(s_aPlayerSlots[player].joy.numHats = SDL_JoystickNumHats(joystick)) >= 0 &&
			(s_aPlayerSlots[player].joy.numButtons = SDL_JoystickNumButtons(joystick)) >= 0
		) {
			if (s_aPlayerSlots[player].joy.numAxes > 0 && !(s_aPlayerSlots[player].joy.axesRepeat = (int*)calloc((size_t)s_aPlayerSlots[player].joy.numAxes * 2, sizeof(int)))) {
				fprintf(stderr, "Failed to allocate axes repeat array for joystick\n");
				goto fail;
			}
			if (s_aPlayerSlots[player].joy.numHats > 0 && !(s_aPlayerSlots[player].joy.hatsRepeat = (int*)calloc((size_t)s_aPlayerSlots[player].joy.numHats * 4, sizeof(int)))) {
				fprintf(stderr, "Failed to allocate hats repeat array for joystick\n");
				goto fail;
			}
			if (s_aPlayerSlots[player].joy.numButtons > 0 && !(s_aPlayerSlots[player].joy.buttonsRepeat = (int*)calloc((size_t)s_aPlayerSlots[player].joy.numButtons, sizeof(int)))) {
				fprintf(stderr, "Failed to allocate buttons repeat array for joystick\n");
				goto fail;
			}
		}
		if (false) {
		fail:
			if (s_aPlayerSlots[player].joy.axesRepeat) free(s_aPlayerSlots[player].joy.axesRepeat);
			if (s_aPlayerSlots[player].joy.hatsRepeat) free(s_aPlayerSlots[player].joy.hatsRepeat);
			if (s_aPlayerSlots[player].joy.buttonsRepeat) free(s_aPlayerSlots[player].joy.buttonsRepeat);
			if (joystick) SDL_JoystickClose(joystick);
			return false;
		}
		s_aPlayerSlots[player].joy.joystick = joystick;
		s_aPlayerSlots[player].type = YGS_PLAYERSLOT_JOY;
	}
	#endif

	int numRequiredSlots = 0;
	for (int player = 0; player < s_iNumPlayerSlots; player++) {
		if (s_aPlayerSlots[player].type != YGS_PLAYERSLOT_NULL) {
			numRequiredSlots = player + 1;
		}
	}
	return YGS2kResizePlayerSlots(numRequiredSlots);
}

static void YGS2kPlayerSlotsClose()
{
	if (s_aPlayerSlots)
	{
		for (int player = 0; player < s_iNumPlayerSlots; player++)
		{
			switch (s_aPlayerSlots[player].type)
			{
			#ifdef ENABLE_JOYSTICK
			case YGS_PLAYERSLOT_JOY: {
				YGS2kSJoy* const joy = &s_aPlayerSlots[player].joy;
				if (SDL_JoystickGetAttached(joy->joystick))
				{
					SDL_JoystickClose(joy->joystick);
				}
				if (joy->axesRepeat) free(joy->axesRepeat);
				if (joy->hatsRepeat) free(joy->hatsRepeat);
				if (joy->buttonsRepeat) free(joy->buttonsRepeat);
				break;
			}
			#endif

			#ifdef ENABLE_GAME_CONTROLLER
			case YGS_PLAYERSLOT_CON: {
				if (SDL_GameControllerGetAttached(s_aPlayerSlots[player].con.controller))
				{
					SDL_GameControllerClose(s_aPlayerSlots[player].con.controller);
				}
				break;
			}
			#endif

			default:
			case YGS_PLAYERSLOT_NULL:
				break;
			}
		}
		free(s_aPlayerSlots);
	}
	s_aPlayerSlots = NULL;
	s_iNumPlayerSlots = 0;
}

int YGS2kGetNumPlayerSlots()
{
	return s_iNumPlayerSlots;
}

YGS2kEPlayerSlotType YGS2kGetPlayerSlotType(int player)
{
	if (player < 0 || player >= s_iNumPlayerSlots) {
		return YGS_PLAYERSLOT_NULL;
	}
	else {
		return s_aPlayerSlots[player].type;
	}
}

#endif

#ifdef ENABLE_JOYSTICK

static void YGS2kJoyInputsUpdate ()
{
	if (!s_aPlayerSlots) return;

	for (int player = 0; player < s_iNumPlayerSlots; player++)
	{
		if (
			s_aPlayerSlots[player].type != YGS_PLAYERSLOT_JOY ||
			!SDL_JoystickGetAttached(s_aPlayerSlots[player].joy.joystick)
		) {
			continue;
		}

		const YGS2kSJoyGUID checkGUID = YGS2kGetJoyGUID(player);
		const YGS2kSJoyGUID zeroGUID = { 0 };
		if (memcmp(checkGUID.data, zeroGUID.data, sizeof(checkGUID.data)) == 0) continue;

		YGS2kSJoy* const joy = &s_aPlayerSlots[player].joy;

		for (int axis = 0; axis < joy->numAxes; axis++)
		{
			const int value = SDL_JoystickGetAxis(joy->joystick, axis);

			if (value < -YGS_DEADZONE_MAX)
			{
				if (++joy->axesRepeat[axis*2 + 0] == 1) YGS2kLastInputType = YGS_INPUT_JOYSTICK;
			}
			else
			{
				joy->axesRepeat[axis*2 + 0] = 0;
			}

			if (value > +YGS_DEADZONE_MAX)
			{
				if (++joy->axesRepeat[axis*2 + 1] == 1) YGS2kLastInputType = YGS_INPUT_JOYSTICK;
			}
			else
			{
				joy->axesRepeat[axis*2 + 1] = 0;
			}
		}

		for (int hat = 0; hat < joy->numHats; hat++)
		{
			Uint8 value = SDL_JoystickGetHat(joy->joystick, hat);
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
					if (++joy->hatsRepeat[hat*4 + valueIndex] == 1) YGS2kLastInputType = YGS_INPUT_JOYSTICK;
				}
				else
				{
					joy->hatsRepeat[hat*4 + valueIndex] = 0;
				}
			}
		}

		for (int button = 0; button < joy->numButtons; button++)
		{
			if (SDL_JoystickGetButton(joy->joystick, button))
			{
				if (++joy->buttonsRepeat[button] == 1) YGS2kLastInputType = YGS_INPUT_JOYSTICK;
			}
			else
			{
				joy->buttonsRepeat[button] = 0;
			}
		}
	}
}

bool YGS2kIsPushJoyKey ( const YGS2kSJoyKey* const key )
{
	if (!s_aPlayerSlots || s_iNumPlayerSlots <= 0 || key == NULL || key->player >= s_iNumPlayerSlots) return false;

	int player = 0;
	int playerMax = 0;
	if (key->player >= 0)
	{
		YGS2kSJoyGUID checkGUID = YGS2kGetJoyGUID(key->player);
		YGS2kSJoyGUID zeroGUID = { 0 };
		if (memcmp(checkGUID.data, zeroGUID.data, sizeof(checkGUID.data)) != 0 && memcmp(key->guid.data, checkGUID.data, sizeof(checkGUID.data)) == 0)
		{
			player = key->player;
			playerMax = key->player + 1;
		}
	}
	else
	{
		player = 0;
		playerMax = s_iNumPlayerSlots;
	}
	for (; player < playerMax; player++)
	{
		YGS2kSJoy* const joy = &s_aPlayerSlots[player].joy;

		switch (key->type)
		{
		case YGS_JOYKEY_ANY:
			for (int axis = 0; axis < joy->numAxes; axis++)
			{
				if (joy->axesRepeat[axis*2 + 0] == 1) return true;
				if (joy->axesRepeat[axis*2 + 1] == 1) return true;
			}
			for (int hat = 0; hat < joy->numHats; hat++)
			{
				if (joy->hatsRepeat[hat] == 1) return true;
			}
			for (int button = 0; button < joy->numButtons; button++)
			{
				if (joy->buttonsRepeat[button] == 1) return true;
			}
			break;
		case YGS_JOYKEY_AXIS:
			if (key->setting.index < joy->numAxes)
			{
				if (key->setting.value == -YGS_DEADZONE_MAX)
				{
					return joy->axesRepeat[key->setting.index * 2 + 0] == 1;
				}
				else if (key->setting.value == +YGS_DEADZONE_MAX)
				{
					return joy->axesRepeat[key->setting.index * 2 + 1] == 1;
				}
			}
			break;
		case YGS_JOYKEY_HAT:
			if (key->setting.index < joy->numHats)
			{
				switch (key->setting.value)
				{
				case SDL_HAT_LEFT:
					return joy->hatsRepeat[key->setting.index * 4 + 0] == 1;
				case SDL_HAT_RIGHT:
					return joy->hatsRepeat[key->setting.index * 4 + 1] == 1;
				case SDL_HAT_UP:
					return joy->hatsRepeat[key->setting.index * 4 + 2] == 1;
				case SDL_HAT_DOWN:
					return joy->hatsRepeat[key->setting.index * 4 + 3] == 1;
				default:
					break;
				}
			}
			break;
		case YGS_JOYKEY_BUTTON:
			if (key->setting.button < joy->numButtons) return joy->buttonsRepeat[key->setting.button] == 1;
			break;
		default:
			break;
		}
	}
	return false;
}

bool YGS2kIsPressJoyKey ( const YGS2kSJoyKey* const key )
{
	if (!s_aPlayerSlots || s_iNumPlayerSlots <= 0 || key == NULL || key->player >= s_iNumPlayerSlots) return false;

	int player = 0;
	int playerMax = 0;
	if (key->player >= 0)
	{
		YGS2kSJoyGUID checkGUID = YGS2kGetJoyGUID(key->player);
		YGS2kSJoyGUID zeroGUID = { 0 };
		if (memcmp(checkGUID.data, zeroGUID.data, sizeof(checkGUID.data)) != 0 && memcmp(key->guid.data, checkGUID.data, sizeof(checkGUID.data)) == 0)
		{
			player = key->player;
			playerMax = key->player + 1;
		}
	}
	else
	{
		player = 0;
		playerMax = s_iNumPlayerSlots;
	}
	for (; player < playerMax; player++)
	{
		YGS2kSJoy* const joy = &s_aPlayerSlots[player].joy;

		switch (key->type)
		{
		case YGS_JOYKEY_ANY:
			for (int axis = 0; axis < joy->numAxes; axis++)
			{
				if (joy->axesRepeat[axis*2 + 0] != 0) return true;
				if (joy->axesRepeat[axis*2 + 1] != 0) return true;
			}
			for (int hat = 0; hat < joy->numHats; hat++)
			{
				if (joy->hatsRepeat[hat] != 0) return true;
			}
			for (int button = 0; button < joy->numButtons; button++)
			{
				if (joy->buttonsRepeat[button] != 0) return true;
			}
			break;
		case YGS_JOYKEY_AXIS:
			if (key->setting.index < joy->numAxes)
			{
				if (key->setting.value == -YGS_DEADZONE_MAX)
				{
					return joy->axesRepeat[key->setting.index * 2 + 0] != 0;
				}
				else if (key->setting.value == +YGS_DEADZONE_MAX)
				{
					return joy->axesRepeat[key->setting.index * 2 + 1] != 0;
				}
			}
			break;
		case YGS_JOYKEY_HAT:
			if (key->setting.index < joy->numHats)
			{
				switch (key->setting.value)
				{
				case SDL_HAT_LEFT:
					return joy->hatsRepeat[key->setting.index * 4 + 0] != 0;
				case SDL_HAT_RIGHT:
					return joy->hatsRepeat[key->setting.index * 4 + 1] != 0;
				case SDL_HAT_UP:
					return joy->hatsRepeat[key->setting.index * 4 + 2] != 0;
				case SDL_HAT_DOWN:
					return joy->hatsRepeat[key->setting.index * 4 + 3] != 0;
				default:
					break;
				}
			}
			break;
		case YGS_JOYKEY_BUTTON:
			if (key->setting.button < joy->numButtons) return joy->buttonsRepeat[key->setting.button] != 0;
			break;
		default:
			break;
		}
	}
	return false;
}

int YGS2kGetJoyKeyRepeat ( const YGS2kSJoyKey* const key )
{
	if (!s_aPlayerSlots || s_iNumPlayerSlots <= 0 || key == NULL || key->player >= s_iNumPlayerSlots) return 0;

	bool multi;
	int player = 0;
	int playerMax = 0;
	int maxRepeat = 0;
	if (key->player >= 0)
	{
		YGS2kSJoyGUID checkGUID = YGS2kGetJoyGUID(key->player);
		YGS2kSJoyGUID zeroGUID = { 0 };
		if (memcmp(checkGUID.data, zeroGUID.data, sizeof(checkGUID.data)) != 0 && memcmp(key->guid.data, checkGUID.data, sizeof(checkGUID.data)) == 0)
		{
			player = key->player;
			playerMax = key->player + 1;
			multi = false;
		}
	}
	else
	{
		player = 0;
		playerMax = s_iNumPlayerSlots;
		multi = true;
	}
	for (; player < playerMax; player++)
	{
		YGS2kSJoy* const joy = &s_aPlayerSlots[player].joy;

		switch (key->type)
		{
		case YGS_JOYKEY_ANY:
			for (int axis = 0; axis < joy->numAxes; axis++)
			{
				if (joy->axesRepeat[axis*2 + 0] > maxRepeat) maxRepeat = joy->axesRepeat[axis*2 + 0];
				if (joy->axesRepeat[axis*2 + 1] > maxRepeat) maxRepeat = joy->axesRepeat[axis*2 + 1];
			}
			for (int hat = 0; hat < joy->numHats; hat++)
			{
				if (joy->hatsRepeat[hat] > maxRepeat) maxRepeat = joy->hatsRepeat[hat];
			}
			for (int button = 0; button < joy->numButtons; button++)
			{
				if (joy->buttonsRepeat[button] > maxRepeat) maxRepeat = joy->buttonsRepeat[button];
			}
			break;
		case YGS_JOYKEY_AXIS:
			if (key->setting.index < joy->numAxes)
			{
				if (key->setting.value == -YGS_DEADZONE_MAX)
				{
					if (multi)
					{
						if (joy->axesRepeat[key->setting.index * 2 + 0] > maxRepeat) maxRepeat = joy->axesRepeat[key->setting.index * 2 + 0];
					}
					else return joy->axesRepeat[key->setting.index * 2 + 0];
				}
				else if (key->setting.value == +YGS_DEADZONE_MAX)
				{
					if (multi)
					{
						if (joy->axesRepeat[key->setting.index * 2 + 1] > maxRepeat) maxRepeat = joy->axesRepeat[key->setting.index * 2 + 1];
					}
					else return joy->axesRepeat[key->setting.index * 2 + 1];
				}
			}
			break;
		case YGS_JOYKEY_HAT:
			if (key->setting.index < joy->numHats)
			{
				switch (key->setting.value)
				{
				case SDL_HAT_LEFT:
					if (multi)
					{
						if (joy->hatsRepeat[key->setting.index * 4 + 0] > maxRepeat) maxRepeat = joy->hatsRepeat[key->setting.index * 4 + 0];
					}
					else return joy->hatsRepeat[key->setting.index * 4 + 0];
				case SDL_HAT_RIGHT:
					if (multi)
					{
						if (joy->hatsRepeat[key->setting.index * 4 + 1] > maxRepeat) maxRepeat = joy->hatsRepeat[key->setting.index * 4 + 1];
					}
					else return joy->hatsRepeat[key->setting.index * 4 + 1];
				case SDL_HAT_UP:
					if (multi)
					{
						if (joy->hatsRepeat[key->setting.index * 4 + 2] > maxRepeat) maxRepeat = joy->hatsRepeat[key->setting.index * 4 + 2];
					}
					else return joy->hatsRepeat[key->setting.index * 4 + 2];
				case SDL_HAT_DOWN:
					if (multi)
					{
						if (joy->hatsRepeat[key->setting.index * 4 + 3] > maxRepeat) maxRepeat = joy->hatsRepeat[key->setting.index * 4 + 3];
					}
					else return joy->hatsRepeat[key->setting.index * 4 + 3];
				default:
					break;
				}
			}
			break;
		case YGS_JOYKEY_BUTTON:
			if (key->setting.button < joy->numButtons)
			{
				if (multi)
				{
					if (joy->buttonsRepeat[key->setting.button] > maxRepeat) maxRepeat = joy->buttonsRepeat[key->setting.button];
				}
				else return joy->buttonsRepeat[key->setting.button];
			}
			break;
		default:
			break;
		}
	}
	return maxRepeat;
}

int YGS2kGetNumJoys()
{
	int foundJoys = 0;
	for (int player = 0; player < s_iNumPlayerSlots; player++) {
		if (s_aPlayerSlots[player].type == YGS_PLAYERSLOT_JOY) {
			foundJoys++;
		}
	}
	return foundJoys;
}

YGS2kSJoyGUID YGS2kGetJoyGUID(int player)
{
	if (
		player < 0 ||
		s_iNumPlayerSlots <= 0 ||
		player >= s_iNumPlayerSlots ||
		s_aPlayerSlots[player].type != YGS_PLAYERSLOT_JOY
	) {
		return (YGS2kSJoyGUID) { 0 };
	}

	SDL_JoystickGUID sdlGUID;
	sdlGUID = SDL_JoystickGetGUID(s_aPlayerSlots[player].joy.joystick);

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

int YGS2kGetMaxJoyAxis(int player)
{
	if (player >= 0 && s_iNumPlayerSlots > 0 && player < s_iNumPlayerSlots)
	{
		if (s_aPlayerSlots[player].type == YGS_PLAYERSLOT_JOY)
		{
			return s_aPlayerSlots[player].joy.numAxes;
		}
		else
		{
			return -1;
		}
	}
	else
	{
		return -1;
	}
}

int YGS2kGetMaxJoyHat(int player)
{
	if (player >= 0 && s_iNumPlayerSlots > 0 && player < s_iNumPlayerSlots)
	{
		if (s_aPlayerSlots[player].type == YGS_PLAYERSLOT_JOY)
		{
			return s_aPlayerSlots[player].joy.numHats;
		}
		else
		{
			return -1;
		}
	}
	else
	{
		return -1;
	}
}

int YGS2kGetMaxJoyButton(int player)
{
	if (player >= 0 && s_iNumPlayerSlots > 0 && player < s_iNumPlayerSlots)
	{
		if (s_aPlayerSlots[player].type == YGS_PLAYERSLOT_JOY)
		{
			return s_aPlayerSlots[player].joy.numButtons;
		}
		else
		{
			return -1;
		}
	}
	else
	{
		return -1;
	}
}
#endif

#ifdef ENABLE_GAME_CONTROLLER

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
	if (!s_aPlayerSlots) return;

	for (int player = 0; player < s_iNumPlayerSlots; player++)
	{
		YGS2kSPlayerSlot* const slot = &s_aPlayerSlots[player];
		if (slot->type != YGS_PLAYERSLOT_CON) {
			continue;
		}
		YGS2kSCon* const con = &slot->con;

		if (!SDL_GameControllerGetAttached(con->controller)) continue;

		YGS2kEInputType const inputType = YGS2kGetConType(player);
		Sint16 axisValue;

		axisValue = SDL_GameControllerGetAxis(con->controller, SDL_CONTROLLER_AXIS_LEFTX);
		if (axisValue > 0 && axisValue > +YGS_DEADZONE_MAX)
		{
			if (++con->axesRepeat[0] == 1)
			{
				YGS2kLastInputType = inputType;
				s_iLastActiveCon = player;
			}
			con->axesRepeat[1] = 0;
		}
		else if (axisValue < 0 && axisValue < -YGS_DEADZONE_MAX)
		{
			con->axesRepeat[0] = 0;
			if (++con->axesRepeat[1] == 1)
			{
				YGS2kLastInputType = inputType;
				s_iLastActiveCon = player;
			}
		}
		else
		{
			con->axesRepeat[0] = 0;
			con->axesRepeat[1] = 0;
		}

		axisValue = SDL_GameControllerGetAxis(con->controller, SDL_CONTROLLER_AXIS_LEFTY);
		if (axisValue > 0 && axisValue > +YGS_DEADZONE_MAX)
		{
			if (++con->axesRepeat[2] == 1)
			{
				YGS2kLastInputType = inputType;
				s_iLastActiveCon = player;
			}
			con->axesRepeat[3] = 0;
		}
		else if (axisValue < 0 && axisValue < -YGS_DEADZONE_MAX)
		{
			con->axesRepeat[2] = 0;
			if (++con->axesRepeat[3] == 1)
			{
				YGS2kLastInputType = inputType;
				s_iLastActiveCon = player;
			}
		}
		else
		{
			con->axesRepeat[2] = 0;
			con->axesRepeat[3] = 0;
		}

		axisValue = SDL_GameControllerGetAxis(con->controller, SDL_CONTROLLER_AXIS_RIGHTX);
		if (axisValue > 0 && axisValue > +YGS_DEADZONE_MAX)
		{
			if (++con->axesRepeat[4] == 1)
			{
				YGS2kLastInputType = inputType;
				s_iLastActiveCon = player;
			}
			con->axesRepeat[5] = 0;
		}
		else if (axisValue < 0 && axisValue < -YGS_DEADZONE_MAX)
		{
			con->axesRepeat[4] = 0;
			if (++con->axesRepeat[5] == 1)
			{
				YGS2kLastInputType = inputType;
				s_iLastActiveCon = player;
			}
		}
		else
		{
			con->axesRepeat[4] = 0;
			con->axesRepeat[5] = 0;
		}

		axisValue = SDL_GameControllerGetAxis(con->controller, SDL_CONTROLLER_AXIS_RIGHTY);
		if (axisValue > 0 && axisValue > +YGS_DEADZONE_MAX)
		{
			if (++con->axesRepeat[6] == 1)
			{
				YGS2kLastInputType = inputType;
				s_iLastActiveCon = player;
			}
			con->axesRepeat[7] = 0;
		}
		else if (axisValue < 0 && axisValue < -YGS_DEADZONE_MAX)
		{
			con->axesRepeat[6] = 0;
			if (++con->axesRepeat[7] == 1)
			{
				YGS2kLastInputType = inputType;
				s_iLastActiveCon = player;
			}
		}
		else
		{
			con->axesRepeat[6] = 0;
			con->axesRepeat[7] = 0;
		}

		axisValue = SDL_GameControllerGetAxis(con->controller, SDL_CONTROLLER_AXIS_TRIGGERLEFT);
		if (axisValue > YGS_DEADZONE_MAX)
		{
			if (++con->axesRepeat[8] == 1)
			{
				YGS2kLastInputType = inputType;
				s_iLastActiveCon = player;
			}
		}
		else
		{
			con->axesRepeat[8] = 0;
		}

		axisValue = SDL_GameControllerGetAxis(con->controller, SDL_CONTROLLER_AXIS_TRIGGERRIGHT);
		if (axisValue > YGS_DEADZONE_MAX)
		{
			if (++con->axesRepeat[9] == 1)
			{
				YGS2kLastInputType = inputType;
				s_iLastActiveCon = player;
			}
		}
		else
		{
			con->axesRepeat[9] = 0;
		}

		for (SDL_GameControllerButton button = 0; button < SDL_CONTROLLER_BUTTON_MAX; button++)
		{
			if (SDL_GameControllerGetButton(con->controller, button))
			{
				if (++con->buttonsRepeat[button] == 1)
				{
					YGS2kLastInputType = inputType;
					s_iLastActiveCon = player;
				}
			}
			else
			{
				con->buttonsRepeat[button] = 0;
			}
		}
	}
}

bool YGS2kIsPushConKey ( const int player, const YGS2kSConKey* const key )
{
	if (!key || !s_aPlayerSlots || s_iNumPlayerSlots == 0 || player >= s_iNumPlayerSlots) return false;

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
		playerMax = s_iNumPlayerSlots;
	}
	for (; playerStart < playerMax; playerStart++)
	{
		if (s_aPlayerSlots[playerStart].type != YGS_PLAYERSLOT_CON)
		{
			continue;
		}

		YGS2kSCon* const con = &s_aPlayerSlots[playerStart].con;

		switch (key->type)
		{
		case YGS_CONKEY_ANY:
			for (int axis = 0; axis < YGS_CONAXIS_MAX; axis++)
			{
				if (con->axesRepeat[axis] == 1) return true;
			}
			for (int button = 0; button < YGS_CONBUTTON_MAX; button++)
			{
				if (con->buttonsRepeat[button] == 1) return true;
			}
			break;
		case YGS_CONKEY_AXIS:
			if (key->index < YGS_CONAXIS_MAX)
			{
				if (con->axesRepeat[key->index] == 1) return true;
			}
			break;
		case YGS_CONKEY_BUTTON:
			if (key->index < YGS_CONBUTTON_MAX)
			{
				if (con->buttonsRepeat[key->index] == 1) return true;
			}
			break;
		default:
			break;
		}
	}
	return false;
}

bool YGS2kIsPressConKey ( const int player, const YGS2kSConKey* const key )
{
	if (!key || !s_aPlayerSlots || s_iNumPlayerSlots == 0 || player >= s_iNumPlayerSlots) return false;

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
		playerMax = s_iNumPlayerSlots;
	}
	for (; playerStart < playerMax; playerStart++)
	{
		if (s_aPlayerSlots[playerStart].type != YGS_PLAYERSLOT_CON)
		{
			continue;
		}

		YGS2kSCon* const con = &s_aPlayerSlots[playerStart].con;

		switch (key->type)
		{
		case YGS_CONKEY_ANY:
			for (int axis = 0; axis < YGS_CONAXIS_MAX; axis++)
			{
				if (con->axesRepeat[axis] != 0) return true;
			}
			for (int button = 0; button < YGS_CONBUTTON_MAX; button++)
			{
				if (con->buttonsRepeat[button] != 0) return true;
			}
			break;
		case YGS_CONKEY_AXIS:
			if (key->index < YGS_CONAXIS_MAX)
			{
				if (con->axesRepeat[key->index] != 0) return true;
			}
			break;
		case YGS_CONKEY_BUTTON:
			if (key->index < YGS_CONBUTTON_MAX)
			{
				if (con->buttonsRepeat[key->index] != 0) return true;
			}
			break;
		default:
			break;
		}
	}
	return false;
}

int YGS2kGetConKeyRepeat ( const int player, YGS2kSConKey* const key )
{
	if (!key || !s_aPlayerSlots || s_iNumPlayerSlots == 0 || player >= s_iNumPlayerSlots) return 0;

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
		playerMax = s_iNumPlayerSlots;
		multi = true;
	}
	for (; playerStart < playerMax; playerStart++)
	{
		if (s_aPlayerSlots[playerStart].type != YGS_PLAYERSLOT_CON)
		{
			continue;
		}

		YGS2kSCon* const con = &s_aPlayerSlots[playerStart].con;

		switch (key->type)
		{
		case YGS_CONKEY_ANY:
			for (int axis = 0; axis < YGS_CONAXIS_MAX; axis++)
			{
				if (con->axesRepeat[axis] > maxRepeat) maxRepeat = con->axesRepeat[axis];
			}
			for (int button = 0; button < YGS_CONBUTTON_MAX; button++)
			{
				if (con->buttonsRepeat[button] > maxRepeat) maxRepeat = con->buttonsRepeat[button];
			}
			break;
		case YGS_CONKEY_AXIS:
			if (key->index < YGS_CONAXIS_MAX)
			{
					if (multi)
					{
						if (con->axesRepeat[key->index] > maxRepeat) maxRepeat = con->axesRepeat[key->index];
					}
					else return con->axesRepeat[key->index];
			}
			break;
		case YGS_CONKEY_BUTTON:
			if (key->index < YGS_CONBUTTON_MAX)
			{
				if (multi)
				{
					if (con->buttonsRepeat[key->index] > maxRepeat) maxRepeat = con->buttonsRepeat[key->index];
				}
				else return con->buttonsRepeat[key->index];
			}
			break;
		default:
			break;
		}
	}
	return maxRepeat;
}

int YGS2kGetNumCons()
{
	int foundCons = 0;
	for (int player = 0; player < s_iNumPlayerSlots; player++) {
		if (s_aPlayerSlots[player].type == YGS_PLAYERSLOT_CON) {
			foundCons++;
		}
	}
	return foundCons;
}

void YGS2kResetLastActiveCon()
{
	s_iLastActiveCon = -1;
}

int YGS2kGetLastActiveCon()
{
	return s_iLastActiveCon;
}

/*
 * Controller type constants were introduced in SDL 2.0.12, with later versions
 * introducing more.
 *
 * SDL 2.0.12:
 * SDL_CONTROLLER_TYPE_UNKNOWN
 * SDL_CONTROLLER_TYPE_XBOX360
 * SDL_CONTROLLER_TYPE_XBOXONE
 * SDL_CONTROLLER_TYPE_PS3
 * SDL_CONTROLLER_TYPE_PS4
 * SDL_CONTROLLER_TYPE_NINTENDO_SWITCH_PRO
 *
 * SDL 2.0.14:
 * SDL_CONTROLLER_TYPE_VIRTUAL
 * SDL_CONTROLLER_TYPE_PS5
 *
 * SDL 2.0.16:
 * SDL_CONTROLLER_TYPE_AMAZON_LUNA
 * SDL_CONTROLLER_TYPE_GOOGLE_STADIA
 *
 * SDL 2.24.0:
 * SDL_CONTROLLER_TYPE_NVIDIA_SHIELD
 * SDL_CONTROLLER_TYPE_NINTENDO_SWITCH_JOYCON_LEFT
 * SDL_CONTROLLER_TYPE_NINTENDO_SWITCH_JOYCON_RIGHT
 * SDL_CONTROLLER_TYPE_NINTENDO_SWITCH_JOYCON_PAIR
 */
YGS2kEInputType YGS2kGetConType(const int player)
{
	if (!s_aPlayerSlots || s_iNumPlayerSlots == 0 || player < 0 || player >= s_iNumPlayerSlots || s_aPlayerSlots[player].type != YGS_PLAYERSLOT_CON) return YGS_INPUT_NULL;

	#ifdef ONLY_INPUT_TYPE
	return ONLY_INPUT_TYPE;
	#else
	switch(YGS2kGetSDLGameControllerType(s_aPlayerSlots[player].con.controller))
	{
	default:
	case SDL_CONTROLLER_TYPE_XBOX360:
	case SDL_CONTROLLER_TYPE_XBOXONE:
		return YGS_INPUT_XBOX;

	case SDL_CONTROLLER_TYPE_PS3:
	case SDL_CONTROLLER_TYPE_PS4:
		return YGS_INPUT_PLAYSTATION;

	case SDL_CONTROLLER_TYPE_NINTENDO_SWITCH_PRO:
		return YGS_INPUT_NINTENDO;

	#if SDL_VERSION_ATLEAST(2, 0, 14)
	case SDL_CONTROLLER_TYPE_VIRTUAL:
		return YGS_INPUT_XBOX;

	case SDL_CONTROLLER_TYPE_PS5:
		return YGS_INPUT_PLAYSTATION;
	#endif

	#if SDL_VERSION_ATLEAST(2, 0, 16)
	case SDL_CONTROLLER_TYPE_AMAZON_LUNA:
	case SDL_CONTROLLER_TYPE_GOOGLE_STADIA:
		return YGS_INPUT_XBOX;
	#endif

	#if SDL_VERSION_ATLEAST(2, 24, 0)
	case SDL_CONTROLLER_TYPE_NVIDIA_SHIELD:
		return YGS_INPUT_XBOX;

	case SDL_CONTROLLER_TYPE_NINTENDO_SWITCH_JOYCON_LEFT:
	case SDL_CONTROLLER_TYPE_NINTENDO_SWITCH_JOYCON_RIGHT:
	case SDL_CONTROLLER_TYPE_NINTENDO_SWITCH_JOYCON_PAIR:
		return YGS_INPUT_NINTENDO;
	#endif
	}
	#endif
}

bool YGS2kGetConKeyDesc(const int player, const YGS2kSConKey* const key, const char** text, EButton* button)
{
	if (
		!s_aPlayerSlots ||
		s_iNumPlayerSlots == 0 ||
		player < 0 ||
		player >= s_iNumPlayerSlots ||
		s_aPlayerSlots[player].type != YGS_PLAYERSLOT_CON ||
		!key ||
		!text ||
		!button
	) {
		return false;
	}

	*text = NULL;
	*button = BTN_NULL;
	const SDL_GameControllerType sdlGameControllerType = YGS2kGetSDLGameControllerType(s_aPlayerSlots[player].con.controller);
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
			#if SDL_VERSION_ATLEAST(2, 0, 14)
			case SDL_CONTROLLER_TYPE_VIRTUAL:
			#endif
			#if SDL_VERSION_ATLEAST(2, 0, 16)
			case SDL_CONTROLLER_TYPE_AMAZON_LUNA:
			#endif
			#if SDL_VERSION_ATLEAST(2, 24, 0)
			case SDL_CONTROLLER_TYPE_NVIDIA_SHIELD:
			#endif
				*text = "LT";
				break;

			case SDL_CONTROLLER_TYPE_PS3:
			case SDL_CONTROLLER_TYPE_PS4:
			#if SDL_VERSION_ATLEAST(2, 0, 14)
			case SDL_CONTROLLER_TYPE_PS5:
			#endif
			#if SDL_VERSION_ATLEAST(2, 0, 16)
			case SDL_CONTROLLER_TYPE_GOOGLE_STADIA:
			#endif
				*text = "L2";
				break;

			case SDL_CONTROLLER_TYPE_NINTENDO_SWITCH_PRO:
			#if SDL_VERSION_ATLEAST(2, 24, 0)
			case SDL_CONTROLLER_TYPE_NINTENDO_SWITCH_JOYCON_PAIR:
			#endif
				*text = "ZL";
				break;

			#if SDL_VERSION_ATLEAST(2, 24, 0)
			case SDL_CONTROLLER_TYPE_NINTENDO_SWITCH_JOYCON_LEFT:
			case SDL_CONTROLLER_TYPE_NINTENDO_SWITCH_JOYCON_RIGHT:
				*text = "SL";
				break;
			#endif
			}
			break;
		case 9:
			switch(sdlGameControllerType)
			{
			default:
			case SDL_CONTROLLER_TYPE_XBOX360:
			case SDL_CONTROLLER_TYPE_XBOXONE:
			#if SDL_VERSION_ATLEAST(2, 0, 14)
			case SDL_CONTROLLER_TYPE_VIRTUAL:
			#endif
			#if SDL_VERSION_ATLEAST(2, 0, 16)
			case SDL_CONTROLLER_TYPE_AMAZON_LUNA:
			#endif
			#if SDL_VERSION_ATLEAST(2, 24, 0)
			case SDL_CONTROLLER_TYPE_NVIDIA_SHIELD:
			#endif
				*text = "RT";
				break;

			case SDL_CONTROLLER_TYPE_PS3:
			case SDL_CONTROLLER_TYPE_PS4:
			#if SDL_VERSION_ATLEAST(2, 0, 14)
			case SDL_CONTROLLER_TYPE_PS5:
			#endif
			#if SDL_VERSION_ATLEAST(2, 0, 16)
			case SDL_CONTROLLER_TYPE_GOOGLE_STADIA:
			#endif
				*text = "R2";
				break;

			case SDL_CONTROLLER_TYPE_NINTENDO_SWITCH_PRO:
			#if SDL_VERSION_ATLEAST(2, 24, 0)
			case SDL_CONTROLLER_TYPE_NINTENDO_SWITCH_JOYCON_PAIR:
			#endif
				*text = "ZR";
				break;

			#if SDL_VERSION_ATLEAST(2, 24, 0)
			case SDL_CONTROLLER_TYPE_NINTENDO_SWITCH_JOYCON_LEFT:
			case SDL_CONTROLLER_TYPE_NINTENDO_SWITCH_JOYCON_RIGHT:
				*text = "SR";
				break;
			#endif
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
			#if SDL_VERSION_ATLEAST(2, 24, 0)
			case SDL_CONTROLLER_TYPE_NINTENDO_SWITCH_JOYCON_LEFT:
			case SDL_CONTROLLER_TYPE_NINTENDO_SWITCH_JOYCON_PAIR:
			#endif
				*text = "CAPTURE";
				break;

			#if SDL_VERSION_ATLEAST(2, 0, 14)
			case SDL_CONTROLLER_TYPE_PS5:
			#if SDL_VERSION_ATLEAST(2, 0, 16)
			case SDL_CONTROLLER_TYPE_AMAZON_LUNA:
			case SDL_CONTROLLER_TYPE_GOOGLE_STADIA:
			#endif
			#if SDL_VERSION_ATLEAST(2, 24, 0)
			case SDL_CONTROLLER_TYPE_NVIDIA_SHIELD:
			#endif
				*text = "MICROPHONE";
				break;
			#endif

			#if SDL_VERSION_ATLEAST(2, 0, 14)
			case SDL_CONTROLLER_TYPE_VIRTUAL:
			default:
				*text = "MISC1";
				break;
			#endif
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
			case SDL_CONTROLLER_TYPE_NINTENDO_SWITCH_PRO:
			#if SDL_VERSION_ATLEAST(2, 0, 14)
			case SDL_CONTROLLER_TYPE_VIRTUAL:
			#endif
			#if SDL_VERSION_ATLEAST(2, 24, 0)
			case SDL_CONTROLLER_TYPE_NVIDIA_SHIELD:
			#endif
			#if SDL_VERSION_ATLEAST(2, 24, 0)
			case SDL_CONTROLLER_TYPE_NINTENDO_SWITCH_JOYCON_LEFT:
			case SDL_CONTROLLER_TYPE_NINTENDO_SWITCH_JOYCON_RIGHT:
			case SDL_CONTROLLER_TYPE_NINTENDO_SWITCH_JOYCON_PAIR:
			#endif
				*text = "LS";
				break;

			case SDL_CONTROLLER_TYPE_PS3:
			case SDL_CONTROLLER_TYPE_PS4:
			#if SDL_VERSION_ATLEAST(2, 0, 14)
			case SDL_CONTROLLER_TYPE_PS5:
			#endif
			#if SDL_VERSION_ATLEAST(2, 0, 16)
			case SDL_CONTROLLER_TYPE_AMAZON_LUNA:
			case SDL_CONTROLLER_TYPE_GOOGLE_STADIA:
			#endif
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
			case SDL_CONTROLLER_TYPE_NINTENDO_SWITCH_PRO:
			#if SDL_VERSION_ATLEAST(2, 0, 14)
			case SDL_CONTROLLER_TYPE_VIRTUAL:
			#endif
			#if SDL_VERSION_ATLEAST(2, 24, 0)
			case SDL_CONTROLLER_TYPE_NVIDIA_SHIELD:
			case SDL_CONTROLLER_TYPE_NINTENDO_SWITCH_JOYCON_PAIR:
			#endif
				*text = "RS";
				break;

			case SDL_CONTROLLER_TYPE_PS3:
			case SDL_CONTROLLER_TYPE_PS4:
			#if SDL_VERSION_ATLEAST(2, 0, 14)
			case SDL_CONTROLLER_TYPE_PS5:
			#endif
			#if SDL_VERSION_ATLEAST(2, 0, 16)
			case SDL_CONTROLLER_TYPE_AMAZON_LUNA:
			case SDL_CONTROLLER_TYPE_GOOGLE_STADIA:
			#endif
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
			#if SDL_VERSION_ATLEAST(2, 0, 14)
			case SDL_CONTROLLER_TYPE_VIRTUAL:
			#endif
			#if SDL_VERSION_ATLEAST(2, 0, 16)
			case SDL_CONTROLLER_TYPE_AMAZON_LUNA:
			#endif
			#if SDL_VERSION_ATLEAST(2, 24, 0)
			case SDL_CONTROLLER_TYPE_NVIDIA_SHIELD:
			#endif
				*text = "LB";
				break;

			case SDL_CONTROLLER_TYPE_PS3:
			case SDL_CONTROLLER_TYPE_PS4:
			#if SDL_VERSION_ATLEAST(2, 0, 14)
			case SDL_CONTROLLER_TYPE_PS5:
			#endif
			#if SDL_VERSION_ATLEAST(2, 0, 16)
			case SDL_CONTROLLER_TYPE_GOOGLE_STADIA:
			#endif
				*text = "L1";
				break;

			case SDL_CONTROLLER_TYPE_NINTENDO_SWITCH_PRO:
			#if SDL_VERSION_ATLEAST(2, 24, 0)
			case SDL_CONTROLLER_TYPE_NINTENDO_SWITCH_JOYCON_PAIR:
			#endif
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
			#if SDL_VERSION_ATLEAST(2, 0, 14)
			case SDL_CONTROLLER_TYPE_VIRTUAL:
			#endif
			#if SDL_VERSION_ATLEAST(2, 0, 16)
			case SDL_CONTROLLER_TYPE_AMAZON_LUNA:
			#endif
			#if SDL_VERSION_ATLEAST(2, 24, 0)
			case SDL_CONTROLLER_TYPE_NVIDIA_SHIELD:
			#endif
				*text = "RB";
				break;

			case SDL_CONTROLLER_TYPE_PS3:
			case SDL_CONTROLLER_TYPE_PS4:
			#if SDL_VERSION_ATLEAST(2, 0, 14)
			case SDL_CONTROLLER_TYPE_PS5:
			#endif
			#if SDL_VERSION_ATLEAST(2, 0, 16)
			case SDL_CONTROLLER_TYPE_GOOGLE_STADIA:
			#endif
				*text = "R1";
				break;

			case SDL_CONTROLLER_TYPE_NINTENDO_SWITCH_PRO:
			#if SDL_VERSION_ATLEAST(2, 24, 0)
			case SDL_CONTROLLER_TYPE_NINTENDO_SWITCH_JOYCON_PAIR:
			#endif
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

	#ifdef ENABLE_GAME_CONTROLLER
	/* パッドの初期化 */
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

	#if defined(ENABLE_JOYSTICK) || defined(ENABLE_GAME_CONTROLLER)
	YGS2kPlayerSlotsClose();
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
