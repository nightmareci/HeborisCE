#ifndef APP_audio_h_
#define APP_audio_h_

#include "APP_stdinc.h"

// Playable wave numbers must be 0 or greater; all negative wave numbers are treated the same as APP_WAVE_NUM_ALL.
// Volume is a percentage, so a volume value of 100 corresponds to full volume, 0 corresponds to silent.

// The maximum number of waves that can be used. Adjust to meet requirements.
#define APP_WAVES_COUNT 100

typedef Uint8 APP_SoundBits;

#define APP_SOUND_BITS_MASK 0xFF
#define APP_SOUND_BITS_SIMPLE 0x80
#define APP_SOUND_BITS_FORMAT 0x7F

// Sound formats that may or may not be supported, depending on how the game was built.
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

// A special wave number that causes operations to act upon all waves.
#define APP_WAVE_NUM_ALL -1

// Init the audio system.
bool APP_InitAudio(void);

// Quit the audio system.
void APP_QuitAudio(void);

// Get if a sound format is currently supported. Depending on how the game is built, some formats might not be supported.
bool APP_IsSoundFormatSupported(APP_SoundFormat format);

// Load a wave. Does not support APP_WAVE_NUM_ALL.
void APP_LoadWave(int num, const char* filename);

// Play waves. APP_WAVE_NUM_ALL plays all waves.
void APP_PlayWave(int num);

// Resume waves. APP_WAVE_NUM_ALL resumes all waves.
void APP_ResumeWave(int num);

// Stop waves. APP_WAVE_NUM_ALL stops all waves.
void APP_StopWave(int num);

// Pause waves. APP_WAVE_NUM_ALL pauses all waves.
void APP_PauseWave(int num);

// Get if a wave is playing. APP_WAVE_NUM_ALL checks all waves, returning true if at least one is playing.
bool APP_IsWavePlaying(int num);

// Set wave volume. APP_WAVE_NUM_ALL sets all waves' volumes the same.
void APP_SetWaveVolume(int num, int volume);

// Set wave looping state. APP_WAVE_NUM_ALL sets all waves' looping state.
void APP_SetWaveLooping(int num, bool looping);

// Load music.
void APP_LoadMusic(const char* filename);

// Play music.
void APP_PlayMusic(void);

// Resume music.
void APP_ResumeMusic(void);

// Stop music.
void APP_StopMusic(void);

// Pause music.
void APP_PauseMusic(void);

// Get if music is playing.
bool APP_IsMusicPlaying(void);

// Set music volume.
void APP_SetMusicVolume(int volume);

#endif
