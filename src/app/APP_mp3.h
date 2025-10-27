#ifndef APP_mp3_h_
#define APP_mp3_h_

#include "APP.h"

bool APP_LoadMP3(SDL_IOStream* file, const SDL_AudioSpec* format, uint8_t** data, uint32_t* size);

#endif
