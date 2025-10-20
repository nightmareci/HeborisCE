#include "APP_audio.h"
#include "APP_filesystem.h"
#include "APP_main.h"

typedef struct APP_Sound
{
	SDL_AudioStream* stream;
	uint8_t* leadinData;
	uint8_t* mainData;
	SDL_AtomicInt playing;
	SDL_AtomicInt looping;
	float gain;
	uint32_t leadinSize;
	uint32_t leadinPos;
	uint32_t mainSize;
	uint32_t mainPos;
	bool paused;
} APP_Sound;

static const char* APP_SoundFormatsSupported[] =
{
	".wav"
};

static bool APP_WasAudioInit;
static SDL_AudioDeviceID APP_AudioDevice;
static SDL_AudioSpec APP_AudioDeviceFormat;
static APP_Sound* APP_Waves;
static int APP_WavesCount;
static APP_Sound APP_Music;

static void SDLCALL APP_GetAudioStreamData(void* userdata, SDL_AudioStream* stream, int additionalAmount, int totalAmount)
{
	APP_Sound* const wave = userdata;
	(void)totalAmount;

	if (additionalAmount == 0) {
		return;
	}

	const int playing = SDL_GetAtomicInt(&wave->playing);
	SDL_MemoryBarrierAcquire();
	if (!playing) {
		return;
	}

	if (wave->leadinPos < wave->leadinSize) {
		if (additionalAmount > wave->leadinSize - wave->leadinPos) {
			SDL_PutAudioStreamData(stream, wave->leadinData + wave->leadinPos, wave->leadinSize - wave->leadinPos);
			wave->leadinPos = wave->leadinSize;
			if (!wave->mainData) {
				SDL_MemoryBarrierRelease();
				SDL_SetAtomicInt(&wave->playing, 0);
				return;
			}
			else {
				additionalAmount -= wave->leadinSize - wave->leadinPos;
			}
		}
		else {
			SDL_PutAudioStreamData(stream, wave->leadinData + wave->leadinPos, additionalAmount);
			wave->leadinPos += additionalAmount;
			if (wave->leadinPos == wave->leadinSize && !wave->mainData) {
				SDL_MemoryBarrierRelease();
				SDL_SetAtomicInt(&wave->playing, 0);
			}
			return;
		}
	}

	if (!SDL_GetAtomicInt(&wave->looping)) {
		if (additionalAmount >= wave->mainSize - wave->mainPos) {
			SDL_PutAudioStreamData(stream, wave->mainData + wave->mainPos, wave->mainSize - wave->mainPos);
			wave->mainPos = wave->mainSize;
			SDL_MemoryBarrierRelease();
			SDL_SetAtomicInt(&wave->playing, 0);
		}
		else {
			SDL_PutAudioStreamData(stream, wave->mainData + wave->mainPos, additionalAmount);
			wave->mainPos += additionalAmount;
		}
	}
	else if (additionalAmount >= wave->mainSize - wave->mainPos) {
		int remaining = additionalAmount;
		while (remaining > 0) {
			if (remaining >= wave->mainSize - wave->mainPos) {
				SDL_PutAudioStreamData(stream, wave->mainData + wave->mainPos, wave->mainSize - wave->mainPos);
				remaining -= wave->mainSize - wave->mainPos;
				wave->mainPos = 0;
			}
			else {
				SDL_PutAudioStreamData(stream, wave->mainData + wave->mainPos, remaining);
				wave->mainPos += remaining;
				remaining = 0;
			}
		}
	}
	else {
		SDL_PutAudioStreamData(stream, wave->mainData + wave->mainPos, additionalAmount);
		wave->mainPos += additionalAmount;
	}
}

static bool APP_InitSound(APP_Sound* sound)
{
	sound->gain = 1.0f;
	SDL_MemoryBarrierRelease();
	SDL_SetAtomicInt(&sound->playing, 0);
	sound->stream = SDL_CreateAudioStream(&APP_AudioDeviceFormat, &APP_AudioDeviceFormat);
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

	bool found = false;
	char* filename = NULL;
	for (size_t i = 0; i < SDL_arraysize(APP_SoundFormatsSupported); i++) {
		if (APP_SoundFormatsSupported[i]) {
			if (SDL_asprintf(&filename, "%s%s", filename_no_ext, APP_SoundFormatsSupported[i]) < 0) {
				APP_Exit(EXIT_FAILURE);
			}
			if (APP_FileExists(filename)) {
				found = true;
				break;
			}
			SDL_free(filename);
			filename = NULL;
		}
	}
	if (!found) {
		SDL_free(filename);
		return;
	}

	SDL_UnbindAudioStream(sound->stream);

	SDL_IOStream* const file = APP_OpenRead(filename);
	SDL_free(filename);
	if (!file) {
		APP_Exit(EXIT_FAILURE);
	}
	uint8_t** data = &sound->mainData;
	uint32_t* size = &sound->mainSize;
	uint32_t* pos = &sound->mainPos;
	if (leadin) {
		data = &sound->leadinData;
		size = &sound->leadinSize;
		pos = &sound->leadinPos;
	}
	if (*data) {
		SDL_free(*data);
		*data = NULL;
	}
	SDL_AudioSpec waveFormat;
	if (!SDL_LoadWAV_IO(file, true, &waveFormat, data, size)) {
		APP_Exit(EXIT_FAILURE);
	}
	if (*size > INT_MAX) {
		SDL_free(*data);
		*data = NULL;
		APP_Exit(EXIT_FAILURE);
	}
	if (
		waveFormat.format != APP_AudioDeviceFormat.format ||
		waveFormat.channels != APP_AudioDeviceFormat.channels ||
		waveFormat.freq != APP_AudioDeviceFormat.freq
	) {
		uint8_t* convertedData;
		int convertedSize;
		if (!SDL_ConvertAudioSamples(&waveFormat, *data, (int)*size, &APP_AudioDeviceFormat, &convertedData, &convertedSize)) {
			SDL_free(*data);
			APP_Exit(EXIT_FAILURE);
		}
		SDL_free(*data);
		*data = convertedData;
		*size = convertedSize;
	}

	*pos = 0;
	sound->paused = false;
	SDL_MemoryBarrierRelease();
	SDL_SetAtomicInt(&sound->playing, 0);
	if (!SDL_BindAudioStream(APP_AudioDevice, sound->stream)) {
		APP_Exit(EXIT_FAILURE);
	}
}

static void APP_StartSound(APP_Sound* sound, bool resume)
{
	if (!APP_WasAudioInit || (!sound->leadinData && !sound->mainData) || (resume && !sound->paused)) {
		return;
	}
	SDL_UnbindAudioStream(sound->stream);
	if (!resume) {
		sound->leadinPos = 0;
		sound->mainPos = 0;
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
	sound->leadinPos = 0;
	sound->mainPos = 0;
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

static void APP_SetSoundVolume(APP_Sound* sound, int volume)
{
	if (!APP_WasAudioInit) {
		return;
	}

	float gain;
	if (volume < 0) {
		gain = 0.0f;
	}
	else if (volume > 100) {
		gain = 1.0f;
	}
	else {
		gain = volume / 100.0f;
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
