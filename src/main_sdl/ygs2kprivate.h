#pragma once

#include "ygs2kfunc.h"

#define		YGS_TEXTURE_MAX		100
#define		YGS_SOUND_MAX		100
#define		YGS_MUSIC_MAX		100
#define		YGS_TEXTLAYER_MAX	16

#define 	YGS_VOLUME_MAX		MIX_MAX_VOLUME

#define		YGS_WIDE_SCREEN (4.0f / 3.0f)
#define		YGS_NARROW_SCREEN (3.0f / 4.0f)

extern YGS2kEInputType YGS2kLastInputType;

#ifdef ENABLE_JOYSTICK
// TODO: Replace YGS2kJoysClose() + YGS2kJoysOpen() with YGS2kJoysChanged() and
// make joysticks use player indices instead of device indices, like
// controllers

void YGS2kJoysClose();
bool YGS2kJoysOpen();
#endif

#ifdef ENABLE_GAME_CONTROLLER
void YGS2kConsClose();
bool YGS2kConsChanged();
#endif

bool YGS2kInputsOpen();
void YGS2kInputsClose();
