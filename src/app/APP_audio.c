#include "APP_audio.h"
#include "APP_main.h"

typedef struct APP_Wave
{
	SDL_AudioStream* stream;
	void* data;
	bool looping;
} APP_Wave;

static APP_Wave			APP_Waves[APP_WAVE_MAX];
static SDL_AudioDeviceID	APP_AudioDevice;
static APP_Wave			APP_Music;
static bool			APP_WaveFormatsSupported[APP_WAVE_MAXFORMAT];

bool APP_InitAudio(unsigned soundBufferSize)
{
	APP_WaveFormatsSupported[APP_WAVE_WAV] = true;

	APP_AudioDevice = SDL_OpenAudioDevice(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, NULL);
	if (!APP_AudioDevice) {
		fprintf(stderr, "Couldn't open audio device: %s\n", SDL_GetError());
		return false;
	}

	// サウンドの初期化
	// Initialize sounds
	SDL_zero(APP_Waves);
	SDL_zero(APP_Music);

	return true;
}

void APP_QuitAudio(void)
{
	// サウンドの解放
	// Free sounds

	for (int i = 0; i < APP_WAVE_MAX; i++) {
		SDL_DestroyAudioStream(APP_Waves[i].stream);
		SDL_free(APP_Waves[i].data);
	}
	SDL_zero(APP_Waves);

	SDL_DestroyAudioStream(APP_Music.stream);
	SDL_free(APP_Music.data);
	SDL_zero(APP_Music);

}

bool APP_IsPlayMusic(void)
{
	return APP_Music.stream && SDL_GetAudioStreamAvailable(APP_Music.stream) > 0;
}

void APP_PauseMusic(void)
{
	// TODO
#if 0
	Mix_PauseMusic();
#endif
}

void APP_ReplayMusic(void)
{
	// TODO
#if 0
	Mix_ResumeMusic();
#endif
}

void APP_PlayWave(unsigned num)
{
	// TODO
#if 0
	if ( APP_Waves[num].chunk )
	{
		Mix_PlayChannel(num, APP_Waves[num].chunk, APP_Waves[num].loops);
	}
#endif
}

void APP_ReplayWave(unsigned num)
{
	// TODO
#if 0
	Mix_Resume(num);
#endif
}

void APP_StopWave(unsigned num)
{
	// TODO
#if 0
	Mix_HaltChannel(num);
#endif
}

void APP_PauseWave(unsigned num)
{
	// TODO
#if 0
	Mix_Pause(num);
#endif
}

void APP_SetVolumeWave(unsigned num, unsigned vol)
{
	// TODO
#if 0
	int volume = (int)((vol / 100.0f) * APP_VOLUME_MAX);
	if ( volume > APP_VOLUME_MAX ) { volume = APP_VOLUME_MAX; }
	if ( volume < 0 )   { volume = 0; }
	Mix_Volume(num, volume);
#endif
}

bool APP_IsPlayWave(unsigned num)
{
	return num < APP_WAVE_MAX && APP_Waves[num].stream && SDL_GetAudioStreamAvailable(APP_Waves[num].stream) > 0;
}

void APP_LoadWave(const char* filename, unsigned num)
{
#if 0
	int len = strlen(filename);
	if ( len < 4 ) { return; }

	SDL_RWops *src;
	src = APP_OpenRead(filename);
	if ( !src ) return;
	if ( APP_Waves[num].chunk )
	{
		Mix_FreeChunk(APP_Waves[num].chunk);
		APP_Waves[num].chunk = NULL;
	}
	APP_Waves[num].chunk = Mix_LoadWAV_RW(src, 1);
	Mix_VolumeChunk(APP_Waves[num].chunk, APP_VOLUME_MAX);
	APP_Waves[num].loops = 0;
#endif

#if 0
	// TODO
	if (num >= APP_WAVE_MAX) {
		APP_Exit(EXIT_FAILURE);
	}
	SDL_IOStream* file = APP_OpenRead(filename);
	if (!file) {
		APP_Exit(EXIT_FAILURE);
	}
	if (APP_Waves[num].stream) {
		if (!SDL_ClearAudioStream(APP_Waves[num].stream)) {
			APP_Exit(EXIT_FAILURE);
		}
	}
#endif
}

void APP_SetLoopModeWave(unsigned num, bool looping)
{
	if (num >= APP_WAVE_MAX) {
		APP_Exit(EXIT_FAILURE);
	}
	APP_Waves[num].looping = looping;
}

void APP_LoadMusic(const char* filename)
{
	// TODO
#if 0
	if ( APP_Music )
	{
		Mix_FreeMusic(APP_Music);
		APP_Music = NULL;
	}

	SDL_RWops* src;
	if (!(src = APP_OpenRead(filename))) return;
	APP_Music = Mix_LoadMUS_RW(src, true);
	Mix_VolumeMusic(APP_VOLUME_MAX);
#endif
}

void APP_PlayMusic(void)
{
	// TODO
#if 0
	if ( APP_Music )
	{
		Mix_PlayMusic(APP_Music, -1);
	}
#endif
}

void APP_StopMusic(void)
{
	// TODO
#if 0
	Mix_HaltMusic();
#endif
}

void APP_SetVolumeMusic(unsigned volume)
{
	// TODO
#if 0
	int volume = (int)((vol / 100.0f) * APP_VOLUME_MAX);
	if ( volume > APP_VOLUME_MAX ) { volume = APP_VOLUME_MAX; }
	if ( volume < 0 )   { volume = 0; }
	Mix_VolumeMusic(volume);
#endif
}

bool APP_WaveFormatSupported(APP_WaveFormat format) {
	const int i = format & APP_WAVE_FORMAT;
	return i >= 1 && i < APP_WAVE_MAXFORMAT && APP_WaveFormatsSupported[i];
}
