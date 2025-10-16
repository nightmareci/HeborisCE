#include "APP_audio.h"
#include "APP_filesystem.h"
#include "APP_main.h"

typedef struct APP_Sound
{
	SDL_AudioStream* stream;
	uint8_t* data;
	SDL_AtomicInt playing;
	SDL_AtomicInt looping;
	float gain;
	uint32_t size;
	uint32_t pos;
	bool paused;
} APP_Sound;

static bool APP_SoundFormatsSupported[APP_SOUND_FORMAT_COUNT];
static bool APP_WasAudioInit;
static SDL_AudioDeviceID APP_AudioDevice;
static SDL_AudioSpec APP_AudioDeviceFormat;
static APP_Sound APP_Waves[APP_WAVES_COUNT];
static APP_Sound APP_Music;

static void SDLCALL APP_GetAudioStreamData(void* userdata, SDL_AudioStream* stream, int additionalAmount, int totalAmount)
{
	(void)totalAmount;
	if (additionalAmount == 0) {
		return;
	}
	APP_Sound* const wave = userdata;
	const int playing = SDL_GetAtomicInt(&wave->playing);
	SDL_MemoryBarrierAcquire();
	if (!playing) {
		return;
	}
	else if (!SDL_GetAtomicInt(&wave->looping)) {
		if (additionalAmount >= wave->size - wave->pos) {
			SDL_PutAudioStreamData(stream, wave->data + wave->pos, wave->size - wave->pos);
			wave->pos = wave->size;
			SDL_MemoryBarrierRelease();
			SDL_SetAtomicInt(&wave->playing, 0);
		}
		else {
			SDL_PutAudioStreamData(stream, wave->data + wave->pos, additionalAmount);
			wave->pos += additionalAmount;
		}
	}
	else if (additionalAmount >= wave->size - wave->pos) {
		int remaining = additionalAmount;
		while (remaining > 0) {
			if (remaining >= wave->size - wave->pos) {
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

static bool APP_InitSound(APP_Sound* sound)
{
	SDL_zero(*sound);
	sound->gain = 1.0f;
	SDL_MemoryBarrierRelease();
	SDL_SetAtomicInt(&sound->playing, 0);
	const SDL_AudioSpec waveFormat = { SDL_AUDIO_S16, 2, 44100 };
	sound->stream = SDL_CreateAudioStream(&waveFormat, &APP_AudioDeviceFormat);
	if (
		!sound->stream ||
		!SDL_SetAudioStreamGetCallback(sound->stream, APP_GetAudioStreamData, sound) ||
		!SDL_BindAudioStream(APP_AudioDevice, sound->stream)
	) {
		return false;
	}
	else {
		return true;
	}
}

bool APP_InitAudio(void)
{
	if (APP_WasAudioInit) {
		return true;
	}

	// Initialize supported formats
	APP_SoundFormatsSupported[APP_SOUND_FORMAT_WAV] = true;

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
	for (int i = 0; i < APP_WAVES_COUNT; i++) {
		if (!APP_InitSound(&APP_Waves[i])) {
			return false;
		}
	}
	if (!APP_InitSound(&APP_Music)) {
		return false;
	}
	SDL_SetAtomicInt(&APP_Music.looping, 1);

	APP_WasAudioInit = true;
	return true;
}

static void APP_QuitSound(APP_Sound* sound)
{
	SDL_DestroyAudioStream(sound->stream);
	SDL_free(sound->data);
	SDL_zero(*sound);
}

void APP_QuitAudio(void)
{
	if (!APP_WasAudioInit) {
		return;
	}

	// サウンドの解放
	// Free sounds
	for (int i = 0; i < APP_WAVES_COUNT; i++) {
		APP_QuitSound(&APP_Waves[i]);
	}
	APP_QuitSound(&APP_Music);

	SDL_CloseAudioDevice(APP_AudioDevice);
	APP_AudioDevice = 0;

	APP_WasAudioInit = false;
}

bool APP_IsSoundFormatSupported(APP_SoundFormat format) {
	return format >= 0 && format < APP_SOUND_FORMAT_COUNT && APP_SoundFormatsSupported[format];
}

static void APP_LoadSound(APP_Sound* sound, const char* filename)
{
	if (!APP_WasAudioInit || !filename || SDL_strlen(filename) < 5) {
		return;
	}

	const char* const extension = filename + SDL_strlen(filename) - 4;
	if (SDL_strcasecmp(extension, ".wav") != 0) {
		return;
	}
	if (!APP_FileExists(filename)) {
		return;
	}

	SDL_UnbindAudioStream(sound->stream);

	SDL_IOStream* const file = APP_OpenRead(filename);
	if (!file) {
		APP_Exit(EXIT_FAILURE);
	}
	if (sound->data) {
		SDL_free(sound->data);
		sound->data = NULL;
	}
	SDL_AudioSpec waveFormat;
	if (!SDL_LoadWAV_IO(file, true, &waveFormat, &sound->data, &sound->size)) {
		APP_Exit(EXIT_FAILURE);
	}

	if (sound != &APP_Music) {
		SDL_SetAtomicInt(&sound->looping, 0);
	}
	sound->gain = 1.0f;
	sound->pos = 0;
	sound->paused = false;
	SDL_MemoryBarrierRelease();
	SDL_SetAtomicInt(&sound->playing, 0);
	if (
		!SDL_SetAudioStreamFormat(sound->stream, &waveFormat, &APP_AudioDeviceFormat) ||
		!SDL_SetAudioStreamGain(sound->stream, 1.0f) ||
		!SDL_BindAudioStream(APP_AudioDevice, sound->stream)
	) {
		APP_Exit(EXIT_FAILURE);
	}
}

static void APP_StartSound(APP_Sound* sound, bool replay)
{
	if (!APP_WasAudioInit || !sound->data || (replay && !sound->paused)) {
		return;
	}
	SDL_UnbindAudioStream(sound->stream);
	if (!replay) {
		sound->pos = 0;
	}
	sound->paused = false;
	SDL_MemoryBarrierRelease();
	SDL_SetAtomicInt(&sound->playing, 1);
	if (!SDL_BindAudioStream(APP_AudioDevice, sound->stream)) {
		APP_Exit(EXIT_FAILURE);
	}
}

static void APP_StopSound(APP_Sound* sound)
{
	if (!APP_WasAudioInit) {
		return;
	}

	SDL_UnbindAudioStream(sound->stream);
	sound->pos = 0;
	sound->paused = false;
	SDL_MemoryBarrierRelease();
	SDL_SetAtomicInt(&sound->playing, 0);
	if (!SDL_BindAudioStream(APP_AudioDevice, sound->stream)) {
		APP_Exit(EXIT_FAILURE);
	}
}

static void APP_PauseSound(APP_Sound* sound)
{
	if (!APP_WasAudioInit) {
		return;
	}

	SDL_UnbindAudioStream(sound->stream);
	const int playing = SDL_GetAtomicInt(&sound->playing);
	SDL_MemoryBarrierAcquire();
	if (playing) {
		sound->paused = true;
		SDL_MemoryBarrierRelease();
		SDL_SetAtomicInt(&sound->playing, 0);
	}
	if (!SDL_BindAudioStream(APP_AudioDevice, sound->stream)) {
		APP_Exit(EXIT_FAILURE);
	}
}

static bool APP_IsSoundPlaying(APP_Sound* sound)
{
	if (!APP_WasAudioInit) {
		return false;
	}

	const int playing = SDL_GetAtomicInt(&sound->playing);
	SDL_MemoryBarrierAcquire();
	return !!playing;
}

static void APP_SetSoundVolume(APP_Sound* sound, int vol)
{
	if (!APP_WasAudioInit) {
		return;
	}

	float gain;
	if (vol < 0) {
		gain = 0.0f;
	}
	else if (vol > 100) {
		gain = 1.0f;
	}
	else {
		gain = vol / 100.0f;
	}
	sound->gain = gain;
	if (!SDL_SetAudioStreamGain(sound->stream, gain)) {
		APP_Exit(EXIT_FAILURE);
	}
}

static void APP_SetSoundLooping(APP_Sound* sound, bool looping)
{
	if (!APP_WasAudioInit) {
		return;
	}

	SDL_SetAtomicInt(&sound->looping, !!looping);
}

void APP_LoadWave(int num, const char* filename)
{
	if (num < 0 || num >= APP_WAVES_COUNT) {
		return;
	}

	APP_LoadSound(&APP_Waves[num], filename);
}

static void APP_StartWave(int num, bool replay)
{
	if (!APP_WasAudioInit || num >= APP_WAVES_COUNT) {
		return;
	}
	else if (num >= 0) {
		APP_StartSound(&APP_Waves[num], replay);
	}
	else {
		for (int i = 0; i < APP_WAVES_COUNT; i++) {
			APP_StartSound(&APP_Waves[i], replay);
		}
	}
}
void APP_PlayWave(int num)
{
	APP_StartWave(num, false);
}

void APP_ResumeWave(int num)
{
	APP_StartWave(num, true);
}

void APP_StopWave(int num)
{
	if (!APP_WasAudioInit || num >= APP_WAVES_COUNT) {
		return;
	}
	else if (num >= 0) {
		APP_StopSound(&APP_Waves[num]);
	}
	else {
		for (int i = 0; i < APP_WAVES_COUNT; i++) {
			APP_StopSound(&APP_Waves[i]);
		}
	}
}

void APP_PauseWave(int num)
{
	if (!APP_WasAudioInit || num >= APP_WAVES_COUNT) {
		return;
	}
	else if (num >= 0) {
		APP_PauseSound(&APP_Waves[num]);
	}
	else {
		for (int i = 0; i < APP_WAVES_COUNT; i++) {
			APP_PauseSound(&APP_Waves[i]);
		}
	}
}

bool APP_IsWavePlaying(int num)
{
	if (!APP_WasAudioInit || num >= APP_WAVES_COUNT) {
		return false;
	}
	else if (num >= 0) {
		return APP_IsSoundPlaying(&APP_Waves[num]);
	}
	else {
		for (int i = 0; i < APP_WAVES_COUNT; i++) {
			if (APP_IsSoundPlaying(&APP_Waves[i])) {
				return true;
			}
		}
		return false;
	}
}

void APP_SetWaveVolume(int num, int volume)
{
	if (!APP_WasAudioInit || num >= APP_WAVES_COUNT) {
		return;
	}
	else if (num >= 0) {
		APP_SetSoundVolume(&APP_Waves[num], volume);
	}
	else {
		for (int i = 0; i < APP_WAVES_COUNT; i++) {
			APP_SetSoundVolume(&APP_Waves[i], volume);
		}
	}
}

void APP_SetWaveLooping(int num, bool looping)
{
	if (!APP_WasAudioInit || num >= APP_WAVES_COUNT) {
		return;
	}
	else if (num >= 0) {
		APP_SetSoundLooping(&APP_Waves[num], looping);
	}
	else {
		for (int i = 0; i < APP_WAVES_COUNT; i++) {
			APP_SetSoundLooping(&APP_Waves[i], looping);
		}
	}
}

void APP_LoadMusic(const char* filename)
{
	APP_LoadSound(&APP_Music, filename);
}

void APP_PlayMusic(void)
{
	APP_StartSound(&APP_Music, false);
}

void APP_ResumeMusic(void)
{
	APP_StartSound(&APP_Music, true);
}

void APP_StopMusic(void)
{
	APP_StopSound(&APP_Music);
}

void APP_PauseMusic(void)
{
	APP_PauseSound(&APP_Music);
}

bool APP_IsMusicPlaying(void)
{
	return APP_IsSoundPlaying(&APP_Music);
}

void APP_SetMusicVolume(int volume)
{
	APP_SetSoundVolume(&APP_Music, volume);
}
