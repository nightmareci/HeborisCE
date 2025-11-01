#ifndef APP_wav_h_
#define APP_wav_h_

#include "APP.h"

bool APP_PreloadWAV(SDL_IOStream* file, const SDL_AudioSpec* format, uint8_t** data, uint32_t* size);

#endif
