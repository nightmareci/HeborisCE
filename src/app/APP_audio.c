#include "APP_audio.h"
#include "APP_filesystem.h"
#include "APP_main.h"
#include "APP_wav.h"
#include "APP_ogg.h"
#include "APP_mp3.h"

typedef struct APP_Sound
{
	SDL_AudioStream* stream;
	union {
		struct {
			uint8_t* leadinData;
			uint8_t* mainData;
			uint32_t leadinSize;
			uint32_t leadinPos;
			uint32_t mainSize;
			uint32_t mainPos;
		} preloaded;
		// TODO
		#if 0
		struct {
		} streamed;
		#endif
	};
	bool streaming;
	bool looping;
	bool playing;
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
	bool (* preload)(SDL_IOStream* file, const SDL_AudioSpec* format, uint8_t** data, uint32_t* size);
} APP_AudioDataPreloaders[] = {
	{ ".wav", APP_PreloadWAV },
	{ ".ogg", APP_PreloadOGG },
	{ ".mp3", APP_PreloadMP3 }
};

static void SDLCALL APP_GetPreloadedAudioStreamData(void* userdata, SDL_AudioStream* stream, int additionalAmount, int totalAmount)
{
	APP_Sound* const sound = userdata;
	(void)totalAmount;

	if (!sound->playing) {
		return;
	}

	if (sound->preloaded.leadinPos < sound->preloaded.leadinSize) {
		if (additionalAmount > sound->preloaded.leadinSize - sound->preloaded.leadinPos) {
			SDL_PutAudioStreamData(stream, sound->preloaded.leadinData + sound->preloaded.leadinPos, sound->preloaded.leadinSize - sound->preloaded.leadinPos);
			sound->preloaded.leadinPos = sound->preloaded.leadinSize;
			if (!sound->preloaded.mainData) {
				sound->playing = false;
				return;
			}
			else {
				additionalAmount -= sound->preloaded.leadinSize - sound->preloaded.leadinPos;
			}
		}
		else {
			SDL_PutAudioStreamData(stream, sound->preloaded.leadinData + sound->preloaded.leadinPos, additionalAmount);
			sound->preloaded.leadinPos += additionalAmount;
			if (sound->preloaded.leadinPos == sound->preloaded.leadinSize && !sound->preloaded.mainData) {
				sound->playing = false;
			}
			return;
		}
	}

	if (!sound->looping) {
		if (additionalAmount >= sound->preloaded.mainSize - sound->preloaded.mainPos) {
			SDL_PutAudioStreamData(stream, sound->preloaded.mainData + sound->preloaded.mainPos, sound->preloaded.mainSize - sound->preloaded.mainPos);
			sound->preloaded.mainPos = sound->preloaded.mainSize;
			sound->playing = false;
		}
		else {
			SDL_PutAudioStreamData(stream, sound->preloaded.mainData + sound->preloaded.mainPos, additionalAmount);
			sound->preloaded.mainPos += additionalAmount;
		}
	}
	else if (additionalAmount >= sound->preloaded.mainSize - sound->preloaded.mainPos) {
		int remaining = additionalAmount;
		while (remaining > 0) {
			if (remaining >= sound->preloaded.mainSize - sound->preloaded.mainPos) {
				SDL_PutAudioStreamData(stream, sound->preloaded.mainData + sound->preloaded.mainPos, sound->preloaded.mainSize - sound->preloaded.mainPos);
				remaining -= sound->preloaded.mainSize - sound->preloaded.mainPos;
				sound->preloaded.mainPos = 0;
			}
			else {
				SDL_PutAudioStreamData(stream, sound->preloaded.mainData + sound->preloaded.mainPos, remaining);
				sound->preloaded.mainPos += remaining;
				remaining = 0;
			}
		}
	}
	else {
		SDL_PutAudioStreamData(stream, sound->preloaded.mainData + sound->preloaded.mainPos, additionalAmount);
		sound->preloaded.mainPos += additionalAmount;
	}
}

bool APP_InitAudio(int wavesCount)
{
	if (APP_WasAudioInit) {
		return true;
	}
	else if (wavesCount > INT_MAX / sizeof(APP_Sound)) {
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
	if (wavesCount > 0) {
		APP_Waves = SDL_calloc(sizeof(APP_Sound), wavesCount);
		if (!APP_Waves) {
			return false;
		}
	}
	APP_WavesCount = wavesCount;
	SDL_zero(APP_Music);
	//APP_Music.streaming = true; // TODO
	APP_Music.looping = true;

	APP_WasAudioInit = true;
	return true;
}

static void APP_QuitSound(APP_Sound* sound)
{
	if (!sound->stream) {
		return;
	}
	else if (sound->streaming) {
		// TODO
	}
	else {
		SDL_DestroyAudioStream(sound->stream);
		SDL_free(sound->preloaded.mainData);
		SDL_free(sound->preloaded.leadinData);
	}
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

static bool APP_LoadStreamedSound(APP_Sound* sound, const char* leadin_filename, const char* main_filename)
{
	if (!leadin_filename || !*leadin_filename || !main_filename || !*main_filename) {
		return false;
	}

	if (sound->stream) {
		APP_QuitSound(sound);
		sound->streaming = true;
	}

	// TODO

	return true;
}

static bool APP_LoadPreloadedSound(APP_Sound* sound, const char* filename, bool leadin)
{
	if (!filename || !*filename) {
		return false;
	}

	if (sound->streaming) {
		APP_QuitSound(sound);
		sound->streaming = false;
	}

	bool (* load)(SDL_IOStream*, const SDL_AudioSpec*, uint8_t**, uint32_t*);
	char* filename_ext = NULL;
	for (size_t i = 0; i < SDL_arraysize(APP_AudioDataPreloaders); i++) {
		if (SDL_asprintf(&filename_ext, "%s%s", filename, APP_AudioDataPreloaders[i].ext) < 0) {
			APP_Exit(EXIT_FAILURE);
		}
		if (!APP_FileExists(filename_ext)) {
			SDL_free(filename_ext);
			filename_ext = NULL;
			continue;
		}
		else {
			load = APP_AudioDataPreloaders[i].preload;
			break;
		}
	}
	if (!filename_ext) {
		return false;
	}
	SDL_IOStream* const file = APP_OpenRead(filename_ext);
	SDL_free(filename_ext);
	if (!file) {
		APP_Exit(EXIT_FAILURE);
	}
	uint8_t* oldData;
	uint8_t* data;
	uint32_t size;
	if (!load(file, &APP_AudioDeviceFormat, &data, &size)) {
		APP_Exit(EXIT_FAILURE);
	}
	if (!sound->stream) {
		sound->stream = SDL_CreateAudioStream(&APP_AudioDeviceFormat, &APP_AudioDeviceFormat);
		if (!sound->stream) {
			APP_Exit(EXIT_FAILURE);
		}
		if (!SDL_SetAudioStreamGetCallback(sound->stream, APP_GetPreloadedAudioStreamData, sound)) {
			APP_Exit(EXIT_FAILURE);
		}
		if (!SDL_BindAudioStream(APP_AudioDevice, sound->stream)) {
			APP_Exit(EXIT_FAILURE);
		}
	}
	if (!SDL_LockAudioStream(sound->stream)) {
		APP_Exit(EXIT_FAILURE);
	}
	if (leadin) {
		oldData = sound->preloaded.leadinData;
		sound->preloaded.leadinData = data;
		sound->preloaded.leadinSize = size;
		sound->preloaded.leadinPos = 0;
	}
	else {
		oldData = sound->preloaded.mainData;
		sound->preloaded.mainData = data;
		sound->preloaded.mainSize = size;
		sound->preloaded.mainPos = 0;
	}
	sound->paused = false;
	sound->playing = false;
	if (!SDL_UnlockAudioStream(sound->stream)) {
		SDL_free(oldData);
		APP_Exit(EXIT_FAILURE);
	}
	SDL_free(oldData);
	return true;
}

static void APP_StartPreloadedSound(APP_Sound* sound, bool resume)
{
	if (resume && !sound->paused) {
		return;
	}

	if (!SDL_LockAudioStream(sound->stream)) {
		APP_Exit(EXIT_FAILURE);
	}
	if (!resume) {
		sound->preloaded.leadinPos = 0;
		sound->preloaded.mainPos = 0;
	}
	sound->paused = false;
	sound->playing = true;
	if (!SDL_UnlockAudioStream(sound->stream)) {
		APP_Exit(EXIT_FAILURE);
	}
}

static void APP_StopPreloadedSound(APP_Sound* sound)
{
	if (!SDL_LockAudioStream(sound->stream)) {
		APP_Exit(EXIT_FAILURE);
	}
	sound->preloaded.leadinPos = 0;
	sound->preloaded.mainPos = 0;
	sound->paused = false;
	sound->playing = false;
	if (!SDL_UnlockAudioStream(sound->stream)) {
		APP_Exit(EXIT_FAILURE);
	}
}

static void APP_PauseSound(APP_Sound* sound)
{
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
	const float gain = SDL_clamp(volume, 0, 100) / 100.0f;
	if (!SDL_SetAudioStreamGain(sound->stream, gain)) {
		APP_Exit(EXIT_FAILURE);
	}
}

static void APP_SetSoundLooping(APP_Sound* sound, bool looping)
{
	if (!SDL_LockAudioStream(sound->stream)) {
		APP_Exit(EXIT_FAILURE);
	}
	sound->looping = !!looping;
	if (!SDL_UnlockAudioStream(sound->stream)) {
		APP_Exit(EXIT_FAILURE);
	}
}

void APP_LoadWave(int num, const char* leadin_filename, const char* main_filename, bool looping, bool streaming)
{
	if (num < 0 || num >= APP_WavesCount) {
		return;
	}

	// TODO: Streaming support
	#if 0
	if (streaming) {
		if (APP_LoadStreamedSound(&APP_Waves[num], leadin_filename, main_filename)) {
			APP_SetSoundLooping(&APP_Waves[num], looping);
		}
	}
	else {
	#endif
		if (APP_LoadPreloadedSound(&APP_Waves[num], leadin_filename, true) || APP_LoadPreloadedSound(&APP_Waves[num], main_filename, false)) {
			APP_SetSoundLooping(&APP_Waves[num], looping);
		}
	#if 0
	}
	#endif
}

static void APP_StartWave(int num, bool resume)
{
	if (num >= 0) {
		if (!APP_Waves[num].stream) {
			return;
		}
		if (APP_Waves[num].streaming) {
			// TODO
		}
		else {
			APP_StartPreloadedSound(&APP_Waves[num], resume);
		}
	}
	else {
		for (int i = 0; i < APP_WavesCount; i++) {
			if (!APP_Waves[i].stream) {
				continue;
			}
			if (APP_Waves[i].streaming) {
				// TODO
			}
			else {
				APP_StartPreloadedSound(&APP_Waves[i], resume);
			}
		}
	}
}

void APP_PlayWave(int num)
{
	if (num < APP_WavesCount) {
		APP_StartWave(num, false);
	}
}

void APP_ResumeWave(int num)
{
	if (num < APP_WavesCount) {
		APP_StartWave(num, true);
	}
}

void APP_StopWave(int num)
{
	if (num >= APP_WavesCount) {
		return;
	}
	else if (num >= 0) {
		if (!APP_Waves[num].stream) {
			return;
		}
		if (APP_Waves[num].streaming) {
			// TODO
		}
		else {
			APP_StopPreloadedSound(&APP_Waves[num]);
		}
	}
	else {
		for (int i = 0; i < APP_WavesCount; i++) {
			if (APP_Waves[i].stream) {
				APP_StopPreloadedSound(&APP_Waves[i]);
			}
		}
	}
}

void APP_PauseWave(int num)
{
	if (num >= APP_WavesCount) {
		return;
	}
	else if (num >= 0) {
		if (!APP_Waves[num].stream) {
			return;
		}
		APP_PauseSound(&APP_Waves[num]);
	}
	else {
		for (int i = 0; i < APP_WavesCount; i++) {
			if (APP_Waves[i].stream) {
				APP_PauseSound(&APP_Waves[i]);
			}
		}
	}
}

bool APP_IsWavePlaying(int num)
{
	if (num >= APP_WavesCount) {
		return false;
	}
	else if (num >= 0) {
		return APP_Waves[num].stream != NULL && APP_IsSoundPlaying(&APP_Waves[num]);
	}
	else {
		for (int i = 0; i < APP_WavesCount; i++) {
			if (APP_Waves[i].stream && APP_IsSoundPlaying(&APP_Waves[i])) {
				return true;
			}
		}
		return false;
	}
}

void APP_SetWaveVolume(int num, int volume)
{
	if (num >= APP_WavesCount) {
		return;
	}
	else if (num >= 0) {
		if (APP_Waves[num].stream) {
			APP_SetSoundVolume(&APP_Waves[num], volume);
		}
	}
	else {
		for (int i = 0; i < APP_WavesCount; i++) {
			if (APP_Waves[i].stream) {
				APP_SetSoundVolume(&APP_Waves[i], volume);
			}
		}
	}
}

void APP_SetWaveLooping(int num, bool looping)
{
	if (num >= APP_WavesCount) {
		return;
	}
	else if (num >= 0) {
		if (APP_Waves[num].stream) {
			APP_SetSoundLooping(&APP_Waves[num], looping);
		}
	}
	else {
		for (int i = 0; i < APP_WavesCount; i++) {
			if (APP_Waves[num].stream) {
				APP_SetSoundLooping(&APP_Waves[i], looping);
			}
		}
	}
}

void APP_LoadMusic(const char* leadin_filename, const char* main_filename)
{
	// TODO: Streaming support
	APP_LoadPreloadedSound(&APP_Music, leadin_filename, true);
	APP_LoadPreloadedSound(&APP_Music, main_filename, false);
}

void APP_PlayMusic(void)
{
	// TODO: Streaming support
	if (APP_Music.stream) {
		APP_StartPreloadedSound(&APP_Music, false);
	}
}

void APP_ResumeMusic(void)
{
	// TODO: Streaming support
	if (APP_Music.stream) {
		APP_StartPreloadedSound(&APP_Music, true);
	}
}

void APP_StopMusic(void)
{
	// TODO: Streaming support
	if (APP_Music.stream) {
		APP_StopPreloadedSound(&APP_Music);
	}
}

void APP_PauseMusic(void)
{
	if (APP_Music.stream) {
		APP_PauseSound(&APP_Music);
	}
}

bool APP_IsMusicPlaying(void)
{
	return APP_Music.stream != NULL && APP_IsSoundPlaying(&APP_Music);
}

void APP_SetMusicVolume(int volume)
{
	if (APP_Music.stream) {
		APP_SetSoundVolume(&APP_Music, volume);
	}
}
