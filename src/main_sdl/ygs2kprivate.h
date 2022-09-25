#pragma once

#include "ygs2kfunc.h"

#define		YGS_TEXTURE_MAX		100
#define		YGS_SOUND_MAX		100
#define		YGS_MUSIC_MAX		100
#define		YGS_TEXTLAYER_MAX	16

#define 	YGS_VOLUME_MAX		MIX_MAX_VOLUME

#define		YGS_WIDE_SCREEN (4.0f / 3.0f)
#define		YGS_NARROW_SCREEN (3.0f / 4.0f)

extern YGS2kEControllerType YGS2kLastControllerType;

#ifdef ENABLE_JOYSTICK
void YGS2kJoyClose();
int YGS2kJoyOpen();
#endif

#ifdef ENABLE_GAME_CONTROLLER
void YGS2kConClose();
int YGS2kConOpen();
#endif

int YGS2kInputOpen();
void YGS2kInputClose();