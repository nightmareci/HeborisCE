#pragma once

// TODO: Use button type everywhere instead of bare integers.

typedef enum EButton
{
	BTN_NULL = -1,
	BTN_UP = 0,
	BTN_DOWN,
	BTN_LEFT,
	BTN_RIGHT,
	BTN_A,
	BTN_B,
	BTN_C,
	BTN_D,
	NUMGAMEBTNS,
	BTN_GIVEUP = NUMGAMEBTNS,
	BTN_PAUSE,
	NUMBTNS
} EButton;