#include "APP_audio.h"
#include "APP_filesystem.h"
#include "APP_main.h"
#include "APP_wav.h"
#include "APP_ogg.h"
#include "APP_mp3.h"

typedef struct APP_Sound
{
	SDL_AudioStream* stream;
	uint8_t* leadinData;
	uint8_t* mainData;
	uint32_t leadinSize;
	uint32_t leadinPos;
	uint32_t mainSize;
	uint32_t mainPos;
	bool playing;
	bool looping;
	bool paused;
} APP_Sound;

static bool APP_WasAudioInit;
static SDL_AudioDeviceID APP_AudioDevice;
static SDL_AudioSpec APP_AudioDeviceFormat;
static APP_Sound* APP_Waves;
static int APP_WavesCount;
static APP_Sound APP_Music;

struct {
	const char* ext;
	bool (* load)(SDL_IOStream* file, const SDL_AudioSpec* format, uint8_t** data, uint32_t* size);
} APP_AudioDataLoaders[] = {
	{ ".wav", APP_LoadWAV },
	{ ".ogg", APP_LoadOGG },
	{ ".mp3", APP_LoadMP3 }
};

static void SDLCALL APP_GetAudioStreamData(void* userdata, SDL_AudioStream* stream, int additionalAmount, int totalAmount)
{
	APP_Sound* const sound = userdata;
	(void)totalAmount;

	if (!sound->playing) {
		return;
	}

	if (sound->leadinPos < sound->leadinSize) {
		if (additionalAmount > sound->leadinSize - sound->leadinPos) {
			SDL_PutAudioStreamData(stream, sound->leadinData + sound->leadinPos, sound->leadinSize - sound->leadinPos);
			sound->leadinPos = sound->leadinSize;
			if (!sound->mainData) {
				sound->playing = false;
				return;
			}
			else {
				additionalAmount -= sound->leadinSize - sound->leadinPos;
			}
		}
		else {
			SDL_PutAudioStreamData(stream, sound->leadinData + sound->leadinPos, additionalAmount);
			sound->leadinPos += additionalAmount;
			if (sound->leadinPos == sound->leadinSize && !sound->mainData) {
				sound->playing = false;
			}
			return;
		}
	}

	if (!sound->looping) {
		if (additionalAmount >= sound->mainSize - sound->mainPos) {
			SDL_PutAudioStreamData(stream, sound->mainData + sound->mainPos, sound->mainSize - sound->mainPos);
			sound->mainPos = sound->mainSize;
			sound->playing = false;
		}
		else {
			SDL_PutAudioStreamData(stream, sound->mainData + sound->mainPos, additionalAmount);
			sound->mainPos += additionalAmount;
		}
	}
	else if (additionalAmount >= sound->mainSize - sound->mainPos) {
		int remaining = additionalAmount;
		while (remaining > 0) {
			if (remaining >= sound->mainSize - sound->mainPos) {
				SDL_PutAudioStreamData(stream, sound->mainData + sound->mainPos, sound->mainSize - sound->mainPos);
				remaining -= sound->mainSize - sound->mainPos;
				sound->mainPos = 0;
			}
			else {
				SDL_PutAudioStreamData(stream, sound->mainData + sound->mainPos, remaining);
				sound->mainPos += remaining;
				remaining = 0;
			}
		}
	}
	else {
		SDL_PutAudioStreamData(stream, sound->mainData + sound->mainPos, additionalAmount);
		sound->mainPos += additionalAmount;
	}
}

static bool APP_InitSound(APP_Sound* sound)
{
	sound->stream = SDL_CreateAudioStream(&APP_AudioDeviceFormat, &APP_AudioDeviceFormat);
	return
		sound->stream &&
		SDL_SetAudioStreamGetCallback(sound->stream, APP_GetAudioStreamData, sound) &&
		SDL_BindAudioStream(APP_AudioDevice, sound->stream);
}

bool APP_InitAudio(int wavesCount)
{
	if (APP_WasAudioInit) {
		return true;
	}
	else if (wavesCount == 0 || wavesCount > INT_MAX / sizeof(APP_Sound)) {
		return false;
	}

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
	APP_Waves = SDL_calloc(sizeof(APP_Sound), wavesCount);
	if (!APP_Waves) {
		return false;
	}
	APP_WavesCount = wavesCount;
	for (int i = 0; i < wavesCount; i++) {
		if (!APP_InitSound(&APP_Waves[i])) {
			return false;
		}
	}
	SDL_zero(APP_Music);
	APP_Music.looping = true;
	if (!APP_InitSound(&APP_Music)) {
		return false;
	}

	APP_WasAudioInit = true;
	return true;
}

static void APP_QuitSound(APP_Sound* sound)
{
	SDL_DestroyAudioStream(sound->stream);
	SDL_free(sound->mainData);
	SDL_free(sound->leadinData);
}

void APP_QuitAudio(void)
{
	if (!APP_WasAudioInit) {
		return;
	}

	// サウンドの解放
	// Free sounds
	for (int i = 0; i < APP_WavesCount; i++) {
		APP_QuitSound(&APP_Waves[i]);
	}
	APP_WavesCount = 0;
	SDL_free(APP_Waves);
	APP_Waves = NULL;
	APP_QuitSound(&APP_Music);
	SDL_zero(APP_Music);

	SDL_CloseAudioDevice(APP_AudioDevice);
	APP_AudioDevice = 0;

	APP_WasAudioInit = false;
}

static void APP_LoadSound(APP_Sound* sound, const char* filename_no_ext, bool leadin)
{
	if (!APP_WasAudioInit || !filename_no_ext || SDL_strlen(filename_no_ext) < 5) {
		return;
	}

	bool (* load)(SDL_IOStream*, const SDL_AudioSpec*, uint8_t**, uint32_t*);
	char* filename = NULL;
	for (size_t i = 0; i < SDL_arraysize(APP_AudioDataLoaders); i++) {
		if (SDL_asprintf(&filename, "%s%s", filename_no_ext, APP_AudioDataLoaders[i].ext) < 0) {
			APP_Exit(EXIT_FAILURE);
		}
		if (!APP_FileExists(filename)) {
			SDL_free(filename);
			filename = NULL;
			continue;
		}
		else {
			load = APP_AudioDataLoaders[i].load;
			break;
		}
	}
	if (!filename) {
		return;
	}
	SDL_IOStream* const file = APP_OpenRead(filename);
	SDL_free(filename);
	if (!file) {
		APP_Exit(EXIT_FAILURE);
	}
	uint8_t* oldData;
	uint8_t* data;
	uint32_t size;
	if (!load(file, &APP_AudioDeviceFormat, &data, &size)) {
		APP_Exit(EXIT_FAILURE);
	}
	if (!SDL_LockAudioStream(sound->stream)) {
		APP_Exit(EXIT_FAILURE);
	}
	if (leadin) {
		oldData = sound->leadinData;
		sound->leadinData = data;
		sound->leadinSize = size;
		sound->leadinPos = 0;
	}
	else {
		oldData = sound->mainData;
		sound->mainData = data;
		sound->mainSize = size;
		sound->mainPos = 0;
	}
	sound->paused = false;
	sound->playing = false;
	if (!SDL_UnlockAudioStream(sound->stream)) {
		SDL_free(oldData);
		APP_Exit(EXIT_FAILURE);
	}
	SDL_free(oldData);
}

static void APP_StartSound(APP_Sound* sound, bool resume)
{
	if (!APP_WasAudioInit || (!sound->leadinData && !sound->mainData) || (resume && !sound->paused)) {
		return;
	}

	if (!SDL_LockAudioStream(sound->stream)) {
		APP_Exit(EXIT_FAILURE);
	}
	if (!resume) {
		sound->leadinPos = 0;
		sound->mainPos = 0;
	}
	sound->paused = false;
	sound->playing = true;
	if (!SDL_UnlockAudioStream(sound->stream)) {
		APP_Exit(EXIT_FAILURE);
	}
}

static void APP_StopSound(APP_Sound* sound)
{
	if (!APP_WasAudioInit) {
		return;
	}

	if (!SDL_LockAudioStream(sound->stream)) {
		APP_Exit(EXIT_FAILURE);
	}
	sound->leadinPos = 0;
	sound->mainPos = 0;
	sound->paused = false;
	sound->playing = false;
	if (!SDL_UnlockAudioStream(sound->stream)) {
		APP_Exit(EXIT_FAILURE);
	}
}

static void APP_PauseSound(APP_Sound* sound)
{
	if (!APP_WasAudioInit) {
		return;
	}

	if (!SDL_LockAudioStream(sound->stream)) {
		APP_Exit(EXIT_FAILURE);
	}
	if (sound->playing) {
		sound->paused = true;
	}
	if (!SDL_UnlockAudioStream(sound->stream)) {
		APP_Exit(EXIT_FAILURE);
	}
}

static bool APP_IsSoundPlaying(APP_Sound* sound)
{
	if (!APP_WasAudioInit) {
		return false;
	}

	if (!SDL_LockAudioStream(sound->stream)) {
		APP_Exit(EXIT_FAILURE);
	}
	const bool playing = sound->playing;
	if (!SDL_UnlockAudioStream(sound->stream)) {
		APP_Exit(EXIT_FAILURE);
	}
	return playing;
}

static void APP_SetSoundVolume(APP_Sound* sound, int volume)
{
	if (!APP_WasAudioInit) {
		return;
	}

	const float gain = SDL_clamp(volume, 0, 100) / 100.0f;
	if (!SDL_SetAudioStreamGain(sound->stream, gain)) {
		APP_Exit(EXIT_FAILURE);
	}
}

static void APP_SetSoundLooping(APP_Sound* sound, bool looping)
{
	if (!APP_WasAudioInit) {
		return;
	}

	if (!SDL_LockAudioStream(sound->stream)) {
		APP_Exit(EXIT_FAILURE);
	}
	sound->looping = !!looping;
	if (!SDL_UnlockAudioStream(sound->stream)) {
		APP_Exit(EXIT_FAILURE);
	}
}

void APP_LoadWaveLeadin(int num, const char* filename_no_ext)
{
	if (num < 0 || num >= APP_WavesCount) {
		return;
	}

	APP_SetSoundLooping(&APP_Waves[num], true);
	APP_LoadSound(&APP_Waves[num], filename_no_ext, true);
}

void APP_LoadWave(int num, const char* filename_no_ext)
{
	if (num < 0 || num >= APP_WavesCount) {
		return;
	}

	APP_LoadSound(&APP_Waves[num], filename_no_ext, false);
}

static void APP_StartWave(int num, bool resume)
{
	if (!APP_WasAudioInit || num >= APP_WavesCount) {
		return;
	}
	else if (num >= 0) {
		APP_StartSound(&APP_Waves[num], resume);
	}
	else {
		for (int i = 0; i < APP_WavesCount; i++) {
			APP_StartSound(&APP_Waves[i], resume);
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
	if (!APP_WasAudioInit || num >= APP_WavesCount) {
		return;
	}
	else if (num >= 0) {
		APP_StopSound(&APP_Waves[num]);
	}
	else {
		for (int i = 0; i < APP_WavesCount; i++) {
			APP_StopSound(&APP_Waves[i]);
		}
	}
}

void APP_PauseWave(int num)
{
	if (!APP_WasAudioInit || num >= APP_WavesCount) {
		return;
	}
	else if (num >= 0) {
		APP_PauseSound(&APP_Waves[num]);
	}
	else {
		for (int i = 0; i < APP_WavesCount; i++) {
			APP_PauseSound(&APP_Waves[i]);
		}
	}
}

bool APP_IsWavePlaying(int num)
{
	if (!APP_WasAudioInit || num >= APP_WavesCount) {
		return false;
	}
	else if (num >= 0) {
		return APP_IsSoundPlaying(&APP_Waves[num]);
	}
	else {
		for (int i = 0; i < APP_WavesCount; i++) {
			if (APP_IsSoundPlaying(&APP_Waves[i])) {
				return true;
			}
		}
		return false;
	}
}

void APP_SetWaveVolume(int num, int volume)
{
	if (!APP_WasAudioInit || num >= APP_WavesCount) {
		return;
	}
	else if (num >= 0) {
		APP_SetSoundVolume(&APP_Waves[num], volume);
	}
	else {
		for (int i = 0; i < APP_WavesCount; i++) {
			APP_SetSoundVolume(&APP_Waves[i], volume);
		}
	}
}

void APP_SetWaveLooping(int num, bool looping)
{
	if (!APP_WasAudioInit || num >= APP_WavesCount) {
		return;
	}
	else if (num >= 0) {
		APP_SetSoundLooping(&APP_Waves[num], looping);
	}
	else {
		for (int i = 0; i < APP_WavesCount; i++) {
			APP_SetSoundLooping(&APP_Waves[i], looping);
		}
	}
}

void APP_LoadMusicLeadin(const char* filename_no_ext)
{
	APP_LoadSound(&APP_Music, filename_no_ext, true);
}

void APP_LoadMusic(const char* filename_no_ext)
{
	APP_LoadSound(&APP_Music, filename_no_ext, false);
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
