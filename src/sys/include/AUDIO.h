#ifndef AUDIO_h_
#define AUDIO_h_

#include <stdbool.h>

// Playable wave numbers must be 0 or greater; all negative wave numbers are
// treated the same as AUDIO_WAVE_NUM_ALL.
//
// Volume is a percentage, so a volume value of 100 corresponds to full volume,
// 0 corresponds to silent.
//
// Filenames must have no file extension; load functions will try extensions
// for all supported formats.

// A special wave number that causes operations to act upon all waves.
#define AUDIO_WAVE_NUM_ALL -1

// Init the audio system.
bool AUDIO_Init(int wavesCount);

// Quit the audio system.
void AUDIO_Quit(void);

// Load a wave. Does not support AUDIO_WAVE_NUM_ALL.
void AUDIO_LoadWave(int num, const char* filename, bool looping);

// Play waves. AUDIO_WAVE_NUM_ALL plays all waves.
void AUDIO_PlayWave(int num);

// Resume waves. AUDIO_WAVE_NUM_ALL resumes all waves.
void AUDIO_ResumeWave(int num);

// Stop waves. AUDIO_WAVE_NUM_ALL stops all waves.
void AUDIO_StopWave(int num);

// Pause waves. AUDIO_WAVE_NUM_ALL pauses all waves.
void AUDIO_PauseWave(int num);

// Get if a wave is playing. AUDIO_WAVE_NUM_ALL checks all waves, returning true
// if at least one is playing.
bool AUDIO_IsWavePlaying(int num);

// Set wave volume. AUDIO_WAVE_NUM_ALL sets the same volume for all waves.
void AUDIO_SetWaveVolume(int num, int volume);

// Set wave looping state. AUDIO_WAVE_NUM_ALL sets the same looping state for all waves.
void AUDIO_SetWaveLooping(int num, bool looping);

// Load music.
void AUDIO_LoadMusic(const char* filename);

// Play music.
void AUDIO_PlayMusic(void);

// Resume music.
void AUDIO_ResumeMusic(void);

// Stop music.
void AUDIO_StopMusic(void);

// Pause music.
void AUDIO_PauseMusic(void);

// Get if music is playing.
bool AUDIO_IsMusicPlaying(void);

// Set music volume.
void AUDIO_SetMusicVolume(int volume);

#endif
