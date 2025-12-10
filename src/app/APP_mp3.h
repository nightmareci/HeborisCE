#ifndef APP_mp3_h_
#define APP_mp3_h_

#include "APP_audio.h"
#include "APP_audio_private.h"

bool APP_PreloadMP3(SDL_IOStream* file, const SDL_AudioSpec* format, uint8_t** data, uint32_t* size);
APP_StreamingAudioData* APP_CreateStreamingMP3AudioData(SDL_IOStream* file, SDL_AudioSpec* dstSpec);

#endif
