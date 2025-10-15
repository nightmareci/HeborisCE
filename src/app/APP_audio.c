#include "APP_audio.h"
#include "APP_filesystem.h"
#include "APP_main.h"

typedef struct APP_Wave
{
	SDL_AudioStream* stream;
	uint8_t* data;
	float gain;
	SDL_AtomicInt playing;
	SDL_AtomicInt looping;
	uint32_t size;
	uint32_t pos;
	bool paused;
} APP_Wave;

static bool APP_WasAudioInit;
static APP_Wave APP_Waves[APP_WAVE_MAX];
static SDL_AudioDeviceID APP_AudioDevice;
static SDL_AudioSpec APP_AudioDeviceFormat;
static APP_Wave APP_Music;
static bool APP_WaveFormatsSupported[APP_WAVE_MAXFORMAT];

static void SDLCALL APP_GetAudioStreamData(void* userdata, SDL_AudioStream* stream, int additionalAmount, int totalAmount)
{
	(void)totalAmount;
	APP_Wave* const wave = userdata;
	if (additionalAmount == 0 || !SDL_GetAtomicInt(&wave->playing)) {
		return;
	}
	else if (!SDL_GetAtomicInt(&wave->looping)) {
		if (wave->pos == wave->size) {
			SDL_SetAtomicInt(&wave->playing, 0);
		}
		else if (additionalAmount > wave->size - wave->pos) {
			SDL_PutAudioStreamData(stream, wave->data + wave->pos, wave->size - wave->pos);
			wave->pos = wave->size;
			SDL_SetAtomicInt(&wave->playing, 0);
		}
		else {
			SDL_PutAudioStreamData(stream, wave->data + wave->pos, additionalAmount);
			wave->pos += additionalAmount;
		}
	}
	else if (additionalAmount > wave->size - wave->pos) {
		int remaining = additionalAmount;
		while (remaining > 0) {
			if (remaining > wave->size - wave->pos) {
				SDL_PutAudioStreamData(stream, wave->data + wave->pos, wave->size - wave->pos);
				remaining -= wave->size - wave->pos;
				wave->pos = 0;
			}
			else {
				SDL_PutAudioStreamData(stream, wave->data + wave->pos, remaining);
				wave->pos += remaining;
				remaining = 0;
			}
		}
	}
	else {
		SDL_PutAudioStreamData(stream, wave->data + wave->pos, additionalAmount);
		wave->pos += additionalAmount;
	}
}

bool APP_InitAudio(void)
{
	if (APP_WasAudioInit) {
		return true;
	}

	// Initialize supported formats
	APP_WaveFormatsSupported[APP_WAVE_WAV] = true;

	// Initialize audio device
	APP_AudioDevice = SDL_OpenAudioDevice(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, NULL);
	if (!APP_AudioDevice) {
		fprintf(stderr, "Couldn't open audio device: %s\n", SDL_GetError());
		return false;
	}
	if (!SDL_GetAudioDeviceFormat(APP_AudioDevice, &APP_AudioDeviceFormat, NULL)) {
		fprintf(stderr, "Couldn't get audio device format: %s\n", SDL_GetError());
		return false;
	}

	// サウンドの初期化
	// Initialize sounds
	SDL_zero(APP_Waves);
	SDL_zero(APP_Music);

	APP_WasAudioInit = true;
	return true;
}

void APP_QuitAudio(void)
{
	if (!APP_WasAudioInit) {
		return;
	}

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

	SDL_CloseAudioDevice(APP_AudioDevice);

	APP_WasAudioInit = false;
}

bool APP_IsPlayMusic(void)
{
	return !!SDL_GetAtomicInt(&APP_Music.playing);
}

void APP_PauseMusic(void)
{
	// TODO
#if 0
	Mix_PauseMusic();
#endif
}

static void APP_PlayWavePrivate(int num, bool replay)
{
	if (num >= APP_WAVE_MAX) {
		return;
	}

	if (num >= 0) {
		APP_Wave* const wave = &APP_Waves[num];
		if (!wave->stream || !wave->data) {
			return;
		}
		if (replay && !wave->paused) {
			return;
		}
		SDL_UnbindAudioStream(wave->stream);
		if (!replay && !SDL_ClearAudioStream(wave->stream)) {
			APP_Exit(EXIT_FAILURE);
		}
		if (!SDL_SetAudioStreamGain(wave->stream, wave->gain)) {
			APP_Exit(EXIT_FAILURE);
		}
		SDL_SetAtomicInt(&wave->playing, 1);
		if (!replay) {
			wave->pos = 0;
		}
		if (!SDL_BindAudioStream(APP_AudioDevice, wave->stream)) {
			APP_Exit(EXIT_FAILURE);
		}
		wave->paused = false;
	}
	else {
		for (int i = 0; i < APP_WAVE_MAX; i++) {
			APP_Wave* const wave = &APP_Waves[i];
			if (!wave->stream || !wave->data) {
				continue;
			}
			if (replay && !wave->paused) {
				continue;
			}
			SDL_UnbindAudioStream(wave->stream);
			if (!replay && !SDL_ClearAudioStream(wave->stream)) {
				APP_Exit(EXIT_FAILURE);
			}
			if (!SDL_SetAudioStreamGain(wave->stream, wave->gain)) {
				APP_Exit(EXIT_FAILURE);
			}
			SDL_SetAtomicInt(&wave->playing, 1);
			if (!replay) {
				wave->pos = 0;
			}
			if (!SDL_BindAudioStream(APP_AudioDevice, wave->stream)) {
				APP_Exit(EXIT_FAILURE);
			}
			wave->paused = false;
		}
	}
}

void APP_ReplayMusic(void)
{
	// TODO
#if 0
	Mix_ResumeMusic();
#endif
}

void APP_PlayWave(int num)
{
	APP_PlayWavePrivate(num, false);
}

void APP_ReplayWave(int num)
{
	APP_PlayWavePrivate(num, true);
}

void APP_StopWave(int num)
{
	if (num >= APP_WAVE_MAX) {
		return;
	}

	if (num >= 0) {
		APP_Wave* const wave = &APP_Waves[num];
		SDL_UnbindAudioStream(wave->stream);
		SDL_SetAtomicInt(&wave->playing, 0);
		wave->pos = 0;
		wave->paused = false;
	}
	else {
		for (int i = 0; i < APP_WAVE_MAX; i++) {
			APP_Wave* const wave = &APP_Waves[i];
			SDL_UnbindAudioStream(wave->stream);
			SDL_SetAtomicInt(&wave->playing, 0);
			wave->pos = 0;
			wave->paused = false;
		}
	}
}

void APP_PauseWave(int num)
{
	if (num >= APP_WAVE_MAX) {
		return;
	}

	if (num >= 0) {
		APP_Wave* const wave = &APP_Waves[num];
		if (!wave->stream) {
			return;
		}
		if (SDL_GetAtomicInt(&wave->playing)) {
			SDL_UnbindAudioStream(wave->stream);
			SDL_SetAtomicInt(&wave->playing, 0);
			wave->paused = true;
		}
	}
	else {
		for (int i = 0; i < APP_WAVE_MAX; i++) {
			APP_Wave* const wave = &APP_Waves[i];
			if (!wave->stream) {
				continue;
			}
			if (SDL_GetAtomicInt(&wave->playing)) {
				SDL_UnbindAudioStream(wave->stream);
				SDL_SetAtomicInt(&wave->playing, 0);
				wave->paused = true;
			}
		}
	}
}

void APP_SetVolumeWave(int num, int vol)
{
	if (num >= APP_WAVE_MAX) {
		return;
	}
	float gain;
	if (vol < 0) {
		gain = 0.0f;
	}
	else {
		gain = vol / 100.0f;
	}
	if (num >= 0) {
		APP_Wave* const wave = &APP_Waves[num];
		wave->gain = gain;
		if (wave->stream && !SDL_SetAudioStreamGain(wave->stream, gain)) {
			APP_Exit(EXIT_FAILURE);
		}
	}
	else {
		for (int i = 0; i < APP_WAVE_MAX; i++) {
			APP_Wave* const wave = &APP_Waves[i];
			wave->gain = gain;
			if (wave->stream && !SDL_SetAudioStreamGain(wave->stream, gain)) {
				APP_Exit(EXIT_FAILURE);
			}
		}
	}
}

bool APP_IsPlayWave(int num)
{
	return
		num >= 0 &&
		num < APP_WAVE_MAX &&
		!!SDL_GetAtomicInt(&APP_Waves[num].playing);
}

void APP_LoadWave(const char* filename, int num)
{
	if (!filename || SDL_strlen(filename) < 5 || num < 0 || num >= APP_WAVE_MAX) {
		return;
	}
	const char* const extension = filename + SDL_strlen(filename) - 4;
	if (SDL_strcasecmp(extension, ".wav") != 0) {
		return;
	}
	if (!APP_FileExists(filename)) {
		return;
	}

	APP_Wave* const wave = &APP_Waves[num];
	SDL_UnbindAudioStream(wave->stream);

	SDL_IOStream* const file = APP_OpenRead(filename);
	if (!file) {
		APP_Exit(EXIT_FAILURE);
	}
	if (wave->data) {
		SDL_free(wave->data);
		wave->data = NULL;
	}
	SDL_AudioSpec waveFormat;
	if (!SDL_LoadWAV_IO(file, true, &waveFormat, &wave->data, &wave->size)) {
		APP_Exit(EXIT_FAILURE);
	}

	if (wave->stream) {
		if (!SDL_ClearAudioStream(wave->stream)) {
			APP_Exit(EXIT_FAILURE);
		}
		if (!SDL_SetAudioStreamFormat(wave->stream, &waveFormat, &APP_AudioDeviceFormat)) {
			APP_Exit(EXIT_FAILURE);
		}
		if (!SDL_SetAudioStreamGain(wave->stream, 1.0f)) {
			APP_Exit(EXIT_FAILURE);
		}
	}
	else {
		wave->stream = SDL_CreateAudioStream(&waveFormat, &APP_AudioDeviceFormat);
		if (!wave->stream) {
			APP_Exit(EXIT_FAILURE);
		}
	}

	SDL_SetAtomicInt(&wave->playing, 0);
	SDL_SetAtomicInt(&wave->looping, 0);
	wave->paused = false;
	if (!SDL_SetAudioStreamGetCallback(wave->stream, APP_GetAudioStreamData, wave)) {
		APP_Exit(EXIT_FAILURE);
	}
}

void APP_SetLoopModeWave(int num, bool looping)
{
	if (num >= APP_WAVE_MAX) {
		return;
	}
	if (num >= 0) {
		SDL_SetAtomicInt(&APP_Waves[num].looping, !!looping);
	}
	else {
		for (int i = 0; i < APP_WAVE_MAX; i++) {
			SDL_SetAtomicInt(&APP_Waves[i].looping, !!looping);
		}
	}
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

void APP_SetVolumeMusic(int volume)
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
	APP_WaveFormat i = format & APP_WAVE_FORMAT;
	return i >= 1 && i < APP_WAVE_MAXFORMAT && APP_WaveFormatsSupported[i];
}
