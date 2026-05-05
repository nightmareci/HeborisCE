// TODO: Explicitly handle GameCube controllers. So, handle SDL_GAMEPAD_TYPE_GAMECUBE for input type. Rename INPUT_DEVICE_TYPE_NINTENDO => INPUT_DEVICE_TYPE_SNES, add INPUT_DEVICE_TYPE_GAMECUBE

#include <INPUT.h>
#include <ERROR.h>
#ifdef INPUT_ENABLE_GAME_CONTROLLER_DB
#include <FILESYSTEM.h>
#endif
#include <MAIN.h>

#ifdef INPUT_ONLY_DEVICE_TYPE
static INPUT_DeviceType INPUT_LastDeviceType = INPUT_ONLY_DEVICE_TYPE;
#else
static INPUT_DeviceType INPUT_LastDeviceType = INPUT_DEVICE_TYPE_NULL;
#endif

#ifdef INPUT_ENABLE_LINUX_GPIO
static int INPUT_GPIORepeat[10];
static struct gpiod_chip* INPUT_GPIOChip;
static struct gpiod_line* INPUT_GPIOLines[10];
#endif

#ifdef INPUT_ENABLE_KEYBOARD
static int INPUT_KeyRepeat[INPUT_KEY_MAX];
#endif

#ifdef INPUT_ENABLE_JOYSTICK
typedef struct INPUT_Joy
{
	SDL_Joystick* joystick;
	int numAxes;
	int numHats;
	int numButtons;
	int* axesRepeat;
	int* hatsRepeat;
	int* buttonsRepeat;
} INPUT_Joy;
#endif

#ifdef INPUT_ENABLE_GAME_CONTROLLER
typedef struct INPUT_Con
{
	SDL_Gamepad* controller;
	int axesRepeat[INPUT_CON_AXIS_MAX];
	int buttonsRepeat[INPUT_CON_BUTTON_MAX];
} INPUT_Con;
static int INPUT_LastActiveCon = -1;
#endif

#if defined(INPUT_ENABLE_JOYSTICK) || defined(INPUT_ENABLE_GAME_CONTROLLER)
typedef struct INPUT_PlayerSlot
{
	INPUT_PlayerSlotType type;
	union {
		#ifdef INPUT_ENABLE_JOYSTICK
		INPUT_Joy joy;
		#endif
		#ifdef INPUT_ENABLE_GAME_CONTROLLER
		INPUT_Con con;
		#endif
	};
} INPUT_PlayerSlot;

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
#define INPUT_CON_ID(vendor, product) (Uint32)(((Uint32)(Uint16)(vendor) << 16) | (Uint16)(product))
static Uint32 INPUT_BuiltinConIDs[] =
{
	INPUT_CON_ID(0x28de, 0x1205) // Steam Deck
};
static INPUT_Con INPUT_BuiltinCon = { 0 };
static bool INPUT_LastInputWasBuiltinCon = true;

static INPUT_PlayerSlot* INPUT_PlayerSlots = NULL;
static int INPUT_NumPlayerSlots = 0;
#endif

INPUT_DeviceType INPUT_GetLastDeviceType(void)
{
	return INPUT_LastDeviceType;
}

#ifdef INPUT_ENABLE_LINUX_GPIO
bool INPUT_IsPushGPIO(int key)
{
	return key >= 0 && key < NUMBTNS && INPUT_GPIORepeat[key] == 1;
}

bool INPUT_IsPressGPIO(int key)
{
	return key >= 0 && key < NUMBTNS && INPUT_GPIORepeat[key] != 0;
}

int INPUT_GetGPIORepeat(int key)
{
	return key >= 0 && key < NUMBTNS ? INPUT_GPIORepeat[key] : 0;
}
#endif

#ifdef INPUT_ENABLE_KEYBOARD
bool INPUT_IsPushKey(int key)
{
	return key >= 0 && key < INPUT_KEY_MAX && INPUT_KeyRepeat[key] == 1;
}

bool INPUT_IsPressKey(int key)
{
	return key >= 0 && key < INPUT_KEY_MAX && INPUT_KeyRepeat[key] != 0;
}

int INPUT_GetKeyRepeat(int key)
{
	return key >= 0 && key < INPUT_KEY_MAX ? INPUT_KeyRepeat[key] : 0;
}

int INPUT_GetMaxKey(void)
{
	return INPUT_KEY_MAX;
}
#endif

#if defined(INPUT_ENABLE_JOYSTICK) || defined(INPUT_ENABLE_GAME_CONTROLLER)

static bool INPUT_IsCon(SDL_JoystickID joy)
{
	if (!joy) {
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

static bool INPUT_ResizePlayerSlots(int numSlots) {
	if (numSlots == INPUT_NumPlayerSlots) {
		return true;
	}
	else if (numSlots == 0) {
		SDL_free(INPUT_PlayerSlots);
		INPUT_PlayerSlots = NULL;
		INPUT_NumPlayerSlots = 0;
		return true;
	}

	INPUT_PlayerSlot* const oldSlots = INPUT_PlayerSlots;
	if (INPUT_PlayerSlots) {
		INPUT_PlayerSlots = SDL_realloc(INPUT_PlayerSlots, sizeof(INPUT_PlayerSlot) * numSlots);
	}
	else {
		INPUT_PlayerSlots = SDL_malloc(sizeof(INPUT_PlayerSlot) * numSlots);
	}
	if (!INPUT_PlayerSlots) {
		INPUT_PlayerSlots = oldSlots;
		return ERROR_Set("Failed to resize joysticks/controllers array");
	}
	for (int player = INPUT_NumPlayerSlots; player < numSlots; player++) {
		INPUT_PlayerSlots[player] = (INPUT_PlayerSlot) { 0 };
	}
	INPUT_NumPlayerSlots = numSlots;
	return true;
}

static bool INPUT_ConIsBuiltin(SDL_Gamepad* controller) {
	for (size_t i = 0; i < SDL_arraysize(INPUT_BuiltinConIDs); i++) {
		const Uint16 vendor = SDL_GetGamepadVendor(controller);
		const Uint16 product = SDL_GetGamepadProduct(controller);
		const Uint32 conID = INPUT_CON_ID(vendor, product);
		if (conID == INPUT_BuiltinConIDs[i]) {
			return true;
		}
	}
	return false;
}

bool INPUT_UpdatePlayerSlots(void) {
	#ifdef INPUT_ENABLE_GAME_CONTROLLER
	// This should never be needed, as the builtin controller most likely will
	// always be connected, but you never know, maybe a modder detached it.
	if (INPUT_BuiltinCon.controller && !SDL_GamepadConnected(INPUT_BuiltinCon.controller)) {
		INPUT_BuiltinCon = (INPUT_Con) { 0 };
	}
	#endif
	for (int player = 0; player < INPUT_NumPlayerSlots; player++) {
		INPUT_PlayerSlot* const slot = &INPUT_PlayerSlots[player];
		switch (slot->type) {
		#ifdef INPUT_ENABLE_JOYSTICK
		case INPUT_PLAYER_SLOT_JOY:
			if (!SDL_JoystickConnected(slot->joy.joystick)) {
				if (slot->joy.axesRepeat) SDL_free(slot->joy.axesRepeat);
				if (slot->joy.hatsRepeat) SDL_free(slot->joy.hatsRepeat);
				if (slot->joy.buttonsRepeat) SDL_free(slot->joy.buttonsRepeat);
				*slot = (INPUT_PlayerSlot) { 0 };
			}
			break;
		#endif

		#ifdef INPUT_ENABLE_GAME_CONTROLLER
		case INPUT_PLAYER_SLOT_CON:
			if (!SDL_GamepadConnected(slot->con.controller)) {
				*slot = (INPUT_PlayerSlot) { 0 };
			}
			break;
		#endif

		default:
		case INPUT_PLAYER_SLOT_NULL:
			break;
		}
	}

	int numJoys;
	SDL_JoystickID* const joys = SDL_GetJoysticks(&numJoys);
	if (!joys) {
		return ERROR_Set("Failed to get joysticks list: %s", SDL_GetError());
	}

	#ifdef INPUT_ENABLE_GAME_CONTROLLER
	for (int device = 0; device < numJoys; device++) {
		if (!INPUT_IsCon(joys[device])) {
			continue;
		}

		SDL_Gamepad* const controller = SDL_OpenGamepad(joys[device]);
		if (!controller) {
			SDL_free(joys);
			return ERROR_Set("Failed to open controller: %s", SDL_GetError());
		}
		if (!INPUT_BuiltinCon.controller && INPUT_ConIsBuiltin(controller)) {
			INPUT_BuiltinCon.controller = controller;
			continue;
		}
		bool playerIndexFound = false;
		int player;
		for (player = 0; player < INPUT_NumPlayerSlots; player++) {
			if (INPUT_PlayerSlots[player].type == INPUT_PLAYER_SLOT_NULL) {
				playerIndexFound = true;
				break;
			}
		}
		if (!playerIndexFound) {
			if (!INPUT_ResizePlayerSlots(INPUT_NumPlayerSlots + 1)) {
				SDL_CloseGamepad(controller);
				SDL_free(joys);
				return ERROR_Set("Failed to open controller: %s", SDL_GetError());
			}
			player = INPUT_NumPlayerSlots - 1;
		}
		INPUT_PlayerSlots[player].con.controller = controller;
		INPUT_PlayerSlots[player].type = INPUT_PLAYER_SLOT_CON;
	}
	#endif

	#ifdef INPUT_ENABLE_JOYSTICK
	for (int device = 0; device < numJoys; device++) {
		if (INPUT_IsCon(joys[device])) {
			continue;
		}

		SDL_Joystick* const joystick = SDL_OpenJoystick(joys[device]);
		if (!joystick) {
			ERROR_Set("Failed to open joystick: %s", SDL_GetError());
			goto fail;
		}
		bool playerIndexFound = false;
		int player;
		for (player = 0; player < INPUT_NumPlayerSlots; player++) {
			if (INPUT_PlayerSlots[player].type == INPUT_PLAYER_SLOT_NULL) {
				playerIndexFound = true;
				break;
			}
		}
		if (!playerIndexFound) {
			if (!INPUT_ResizePlayerSlots(INPUT_NumPlayerSlots + 1)) {
				goto fail;
			}
			player = INPUT_NumPlayerSlots - 1;
		}

		if (
			(INPUT_PlayerSlots[player].joy.numAxes = SDL_GetNumJoystickAxes(joystick)) >= 0 &&
			(INPUT_PlayerSlots[player].joy.numHats = SDL_GetNumJoystickHats(joystick)) >= 0 &&
			(INPUT_PlayerSlots[player].joy.numButtons = SDL_GetNumJoystickButtons(joystick)) >= 0
		) {
			if (INPUT_PlayerSlots[player].joy.numAxes > 0 && !(INPUT_PlayerSlots[player].joy.axesRepeat = (int*)SDL_calloc((size_t)INPUT_PlayerSlots[player].joy.numAxes * 2, sizeof(int)))) {
				ERROR_Set("Failed to allocate axes repeat array for joystick");
				goto fail;
			}
			if (INPUT_PlayerSlots[player].joy.numHats > 0 && !(INPUT_PlayerSlots[player].joy.hatsRepeat = (int*)SDL_calloc((size_t)INPUT_PlayerSlots[player].joy.numHats * 4, sizeof(int)))) {
				ERROR_Set("Failed to allocate hats repeat array for joystick");
				goto fail;
			}
			if (INPUT_PlayerSlots[player].joy.numButtons > 0 && !(INPUT_PlayerSlots[player].joy.buttonsRepeat = (int*)SDL_calloc((size_t)INPUT_PlayerSlots[player].joy.numButtons, sizeof(int)))) {
				ERROR_Set("Failed to allocate buttons repeat array for joystick");
				goto fail;
			}
		}
		if (false) {
		fail:
			if (INPUT_PlayerSlots[player].joy.axesRepeat) SDL_free(INPUT_PlayerSlots[player].joy.axesRepeat);
			if (INPUT_PlayerSlots[player].joy.hatsRepeat) SDL_free(INPUT_PlayerSlots[player].joy.hatsRepeat);
			if (INPUT_PlayerSlots[player].joy.buttonsRepeat) SDL_free(INPUT_PlayerSlots[player].joy.buttonsRepeat);
			if (joystick) SDL_CloseJoystick(joystick);
			SDL_free(joys);
			return false;
		}
		INPUT_PlayerSlots[player].joy.joystick = joystick;
		INPUT_PlayerSlots[player].type = INPUT_PLAYER_SLOT_JOY;
	}
	#endif

	SDL_free(joys);

	int numRequiredSlots = 0;
	for (int player = 0; player < INPUT_NumPlayerSlots; player++) {
		if (INPUT_PlayerSlots[player].type != INPUT_PLAYER_SLOT_NULL) {
			numRequiredSlots = player + 1;
		}
	}
	return INPUT_ResizePlayerSlots(numRequiredSlots);
}

static void INPUT_PlayerSlotsClose(void)
{
	if (INPUT_PlayerSlots) {
		for (int player = 0; player < INPUT_NumPlayerSlots; player++) {
			switch (INPUT_PlayerSlots[player].type) {
			#ifdef INPUT_ENABLE_JOYSTICK
			case INPUT_PLAYER_SLOT_JOY: {
				INPUT_Joy* const joy = &INPUT_PlayerSlots[player].joy;
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

			#ifdef INPUT_ENABLE_GAME_CONTROLLER
			case INPUT_PLAYER_SLOT_CON: {
				if (SDL_GamepadConnected(INPUT_PlayerSlots[player].con.controller))
				{
					SDL_CloseGamepad(INPUT_PlayerSlots[player].con.controller);
				}
				break;
			}
			#endif

			default:
			case INPUT_PLAYER_SLOT_NULL:
				break;
			}
		}
		SDL_free(INPUT_PlayerSlots);
	}
	INPUT_PlayerSlots = NULL;
	INPUT_NumPlayerSlots = 0;
}

int INPUT_GetNumPlayerSlots(void)
{
	if (INPUT_NumPlayerSlots <= 0 && INPUT_BuiltinCon.controller) {
		return 1;
	}
	else {
		return INPUT_NumPlayerSlots;
	}
}

INPUT_PlayerSlotType INPUT_GetPlayerSlotType(int player)
{
	if (player == 0 && INPUT_BuiltinCon.controller && INPUT_LastInputWasBuiltinCon) {
		return INPUT_PLAYER_SLOT_CON;
	}
	else if (player < 0 || player >= INPUT_NumPlayerSlots) {
		return INPUT_PLAYER_SLOT_NULL;
	}
	else {
		return INPUT_PlayerSlots[player].type;
	}
}

#endif

#ifdef INPUT_ENABLE_JOYSTICK

static void INPUT_JoyInputsUpdate(void)
{
	if (!INPUT_PlayerSlots) return;

	for (int player = 0; player < INPUT_NumPlayerSlots; player++) {
		if (
			INPUT_PlayerSlots[player].type != INPUT_PLAYER_SLOT_JOY ||
			!SDL_JoystickConnected(INPUT_PlayerSlots[player].joy.joystick)
		) {
			continue;
		}

		const INPUT_JoyGUID checkGUID = INPUT_GetJoyGUID(player);
		const INPUT_JoyGUID zeroGUID = { 0 };
		if (SDL_memcmp(checkGUID.data, zeroGUID.data, sizeof(checkGUID.data)) == 0) continue;

		INPUT_Joy* const joy = &INPUT_PlayerSlots[player].joy;

		for (int axis = 0; axis < joy->numAxes; axis++) {
			const int value = SDL_GetJoystickAxis(joy->joystick, axis);

			if (value < INPUT_DEADZONE_MIN) {
				if (++joy->axesRepeat[axis*2 + 0] == 1) INPUT_LastDeviceType = INPUT_DEVICE_TYPE_JOYSTICK;
			}
			else {
				joy->axesRepeat[axis*2 + 0] = 0;
			}

			if (value > INPUT_DEADZONE_MAX) {
				if (++joy->axesRepeat[axis*2 + 1] == 1) INPUT_LastDeviceType = INPUT_DEVICE_TYPE_JOYSTICK;
			}
			else
			{
				joy->axesRepeat[axis*2 + 1] = 0;
			}
		}

		for (int hat = 0; hat < joy->numHats; hat++) {
			Uint8 value = SDL_GetJoystickHat(joy->joystick, hat);
			const Uint8 hatValues[4] = {
				SDL_HAT_LEFT,
				SDL_HAT_RIGHT,
				SDL_HAT_UP,
				SDL_HAT_DOWN
			};
			for (int valueIndex = 0; valueIndex < 4; valueIndex++) {
				if (value & hatValues[valueIndex]) {
					if (++joy->hatsRepeat[hat*4 + valueIndex] == 1) INPUT_LastDeviceType = INPUT_DEVICE_TYPE_JOYSTICK;
				}
				else {
					joy->hatsRepeat[hat*4 + valueIndex] = 0;
				}
			}
		}

		for (int button = 0; button < joy->numButtons; button++) {
			if (SDL_GetJoystickButton(joy->joystick, button)) {
				if (++joy->buttonsRepeat[button] == 1) INPUT_LastDeviceType = INPUT_DEVICE_TYPE_JOYSTICK;
			}
			else {
				joy->buttonsRepeat[button] = 0;
			}
		}
	}
}

bool INPUT_IsPushJoyKey(const INPUT_JoyKey* key)
{
	if (!INPUT_PlayerSlots || INPUT_NumPlayerSlots <= 0 || key == NULL || key->player >= INPUT_NumPlayerSlots) return false;

	int player = 0;
	int playerMax = 0;
	if (key->player >= 0) {
		INPUT_JoyGUID checkGUID = INPUT_GetJoyGUID(key->player);
		INPUT_JoyGUID zeroGUID = { 0 };
		if (SDL_memcmp(checkGUID.data, zeroGUID.data, sizeof(checkGUID.data)) != 0 && SDL_memcmp(key->guid.data, checkGUID.data, sizeof(checkGUID.data)) == 0) {
			player = key->player;
			playerMax = key->player + 1;
		}
	}
	else {
		player = 0;
		playerMax = INPUT_NumPlayerSlots;
	}
	for (; player < playerMax; player++) {
		INPUT_Joy* const joy = &INPUT_PlayerSlots[player].joy;

		switch (key->type) {
		case INPUT_JOY_KEY_ANY:
			for (int axis = 0; axis < joy->numAxes; axis++) {
				if (joy->axesRepeat[axis*2 + 0] == 1) return true;
				if (joy->axesRepeat[axis*2 + 1] == 1) return true;
			}
			for (int hat = 0; hat < joy->numHats; hat++) {
				if (joy->hatsRepeat[hat] == 1) return true;
			}
			for (int button = 0; button < joy->numButtons; button++) {
				if (joy->buttonsRepeat[button] == 1) return true;
			}
			break;
		case INPUT_JOY_KEY_AXIS:
			if (key->setting.index < joy->numAxes) {
				if (key->setting.value == INPUT_DEADZONE_MIN) {
					return joy->axesRepeat[key->setting.index * 2 + 0] == 1;
				}
				else if (key->setting.value == INPUT_DEADZONE_MAX) {
					return joy->axesRepeat[key->setting.index * 2 + 1] == 1;
				}
			}
			break;
		case INPUT_JOY_KEY_HAT:
			if (key->setting.index < joy->numHats) {
				switch (key->setting.value) {
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
		case INPUT_JOY_KEY_BUTTON:
			if (key->setting.button < joy->numButtons) return joy->buttonsRepeat[key->setting.button] == 1;
			break;
		default:
			break;
		}
	}
	return false;
}

bool INPUT_IsPressJoyKey(const INPUT_JoyKey* key)
{
	if (!INPUT_PlayerSlots || INPUT_NumPlayerSlots <= 0 || key == NULL || key->player >= INPUT_NumPlayerSlots) return false;

	int player = 0;
	int playerMax = 0;
	if (key->player >= 0) {
		INPUT_JoyGUID checkGUID = INPUT_GetJoyGUID(key->player);
		INPUT_JoyGUID zeroGUID = { 0 };
		if (SDL_memcmp(checkGUID.data, zeroGUID.data, sizeof(checkGUID.data)) != 0 && SDL_memcmp(key->guid.data, checkGUID.data, sizeof(checkGUID.data)) == 0) {
			player = key->player;
			playerMax = key->player + 1;
		}
	}
	else {
		player = 0;
		playerMax = INPUT_NumPlayerSlots;
	}
	for (; player < playerMax; player++) {
		INPUT_Joy* const joy = &INPUT_PlayerSlots[player].joy;

		switch (key->type) {
		case INPUT_JOY_KEY_ANY:
			for (int axis = 0; axis < joy->numAxes; axis++) {
				if (joy->axesRepeat[axis*2 + 0] != 0) return true;
				if (joy->axesRepeat[axis*2 + 1] != 0) return true;
			}
			for (int hat = 0; hat < joy->numHats; hat++) {
				if (joy->hatsRepeat[hat] != 0) return true;
			}
			for (int button = 0; button < joy->numButtons; button++) {
				if (joy->buttonsRepeat[button] != 0) return true;
			}
			break;
		case INPUT_JOY_KEY_AXIS:
			if (key->setting.index < joy->numAxes) {
				if (key->setting.value == INPUT_DEADZONE_MIN) {
					return joy->axesRepeat[key->setting.index * 2 + 0] != 0;
				}
				else if (key->setting.value == INPUT_DEADZONE_MAX) {
					return joy->axesRepeat[key->setting.index * 2 + 1] != 0;
				}
			}
			break;
		case INPUT_JOY_KEY_HAT:
			if (key->setting.index < joy->numHats) {
				switch (key->setting.value) {
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
		case INPUT_JOY_KEY_BUTTON:
			if (key->setting.button < joy->numButtons) return joy->buttonsRepeat[key->setting.button] != 0;
			break;
		default:
			break;
		}
	}
	return false;
}

int INPUT_GetJoyKeyRepeat(const INPUT_JoyKey* key) {
	if (!INPUT_PlayerSlots || INPUT_NumPlayerSlots <= 0 || key == NULL || key->player >= INPUT_NumPlayerSlots) return 0;

	bool multi;
	int player = 0;
	int playerMax = 0;
	int maxRepeat = 0;
	if (key->player >= 0) {
		INPUT_JoyGUID checkGUID = INPUT_GetJoyGUID(key->player);
		INPUT_JoyGUID zeroGUID = { 0 };
		if (SDL_memcmp(checkGUID.data, zeroGUID.data, sizeof(checkGUID.data)) != 0 && SDL_memcmp(key->guid.data, checkGUID.data, sizeof(checkGUID.data)) == 0) {
			player = key->player;
			playerMax = key->player + 1;
			multi = false;
		}
	}
	else {
		player = 0;
		playerMax = INPUT_NumPlayerSlots;
		multi = true;
	}
	for (; player < playerMax; player++) {
		INPUT_Joy* const joy = &INPUT_PlayerSlots[player].joy;

		switch (key->type) {
		case INPUT_JOY_KEY_ANY:
			for (int axis = 0; axis < joy->numAxes; axis++) {
				if (joy->axesRepeat[axis*2 + 0] > maxRepeat) maxRepeat = joy->axesRepeat[axis*2 + 0];
				if (joy->axesRepeat[axis*2 + 1] > maxRepeat) maxRepeat = joy->axesRepeat[axis*2 + 1];
			}
			for (int hat = 0; hat < joy->numHats; hat++) {
				if (joy->hatsRepeat[hat] > maxRepeat) maxRepeat = joy->hatsRepeat[hat];
			}
			for (int button = 0; button < joy->numButtons; button++) {
				if (joy->buttonsRepeat[button] > maxRepeat) maxRepeat = joy->buttonsRepeat[button];
			}
			break;
		case INPUT_JOY_KEY_AXIS:
			if (key->setting.index < joy->numAxes) {
				if (key->setting.value == INPUT_DEADZONE_MIN) {
					if (multi) {
						if (joy->axesRepeat[key->setting.index * 2 + 0] > maxRepeat) maxRepeat = joy->axesRepeat[key->setting.index * 2 + 0];
					}
					else return joy->axesRepeat[key->setting.index * 2 + 0];
				}
				else if (key->setting.value == INPUT_DEADZONE_MAX) {
					if (multi) {
						if (joy->axesRepeat[key->setting.index * 2 + 1] > maxRepeat) maxRepeat = joy->axesRepeat[key->setting.index * 2 + 1];
					}
					else return joy->axesRepeat[key->setting.index * 2 + 1];
				}
			}
			break;
		case INPUT_JOY_KEY_HAT:
			if (key->setting.index < joy->numHats) {
				switch (key->setting.value) {
				case SDL_HAT_LEFT:
					if (multi) {
						if (joy->hatsRepeat[key->setting.index * 4 + 0] > maxRepeat) maxRepeat = joy->hatsRepeat[key->setting.index * 4 + 0];
					}
					else return joy->hatsRepeat[key->setting.index * 4 + 0];
					break;
				case SDL_HAT_RIGHT:
					if (multi) {
						if (joy->hatsRepeat[key->setting.index * 4 + 1] > maxRepeat) maxRepeat = joy->hatsRepeat[key->setting.index * 4 + 1];
					}
					else return joy->hatsRepeat[key->setting.index * 4 + 1];
					break;
				case SDL_HAT_UP:
					if (multi) {
						if (joy->hatsRepeat[key->setting.index * 4 + 2] > maxRepeat) maxRepeat = joy->hatsRepeat[key->setting.index * 4 + 2];
					}
					else return joy->hatsRepeat[key->setting.index * 4 + 2];
					break;
				case SDL_HAT_DOWN:
					if (multi) {
						if (joy->hatsRepeat[key->setting.index * 4 + 3] > maxRepeat) maxRepeat = joy->hatsRepeat[key->setting.index * 4 + 3];
					}
					else return joy->hatsRepeat[key->setting.index * 4 + 3];
					break;
				default:
					break;
				}
			}
			break;
		case INPUT_JOY_KEY_BUTTON:
			if (key->setting.button < joy->numButtons) {
				if (multi) {
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

int INPUT_GetNumJoys(void)
{
	int foundJoys = 0;
	for (int player = 0; player < INPUT_NumPlayerSlots; player++) {
		if (INPUT_PlayerSlots[player].type == INPUT_PLAYER_SLOT_JOY) {
			foundJoys++;
		}
	}
	return foundJoys;
}

INPUT_JoyGUID INPUT_GetJoyGUID(int player)
{
	if (
		player < 0 ||
		INPUT_NumPlayerSlots <= 0 ||
		player >= INPUT_NumPlayerSlots ||
		INPUT_PlayerSlots[player].type != INPUT_PLAYER_SLOT_JOY
	) {
		return (INPUT_JoyGUID) { 0 };
	}

	SDL_GUID sdlGUID;
	sdlGUID = SDL_GetJoystickGUID(INPUT_PlayerSlots[player].joy.joystick);

	INPUT_JoyGUID joyGUID = { 0 };
	for (int i = 0; i < 4; i++) {
		joyGUID.data[i] = 0;
		for (int32_t j = 0; j < 4; j++)
		{
			joyGUID.data[i] |= (int32_t)((uint32_t)sdlGUID.data[i * 4 + j] << (j * 8));
		}
	}
	return joyGUID;
}

int INPUT_GetMaxJoyAxis(int player)
{
	if (player >= 0 && INPUT_NumPlayerSlots > 0 && player < INPUT_NumPlayerSlots) {
		if (INPUT_PlayerSlots[player].type == INPUT_PLAYER_SLOT_JOY) {
			return INPUT_PlayerSlots[player].joy.numAxes;
		}
		else {
			return -1;
		}
	}
	else {
		return -1;
	}
}

int INPUT_GetMaxJoyHat(int player)
{
	if (player >= 0 && INPUT_NumPlayerSlots > 0 && player < INPUT_NumPlayerSlots) {
		if (INPUT_PlayerSlots[player].type == INPUT_PLAYER_SLOT_JOY) {
			return INPUT_PlayerSlots[player].joy.numHats;
		}
		else {
			return -1;
		}
	}
	else {
		return -1;
	}
}

int INPUT_GetMaxJoyButton(int player)
{
	if (player >= 0 && INPUT_NumPlayerSlots > 0 && player < INPUT_NumPlayerSlots) {
		if (INPUT_PlayerSlots[player].type == INPUT_PLAYER_SLOT_JOY) {
			return INPUT_PlayerSlots[player].joy.numButtons;
		}
		else {
			return -1;
		}
	}
	else {
		return -1;
	}
}
#endif

#ifdef INPUT_ENABLE_GAME_CONTROLLER

static SDL_GamepadType INPUT_GetSDLGamepadType(SDL_Gamepad* const controller)
{
	#ifdef INPUT_ONLY_SDL_CONTROLLER_TYPE
	return INPUT_ONLY_SDL_CONTROLLER_TYPE;
	#else
	return SDL_GetGamepadType(controller);
	#endif
}

static void INPUT_ConInputsUpdate(void)
{
	bool backupLastInputBuiltinCon;
	if (INPUT_BuiltinCon.controller && SDL_GamepadConnected(INPUT_BuiltinCon.controller)) {
		backupLastInputBuiltinCon = INPUT_LastInputWasBuiltinCon;
		INPUT_LastInputWasBuiltinCon = true;
		INPUT_DeviceType const inputType = INPUT_GetConType(-1);
		INPUT_LastInputWasBuiltinCon = backupLastInputBuiltinCon;
		Sint16 axisValue;

		axisValue = SDL_GetGamepadAxis(INPUT_BuiltinCon.controller, SDL_GAMEPAD_AXIS_LEFTX);
		if (axisValue > 0 && axisValue > INPUT_DEADZONE_MAX) {
			if (++INPUT_BuiltinCon.axesRepeat[0] == 1) {
				INPUT_LastDeviceType = inputType;
				INPUT_LastActiveCon = 0;
				INPUT_LastInputWasBuiltinCon = true;
			}
			INPUT_BuiltinCon.axesRepeat[1] = 0;
		}
		else if (axisValue < 0 && axisValue < INPUT_DEADZONE_MIN) {
			INPUT_BuiltinCon.axesRepeat[0] = 0;
			if (++INPUT_BuiltinCon.axesRepeat[1] == 1) {
				INPUT_LastDeviceType = inputType;
				INPUT_LastActiveCon = 0;
				INPUT_LastInputWasBuiltinCon = true;
			}
		}
		else {
			INPUT_BuiltinCon.axesRepeat[0] = 0;
			INPUT_BuiltinCon.axesRepeat[1] = 0;
		}

		axisValue = SDL_GetGamepadAxis(INPUT_BuiltinCon.controller, SDL_GAMEPAD_AXIS_LEFTY);
		if (axisValue > 0 && axisValue > INPUT_DEADZONE_MAX) {
			if (++INPUT_BuiltinCon.axesRepeat[2] == 1) {
				INPUT_LastDeviceType = inputType;
				INPUT_LastActiveCon = 0;
				INPUT_LastInputWasBuiltinCon = true;
			}
			INPUT_BuiltinCon.axesRepeat[3] = 0;
		}
		else if (axisValue < 0 && axisValue < INPUT_DEADZONE_MIN) {
			INPUT_BuiltinCon.axesRepeat[2] = 0;
			if (++INPUT_BuiltinCon.axesRepeat[3] == 1) {
				INPUT_LastDeviceType = inputType;
				INPUT_LastActiveCon = 0;
				INPUT_LastInputWasBuiltinCon = true;
			}
		}
		else {
			INPUT_BuiltinCon.axesRepeat[2] = 0;
			INPUT_BuiltinCon.axesRepeat[3] = 0;
		}

		axisValue = SDL_GetGamepadAxis(INPUT_BuiltinCon.controller, SDL_GAMEPAD_AXIS_RIGHTX);
		if (axisValue > 0 && axisValue > INPUT_DEADZONE_MAX) {
			if (++INPUT_BuiltinCon.axesRepeat[4] == 1) {
				INPUT_LastDeviceType = inputType;
				INPUT_LastActiveCon = 0;
				INPUT_LastInputWasBuiltinCon = true;
			}
			INPUT_BuiltinCon.axesRepeat[5] = 0;
		}
		else if (axisValue < 0 && axisValue < INPUT_DEADZONE_MIN) {
			INPUT_BuiltinCon.axesRepeat[4] = 0;
			if (++INPUT_BuiltinCon.axesRepeat[5] == 1) {
				INPUT_LastDeviceType = inputType;
				INPUT_LastActiveCon = 0;
				INPUT_LastInputWasBuiltinCon = true;
			}
		}
		else {
			INPUT_BuiltinCon.axesRepeat[4] = 0;
			INPUT_BuiltinCon.axesRepeat[5] = 0;
		}

		axisValue = SDL_GetGamepadAxis(INPUT_BuiltinCon.controller, SDL_GAMEPAD_AXIS_RIGHTY);
		if (axisValue > 0 && axisValue > INPUT_DEADZONE_MAX) {
			if (++INPUT_BuiltinCon.axesRepeat[6] == 1) {
				INPUT_LastDeviceType = inputType;
				INPUT_LastActiveCon = 0;
				INPUT_LastInputWasBuiltinCon = true;
			}
			INPUT_BuiltinCon.axesRepeat[7] = 0;
		}
		else if (axisValue < 0 && axisValue < INPUT_DEADZONE_MIN) {
			INPUT_BuiltinCon.axesRepeat[6] = 0;
			if (++INPUT_BuiltinCon.axesRepeat[7] == 1) {
				INPUT_LastDeviceType = inputType;
				INPUT_LastActiveCon = 0;
				INPUT_LastInputWasBuiltinCon = true;
			}
		}
		else {
			INPUT_BuiltinCon.axesRepeat[6] = 0;
			INPUT_BuiltinCon.axesRepeat[7] = 0;
		}

		axisValue = SDL_GetGamepadAxis(INPUT_BuiltinCon.controller, SDL_GAMEPAD_AXIS_LEFT_TRIGGER);
		if (axisValue > INPUT_DEADZONE_MAX) {
			if (++INPUT_BuiltinCon.axesRepeat[8] == 1) {
				INPUT_LastDeviceType = inputType;
				INPUT_LastActiveCon = 0;
				INPUT_LastInputWasBuiltinCon = true;
			}
		}
		else {
			INPUT_BuiltinCon.axesRepeat[8] = 0;
		}

		axisValue = SDL_GetGamepadAxis(INPUT_BuiltinCon.controller, SDL_GAMEPAD_AXIS_RIGHT_TRIGGER);
		if (axisValue > INPUT_DEADZONE_MAX) {
			if (++INPUT_BuiltinCon.axesRepeat[9] == 1) {
				INPUT_LastDeviceType = inputType;
				INPUT_LastActiveCon = 0;
				INPUT_LastInputWasBuiltinCon = true;
			}
		}
		else {
			INPUT_BuiltinCon.axesRepeat[9] = 0;
		}

		for (SDL_GamepadButton button = 0; button < SDL_GAMEPAD_BUTTON_COUNT; button++) {
			if (SDL_GetGamepadButton(INPUT_BuiltinCon.controller, button)) {
				if (++INPUT_BuiltinCon.buttonsRepeat[button] == 1) {
					INPUT_LastDeviceType = inputType;
					INPUT_LastActiveCon = 0;
					INPUT_LastInputWasBuiltinCon = true;
				}
			}
			else {
				INPUT_BuiltinCon.buttonsRepeat[button] = 0;
			}
		}
	}

	if (!INPUT_PlayerSlots) return;

	for (int player = 0; player < INPUT_NumPlayerSlots; player++) {
		INPUT_PlayerSlot* const slot = &INPUT_PlayerSlots[player];
		if (slot->type != INPUT_PLAYER_SLOT_CON) {
			continue;
		}
		INPUT_Con* const con = &slot->con;

		if (!SDL_GamepadConnected(con->controller)) continue;

		backupLastInputBuiltinCon = INPUT_LastInputWasBuiltinCon;
		INPUT_LastInputWasBuiltinCon = false;
		INPUT_DeviceType const inputType = INPUT_GetConType(player);
		INPUT_LastInputWasBuiltinCon = backupLastInputBuiltinCon;
		Sint16 axisValue;

		axisValue = SDL_GetGamepadAxis(con->controller, SDL_GAMEPAD_AXIS_LEFTX);
		if (axisValue > 0 && axisValue > INPUT_DEADZONE_MAX) {
			if (++con->axesRepeat[0] == 1) {
				INPUT_LastDeviceType = inputType;
				INPUT_LastActiveCon = player;
				if (player == 0) INPUT_LastInputWasBuiltinCon = false;
			}
			con->axesRepeat[1] = 0;
		}
		else if (axisValue < 0 && axisValue < INPUT_DEADZONE_MIN) {
			con->axesRepeat[0] = 0;
			if (++con->axesRepeat[1] == 1) {
				INPUT_LastDeviceType = inputType;
				INPUT_LastActiveCon = player;
				if (player == 0) INPUT_LastInputWasBuiltinCon = false;
			}
		}
		else {
			con->axesRepeat[0] = 0;
			con->axesRepeat[1] = 0;
		}

		axisValue = SDL_GetGamepadAxis(con->controller, SDL_GAMEPAD_AXIS_LEFTY);
		if (axisValue > 0 && axisValue > INPUT_DEADZONE_MAX) {
			if (++con->axesRepeat[2] == 1) {
				INPUT_LastDeviceType = inputType;
				INPUT_LastActiveCon = player;
				if (player == 0) INPUT_LastInputWasBuiltinCon = false;
			}
			con->axesRepeat[3] = 0;
		}
		else if (axisValue < 0 && axisValue < INPUT_DEADZONE_MIN) {
			con->axesRepeat[2] = 0;
			if (++con->axesRepeat[3] == 1) {
				INPUT_LastDeviceType = inputType;
				INPUT_LastActiveCon = player;
				if (player == 0) INPUT_LastInputWasBuiltinCon = false;
			}
		}
		else {
			con->axesRepeat[2] = 0;
			con->axesRepeat[3] = 0;
		}

		axisValue = SDL_GetGamepadAxis(con->controller, SDL_GAMEPAD_AXIS_RIGHTX);
		if (axisValue > 0 && axisValue > INPUT_DEADZONE_MAX) {
			if (++con->axesRepeat[4] == 1) {
				INPUT_LastDeviceType = inputType;
				INPUT_LastActiveCon = player;
				if (player == 0) INPUT_LastInputWasBuiltinCon = false;
			}
			con->axesRepeat[5] = 0;
		}
		else if (axisValue < 0 && axisValue < INPUT_DEADZONE_MIN) {
			con->axesRepeat[4] = 0;
			if (++con->axesRepeat[5] == 1) {
				INPUT_LastDeviceType = inputType;
				INPUT_LastActiveCon = player;
				if (player == 0) INPUT_LastInputWasBuiltinCon = false;
			}
		}
		else {
			con->axesRepeat[4] = 0;
			con->axesRepeat[5] = 0;
		}

		axisValue = SDL_GetGamepadAxis(con->controller, SDL_GAMEPAD_AXIS_RIGHTY);
		if (axisValue > 0 && axisValue > INPUT_DEADZONE_MAX) {
			if (++con->axesRepeat[6] == 1) {
				INPUT_LastDeviceType = inputType;
				INPUT_LastActiveCon = player;
				if (player == 0) INPUT_LastInputWasBuiltinCon = false;
			}
			con->axesRepeat[7] = 0;
		}
		else if (axisValue < 0 && axisValue < INPUT_DEADZONE_MIN) {
			con->axesRepeat[6] = 0;
			if (++con->axesRepeat[7] == 1) {
				INPUT_LastDeviceType = inputType;
				INPUT_LastActiveCon = player;
				if (player == 0) INPUT_LastInputWasBuiltinCon = false;
			}
		}
		else {
			con->axesRepeat[6] = 0;
			con->axesRepeat[7] = 0;
		}

		axisValue = SDL_GetGamepadAxis(con->controller, SDL_GAMEPAD_AXIS_LEFT_TRIGGER);
		if (axisValue > INPUT_DEADZONE_MAX) {
			if (++con->axesRepeat[8] == 1) {
				INPUT_LastDeviceType = inputType;
				INPUT_LastActiveCon = player;
				if (player == 0) INPUT_LastInputWasBuiltinCon = false;
			}
		}
		else {
			con->axesRepeat[8] = 0;
		}

		axisValue = SDL_GetGamepadAxis(con->controller, SDL_GAMEPAD_AXIS_RIGHT_TRIGGER);
		if (axisValue > INPUT_DEADZONE_MAX) {
			if (++con->axesRepeat[9] == 1) {
				INPUT_LastDeviceType = inputType;
				INPUT_LastActiveCon = player;
				if (player == 0) INPUT_LastInputWasBuiltinCon = false;
			}
		}
		else {
			con->axesRepeat[9] = 0;
		}

		for (SDL_GamepadButton button = 0; button < SDL_GAMEPAD_BUTTON_COUNT; button++) {
			if (SDL_GetGamepadButton(con->controller, button)) {
				if (++con->buttonsRepeat[button] == 1) {
					INPUT_LastDeviceType = inputType;
					INPUT_LastActiveCon = player;
					if (player == 0) INPUT_LastInputWasBuiltinCon = false;
				}
			}
			else {
				con->buttonsRepeat[button] = 0;
			}
		}
	}
}

static bool INPUT_IsPushConKeyPrivate(const INPUT_Con* con, const INPUT_ConKey* key)
{
	switch (key->type) {
	case INPUT_CON_KEY_ANY:
		for (int axis = 0; axis < INPUT_CON_AXIS_MAX; axis++) {
			if (con->axesRepeat[axis] == 1) return true;
		}
		for (int button = 0; button < INPUT_CON_BUTTON_MAX; button++) {
			if (con->buttonsRepeat[button] == 1) return true;
		}
		break;
	case INPUT_CON_KEY_AXIS:
		if (key->index < INPUT_CON_AXIS_MAX) {
			if (con->axesRepeat[key->index] == 1) return true;
		}
		break;
	case INPUT_CON_KEY_BUTTON:
		if (key->index < INPUT_CON_BUTTON_MAX) {
			if (con->buttonsRepeat[key->index] == 1) return true;
		}
		break;
	default:
		break;
	}
	return false;
}

bool INPUT_IsPushConKey(int player, const INPUT_ConKey* key)
{
	if (!key) return false;
	if (player <= 0 && INPUT_BuiltinCon.controller && INPUT_IsPushConKeyPrivate(&INPUT_BuiltinCon, key)) return true;
	if (!INPUT_PlayerSlots || INPUT_NumPlayerSlots == 0 || player >= INPUT_NumPlayerSlots) return false;

	int playerStart = 0;
	int playerMax = 0;
	if (player >= 0) {
		playerStart = player;
		playerMax = player + 1;
	}
	else {
		playerStart = 0;
		playerMax = INPUT_NumPlayerSlots;
	}
	for (; playerStart < playerMax; playerStart++) {
		if (INPUT_PlayerSlots[playerStart].type != INPUT_PLAYER_SLOT_CON) continue;
		INPUT_Con* const con = &INPUT_PlayerSlots[playerStart].con;
		if (INPUT_IsPushConKeyPrivate(con, key)) return true;
	}
	return false;
}

static bool INPUT_IsPressConKeyPrivate(const INPUT_Con* con, const INPUT_ConKey* key)
{
	switch (key->type) {
	case INPUT_CON_KEY_ANY:
		for (int axis = 0; axis < INPUT_CON_AXIS_MAX; axis++) {
			if (con->axesRepeat[axis] != 0) return true;
		}
		for (int button = 0; button < INPUT_CON_BUTTON_MAX; button++) {
			if (con->buttonsRepeat[button] != 0) return true;
		}
		break;
	case INPUT_CON_KEY_AXIS:
		if (key->index < INPUT_CON_AXIS_MAX) {
			if (con->axesRepeat[key->index] != 0) return true;
		}
		break;
	case INPUT_CON_KEY_BUTTON:
		if (key->index < INPUT_CON_BUTTON_MAX) {
			if (con->buttonsRepeat[key->index] != 0) return true;
		}
		break;
	default:
		break;
	}
	return false;
}

bool INPUT_IsPressConKey(int player, const INPUT_ConKey* key)
{
	if (!key) return false;
	if (player <= 0 && INPUT_BuiltinCon.controller && INPUT_IsPressConKeyPrivate(&INPUT_BuiltinCon, key)) return true;
	if (!INPUT_PlayerSlots || INPUT_NumPlayerSlots == 0 || player >= INPUT_NumPlayerSlots) return false;

	int playerStart = 0;
	int playerMax = 0;
	if (player >= 0) {
		playerStart = player;
		playerMax = player + 1;
	}
	else {
		playerStart = 0;
		playerMax = INPUT_NumPlayerSlots;
	}
	for (; playerStart < playerMax; playerStart++) {
		if (INPUT_PlayerSlots[playerStart].type != INPUT_PLAYER_SLOT_CON) continue;
		INPUT_Con* const con = &INPUT_PlayerSlots[playerStart].con;
		if (INPUT_IsPressConKeyPrivate(con, key)) return true;
	}
	return false;
}

static void INPUT_GetConKeyRepeatPrivate(const INPUT_Con* con, const INPUT_ConKey* key, int* maxRepeat)
{
	switch (key->type) {
	case INPUT_CON_KEY_ANY:
		for (int axis = 0; axis < INPUT_CON_AXIS_MAX; axis++) {
			if (con->axesRepeat[axis] > *maxRepeat) *maxRepeat = con->axesRepeat[axis];
		}
		for (int button = 0; button < INPUT_CON_BUTTON_MAX; button++) {
			if (con->buttonsRepeat[button] > *maxRepeat) *maxRepeat = con->buttonsRepeat[button];
		}
		break;
	case INPUT_CON_KEY_AXIS:
		if (key->index < INPUT_CON_AXIS_MAX) {
			*maxRepeat = con->axesRepeat[key->index];
		}
		break;
	case INPUT_CON_KEY_BUTTON:
		if (key->index < INPUT_CON_BUTTON_MAX) {
			*maxRepeat = con->buttonsRepeat[key->index];
		}
		break;
	default:
		break;
	}
}

static void INPUT_GetConKeyRepeatMultiPrivate(const INPUT_Con* con, const INPUT_ConKey* key, int* maxRepeat)
{
	switch (key->type) {
	case INPUT_CON_KEY_ANY:
		for (int axis = 0; axis < INPUT_CON_AXIS_MAX; axis++) {
			if (con->axesRepeat[axis] > *maxRepeat) *maxRepeat = con->axesRepeat[axis];
		}
		for (int button = 0; button < INPUT_CON_BUTTON_MAX; button++) {
			if (con->buttonsRepeat[button] > *maxRepeat) *maxRepeat = con->buttonsRepeat[button];
		}
		break;
	case INPUT_CON_KEY_AXIS:
		if (key->index < INPUT_CON_AXIS_MAX) {
			if (con->axesRepeat[key->index] > *maxRepeat) *maxRepeat = con->axesRepeat[key->index];
		}
		break;
	case INPUT_CON_KEY_BUTTON:
		if (key->index < INPUT_CON_BUTTON_MAX) {
			if (con->buttonsRepeat[key->index] > *maxRepeat) *maxRepeat = con->buttonsRepeat[key->index];
		}
		break;
	default:
		break;
	}
}

int INPUT_GetConKeyRepeat(int player, const INPUT_ConKey* key)
{
	if (!key) return false;

	int maxRepeat = 0;
	if (player <= 0 && INPUT_BuiltinCon.controller) {
		INPUT_GetConKeyRepeatPrivate(&INPUT_BuiltinCon, key, &maxRepeat);
	}
	if (!INPUT_PlayerSlots || INPUT_NumPlayerSlots == 0 || player >= INPUT_NumPlayerSlots) return maxRepeat;

	bool multi;
	int playerStart = 0;
	int playerMax = 0;
	if (player >= 0) {
		playerStart = player;
		playerMax = player + 1;
		multi = false;
	}
	else {
		playerStart = 0;
		playerMax = INPUT_NumPlayerSlots;
		multi = true;
	}
	for (; playerStart < playerMax; playerStart++) {
		if (INPUT_PlayerSlots[playerStart].type != INPUT_PLAYER_SLOT_CON) continue;
		INPUT_Con* const con = &INPUT_PlayerSlots[playerStart].con;
		if (multi) INPUT_GetConKeyRepeatMultiPrivate(con, key, &maxRepeat);
		else INPUT_GetConKeyRepeatPrivate(con, key, &maxRepeat);
	}
	return maxRepeat;
}

int INPUT_GetNumCons(void)
{
	int foundCons = 0;
	for (int player = 0; player < INPUT_NumPlayerSlots; player++) {
		if (INPUT_PlayerSlots[player].type == INPUT_PLAYER_SLOT_CON) {
			foundCons++;
		}
	}
	if (foundCons == 0 && INPUT_BuiltinCon.controller) {
		foundCons = 1;
	}
	return foundCons;
}

void INPUT_ResetLastActiveCon(void)
{
	INPUT_LastActiveCon = -1;
}

int INPUT_GetLastActiveCon(void)
{
	return INPUT_LastActiveCon;
}

INPUT_DeviceType INPUT_GetConType(int player)
{
	SDL_Gamepad * controller = NULL;
	if (player <= 0 && INPUT_BuiltinCon.controller && INPUT_LastInputWasBuiltinCon) {
		controller = INPUT_BuiltinCon.controller;
	}
	else if (player >= 0 && player < INPUT_NumPlayerSlots && INPUT_PlayerSlots && INPUT_NumPlayerSlots > 0 && INPUT_PlayerSlots[player].type == INPUT_PLAYER_SLOT_CON) {
		controller = INPUT_PlayerSlots[player].con.controller;
	}
	else {
		return INPUT_DEVICE_TYPE_NULL;
	}

	#ifdef INPUT_ONLY_DEVICE_TYPE
	if (INPUT_ONLY_DEVICE_TYPE >= INPUT_DEVICE_TYPE_FIRST_GAME_CONTROLLER_TYPE && INPUT_ONLY_DEVICE_TYPE <= INPUT_DEVICE_TYPE_LAST_GAME_CONTROLLER_TYPE) {
		return INPUT_ONLY_DEVICE_TYPE;
	}
	else {
		return INPUT_DEVICE_TYPE_NULL;
	}
	#else
	switch(INPUT_GetSDLGamepadType(controller)) {
	default:
	case SDL_GAMEPAD_TYPE_STANDARD:
	case SDL_GAMEPAD_TYPE_XBOX360:
	case SDL_GAMEPAD_TYPE_XBOXONE:
		return INPUT_DEVICE_TYPE_XBOX;

	case SDL_GAMEPAD_TYPE_PS3:
	case SDL_GAMEPAD_TYPE_PS4:
	case SDL_GAMEPAD_TYPE_PS5:
		return INPUT_DEVICE_TYPE_PLAYSTATION;

	case SDL_GAMEPAD_TYPE_NINTENDO_SWITCH_PRO:
	case SDL_GAMEPAD_TYPE_NINTENDO_SWITCH_JOYCON_LEFT:
	case SDL_GAMEPAD_TYPE_NINTENDO_SWITCH_JOYCON_RIGHT:
	case SDL_GAMEPAD_TYPE_NINTENDO_SWITCH_JOYCON_PAIR:
		return INPUT_DEVICE_TYPE_NINTENDO;

	// TODO: SDL_GAMEPAD_TYPE_GAMECUBE
	}
	#endif
}

bool INPUT_GetConKeyDesc(int player, const INPUT_ConKey* key, const char** text, INPUT_Button* button)
{
	SDL_Gamepad* controller;
	if (player == 0 && INPUT_BuiltinCon.controller && INPUT_LastInputWasBuiltinCon) {
		controller = INPUT_BuiltinCon.controller;
	}
	else if (
		INPUT_PlayerSlots &&
		INPUT_NumPlayerSlots > 0 &&
		player >= 0 &&
		player < INPUT_NumPlayerSlots &&
		INPUT_PlayerSlots[player].type == INPUT_PLAYER_SLOT_CON &&
		key &&
		text &&
		button
	) {
		controller = INPUT_PlayerSlots[player].con.controller;
	}
	else {
		return false;
	}

	*text = NULL;
	*button = INPUT_BUTTON_NULL;
	const SDL_GamepadType type = INPUT_GetSDLGamepadType(controller);
	// TODO: SDL_GAMEPAD_TYPE_GAMECUBE
	switch (key->type) {
	case INPUT_CON_KEY_AXIS:
		switch (key->index) {
		case 0:
			*text = "LS";
			*button = INPUT_BUTTON_RIGHT;
			break;
		case 1:
			*text = "LS";
			*button = INPUT_BUTTON_LEFT;
			break;
		case 2:
			*text = "LS";
			*button = INPUT_BUTTON_DOWN;
			break;
		case 3:
			*text = "LS";
			*button = INPUT_BUTTON_UP;
			break;
		case 4:
			*text = "RS";
			*button = INPUT_BUTTON_RIGHT;
			break;
		case 5:
			*text = "RS";
			*button = INPUT_BUTTON_LEFT;
			break;
		case 6:
			*text = "RS";
			*button = INPUT_BUTTON_DOWN;
			break;
		case 7:
			*text = "RS";
			*button = INPUT_BUTTON_UP;
			break;
		case 8:
			switch(type) {
			default:
			case SDL_GAMEPAD_TYPE_STANDARD:
			case SDL_GAMEPAD_TYPE_XBOX360:
			case SDL_GAMEPAD_TYPE_XBOXONE:
				*text = "LT";
				break;

			case SDL_GAMEPAD_TYPE_PS3:
			case SDL_GAMEPAD_TYPE_PS4:
			case SDL_GAMEPAD_TYPE_PS5:
				*text = "L2";
				break;

			case SDL_GAMEPAD_TYPE_NINTENDO_SWITCH_PRO:
			case SDL_GAMEPAD_TYPE_NINTENDO_SWITCH_JOYCON_PAIR:
				*text = "ZL";
				break;

			case SDL_GAMEPAD_TYPE_NINTENDO_SWITCH_JOYCON_LEFT:
			case SDL_GAMEPAD_TYPE_NINTENDO_SWITCH_JOYCON_RIGHT:
				*text = "SL";
				break;
			}
			break;
		case 9:
			switch(type) {
			default:
			case SDL_GAMEPAD_TYPE_STANDARD:
			case SDL_GAMEPAD_TYPE_XBOX360:
			case SDL_GAMEPAD_TYPE_XBOXONE:
				*text = "RT";
				break;

			case SDL_GAMEPAD_TYPE_PS3:
			case SDL_GAMEPAD_TYPE_PS4:
			case SDL_GAMEPAD_TYPE_PS5:
				*text = "R2";
				break;

			case SDL_GAMEPAD_TYPE_NINTENDO_SWITCH_PRO:
			case SDL_GAMEPAD_TYPE_NINTENDO_SWITCH_JOYCON_PAIR:
				*text = "ZR";
				break;

			case SDL_GAMEPAD_TYPE_NINTENDO_SWITCH_JOYCON_LEFT:
			case SDL_GAMEPAD_TYPE_NINTENDO_SWITCH_JOYCON_RIGHT:
				*text = "SR";
				break;
			}
			break;
		default:
			break;
		}
		break;
	case INPUT_CON_KEY_BUTTON:
		switch (key->index) {
		case SDL_GAMEPAD_BUTTON_BACK:
		case SDL_GAMEPAD_BUTTON_START:
			// "Start" and "Back" are reserved for "Pause" and "Give Up", and may not be remapped.
			break;

		case SDL_GAMEPAD_BUTTON_GUIDE:
			*text = "HOME";
			break;

		case SDL_GAMEPAD_BUTTON_SOUTH:
		case SDL_GAMEPAD_BUTTON_EAST:
		case SDL_GAMEPAD_BUTTON_WEST:
		case SDL_GAMEPAD_BUTTON_NORTH:
			*button = INPUT_BUTTON_A + key->index;
			break;

		case SDL_GAMEPAD_BUTTON_DPAD_UP:
		case SDL_GAMEPAD_BUTTON_DPAD_DOWN:
		case SDL_GAMEPAD_BUTTON_DPAD_LEFT:
		case SDL_GAMEPAD_BUTTON_DPAD_RIGHT:
			*text = "DPAD";
			*button = INPUT_BUTTON_UP + key->index - SDL_GAMEPAD_BUTTON_DPAD_UP;
			break;

		case SDL_GAMEPAD_BUTTON_MISC1:
			switch(type) {
			case SDL_GAMEPAD_TYPE_XBOXONE:
				*text = "SHARE";
				break;

			case SDL_GAMEPAD_TYPE_NINTENDO_SWITCH_PRO:
			case SDL_GAMEPAD_TYPE_NINTENDO_SWITCH_JOYCON_LEFT:
			case SDL_GAMEPAD_TYPE_NINTENDO_SWITCH_JOYCON_PAIR:
				*text = "CAPTURE";
				break;

			case SDL_GAMEPAD_TYPE_PS5:
				*text = "MICROPHONE";
				break;

			default:
				*text = "MISC1";
				break;
			}
			break;
		case SDL_GAMEPAD_BUTTON_RIGHT_PADDLE1:
			*text = "P1";
			break;
		case SDL_GAMEPAD_BUTTON_LEFT_PADDLE1:
			*text = "P2";
			break;
		case SDL_GAMEPAD_BUTTON_RIGHT_PADDLE2:
			*text = "P3";
			break;
		case SDL_GAMEPAD_BUTTON_LEFT_PADDLE2:
			*text = "P4";
			break;
		case SDL_GAMEPAD_BUTTON_TOUCHPAD:
			*text = "TOUCHPAD";
			break;
		case SDL_GAMEPAD_BUTTON_LEFT_STICK:
			switch(type) {
			default:
				*text = "LS";
				break;

			case SDL_GAMEPAD_TYPE_PS3:
			case SDL_GAMEPAD_TYPE_PS4:
			case SDL_GAMEPAD_TYPE_PS5:
				*text = "L3";
				break;
			}
			break;
		case SDL_GAMEPAD_BUTTON_RIGHT_STICK:
			switch(type) {
			default:
				*text = "RS";
				break;

			case SDL_GAMEPAD_TYPE_PS3:
			case SDL_GAMEPAD_TYPE_PS4:
			case SDL_GAMEPAD_TYPE_PS5:
				*text = "R3";
				break;
			}
			break;
		case SDL_GAMEPAD_BUTTON_LEFT_SHOULDER:
			switch(type) {
			default:
				*text = "LB";
				break;

			case SDL_GAMEPAD_TYPE_PS3:
			case SDL_GAMEPAD_TYPE_PS4:
			case SDL_GAMEPAD_TYPE_PS5:
				*text = "L1";
				break;

			case SDL_GAMEPAD_TYPE_NINTENDO_SWITCH_PRO:
			case SDL_GAMEPAD_TYPE_NINTENDO_SWITCH_JOYCON_PAIR:
				*text = "L";
				break;
			}
			break;
		case SDL_GAMEPAD_BUTTON_RIGHT_SHOULDER:
			switch(type) {
			default:
				*text = "RB";
				break;

			case SDL_GAMEPAD_TYPE_PS3:
			case SDL_GAMEPAD_TYPE_PS4:
			case SDL_GAMEPAD_TYPE_PS5:
				*text = "R1";
				break;

			case SDL_GAMEPAD_TYPE_NINTENDO_SWITCH_PRO:
			case SDL_GAMEPAD_TYPE_NINTENDO_SWITCH_JOYCON_PAIR:
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
	return *text != NULL || *button != INPUT_BUTTON_NULL;
}
#endif

void INPUT_Open(void)
{
	#ifdef ENABLE_LINUXGPIO
	const char* const chipName = "gpiochip0";

	INPUT_GPIOChip = gpiod_chip_open_by_name(chipName);
	if (!INPUT_GPIOChip) {
		SDL_Log("Failed opening GPIO chip \"%s\". Continuing without GPIO input support", chipName);
	}
	else {
		SDL_memset(INPUT_GPIORepeat, 0, sizeof(INPUT_GPIORepeat));
		SDL_memset(INPUT_GPIOLines, 0, sizeof(INPUT_GPIOLines));
		if (
			!(INPUT_GPIOLines[0] = gpiod_chip_get_line(INPUT_GPIOChip,  5)) ||
			!(INPUT_GPIOLines[1] = gpiod_chip_get_line(INPUT_GPIOChip, 13)) ||
			!(INPUT_GPIOLines[2] = gpiod_chip_get_line(INPUT_GPIOChip,  6)) ||
			!(INPUT_GPIOLines[3] = gpiod_chip_get_line(INPUT_GPIOChip, 12)) ||
			!(INPUT_GPIOLines[4] = gpiod_chip_get_line(INPUT_GPIOChip, 19)) ||
			!(INPUT_GPIOLines[5] = gpiod_chip_get_line(INPUT_GPIOChip, 16)) ||
			!(INPUT_GPIOLines[6] = gpiod_chip_get_line(INPUT_GPIOChip, 26)) ||
			!(INPUT_GPIOLines[7] = gpiod_chip_get_line(INPUT_GPIOChip, 20)) ||
			!(INPUT_GPIOLines[8] = gpiod_chip_get_line(INPUT_GPIOChip, 21)) ||
			!(INPUT_GPIOLines[9] = gpiod_chip_get_line(INPUT_GPIOChip,  4))
		) {
			for (int line = 0; line < 10; line++) {
				if (INPUT_GPIOLines[line]) {
					gpiod_line_release(INPUT_GPIOLines[line]);
					INPUT_GPIOLines[line] = NULL;
				}
			}
			gpiod_chip_close(INPUT_GPIOChip);
			INPUT_GPIOChip = NULL;
			SDL_Log("Failed opening GPIO lines. Continuing without GPIO input support");
		}
		else {
			for (int line = 0; line < 10; line++) {
				if (gpiod_line_request_input(INPUT_GPIOLines[line], "input") < 0) {
					for (line = 0; line < 10; line++) {
						gpiod_line_release(INPUT_GPIOLines[line]);
						INPUT_GPIOLines[line] = NULL;
					}
					gpiod_chip_close(INPUT_GPIOChip);
					INPUT_GPIOChip = NULL;
					SDL_Log("Failed setting GPIO lines for input. Continuing without GPIO input support");
					break;
				}
			}
		}
	}
	#endif

	#ifdef INPUT_ENABLE_KEYBOARD
	SDL_memset(INPUT_KeyRepeat, 0, sizeof(INPUT_KeyRepeat));
	#endif

	#ifdef INPUT_ENABLE_GAME_CONTROLLER_DB
	if (!FILESYSTEM_FileExists("gamecontrollerdb.txt")) {
		SDL_Log("Missing game controller configurations file gamecontrollerdb.txt");
		return;
	}

	SDL_IOStream* const db = FILESYSTEM_OpenRead("gamecontrollerdb.txt");
	if (db) {
		const Sint64 size = SDL_GetIOSize(db);
		if (size > 0) {
			if (SDL_AddGamepadMappingsFromIO(db, true) < 0) {
				ERROR_Set("Failed adding game controller configurations from file gamecontrollerdb.txt: %s", SDL_GetError());
				MAIN_Exit(SDL_APP_FAILURE);
			}
			SDL_Log("Added game controller configurations from file gamecontrollerdb.txt");
		}
		else if (size == 0) {
			SDL_Log("No game controller configurations in file gamecontrollerdb.txt");
			SDL_CloseIO(db);
		}
		else {
			SDL_CloseIO(db);
			ERROR_Set("Failed getting size of game controller configurations file gamecontrollerdb.txt: %s", SDL_GetError());
			MAIN_Exit(SDL_APP_FAILURE);
		}
	}
	else {
		ERROR_Set("Error opening file gamecontrollerdb.txt: %s", SDL_GetError());
		MAIN_Exit(SDL_APP_FAILURE);
	}
	#endif
}

void INPUT_Close(void)
{
	#ifdef INPUT_ENABLE_LINUX_GPIO
	if (INPUT_GPIOChip) {
		for (int i = 0; i < 10; i++) {
			gpiod_line_release(INPUT_GPIOLines[i]);
		}
		gpiod_chip_close(INPUT_GPIOChip);

		INPUT_GPIOChip = NULL;
		SDL_memset(INPUT_GPIOLines, 0, sizeof(INPUT_GPIOLines));
	}
	#endif

	#ifdef INPUT_ENABLE_KEYBOARD
	SDL_memset(INPUT_KeyRepeat, 0, sizeof(INPUT_KeyRepeat));
	#endif

	#if defined(INPUT_ENABLE_JOYSTICK) || defined(INPUT_ENABLE_GAME_CONTROLLER)
	#ifdef INPUT_ENABLE_GAME_CONTROLLER
	if (INPUT_BuiltinCon.controller && SDL_GamepadConnected(INPUT_BuiltinCon.controller)) {
		SDL_CloseGamepad(INPUT_BuiltinCon.controller);
		INPUT_BuiltinCon.controller = NULL;
	}
	#endif
	INPUT_PlayerSlotsClose();
	#endif
}

void INPUT_Update(void)
{
	#ifdef INPUT_ENABLE_LINUX_GPIO
	for (int line = 0; line < 10; line++) {
		if (gpiod_line_get_value(INPUT_GPIOLines[line]) == 1) {
			if (++INPUT_GPIORepeat[line] == 1) LastControllerType = INPUT_DEVICE_TYPE_LINUX_GPIO;
		}
		else {
			INPUT_GPIORepeat[line] = 0;
		}
	}
	#endif

	#ifdef INPUT_ENABLE_KEYBOARD
	int numKeys = 0;
	const bool* keyStates = SDL_GetKeyboardState(&numKeys);

	for (int i = 0; i < INPUT_KEY_MAX; i++) {
		if (i < numKeys && keyStates[i]) {
			if (++INPUT_KeyRepeat[i] == 1) INPUT_LastDeviceType = INPUT_DEVICE_TYPE_KEYBOARD;
		}
		else {
			INPUT_KeyRepeat[i] = 0;
		}
	}
	#endif

	#ifdef INPUT_ENABLE_JOYSTICK
	INPUT_JoyInputsUpdate();
	#endif

	#ifdef INPUT_ENABLE_GAME_CONTROLLER
	INPUT_ConInputsUpdate();
	#endif
}
