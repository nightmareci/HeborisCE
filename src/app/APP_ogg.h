#ifndef APP_ogg_h_
#define APP_ogg_h_

#include "APP_audio.h"
#include "APP_audio_private.h"

bool APP_LoadOGG(SDL_IOStream* file, const SDL_AudioSpec* format, uint8_t** data, uint32_t* size);
APP_StreamingAudioData* APP_CreateStreamingOGGAudioData(SDL_IOStream* file, SDL_AudioSpec* dstSpec);

#endif
