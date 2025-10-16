#ifndef APP_audio_h_
#define APP_audio_h_

#include "APP_stdinc.h"

// Increase this if more waves are needed
#define APP_WAVES_COUNT 100

typedef Uint8 APP_SoundBits;
#define APP_SOUND_BITS_MASK 0xFF
#define APP_SOUND_BITS_SIMPLE 0x80
#define APP_SOUND_BITS_FORMAT 0x7F

typedef enum APP_SoundFormat
{
	APP_SOUND_FORMAT_MID,
	APP_SOUND_FORMAT_WAV,
	APP_SOUND_FORMAT_OGG,
	APP_SOUND_FORMAT_MP3,
	APP_SOUND_FORMAT_FLAC,
	APP_SOUND_FORMAT_OPUS,
	APP_SOUND_FORMAT_MOD,
	APP_SOUND_FORMAT_IT,
	APP_SOUND_FORMAT_XM,
	APP_SOUND_FORMAT_S3M,
	// Add new formats at the end of the list, before COUNT, don't reorder
	APP_SOUND_FORMAT_COUNT
} APP_SoundFormat;

bool APP_InitAudio(void);
void APP_QuitAudio(void);

bool APP_IsSoundFormatSupported(APP_SoundFormat format);

void APP_LoadWave(int num, const char* filename);
void APP_PlayWave(int num);
void APP_ResumeWave(int num);
void APP_StopWave(int num);
void APP_PauseWave(int num);
bool APP_IsWavePlaying(int num);
void APP_SetWaveVolume(int num, int volume);
void APP_SetWaveLooping(int num, bool looping);

void APP_LoadMusic(const char* filename);
void APP_PlayMusic(void);
void APP_ResumeMusic(void);
void APP_StopMusic(void);
void APP_PauseMusic(void);
bool APP_IsMusicPlaying(void);
void APP_SetMusicVolume(int volume);

#endif
