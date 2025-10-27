#ifndef APP_ogg_h_
#define APP_ogg_h_

#include "APP.h"

bool APP_LoadOGG(SDL_IOStream* file, const SDL_AudioSpec* format, uint8_t** data, uint32_t* size);

#endif
