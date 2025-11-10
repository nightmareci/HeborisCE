#include "APP_input.h"

#ifdef APP_ONLY_INPUT_TYPE
static APP_InputType APP_LastInputType = APP_ONLY_INPUT_TYPE;
#else
static APP_InputType APP_LastInputType = APP_INPUT_NULL;
#endif

#ifdef APP_ENABLE_LINUX_GPIO
static int			APP_GPIORepeat[10];
static struct gpiod_chip	*APP_GPIOChip;
static struct gpiod_line	*APP_GPIOLines[10];
#endif

#ifdef APP_ENABLE_KEYBOARD
static int			APP_KeyRepeat[APP_KEY_MAX];
#endif

#ifdef APP_ENABLE_JOYSTICK
typedef struct APP_Joy
{
	SDL_Joystick* joystick;
	int numAxes;
	int numHats;
	int numButtons;
	int* axesRepeat;
	int* hatsRepeat;
	int* buttonsRepeat;
} APP_Joy;
#endif

#ifdef APP_ENABLE_GAME_CONTROLLER
typedef struct APP_Con
{
	SDL_Gamepad* controller;
	int axesRepeat[APP_CONAXIS_MAX];
	int buttonsRepeat[APP_CONBUTTON_MAX];
} APP_Con;
static int APP_LastActiveCon = -1;
#endif

#if defined(APP_ENABLE_JOYSTICK) || defined(APP_ENABLE_GAME_CONTROLLER)
typedef struct APP_PlayerSlot
{
	APP_PlayerSlotType type;
	union
	{
		#ifdef APP_ENABLE_JOYSTICK
		APP_Joy joy;
		#endif
		#ifdef APP_ENABLE_GAME_CONTROLLER
		APP_Con con;
		#endif
	};
} APP_PlayerSlot;

/*
Builtin controllers are controllers that can't be detached from the host system
(normally...), and are always active, even while docked; an external display is
attached and external controllers are attached. The builtin controller is
specially treated as always controlling player 1, with external controllers then
assigned player slots 1, 2, etc. So, in the case of a builtin controller and
only one attached external controller, both controllers control player 1, then
if one more external controller is attached, that controller controls player 2;
this is the behavior desired when docking a handheld system.

If a device has a builtin controller, but its host software automatically
detaches the controller upon docking, that device's builtin controller *should
not* be placed here. This list is *only* for when the builtin controller can
still be used even while external controllers are attached while docked.
*/
#define APP_CON_ID(vendor, product) (Uint32)(((Uint32)(Uint16)(vendor) << 16) | (Uint16)(product))
static Uint32 APP_BuiltinConIDs[] = {
	APP_CON_ID(0x28de, 0x1205) // Steam Deck
};
static APP_Con APP_BuiltinCon = { 0 };
static bool APP_LastInputBuiltinCon = true;

static APP_PlayerSlot* APP_PlayerSlots = NULL;
static int APP_NumPlayerSlots = 0;
#endif

APP_InputType APP_GetLastInputType(void)
{
	return APP_LastInputType;
}

#ifdef APP_ENABLE_LINUX_GPIO
bool APP_IsPushGPIO ( int key )
{
	return key >= 0 && key < NUMBTNS && APP_GPIORepeat[key] == 1;
}

bool APP_IsPressGPIO ( int key )
{
	return key >= 0 && key < NUMBTNS && APP_GPIORepeat[key] != 0;
}

int APP_GetGPIORepeat( int key )
{
	return key >= 0 && key < NUMBTNS ? APP_GPIORepeat[key] : 0;
}
#endif

#ifdef APP_ENABLE_KEYBOARD
bool APP_IsPushKey (int key)
{
	return key >= 0 && key < APP_KEY_MAX && APP_KeyRepeat[key] == 1;
}

bool APP_IsPressKey (int key)
{
	return key >= 0 && key < APP_KEY_MAX && APP_KeyRepeat[key] != 0;
}

int APP_GetKeyRepeat( int key )
{
	return key >= 0 && key < APP_KEY_MAX ? APP_KeyRepeat[key] : 0;
}

int APP_GetMaxKey(void)
{
	return APP_KEY_MAX;
}
#endif

#if defined(APP_ENABLE_JOYSTICK) || defined(APP_ENABLE_GAME_CONTROLLER)

static bool APP_IsCon ( SDL_JoystickID joy )
{
	if ( !joy ) {
		return false;
	}

	if (
		SDL_IsGamepad(joy) &&
		SDL_GetGamepadTypeForID(joy) != SDL_GAMEPAD_TYPE_UNKNOWN
	) {
		return true;
	}

	char* const mapping = SDL_GetGamepadMappingForID(joy);
	const bool isGamepad = mapping != NULL;
	SDL_free(mapping);
	return isGamepad;
}

static bool APP_ResizePlayerSlots(int numSlots) {
	if (numSlots == APP_NumPlayerSlots) {
		return true;
	}
	else if (numSlots == 0) {
		SDL_free(APP_PlayerSlots);
		APP_PlayerSlots = NULL;
		APP_NumPlayerSlots = 0;
		return true;
	}

	APP_PlayerSlot* const oldSlots = APP_PlayerSlots;
	if (APP_PlayerSlots) {
		APP_PlayerSlots = SDL_realloc(APP_PlayerSlots, sizeof(APP_PlayerSlot) * numSlots);
	}
	else {
		APP_PlayerSlots = SDL_malloc(sizeof(APP_PlayerSlot) * numSlots);
	}
	if (!APP_PlayerSlots) {
		APP_PlayerSlots = oldSlots;
		return SDL_SetError("Failed to resize joysticks/controllers array");
	}
	for (int player = APP_NumPlayerSlots; player < numSlots; player++) {
		APP_PlayerSlots[player] = (APP_PlayerSlot) { 0 };
	}
	APP_NumPlayerSlots = numSlots;
	return true;
}

static bool APP_ConIsBuiltin(SDL_Gamepad * const controller) {
	for (size_t i = 0; i < SDL_arraysize(APP_BuiltinConIDs); i++) {
		const Uint16 vendor = SDL_GetGamepadVendor(controller);
		const Uint16 product = SDL_GetGamepadProduct(controller);
		const Uint32 conID = APP_CON_ID(vendor, product);
		if (conID == APP_BuiltinConIDs[i]) {
			return true;
		}
	}
	return false;
}

bool APP_PlayerSlotsChanged(void) {
	#ifdef APP_ENABLE_GAME_CONTROLLER
	// This should never be needed, as the builtin controller most likely will
	// always be connected, but you never know, maybe a modder detached it.
	if (APP_BuiltinCon.controller && !SDL_GamepadConnected(APP_BuiltinCon.controller)) {
		APP_BuiltinCon = (APP_Con) { 0 };
	}
	#endif
	for (int player = 0; player < APP_NumPlayerSlots; player++) {
		APP_PlayerSlot* const slot = &APP_PlayerSlots[player];
		switch (slot->type) {
		#ifdef APP_ENABLE_JOYSTICK
		case APP_PLAYERSLOT_JOY:
			if (!SDL_JoystickConnected(slot->joy.joystick)) {
				if (slot->joy.axesRepeat) SDL_free(slot->joy.axesRepeat);
				if (slot->joy.hatsRepeat) SDL_free(slot->joy.hatsRepeat);
				if (slot->joy.buttonsRepeat) SDL_free(slot->joy.buttonsRepeat);
				*slot = (APP_PlayerSlot) { 0 };
			}
			break;
		#endif

		#ifdef APP_ENABLE_GAME_CONTROLLER
		case APP_PLAYERSLOT_CON:
			if (!SDL_GamepadConnected(slot->con.controller)) {
				*slot = (APP_PlayerSlot) { 0 };
			}
			break;
		#endif

		default:
		case APP_PLAYERSLOT_NULL:
			break;
		}
	}

	int numJoys;
	SDL_JoystickID* const joys = SDL_GetJoysticks(&numJoys);
	if (!joys) {
		return SDL_SetError("Failed to get joysticks list: %s", SDL_GetError());
	}

	#ifdef APP_ENABLE_GAME_CONTROLLER
	for (int device = 0; device < numJoys; device++) {
		if (!APP_IsCon(joys[device])) {
			continue;
		}

		SDL_Gamepad * const controller = SDL_OpenGamepad(joys[device]);
		if (!controller) {
			SDL_free(joys);
			return SDL_SetError("Failed to open controller: %s", SDL_GetError());
		}
		if (!APP_BuiltinCon.controller && APP_ConIsBuiltin(controller)) {
			APP_BuiltinCon.controller = controller;
			continue;
		}
		bool playerIndexFound = false;
		int player;
		for (player = 0; player < APP_NumPlayerSlots; player++) {
			if (APP_PlayerSlots[player].type == APP_PLAYERSLOT_NULL) {
				playerIndexFound = true;
				break;
			}
		}
		if (!playerIndexFound) {
			if (!APP_ResizePlayerSlots(APP_NumPlayerSlots + 1)) {
				SDL_CloseGamepad(controller);
				SDL_free(joys);
				return SDL_SetError("Failed to open controller: %s", SDL_GetError());
			}
			player = APP_NumPlayerSlots - 1;
		}
		APP_PlayerSlots[player].con.controller = controller;
		APP_PlayerSlots[player].type = APP_PLAYERSLOT_CON;
	}
	#endif

	#ifdef APP_ENABLE_JOYSTICK
	for (int device = 0; device < numJoys; device++) {
		if (APP_IsCon(joys[device])) {
			continue;
		}

		SDL_Joystick* const joystick = SDL_OpenJoystick(joys[device]);
		if (!joystick) {
			SDL_SetError("Failed to open joystick: %s", SDL_GetError());
			goto fail;
		}
		bool playerIndexFound = false;
		int player;
		for (player = 0; player < APP_NumPlayerSlots; player++) {
			if (APP_PlayerSlots[player].type == APP_PLAYERSLOT_NULL) {
				playerIndexFound = true;
				break;
			}
		}
		if (!playerIndexFound) {
			if (!APP_ResizePlayerSlots(APP_NumPlayerSlots + 1)) {
				goto fail;
			}
			player = APP_NumPlayerSlots - 1;
		}

		if (
			(APP_PlayerSlots[player].joy.numAxes = SDL_GetNumJoystickAxes(joystick)) >= 0 &&
			(APP_PlayerSlots[player].joy.numHats = SDL_GetNumJoystickHats(joystick)) >= 0 &&
			(APP_PlayerSlots[player].joy.numButtons = SDL_GetNumJoystickButtons(joystick)) >= 0
		) {
			if (APP_PlayerSlots[player].joy.numAxes > 0 && !(APP_PlayerSlots[player].joy.axesRepeat = (int*)SDL_calloc((size_t)APP_PlayerSlots[player].joy.numAxes * 2, sizeof(int)))) {
				SDL_SetError("Failed to allocate axes repeat array for joystick");
				goto fail;
			}
			if (APP_PlayerSlots[player].joy.numHats > 0 && !(APP_PlayerSlots[player].joy.hatsRepeat = (int*)SDL_calloc((size_t)APP_PlayerSlots[player].joy.numHats * 4, sizeof(int)))) {
				SDL_SetError("Failed to allocate hats repeat array for joystick");
				goto fail;
			}
			if (APP_PlayerSlots[player].joy.numButtons > 0 && !(APP_PlayerSlots[player].joy.buttonsRepeat = (int*)SDL_calloc((size_t)APP_PlayerSlots[player].joy.numButtons, sizeof(int)))) {
				SDL_SetError("Failed to allocate buttons repeat array for joystick");
				goto fail;
			}
		}
		if (false) {
		fail:
			if (APP_PlayerSlots[player].joy.axesRepeat) SDL_free(APP_PlayerSlots[player].joy.axesRepeat);
			if (APP_PlayerSlots[player].joy.hatsRepeat) SDL_free(APP_PlayerSlots[player].joy.hatsRepeat);
			if (APP_PlayerSlots[player].joy.buttonsRepeat) SDL_free(APP_PlayerSlots[player].joy.buttonsRepeat);
			if (joystick) SDL_CloseJoystick(joystick);
			SDL_free(joys);
			return false;
		}
		APP_PlayerSlots[player].joy.joystick = joystick;
		APP_PlayerSlots[player].type = APP_PLAYERSLOT_JOY;
	}
	#endif

	SDL_free(joys);

	int numRequiredSlots = 0;
	for (int player = 0; player < APP_NumPlayerSlots; player++) {
		if (APP_PlayerSlots[player].type != APP_PLAYERSLOT_NULL) {
			numRequiredSlots = player + 1;
		}
	}
	return APP_ResizePlayerSlots(numRequiredSlots);
}

static void APP_PlayerSlotsClose(void)
{
	if (APP_PlayerSlots)
	{
		for (int player = 0; player < APP_NumPlayerSlots; player++)
		{
			switch (APP_PlayerSlots[player].type)
			{
			#ifdef APP_ENABLE_JOYSTICK
			case APP_PLAYERSLOT_JOY: {
				APP_Joy* const joy = &APP_PlayerSlots[player].joy;
				if (SDL_JoystickConnected(joy->joystick))
				{
					SDL_CloseJoystick(joy->joystick);
				}
				if (joy->axesRepeat) SDL_free(joy->axesRepeat);
				if (joy->hatsRepeat) SDL_free(joy->hatsRepeat);
				if (joy->buttonsRepeat) SDL_free(joy->buttonsRepeat);
				break;
			}
			#endif

			#ifdef APP_ENABLE_GAME_CONTROLLER
			case APP_PLAYERSLOT_CON: {
				if (SDL_GamepadConnected(APP_PlayerSlots[player].con.controller))
				{
					SDL_CloseGamepad(APP_PlayerSlots[player].con.controller);
				}
				break;
			}
			#endif

			default:
			case APP_PLAYERSLOT_NULL:
				break;
			}
		}
		SDL_free(APP_PlayerSlots);
	}
	APP_PlayerSlots = NULL;
	APP_NumPlayerSlots = 0;
}

int APP_GetNumPlayerSlots(void)
{
	if (APP_NumPlayerSlots <= 0 && APP_BuiltinCon.controller) {
		return 1;
	}
	else {
		return APP_NumPlayerSlots;
	}
}

APP_PlayerSlotType APP_GetPlayerSlotType(int player)
{
	if (player == 0 && APP_BuiltinCon.controller && APP_LastInputBuiltinCon) {
		return APP_PLAYERSLOT_CON;
	}
	else if (player < 0 || player >= APP_NumPlayerSlots) {
		return APP_PLAYERSLOT_NULL;
	}
	else {
		return APP_PlayerSlots[player].type;
	}
}

#endif

#ifdef APP_ENABLE_JOYSTICK

static void APP_JoyInputsUpdate(void)
{
	if (!APP_PlayerSlots) return;

	for (int player = 0; player < APP_NumPlayerSlots; player++)
	{
		if (
			APP_PlayerSlots[player].type != APP_PLAYERSLOT_JOY ||
			!SDL_JoystickConnected(APP_PlayerSlots[player].joy.joystick)
		) {
			continue;
		}

		const APP_JoyGUID checkGUID = APP_GetJoyGUID(player);
		const APP_JoyGUID zeroGUID = { 0 };
		if (SDL_memcmp(checkGUID.data, zeroGUID.data, sizeof(checkGUID.data)) == 0) continue;

		APP_Joy* const joy = &APP_PlayerSlots[player].joy;

		for (int axis = 0; axis < joy->numAxes; axis++)
		{
			const int value = SDL_GetJoystickAxis(joy->joystick, axis);

			if (value < APP_DEADZONE_MIN)
			{
				if (++joy->axesRepeat[axis*2 + 0] == 1) APP_LastInputType = APP_INPUT_JOYSTICK;
			}
			else
			{
				joy->axesRepeat[axis*2 + 0] = 0;
			}

			if (value > APP_DEADZONE_MAX)
			{
				if (++joy->axesRepeat[axis*2 + 1] == 1) APP_LastInputType = APP_INPUT_JOYSTICK;
			}
			else
			{
				joy->axesRepeat[axis*2 + 1] = 0;
			}
		}

		for (int hat = 0; hat < joy->numHats; hat++)
		{
			Uint8 value = SDL_GetJoystickHat(joy->joystick, hat);
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
					if (++joy->hatsRepeat[hat*4 + valueIndex] == 1) APP_LastInputType = APP_INPUT_JOYSTICK;
				}
				else
				{
					joy->hatsRepeat[hat*4 + valueIndex] = 0;
				}
			}
		}

		for (int button = 0; button < joy->numButtons; button++)
		{
			if (SDL_GetJoystickButton(joy->joystick, button))
			{
				if (++joy->buttonsRepeat[button] == 1) APP_LastInputType = APP_INPUT_JOYSTICK;
			}
			else
			{
				joy->buttonsRepeat[button] = 0;
			}
		}
	}
}

bool APP_IsPushJoyKey ( const APP_JoyKey* const key )
{
	if (!APP_PlayerSlots || APP_NumPlayerSlots <= 0 || key == NULL || key->player >= APP_NumPlayerSlots) return false;

	int player = 0;
	int playerMax = 0;
	if (key->player >= 0)
	{
		APP_JoyGUID checkGUID = APP_GetJoyGUID(key->player);
		APP_JoyGUID zeroGUID = { 0 };
		if (SDL_memcmp(checkGUID.data, zeroGUID.data, sizeof(checkGUID.data)) != 0 && SDL_memcmp(key->guid.data, checkGUID.data, sizeof(checkGUID.data)) == 0)
		{
			player = key->player;
			playerMax = key->player + 1;
		}
	}
	else
	{
		player = 0;
		playerMax = APP_NumPlayerSlots;
	}
	for (; player < playerMax; player++)
	{
		APP_Joy* const joy = &APP_PlayerSlots[player].joy;

		switch (key->type)
		{
		case APP_JOYKEY_ANY:
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
		case APP_JOYKEY_AXIS:
			if (key->setting.index < joy->numAxes)
			{
				if (key->setting.value == APP_DEADZONE_MIN)
				{
					return joy->axesRepeat[key->setting.index * 2 + 0] == 1;
				}
				else if (key->setting.value == APP_DEADZONE_MAX)
				{
					return joy->axesRepeat[key->setting.index * 2 + 1] == 1;
				}
			}
			break;
		case APP_JOYKEY_HAT:
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
		case APP_JOYKEY_BUTTON:
			if (key->setting.button < joy->numButtons) return joy->buttonsRepeat[key->setting.button] == 1;
			break;
		default:
			break;
		}
	}
	return false;
}

bool APP_IsPressJoyKey ( const APP_JoyKey* const key )
{
	if (!APP_PlayerSlots || APP_NumPlayerSlots <= 0 || key == NULL || key->player >= APP_NumPlayerSlots) return false;

	int player = 0;
	int playerMax = 0;
	if (key->player >= 0)
	{
		APP_JoyGUID checkGUID = APP_GetJoyGUID(key->player);
		APP_JoyGUID zeroGUID = { 0 };
		if (SDL_memcmp(checkGUID.data, zeroGUID.data, sizeof(checkGUID.data)) != 0 && SDL_memcmp(key->guid.data, checkGUID.data, sizeof(checkGUID.data)) == 0)
		{
			player = key->player;
			playerMax = key->player + 1;
		}
	}
	else
	{
		player = 0;
		playerMax = APP_NumPlayerSlots;
	}
	for (; player < playerMax; player++)
	{
		APP_Joy* const joy = &APP_PlayerSlots[player].joy;

		switch (key->type)
		{
		case APP_JOYKEY_ANY:
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
		case APP_JOYKEY_AXIS:
			if (key->setting.index < joy->numAxes)
			{
				if (key->setting.value == APP_DEADZONE_MIN)
				{
					return joy->axesRepeat[key->setting.index * 2 + 0] != 0;
				}
				else if (key->setting.value == APP_DEADZONE_MAX)
				{
					return joy->axesRepeat[key->setting.index * 2 + 1] != 0;
				}
			}
			break;
		case APP_JOYKEY_HAT:
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
		case APP_JOYKEY_BUTTON:
			if (key->setting.button < joy->numButtons) return joy->buttonsRepeat[key->setting.button] != 0;
			break;
		default:
			break;
		}
	}
	return false;
}

int APP_GetJoyKeyRepeat ( const APP_JoyKey* const key )
{
	if (!APP_PlayerSlots || APP_NumPlayerSlots <= 0 || key == NULL || key->player >= APP_NumPlayerSlots) return 0;

	bool multi;
	int player = 0;
	int playerMax = 0;
	int maxRepeat = 0;
	if (key->player >= 0)
	{
		APP_JoyGUID checkGUID = APP_GetJoyGUID(key->player);
		APP_JoyGUID zeroGUID = { 0 };
		if (SDL_memcmp(checkGUID.data, zeroGUID.data, sizeof(checkGUID.data)) != 0 && SDL_memcmp(key->guid.data, checkGUID.data, sizeof(checkGUID.data)) == 0)
		{
			player = key->player;
			playerMax = key->player + 1;
			multi = false;
		}
	}
	else
	{
		player = 0;
		playerMax = APP_NumPlayerSlots;
		multi = true;
	}
	for (; player < playerMax; player++)
	{
		APP_Joy* const joy = &APP_PlayerSlots[player].joy;

		switch (key->type)
		{
		case APP_JOYKEY_ANY:
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
		case APP_JOYKEY_AXIS:
			if (key->setting.index < joy->numAxes)
			{
				if (key->setting.value == APP_DEADZONE_MIN)
				{
					if (multi)
					{
						if (joy->axesRepeat[key->setting.index * 2 + 0] > maxRepeat) maxRepeat = joy->axesRepeat[key->setting.index * 2 + 0];
					}
					else return joy->axesRepeat[key->setting.index * 2 + 0];
				}
				else if (key->setting.value == APP_DEADZONE_MAX)
				{
					if (multi)
					{
						if (joy->axesRepeat[key->setting.index * 2 + 1] > maxRepeat) maxRepeat = joy->axesRepeat[key->setting.index * 2 + 1];
					}
					else return joy->axesRepeat[key->setting.index * 2 + 1];
				}
			}
			break;
		case APP_JOYKEY_HAT:
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
		case APP_JOYKEY_BUTTON:
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

int APP_GetNumJoys(void)
{
	int foundJoys = 0;
	for (int player = 0; player < APP_NumPlayerSlots; player++) {
		if (APP_PlayerSlots[player].type == APP_PLAYERSLOT_JOY) {
			foundJoys++;
		}
	}
	return foundJoys;
}

APP_JoyGUID APP_GetJoyGUID(int player)
{
	if (
		player < 0 ||
		APP_NumPlayerSlots <= 0 ||
		player >= APP_NumPlayerSlots ||
		APP_PlayerSlots[player].type != APP_PLAYERSLOT_JOY
	) {
		return (APP_JoyGUID) { 0 };
	}

	SDL_GUID sdlGUID;
	sdlGUID = SDL_GetJoystickGUID(APP_PlayerSlots[player].joy.joystick);

	APP_JoyGUID joyGUID = { 0 };
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

int APP_GetMaxJoyAxis(int player)
{
	if (player >= 0 && APP_NumPlayerSlots > 0 && player < APP_NumPlayerSlots)
	{
		if (APP_PlayerSlots[player].type == APP_PLAYERSLOT_JOY)
		{
			return APP_PlayerSlots[player].joy.numAxes;
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

int APP_GetMaxJoyHat(int player)
{
	if (player >= 0 && APP_NumPlayerSlots > 0 && player < APP_NumPlayerSlots)
	{
		if (APP_PlayerSlots[player].type == APP_PLAYERSLOT_JOY)
		{
			return APP_PlayerSlots[player].joy.numHats;
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

int APP_GetMaxJoyButton(int player)
{
	if (player >= 0 && APP_NumPlayerSlots > 0 && player < APP_NumPlayerSlots)
	{
		if (APP_PlayerSlots[player].type == APP_PLAYERSLOT_JOY)
		{
			return APP_PlayerSlots[player].joy.numButtons;
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

#ifdef APP_ENABLE_GAME_CONTROLLER

static SDL_GamepadType APP_GetSDLGameControllerType(SDL_Gamepad * const controller)
{
	#ifdef APP_ONLY_SDL_CONTROLLER_TYPE
	return APP_ONLY_SDL_CONTROLLER_TYPE;
	#else
	return SDL_GetGamepadType(controller);
	#endif
}

static void APP_ConInputsUpdate(void)
{
	bool backupLastInputBuiltinCon;
	if (APP_BuiltinCon.controller && SDL_GamepadConnected(APP_BuiltinCon.controller)) {
		backupLastInputBuiltinCon = APP_LastInputBuiltinCon;
		APP_LastInputBuiltinCon = true;
		APP_InputType const inputType = APP_GetConType(-1);
		APP_LastInputBuiltinCon = backupLastInputBuiltinCon;
		Sint16 axisValue;

		axisValue = SDL_GetGamepadAxis(APP_BuiltinCon.controller, SDL_GAMEPAD_AXIS_LEFTX);
		if (axisValue > 0 && axisValue > APP_DEADZONE_MAX)
		{
			if (++APP_BuiltinCon.axesRepeat[0] == 1)
			{
				APP_LastInputType = inputType;
				APP_LastActiveCon = 0;
				APP_LastInputBuiltinCon = true;
			}
			APP_BuiltinCon.axesRepeat[1] = 0;
		}
		else if (axisValue < 0 && axisValue < APP_DEADZONE_MIN)
		{
			APP_BuiltinCon.axesRepeat[0] = 0;
			if (++APP_BuiltinCon.axesRepeat[1] == 1)
			{
				APP_LastInputType = inputType;
				APP_LastActiveCon = 0;
				APP_LastInputBuiltinCon = true;
			}
		}
		else
		{
			APP_BuiltinCon.axesRepeat[0] = 0;
			APP_BuiltinCon.axesRepeat[1] = 0;
		}

		axisValue = SDL_GetGamepadAxis(APP_BuiltinCon.controller, SDL_GAMEPAD_AXIS_LEFTY);
		if (axisValue > 0 && axisValue > APP_DEADZONE_MAX)
		{
			if (++APP_BuiltinCon.axesRepeat[2] == 1)
			{
				APP_LastInputType = inputType;
				APP_LastActiveCon = 0;
				APP_LastInputBuiltinCon = true;
			}
			APP_BuiltinCon.axesRepeat[3] = 0;
		}
		else if (axisValue < 0 && axisValue < APP_DEADZONE_MIN)
		{
			APP_BuiltinCon.axesRepeat[2] = 0;
			if (++APP_BuiltinCon.axesRepeat[3] == 1)
			{
				APP_LastInputType = inputType;
				APP_LastActiveCon = 0;
				APP_LastInputBuiltinCon = true;
			}
		}
		else
		{
			APP_BuiltinCon.axesRepeat[2] = 0;
			APP_BuiltinCon.axesRepeat[3] = 0;
		}

		axisValue = SDL_GetGamepadAxis(APP_BuiltinCon.controller, SDL_GAMEPAD_AXIS_RIGHTX);
		if (axisValue > 0 && axisValue > APP_DEADZONE_MAX)
		{
			if (++APP_BuiltinCon.axesRepeat[4] == 1)
			{
				APP_LastInputType = inputType;
				APP_LastActiveCon = 0;
				APP_LastInputBuiltinCon = true;
			}
			APP_BuiltinCon.axesRepeat[5] = 0;
		}
		else if (axisValue < 0 && axisValue < APP_DEADZONE_MIN)
		{
			APP_BuiltinCon.axesRepeat[4] = 0;
			if (++APP_BuiltinCon.axesRepeat[5] == 1)
			{
				APP_LastInputType = inputType;
				APP_LastActiveCon = 0;
				APP_LastInputBuiltinCon = true;
			}
		}
		else
		{
			APP_BuiltinCon.axesRepeat[4] = 0;
			APP_BuiltinCon.axesRepeat[5] = 0;
		}

		axisValue = SDL_GetGamepadAxis(APP_BuiltinCon.controller, SDL_GAMEPAD_AXIS_RIGHTY);
		if (axisValue > 0 && axisValue > APP_DEADZONE_MAX)
		{
			if (++APP_BuiltinCon.axesRepeat[6] == 1)
			{
				APP_LastInputType = inputType;
				APP_LastActiveCon = 0;
				APP_LastInputBuiltinCon = true;
			}
			APP_BuiltinCon.axesRepeat[7] = 0;
		}
		else if (axisValue < 0 && axisValue < APP_DEADZONE_MIN)
		{
			APP_BuiltinCon.axesRepeat[6] = 0;
			if (++APP_BuiltinCon.axesRepeat[7] == 1)
			{
				APP_LastInputType = inputType;
				APP_LastActiveCon = 0;
				APP_LastInputBuiltinCon = true;
			}
		}
		else
		{
			APP_BuiltinCon.axesRepeat[6] = 0;
			APP_BuiltinCon.axesRepeat[7] = 0;
		}

		axisValue = SDL_GetGamepadAxis(APP_BuiltinCon.controller, SDL_GAMEPAD_AXIS_LEFT_TRIGGER);
		if (axisValue > APP_DEADZONE_MAX)
		{
			if (++APP_BuiltinCon.axesRepeat[8] == 1)
			{
				APP_LastInputType = inputType;
				APP_LastActiveCon = 0;
				APP_LastInputBuiltinCon = true;
			}
		}
		else
		{
			APP_BuiltinCon.axesRepeat[8] = 0;
		}

		axisValue = SDL_GetGamepadAxis(APP_BuiltinCon.controller, SDL_GAMEPAD_AXIS_RIGHT_TRIGGER);
		if (axisValue > APP_DEADZONE_MAX)
		{
			if (++APP_BuiltinCon.axesRepeat[9] == 1)
			{
				APP_LastInputType = inputType;
				APP_LastActiveCon = 0;
				APP_LastInputBuiltinCon = true;
			}
		}
		else
		{
			APP_BuiltinCon.axesRepeat[9] = 0;
		}

		for (SDL_GamepadButton button = 0; button < SDL_GAMEPAD_BUTTON_COUNT; button++)
		{
			if (SDL_GetGamepadButton(APP_BuiltinCon.controller, button))
			{
				if (++APP_BuiltinCon.buttonsRepeat[button] == 1)
				{
					APP_LastInputType = inputType;
					APP_LastActiveCon = 0;
					APP_LastInputBuiltinCon = true;
				}
			}
			else
			{
				APP_BuiltinCon.buttonsRepeat[button] = 0;
			}
		}
	}

	if (!APP_PlayerSlots) return;

	for (int player = 0; player < APP_NumPlayerSlots; player++)
	{
		APP_PlayerSlot* const slot = &APP_PlayerSlots[player];
		if (slot->type != APP_PLAYERSLOT_CON) {
			continue;
		}
		APP_Con* const con = &slot->con;

		if (!SDL_GamepadConnected(con->controller)) continue;

		backupLastInputBuiltinCon = APP_LastInputBuiltinCon;
		APP_LastInputBuiltinCon = false;
		APP_InputType const inputType = APP_GetConType(player);
		APP_LastInputBuiltinCon = backupLastInputBuiltinCon;
		Sint16 axisValue;

		axisValue = SDL_GetGamepadAxis(con->controller, SDL_GAMEPAD_AXIS_LEFTX);
		if (axisValue > 0 && axisValue > APP_DEADZONE_MAX)
		{
			if (++con->axesRepeat[0] == 1)
			{
				APP_LastInputType = inputType;
				APP_LastActiveCon = player;
				if (player == 0) APP_LastInputBuiltinCon = false;
			}
			con->axesRepeat[1] = 0;
		}
		else if (axisValue < 0 && axisValue < APP_DEADZONE_MIN)
		{
			con->axesRepeat[0] = 0;
			if (++con->axesRepeat[1] == 1)
			{
				APP_LastInputType = inputType;
				APP_LastActiveCon = player;
				if (player == 0) APP_LastInputBuiltinCon = false;
			}
		}
		else
		{
			con->axesRepeat[0] = 0;
			con->axesRepeat[1] = 0;
		}

		axisValue = SDL_GetGamepadAxis(con->controller, SDL_GAMEPAD_AXIS_LEFTY);
		if (axisValue > 0 && axisValue > APP_DEADZONE_MAX)
		{
			if (++con->axesRepeat[2] == 1)
			{
				APP_LastInputType = inputType;
				APP_LastActiveCon = player;
				if (player == 0) APP_LastInputBuiltinCon = false;
			}
			con->axesRepeat[3] = 0;
		}
		else if (axisValue < 0 && axisValue < APP_DEADZONE_MIN)
		{
			con->axesRepeat[2] = 0;
			if (++con->axesRepeat[3] == 1)
			{
				APP_LastInputType = inputType;
				APP_LastActiveCon = player;
				if (player == 0) APP_LastInputBuiltinCon = false;
			}
		}
		else
		{
			con->axesRepeat[2] = 0;
			con->axesRepeat[3] = 0;
		}

		axisValue = SDL_GetGamepadAxis(con->controller, SDL_GAMEPAD_AXIS_RIGHTX);
		if (axisValue > 0 && axisValue > APP_DEADZONE_MAX)
		{
			if (++con->axesRepeat[4] == 1)
			{
				APP_LastInputType = inputType;
				APP_LastActiveCon = player;
				if (player == 0) APP_LastInputBuiltinCon = false;
			}
			con->axesRepeat[5] = 0;
		}
		else if (axisValue < 0 && axisValue < APP_DEADZONE_MIN)
		{
			con->axesRepeat[4] = 0;
			if (++con->axesRepeat[5] == 1)
			{
				APP_LastInputType = inputType;
				APP_LastActiveCon = player;
				if (player == 0) APP_LastInputBuiltinCon = false;
			}
		}
		else
		{
			con->axesRepeat[4] = 0;
			con->axesRepeat[5] = 0;
		}

		axisValue = SDL_GetGamepadAxis(con->controller, SDL_GAMEPAD_AXIS_RIGHTY);
		if (axisValue > 0 && axisValue > APP_DEADZONE_MAX)
		{
			if (++con->axesRepeat[6] == 1)
			{
				APP_LastInputType = inputType;
				APP_LastActiveCon = player;
				if (player == 0) APP_LastInputBuiltinCon = false;
			}
			con->axesRepeat[7] = 0;
		}
		else if (axisValue < 0 && axisValue < APP_DEADZONE_MIN)
		{
			con->axesRepeat[6] = 0;
			if (++con->axesRepeat[7] == 1)
			{
				APP_LastInputType = inputType;
				APP_LastActiveCon = player;
				if (player == 0) APP_LastInputBuiltinCon = false;
			}
		}
		else
		{
			con->axesRepeat[6] = 0;
			con->axesRepeat[7] = 0;
		}

		axisValue = SDL_GetGamepadAxis(con->controller, SDL_GAMEPAD_AXIS_LEFT_TRIGGER);
		if (axisValue > APP_DEADZONE_MAX)
		{
			if (++con->axesRepeat[8] == 1)
			{
				APP_LastInputType = inputType;
				APP_LastActiveCon = player;
				if (player == 0) APP_LastInputBuiltinCon = false;
			}
		}
		else
		{
			con->axesRepeat[8] = 0;
		}

		axisValue = SDL_GetGamepadAxis(con->controller, SDL_GAMEPAD_AXIS_RIGHT_TRIGGER);
		if (axisValue > APP_DEADZONE_MAX)
		{
			if (++con->axesRepeat[9] == 1)
			{
				APP_LastInputType = inputType;
				APP_LastActiveCon = player;
				if (player == 0) APP_LastInputBuiltinCon = false;
			}
		}
		else
		{
			con->axesRepeat[9] = 0;
		}

		for (SDL_GamepadButton button = 0; button < SDL_GAMEPAD_BUTTON_COUNT; button++)
		{
			if (SDL_GetGamepadButton(con->controller, button))
			{
				if (++con->buttonsRepeat[button] == 1)
				{
					APP_LastInputType = inputType;
					APP_LastActiveCon = player;
					if (player == 0) APP_LastInputBuiltinCon = false;
				}
			}
			else
			{
				con->buttonsRepeat[button] = 0;
			}
		}
	}
}

static bool APP_IsPushConKeyPrivate ( const APP_Con* const con, const APP_ConKey* const key )
{
	switch (key->type)
	{
	case APP_CONKEY_ANY:
		for (int axis = 0; axis < APP_CONAXIS_MAX; axis++)
		{
			if (con->axesRepeat[axis] == 1) return true;
		}
		for (int button = 0; button < APP_CONBUTTON_MAX; button++)
		{
			if (con->buttonsRepeat[button] == 1) return true;
		}
		break;
	case APP_CONKEY_AXIS:
		if (key->index < APP_CONAXIS_MAX)
		{
			if (con->axesRepeat[key->index] == 1) return true;
		}
		break;
	case APP_CONKEY_BUTTON:
		if (key->index < APP_CONBUTTON_MAX)
		{
			if (con->buttonsRepeat[key->index] == 1) return true;
		}
		break;
	default:
		break;
	}
	return false;
}

bool APP_IsPushConKey ( const int player, const APP_ConKey* const key )
{
	if (!key) return false;
	if (player <= 0 && APP_BuiltinCon.controller && APP_IsPushConKeyPrivate(&APP_BuiltinCon, key)) return true;
	if (!APP_PlayerSlots || APP_NumPlayerSlots == 0 || player >= APP_NumPlayerSlots) return false;

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
		playerMax = APP_NumPlayerSlots;
	}
	for (; playerStart < playerMax; playerStart++)
	{
		if (APP_PlayerSlots[playerStart].type != APP_PLAYERSLOT_CON) continue;
		APP_Con* const con = &APP_PlayerSlots[playerStart].con;
		if (APP_IsPushConKeyPrivate(con, key)) return true;
	}
	return false;
}

static bool APP_IsPressConKeyPrivate ( const APP_Con* const con, const APP_ConKey* const key)
{
	switch (key->type)
	{
	case APP_CONKEY_ANY:
		for (int axis = 0; axis < APP_CONAXIS_MAX; axis++)
		{
			if (con->axesRepeat[axis] != 0) return true;
		}
		for (int button = 0; button < APP_CONBUTTON_MAX; button++)
		{
			if (con->buttonsRepeat[button] != 0) return true;
		}
		break;
	case APP_CONKEY_AXIS:
		if (key->index < APP_CONAXIS_MAX)
		{
			if (con->axesRepeat[key->index] != 0) return true;
		}
		break;
	case APP_CONKEY_BUTTON:
		if (key->index < APP_CONBUTTON_MAX)
		{
			if (con->buttonsRepeat[key->index] != 0) return true;
		}
		break;
	default:
		break;
	}
	return false;
}

bool APP_IsPressConKey ( const int player, const APP_ConKey* const key )
{
	if (!key) return false;
	if (player <= 0 && APP_BuiltinCon.controller && APP_IsPressConKeyPrivate(&APP_BuiltinCon, key)) return true;
	if (!APP_PlayerSlots || APP_NumPlayerSlots == 0 || player >= APP_NumPlayerSlots) return false;

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
		playerMax = APP_NumPlayerSlots;
	}
	for (; playerStart < playerMax; playerStart++)
	{
		if (APP_PlayerSlots[playerStart].type != APP_PLAYERSLOT_CON) continue;
		APP_Con* const con = &APP_PlayerSlots[playerStart].con;
		if (APP_IsPressConKeyPrivate(con, key)) return true;
	}
	return false;
}

static void APP_GetConKeyRepeatPrivate ( const APP_Con* const con, const APP_ConKey* const key, int* const maxRepeat)
{
	switch (key->type)
	{
	case APP_CONKEY_ANY:
		for (int axis = 0; axis < APP_CONAXIS_MAX; axis++)
		{
			if (con->axesRepeat[axis] > *maxRepeat) *maxRepeat = con->axesRepeat[axis];
		}
		for (int button = 0; button < APP_CONBUTTON_MAX; button++)
		{
			if (con->buttonsRepeat[button] > *maxRepeat) *maxRepeat = con->buttonsRepeat[button];
		}
		break;
	case APP_CONKEY_AXIS:
		if (key->index < APP_CONAXIS_MAX)
		{
			*maxRepeat = con->axesRepeat[key->index];
		}
		break;
	case APP_CONKEY_BUTTON:
		if (key->index < APP_CONBUTTON_MAX)
		{
			*maxRepeat = con->buttonsRepeat[key->index];
		}
		break;
	default:
		break;
	}
}

static void APP_GetConKeyRepeatMultiPrivate ( const APP_Con* const con, const APP_ConKey* const key, int* const maxRepeat)
{
	switch (key->type)
	{
	case APP_CONKEY_ANY:
		for (int axis = 0; axis < APP_CONAXIS_MAX; axis++)
		{
			if (con->axesRepeat[axis] > *maxRepeat) *maxRepeat = con->axesRepeat[axis];
		}
		for (int button = 0; button < APP_CONBUTTON_MAX; button++)
		{
			if (con->buttonsRepeat[button] > *maxRepeat) *maxRepeat = con->buttonsRepeat[button];
		}
		break;
	case APP_CONKEY_AXIS:
		if (key->index < APP_CONAXIS_MAX)
		{
			if (con->axesRepeat[key->index] > *maxRepeat) *maxRepeat = con->axesRepeat[key->index];
		}
		break;
	case APP_CONKEY_BUTTON:
		if (key->index < APP_CONBUTTON_MAX)
		{
			if (con->buttonsRepeat[key->index] > *maxRepeat) *maxRepeat = con->buttonsRepeat[key->index];
		}
		break;
	default:
		break;
	}
}

int APP_GetConKeyRepeat ( const int player, const APP_ConKey* const key )
{
	if (!key) return false;

	int maxRepeat = 0;
	if (player <= 0 && APP_BuiltinCon.controller)
	{
		APP_GetConKeyRepeatPrivate(&APP_BuiltinCon, key, &maxRepeat);
	}
	if (!APP_PlayerSlots || APP_NumPlayerSlots == 0 || player >= APP_NumPlayerSlots) return maxRepeat;

	bool multi;
	int playerStart = 0;
	int playerMax = 0;
	if (player >= 0)
	{
		playerStart = player;
		playerMax = player + 1;
		multi = false;
	}
	else
	{
		playerStart = 0;
		playerMax = APP_NumPlayerSlots;
		multi = true;
	}
	for (; playerStart < playerMax; playerStart++)
	{
		if (APP_PlayerSlots[playerStart].type != APP_PLAYERSLOT_CON) continue;
		APP_Con* const con = &APP_PlayerSlots[playerStart].con;
		if (multi) APP_GetConKeyRepeatMultiPrivate(con, key, &maxRepeat);
		else APP_GetConKeyRepeatPrivate(con, key, &maxRepeat);
	}
	return maxRepeat;
}

int APP_GetNumCons(void)
{
	int foundCons = 0;
	for (int player = 0; player < APP_NumPlayerSlots; player++) {
		if (APP_PlayerSlots[player].type == APP_PLAYERSLOT_CON) {
			foundCons++;
		}
	}
	if (foundCons == 0 && APP_BuiltinCon.controller) {
		foundCons = 1;
	}
	return foundCons;
}

void APP_ResetLastActiveCon(void)
{
	APP_LastActiveCon = -1;
}

int APP_GetLastActiveCon(void)
{
	return APP_LastActiveCon;
}

APP_InputType APP_GetConType(const int player)
{
	SDL_Gamepad * controller = NULL;
	if (player <= 0 && APP_BuiltinCon.controller && APP_LastInputBuiltinCon) {
		controller = APP_BuiltinCon.controller;
	}
	else if (player >= 0 && player < APP_NumPlayerSlots && APP_PlayerSlots && APP_NumPlayerSlots > 0 && APP_PlayerSlots[player].type == APP_PLAYERSLOT_CON) {
		controller = APP_PlayerSlots[player].con.controller;
	}
	else {
		return APP_INPUT_NULL;
	}

	#ifdef APP_ONLY_INPUT_TYPE
	if (APP_ONLY_INPUT_TYPE >= APP_INPUT_FIRSTGAMECONTROLLERTYPE && APP_ONLY_INPUT_TYPE <= APP_INPUT_LASTGAMECONTROLLERTYPE) {
		return APP_ONLY_INPUT_TYPE;
	}
	else {
		return APP_INPUT_NULL;
	}
	#else
	switch(APP_GetSDLGameControllerType(controller))
	{
	default:
	case SDL_GAMEPAD_TYPE_STANDARD :
	case SDL_GAMEPAD_TYPE_XBOX360 :
	case SDL_GAMEPAD_TYPE_XBOXONE :
		return APP_INPUT_XBOX;

	case SDL_GAMEPAD_TYPE_PS3 :
	case SDL_GAMEPAD_TYPE_PS4 :
	case SDL_GAMEPAD_TYPE_PS5 :
		return APP_INPUT_PLAYSTATION;

	case SDL_GAMEPAD_TYPE_NINTENDO_SWITCH_PRO :
	case SDL_GAMEPAD_TYPE_NINTENDO_SWITCH_JOYCON_LEFT :
	case SDL_GAMEPAD_TYPE_NINTENDO_SWITCH_JOYCON_RIGHT :
	case SDL_GAMEPAD_TYPE_NINTENDO_SWITCH_JOYCON_PAIR :
		return APP_INPUT_NINTENDO;
	}
	#endif
}

bool APP_GetConKeyDesc(const int player, const APP_ConKey* const key, const char** text, APP_Button* button)
{
	SDL_Gamepad * controller;
	if (player == 0 && APP_BuiltinCon.controller && APP_LastInputBuiltinCon)
	{
		controller = APP_BuiltinCon.controller;
	}
	else if (
		APP_PlayerSlots &&
		APP_NumPlayerSlots > 0 &&
		player >= 0 &&
		player < APP_NumPlayerSlots &&
		APP_PlayerSlots[player].type == APP_PLAYERSLOT_CON &&
		key &&
		text &&
		button
	)
	{
		controller = APP_PlayerSlots[player].con.controller;
	}
	else
	{
		return false;
	}

	*text = NULL;
	*button = APP_BUTTON_NULL;
	const SDL_GamepadType type = APP_GetSDLGameControllerType(controller);
	switch (key->type)
	{
	case APP_CONKEY_AXIS:
		switch (key->index)
		{
		case 0:
			*text = "LS";
			*button = APP_BUTTON_RIGHT;
			break;
		case 1:
			*text = "LS";
			*button = APP_BUTTON_LEFT;
			break;
		case 2:
			*text = "LS";
			*button = APP_BUTTON_DOWN;
			break;
		case 3:
			*text = "LS";
			*button = APP_BUTTON_UP;
			break;
		case 4:
			*text = "RS";
			*button = APP_BUTTON_RIGHT;
			break;
		case 5:
			*text = "RS";
			*button = APP_BUTTON_LEFT;
			break;
		case 6:
			*text = "RS";
			*button = APP_BUTTON_DOWN;
			break;
		case 7:
			*text = "RS";
			*button = APP_BUTTON_UP;
			break;
		case 8:
			switch(type)
			{
			default:
			case SDL_GAMEPAD_TYPE_STANDARD :
			case SDL_GAMEPAD_TYPE_XBOX360 :
			case SDL_GAMEPAD_TYPE_XBOXONE :
				*text = "LT";
				break;

			case SDL_GAMEPAD_TYPE_PS3 :
			case SDL_GAMEPAD_TYPE_PS4 :
			case SDL_GAMEPAD_TYPE_PS5 :
				*text = "L2";
				break;

			case SDL_GAMEPAD_TYPE_NINTENDO_SWITCH_PRO :
			case SDL_GAMEPAD_TYPE_NINTENDO_SWITCH_JOYCON_PAIR :
				*text = "ZL";
				break;

			case SDL_GAMEPAD_TYPE_NINTENDO_SWITCH_JOYCON_LEFT :
			case SDL_GAMEPAD_TYPE_NINTENDO_SWITCH_JOYCON_RIGHT :
				*text = "SL";
				break;
			}
			break;
		case 9:
			switch(type)
			{
			default:
			case SDL_GAMEPAD_TYPE_STANDARD :
			case SDL_GAMEPAD_TYPE_XBOX360 :
			case SDL_GAMEPAD_TYPE_XBOXONE :
				*text = "RT";
				break;

			case SDL_GAMEPAD_TYPE_PS3 :
			case SDL_GAMEPAD_TYPE_PS4 :
			case SDL_GAMEPAD_TYPE_PS5 :
				*text = "R2";
				break;

			case SDL_GAMEPAD_TYPE_NINTENDO_SWITCH_PRO :
			case SDL_GAMEPAD_TYPE_NINTENDO_SWITCH_JOYCON_PAIR :
				*text = "ZR";
				break;

			case SDL_GAMEPAD_TYPE_NINTENDO_SWITCH_JOYCON_LEFT :
			case SDL_GAMEPAD_TYPE_NINTENDO_SWITCH_JOYCON_RIGHT :
				*text = "SR";
				break;
			}
			break;
		default:
			break;
		}
		break;
	case APP_CONKEY_BUTTON:
		switch (key->index)
		{
		case SDL_GAMEPAD_BUTTON_BACK :
		case SDL_GAMEPAD_BUTTON_START :
			// "Start" and "Back" are reserved for "Pause" and "Give Up", and may not be remapped.
			break;

		case SDL_GAMEPAD_BUTTON_GUIDE :
			*text = "HOME";
			break;

		case SDL_GAMEPAD_BUTTON_SOUTH :
		case SDL_GAMEPAD_BUTTON_EAST :
		case SDL_GAMEPAD_BUTTON_WEST :
		case SDL_GAMEPAD_BUTTON_NORTH :
			*button = APP_BUTTON_A + key->index;
			break;

		case SDL_GAMEPAD_BUTTON_DPAD_UP :
		case SDL_GAMEPAD_BUTTON_DPAD_DOWN :
		case SDL_GAMEPAD_BUTTON_DPAD_LEFT :
		case SDL_GAMEPAD_BUTTON_DPAD_RIGHT :
			*text = "DPAD";
			*button = APP_BUTTON_UP + key->index - SDL_GAMEPAD_BUTTON_DPAD_UP;
			break;

		case SDL_GAMEPAD_BUTTON_MISC1 :
			switch(type)
			{
			case SDL_GAMEPAD_TYPE_XBOXONE :
				*text = "SHARE";
				break;

			case SDL_GAMEPAD_TYPE_NINTENDO_SWITCH_PRO :
			case SDL_GAMEPAD_TYPE_NINTENDO_SWITCH_JOYCON_LEFT :
			case SDL_GAMEPAD_TYPE_NINTENDO_SWITCH_JOYCON_PAIR :
				*text = "CAPTURE";
				break;

			case SDL_GAMEPAD_TYPE_PS5 :
				*text = "MICROPHONE";
				break;

			default:
				*text = "MISC1";
				break;
			}
			break;
		case SDL_GAMEPAD_BUTTON_RIGHT_PADDLE1 :
			*text = "P1";
			break;
		case SDL_GAMEPAD_BUTTON_LEFT_PADDLE1 :
			*text = "P2";
			break;
		case SDL_GAMEPAD_BUTTON_RIGHT_PADDLE2 :
			*text = "P3";
			break;
		case SDL_GAMEPAD_BUTTON_LEFT_PADDLE2 :
			*text = "P4";
			break;
		case SDL_GAMEPAD_BUTTON_TOUCHPAD :
			*text = "TOUCHPAD";
			break;
		case SDL_GAMEPAD_BUTTON_LEFT_STICK :
			switch(type)
			{
			default:
				*text = "LS";
				break;

			case SDL_GAMEPAD_TYPE_PS3 :
			case SDL_GAMEPAD_TYPE_PS4 :
			case SDL_GAMEPAD_TYPE_PS5 :
				*text = "L3";
				break;
			}
			break;
		case SDL_GAMEPAD_BUTTON_RIGHT_STICK :
			switch(type)
			{
			default:
				*text = "RS";
				break;

			case SDL_GAMEPAD_TYPE_PS3 :
			case SDL_GAMEPAD_TYPE_PS4 :
				*text = "R3";
				break;
			}
			break;
		case SDL_GAMEPAD_BUTTON_LEFT_SHOULDER :
			switch(type)
			{
			default:
				*text = "LB";
				break;

			case SDL_GAMEPAD_TYPE_PS3 :
			case SDL_GAMEPAD_TYPE_PS4 :
			case SDL_GAMEPAD_TYPE_PS5 :
				*text = "L1";
				break;

			case SDL_GAMEPAD_TYPE_NINTENDO_SWITCH_PRO :
			case SDL_GAMEPAD_TYPE_NINTENDO_SWITCH_JOYCON_PAIR :
				*text = "L";
				break;
			}
			break;
		case SDL_GAMEPAD_BUTTON_RIGHT_SHOULDER :
			switch(type)
			{
			default:
				*text = "RB";
				break;

			case SDL_GAMEPAD_TYPE_PS3 :
			case SDL_GAMEPAD_TYPE_PS4 :
			case SDL_GAMEPAD_TYPE_PS5 :
				*text = "R1";
				break;

			case SDL_GAMEPAD_TYPE_NINTENDO_SWITCH_PRO :
			case SDL_GAMEPAD_TYPE_NINTENDO_SWITCH_JOYCON_PAIR :
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
	return *text != NULL || *button != APP_BUTTON_NULL;
}
#endif

void APP_OpenInputs(void)
{
	#ifdef ENABLE_LINUXGPIO
	const char* chipName = "gpiochip0";

	APP_GPIOChip = gpiod_chip_open_by_name(chipName);
	if ( !APP_GPIOChip )
	{
		SDL_Log("Failed opening GPIO chip \"%s\". Continuing without GPIO input support", chipName);
	}
	else
	{
		SDL_memset(APP_GPIORepeat, 0, sizeof(APP_GPIORepeat));
		SDL_memset(APP_GPIOLines, 0, sizeof(APP_GPIOLines));
		if (
			!(APP_GPIOLines[0] = gpiod_chip_get_line(APP_GPIOChip,  5)) ||
			!(APP_GPIOLines[1] = gpiod_chip_get_line(APP_GPIOChip, 13)) ||
			!(APP_GPIOLines[2] = gpiod_chip_get_line(APP_GPIOChip,  6)) ||
			!(APP_GPIOLines[3] = gpiod_chip_get_line(APP_GPIOChip, 12)) ||
			!(APP_GPIOLines[4] = gpiod_chip_get_line(APP_GPIOChip, 19)) ||
			!(APP_GPIOLines[5] = gpiod_chip_get_line(APP_GPIOChip, 16)) ||
			!(APP_GPIOLines[6] = gpiod_chip_get_line(APP_GPIOChip, 26)) ||
			!(APP_GPIOLines[7] = gpiod_chip_get_line(APP_GPIOChip, 20)) ||
			!(APP_GPIOLines[8] = gpiod_chip_get_line(APP_GPIOChip, 21)) ||
			!(APP_GPIOLines[9] = gpiod_chip_get_line(APP_GPIOChip,  4))
		)
		{
			for (int line = 0; line < 10; line++)
			{
				if (APP_GPIOLines[line])
				{
					gpiod_line_release(APP_GPIOLines[line]);
					APP_GPIOLines[line] = NULL;
				}
			}
			gpiod_chip_close(APP_GPIOChip);
			APP_GPIOChip = NULL;
			SDL_Log("Failed opening GPIO lines. Continuing without GPIO input support");
		}
		else
		{
			for (int line = 0; line < 10; line++)
			{
				if (gpiod_line_request_input(APP_GPIOLines[line], "input") < 0)
				{
					for (line = 0; line < 10; line++)
					{
						gpiod_line_release(APP_GPIOLines[line]);
						APP_GPIOLines[line] = NULL;
					}
					gpiod_chip_close(APP_GPIOChip);
					APP_GPIOChip = NULL;
					SDL_Log("Failed setting GPIO lines for input. Continuing without GPIO input support");
					break;
				}
			}
		}
	}
	#endif

	#ifdef APP_ENABLE_KEYBOARD
	/* キーリピートバッファ初期化 */
	SDL_memset(APP_KeyRepeat, 0, sizeof(APP_KeyRepeat));
	#endif

	/* パッドの初期化 */
	#ifdef APP_ENABLE_GAME_CONTROLLER_DB
	// The game will just go without the database if it's missing or fails to load.
	SDL_IOStream* db = APP_OpenRead("gamecontrollerdb.txt");
	if (db) {
		if (SDL_GetIOSize(db) > 0) {
			SDL_AddGamepadMappingsFromIO(db, false);
		}
		SDL_CloseIO(db);
	}
	#endif
}

void APP_CloseInputs(void)
{
	#ifdef APP_ENABLE_LINUX_GPIO
	if ( APP_GPIOChip )
	{
		for ( int i = 0 ; i < 10 ; i ++ )
		{
			gpiod_line_release(APP_GPIOLines[i]);
		}
		gpiod_chip_close(APP_GPIOChip);

		APP_GPIOChip = NULL;
		SDL_memset(APP_GPIOLines, 0, sizeof(APP_GPIOLines));
	}
	#endif

	#ifdef APP_ENABLE_KEYBOARD
	SDL_memset(APP_KeyRepeat, 0, sizeof(APP_KeyRepeat));
	#endif

	#if defined(APP_ENABLE_JOYSTICK) || defined(APP_ENABLE_GAME_CONTROLLER)
	#ifdef APP_ENABLE_GAME_CONTROLLER
	if (APP_BuiltinCon.controller && SDL_GamepadConnected(APP_BuiltinCon.controller)) {
		SDL_CloseGamepad(APP_BuiltinCon.controller);
		APP_BuiltinCon.controller = NULL;
	}
	#endif
	APP_PlayerSlotsClose();
	#endif
}

void APP_InputsUpdate(void)
{
	#ifdef APP_ENABLE_LINUX_GPIO
	for (int line = 0; line < 10; line++)
	{
		if (gpiod_line_get_value(APP_GPIOLines[line]) == 1)
		{
			if (++APP_GPIORepeat[line] == 1) LastControllerType = APP_INPUT_LINUXGPIO;
		}
		else
		{
			APP_GPIORepeat[line] = 0;
		}
	}
	#endif

	#ifdef APP_ENABLE_KEYBOARD
	int numKeys = 0;
	const bool* keyStates = SDL_GetKeyboardState(&numKeys);

	for (int i = 0; i < APP_KEY_MAX; i++)
	{
		if (i < numKeys && keyStates[i])
		{
			if (++APP_KeyRepeat[i] == 1) APP_LastInputType = APP_INPUT_KEYBOARD;
		}
		else
		{
			APP_KeyRepeat[i] = 0;
		}
	}
	#endif

	#ifdef APP_ENABLE_JOYSTICK
	APP_JoyInputsUpdate();
	#endif

	#ifdef APP_ENABLE_GAME_CONTROLLER
	APP_ConInputsUpdate();
	#endif
}
