#ifndef APP_audio_h_
#define APP_audio_h_

#include "APP_stdinc.h"

#define APP_WAVE_MAX 100

typedef enum APP_WaveFormat
{
	APP_WAVE_MASK   = 0xFF,

	// Flag BGM type as "simple", only one bgm.ext for everything.
	APP_WAVE_SIMPLE = 0x80,

	APP_WAVE_FORMAT = 0x7F,
	APP_WAVE_MID    =  1,
	APP_WAVE_WAV,
	APP_WAVE_OGG,
	APP_WAVE_MP3,
	APP_WAVE_FLAC,
	APP_WAVE_OPUS,
	APP_WAVE_MOD,
	APP_WAVE_IT,
	APP_WAVE_XM,
	APP_WAVE_S3M,
	// Add new formats at the end of the list, before MAXFORMAT, don't reorder
	APP_WAVE_MAXFORMAT
} APP_WaveFormat;

bool APP_InitAudio(unsigned soundBufferSize);
void APP_QuitAudio(void);

bool APP_WaveFormatSupported(APP_WaveFormat format);

void APP_LoadWave(const char* filename, unsigned num);
void APP_PlayWave(unsigned num);
void APP_ReplayWave(unsigned num);
void APP_StopWave(unsigned num);
void APP_PauseWave(unsigned num);
bool APP_IsPlayWave(unsigned num);
void APP_SetVolumeWave(unsigned num, unsigned vol);
void APP_SetLoopModeWave(unsigned num, bool looping);

void APP_LoadMusic(const char* filename);
void APP_PlayMusic(void);
void APP_ReplayMusic(void);
void APP_StopMusic(void);
void APP_PauseMusic(void);
bool APP_IsPlayMusic(void);
void APP_SetVolumeMusic(unsigned volume);

#endif
