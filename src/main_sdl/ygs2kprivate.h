#pragma once

#include "ygs2kfunc.h"

#define		YGS_TEXTURE_MAX		100
#define		YGS_WAVE_MAX		100
#define		YGS_TEXTLAYER_MAX	16

#define 	YGS_VOLUME_MAX		MIX_MAX_VOLUME

#define		YGS_WIDE_SCREEN (4.0f / 3.0f)
#define		YGS_NARROW_SCREEN (3.0f / 4.0f)

extern YGS2kEInputType YGS2kLastInputType;

#if defined(ENABLE_JOYSTICK) || defined(ENABLE_GAME_CONTROLLER)
bool YGS2kPlayerSlotsChanged();
#endif

bool YGS2kInputsOpen();
void YGS2kInputsClose();
