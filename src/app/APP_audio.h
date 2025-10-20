#ifndef APP_audio_h_
#define APP_audio_h_

#include "APP_stdinc.h"

// Playable wave numbers must be 0 or greater; all negative wave numbers are
// treated the same as APP_WAVE_NUM_ALL.
//
// Volume is a percentage, so a volume value of 100 corresponds to full volume,
// 0 corresponds to silent.

typedef Uint8 APP_SoundBits;

// A special wave number that causes operations to act upon all waves.
#define APP_WAVE_NUM_ALL -1

// Init the audio system.
bool APP_InitAudio(int wavesCount);

// Quit the audio system.
void APP_QuitAudio(void);

// Load leadin for a wave. Does not support APP_WAVE_NUM_ALL.
//
// Loading a leadin on a wave number also enables looping for the main part,
// after the leadin has finished playing. But looping can still be disabled,
// after loading.
//
// The filename must have no file extension; the function will try extensions
// for all supported formats in a priority order.
void APP_LoadWaveLeadin(int num, const char* filename_no_ext);

// Load a wave. Does not support APP_WAVE_NUM_ALL. For waves with a leadin, this
// loads into the looping main part.
//
// The filename must have no file extension; the function will try extensions
// for all supported formats in a priority order.
void APP_LoadWave(int num, const char* filename_no_ext);

// Play waves. APP_WAVE_NUM_ALL plays all waves. This initiates playback regardless
// of whether the wave was paused, from the start of the wave.
void APP_PlayWave(int num);

// Resume only waves that are currently paused. APP_WAVE_NUM_ALL resumes all
// waves.
void APP_ResumeWave(int num);

// Stop waves. APP_WAVE_NUM_ALL stops all waves.
void APP_StopWave(int num);

// Pause waves. APP_WAVE_NUM_ALL pauses all waves.
void APP_PauseWave(int num);

// Get if a wave is playing. APP_WAVE_NUM_ALL checks all waves, returning true
// if at least one is playing.
bool APP_IsWavePlaying(int num);

// Set wave volume. APP_WAVE_NUM_ALL sets all waves' volumes the same.
void APP_SetWaveVolume(int num, int volume);

// Set wave looping state. APP_WAVE_NUM_ALL sets all waves' looping state.
void APP_SetWaveLooping(int num, bool looping);

// Load leadin for music.
//
// The filename must have no file extension; the function will try extensions
// for all supported formats in a priority order.
void APP_LoadMusicLeadin(const char* filename_no_ext);

// Load music.
//
// The filename must have no file extension; the function will try extensions
// for all supported formats in a priority order.
void APP_LoadMusic(const char* filename_no_ext);

// Play music. This initiates playback regardless of whether the music was
// paused, from the start of the music.
void APP_PlayMusic(void);

// Resume music if it's currently paused.
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
