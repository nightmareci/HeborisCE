#include "APP_audio.h"
#include "APP_filesystem.h"
#include "APP_main.h"
#include "APP_wav.h"
#include "APP_ogg.h"
#include "APP_mp3.h"
#include "APP_audio_private.h"
#include <SDL3/SDL_audio.h>
#include <SDL3/SDL_mutex.h>

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
		struct {
			APP_StreamingAudioData* leadinData;
			APP_StreamingAudioData* mainData;
			SDL_Semaphore* wakeStreamer;
			SDL_Thread* streamer;
			uint8_t* chunks;
			SDL_AtomicInt chunkID;
			SDL_AtomicInt ready[2];
			SDL_AtomicInt quit;
			int chunkEnd[2];
			int pos;
			bool lastChunk[2];
			bool leadinFinished;
			bool mainFinished;
		} streamed;
	};
	bool loaded;
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

struct
{
	const char* ext;
	APP_PreloadAudioDataFunction preload;
} APP_AudioDataPreloaders[] =
{
	{ ".wav", APP_PreloadWAV },
	{ ".ogg", APP_PreloadOGG },
	{ ".mp3", APP_PreloadMP3 }
};

struct
{
	const char* ext;
	APP_CreateStreamingAudioDataFunction create;
} APP_StreamingAudioDataCreators[] =
{
	{ ".ogg", APP_CreateStreamingOGGAudioData }
};

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
		for (int i = 0; i < wavesCount; i++) {
			APP_Waves[i].stream = SDL_CreateAudioStream(&APP_AudioDeviceFormat, &APP_AudioDeviceFormat);
			if (!APP_Waves[i].stream) {
				fprintf(stderr, "Couldn't create audio stream for wave: %s\n", SDL_GetError());
				return false;
			}
		}
	}
	APP_WavesCount = wavesCount;
	SDL_zero(APP_Music);
	APP_Music.stream = SDL_CreateAudioStream(&APP_AudioDeviceFormat, &APP_AudioDeviceFormat);
	if (!APP_Music.stream) {
		fprintf(stderr, "Couldn't create audio stream for music: %s\n", SDL_GetError());
		return false;
	}
	APP_Music.streaming = true;
	APP_Music.looping = true;

	APP_WasAudioInit = true;
	return true;
}

static void APP_QuitSound(APP_Sound* sound)
{
	SDL_UnbindAudioStream(sound->stream);
	if (!sound->loaded) {
		return;
	}
	if (sound->streaming) {
		SDL_MemoryBarrierRelease();
		SDL_SetAtomicInt(&sound->streamed.quit, 1);
		SDL_SignalSemaphore(sound->streamed.wakeStreamer);
		SDL_WaitThread(sound->streamed.streamer, NULL);
		SDL_DestroySemaphore(sound->streamed.wakeStreamer);
		if (sound->streamed.leadinData) {
			APP_DestroyStreamingAudioData(sound->streamed.leadinData);
		}
		if (sound->streamed.mainData) {
			APP_DestroyStreamingAudioData(sound->streamed.mainData);
		}
		SDL_free(sound->streamed.chunks);
	}
	else {
		SDL_free(sound->preloaded.mainData);
		SDL_free(sound->preloaded.leadinData);
	}
	SDL_zero(sound->streamed);
	sound->loaded = false;
	sound->streaming = false;
	sound->looping = false;
	sound->playing = false;
	sound->paused = false;
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
		SDL_DestroyAudioStream(APP_Waves[i].stream);
		APP_Waves[i].stream = NULL;
	}
	APP_WavesCount = 0;
	SDL_free(APP_Waves);
	APP_Waves = NULL;
	APP_QuitSound(&APP_Music);
	SDL_DestroyAudioStream(APP_Music.stream);
	APP_Music.stream = NULL;

	SDL_CloseAudioDevice(APP_AudioDevice);
	APP_AudioDevice = 0;

	APP_WasAudioInit = false;
}

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

static bool APP_LoadPreloadedSound(APP_Sound* sound, const char* filename, bool leadin)
{
	if (!filename || !*filename) {
		return false;
	}

	APP_QuitSound(sound);
	sound->streaming = false;

	if (!SDL_SetAudioStreamGetCallback(sound->stream, APP_GetPreloadedAudioStreamData, sound)) {
		APP_Exit(EXIT_FAILURE);
	}

	bool (* load)(SDL_IOStream*, const SDL_AudioSpec*, uint8_t**, uint32_t*) = NULL;
	char* filenameExt = NULL;
	for (size_t i = 0; i < SDL_arraysize(APP_AudioDataPreloaders); i++) {
		if (SDL_asprintf(&filenameExt, "%s%s", filename, APP_AudioDataPreloaders[i].ext) < 0) {
			APP_Exit(EXIT_FAILURE);
		}
		if (!APP_FileExists(filenameExt)) {
			SDL_free(filenameExt);
			filenameExt = NULL;
			continue;
		}
		else {
			load = APP_AudioDataPreloaders[i].preload;
			break;
		}
	}
	if (!load) {
		return false;
	}
	SDL_IOStream* const file = APP_OpenRead(filenameExt);
	SDL_free(filenameExt);
	if (!file) {
		APP_Exit(EXIT_FAILURE);
	}
	uint8_t* oldData;
	uint8_t* data;
	uint32_t size;
	if (!load(file, &APP_AudioDeviceFormat, &data, &size)) {
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
	SDL_free(oldData);
	sound->loaded = true;
	if (!SDL_BindAudioStream(APP_AudioDevice, sound->stream)) {
		APP_Exit(EXIT_FAILURE);
	}
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

static APP_CreateStreamingAudioDataFunction APP_GetCreateStreamingAudioDataFunction(const char* filename, char** filenameExt)
{
	*filenameExt = NULL;
	if (!filename || !*filename) {
		return NULL;
	}
	for (size_t i = 0; i < SDL_arraysize(APP_StreamingAudioDataCreators); i++) {
		if (SDL_asprintf(filenameExt, "%s%s", filename, APP_StreamingAudioDataCreators[i].ext) < 0) {
			APP_Exit(EXIT_FAILURE);
		}
		if (APP_FileExists(*filenameExt)) {
			return APP_StreamingAudioDataCreators[i].create;
		}
		SDL_free(*filenameExt);
	}
	return NULL;
}

static int SDLCALL APP_StreamAudioData(void* userdata)
{
	APP_Sound* const sound = userdata;
	while (true) {
		SDL_WaitSemaphore(sound->streamed.wakeStreamer);
		const int quit = SDL_GetAtomicInt(&sound->streamed.quit);
		SDL_MemoryBarrierAcquire();
		if (quit) {
			return 1;
		}

		const int chunkID = !SDL_GetAtomicInt(&sound->streamed.chunkID);
		SDL_MemoryBarrierAcquire();
		uint8_t* const chunk = &sound->streamed.chunks[chunkID * APP_STREAMED_AUDIO_CHUNK_SIZE];
		int* const chunkEnd = &sound->streamed.chunkEnd[chunkID];
		*chunkEnd = 0;
		bool* const lastChunk = &sound->streamed.lastChunk[chunkID];
		*lastChunk = true;
		if (!SDL_LockAudioStream(sound->stream)) {
			abort();
		}
		const bool looping = sound->looping;
		if (!SDL_UnlockAudioStream(sound->stream)) {
			abort();
		}
		if (sound->streamed.leadinData) {
			if (!sound->streamed.leadinFinished) {
				if (!APP_GetStreamingAudioDataChunk(sound->streamed.leadinData, chunk, APP_STREAMED_AUDIO_CHUNK_SIZE, chunkEnd, &sound->streamed.leadinFinished, false)) {
					abort();
				}
				*lastChunk = !sound->streamed.mainData && sound->streamed.leadinFinished;
			}
		}
		else {
			sound->streamed.leadinFinished = true;
		}
		if (sound->streamed.mainData && sound->streamed.leadinFinished && (!sound->streamed.mainFinished || sound->looping)) {
			int mainSize;
			if (!APP_GetStreamingAudioDataChunk(sound->streamed.mainData, chunk + *chunkEnd, APP_STREAMED_AUDIO_CHUNK_SIZE - *chunkEnd, &mainSize, &sound->streamed.mainFinished, looping)) {
				abort();
			}
			*chunkEnd += mainSize;
			*lastChunk = sound->streamed.mainFinished && !sound->looping;
		}

		SDL_MemoryBarrierRelease();
		SDL_SetAtomicInt(&sound->streamed.ready[chunkID], 1);
	}
}

static void SDLCALL APP_GetStreamedAudioStreamData(void* userdata, SDL_AudioStream* stream, int additionalAmount, int totalAmount)
{
	APP_Sound* const sound = userdata;
	(void)totalAmount;

	if (!sound->playing) {
		return;
	}

	int chunkID = SDL_GetAtomicInt(&sound->streamed.chunkID);
	int ready = SDL_GetAtomicInt(&sound->streamed.ready[chunkID]);
	SDL_MemoryBarrierAcquire();
	if (!ready) {
		chunkID = !chunkID;
		ready = SDL_GetAtomicInt(&sound->streamed.ready[chunkID]);
		SDL_MemoryBarrierAcquire();
		if (!ready) {
			return;
		}
	}
	int chunkEnd = sound->streamed.chunkEnd[chunkID];
	bool lastChunk = sound->streamed.lastChunk[chunkID];
	uint8_t* chunk = &sound->streamed.chunks[chunkID * APP_STREAMED_AUDIO_CHUNK_SIZE];
	if (sound->streamed.pos + additionalAmount <= chunkEnd) {
		SDL_PutAudioStreamData(stream, chunk + sound->streamed.pos, additionalAmount);
		sound->streamed.pos += additionalAmount;
		if (sound->streamed.pos == chunkEnd && lastChunk) {
			sound->playing = false;
		}
	}
	else {
		SDL_PutAudioStreamData(stream, chunk + sound->streamed.pos, chunkEnd - sound->streamed.pos);
		additionalAmount -= chunkEnd - sound->streamed.pos;
		ready = SDL_GetAtomicInt(&sound->streamed.ready[!chunkID]);
		SDL_MemoryBarrierAcquire();
		if (ready) {
			SDL_MemoryBarrierRelease();
			SDL_SetAtomicInt(&sound->streamed.ready[chunkID], 0);
			chunkID = !chunkID;
			SDL_MemoryBarrierRelease();
			SDL_SetAtomicInt(&sound->streamed.chunkID, chunkID);
			chunkEnd = sound->streamed.chunkEnd[chunkID];
			lastChunk = sound->streamed.lastChunk[chunkID];
			chunk = &sound->streamed.chunks[chunkID * APP_STREAMED_AUDIO_CHUNK_SIZE];
			if (additionalAmount <= chunkEnd) {
				SDL_PutAudioStreamData(stream, chunk, additionalAmount);
				sound->streamed.pos = additionalAmount;
			}
			else {
				SDL_PutAudioStreamData(stream, chunk, chunkEnd);
				sound->streamed.pos = chunkEnd;
			}
			if (sound->streamed.pos == chunkEnd && lastChunk) {
				sound->playing = false;
			}
		}
		if (!lastChunk) {
			SDL_SignalSemaphore(sound->streamed.wakeStreamer);
		}
	}
}

static bool APP_LoadStreamedSound(APP_Sound* sound, const char* leadinFilename, const char* mainFilename, bool looping)
{
	if ((!leadinFilename || !*leadinFilename) && (!mainFilename || !*mainFilename)) {
		return false;
	}

	APP_QuitSound(sound);
	sound->streaming = true;

	if (!SDL_SetAudioStreamGetCallback(sound->stream, APP_GetStreamedAudioStreamData, sound)) {
		APP_Exit(EXIT_FAILURE);
	}
	sound->looping = looping;

	char* filenameExt;
	APP_CreateStreamingAudioDataFunction create;
	SDL_IOStream* file;

	create = APP_GetCreateStreamingAudioDataFunction(leadinFilename, &filenameExt);
	if (create) {
		file = APP_OpenRead(filenameExt);
		if (!file) {
			SDL_free(filenameExt);
			APP_Exit(EXIT_FAILURE);
		}
		SDL_free(filenameExt);
		sound->streamed.leadinData = create(file, &APP_AudioDeviceFormat);
		if (!sound->streamed.leadinData) {
			APP_Exit(EXIT_FAILURE);
		}
	}

	create = APP_GetCreateStreamingAudioDataFunction(mainFilename, &filenameExt);
	if (create) {
		file = APP_OpenRead(filenameExt);
		SDL_free(filenameExt);
		if (!file) {
			APP_Exit(EXIT_FAILURE);
		}
		sound->streamed.mainData = create(file, &APP_AudioDeviceFormat);
		if (!sound->streamed.mainData) {
			APP_Exit(EXIT_FAILURE);
		}
	}

	sound->streamed.chunks = SDL_malloc(APP_STREAMED_AUDIO_CHUNK_SIZE * 2);
	if (!sound->streamed.chunks) {
		APP_Exit(EXIT_FAILURE);
	}
	sound->streamed.wakeStreamer = SDL_CreateSemaphore(0);
	if (!sound->streamed.wakeStreamer) {
		APP_Exit(EXIT_FAILURE);
	}
	sound->streamed.streamer = SDL_CreateThread(APP_StreamAudioData, "APP_StreamAudioData", sound);
	if (!sound->streamed.streamer) {
		APP_Exit(EXIT_FAILURE);
	}
	if (!SDL_BindAudioStream(APP_AudioDevice, sound->stream)) {
		APP_Exit(EXIT_FAILURE);
	}
	sound->loaded = true;
	return true;
}

static void APP_ReadyStreamedSound(APP_Sound* sound)
{
	sound->streamed.pos = 0;
	sound->streamed.chunkEnd[0] = 0;
	sound->streamed.lastChunk[0] = false;
	if (sound->streamed.leadinData) {
		if (!APP_RestartStreamingAudioData(sound->streamed.leadinData)) {
			abort();
		}
		if (!APP_GetStreamingAudioDataChunk(sound->streamed.leadinData, sound->streamed.chunks, APP_STREAMED_AUDIO_CHUNK_SIZE, &sound->streamed.chunkEnd[0], &sound->streamed.leadinFinished, false)) {
			abort();
		}
		sound->streamed.lastChunk[0] = !sound->streamed.mainData && sound->streamed.leadinFinished;
	}
	else {
		sound->streamed.leadinFinished = true;
	}
	if (sound->streamed.mainData) {
		if (!APP_RestartStreamingAudioData(sound->streamed.mainData)) {
			abort();
		}
		if (sound->streamed.leadinFinished) {
			int mainSize;
			if (!APP_GetStreamingAudioDataChunk(sound->streamed.mainData, sound->streamed.chunks + sound->streamed.chunkEnd[0], APP_STREAMED_AUDIO_CHUNK_SIZE - sound->streamed.chunkEnd[0], &mainSize, &sound->streamed.mainFinished, sound->looping)) {
				abort();
			}
			sound->streamed.chunkEnd[0] += mainSize;
			sound->streamed.lastChunk[0] = sound->streamed.mainFinished && !sound->looping;
		}
	}
	SDL_MemoryBarrierRelease();
	SDL_SetAtomicInt(&sound->streamed.ready[0], 1);

	if (sound->streamed.leadinData && !sound->streamed.leadinFinished) {
		if (!APP_GetStreamingAudioDataChunk(sound->streamed.leadinData, sound->streamed.chunks + APP_STREAMED_AUDIO_CHUNK_SIZE, APP_STREAMED_AUDIO_CHUNK_SIZE, &sound->streamed.chunkEnd[1], &sound->streamed.leadinFinished, false)) {
			abort();
		}
		sound->streamed.lastChunk[1] = !sound->streamed.mainData && sound->streamed.leadinFinished;
	}
	else {
		sound->streamed.leadinFinished = true;
	}
	if (sound->streamed.mainData) {
		if (sound->streamed.leadinFinished) {
			int mainSize;
			if (!APP_GetStreamingAudioDataChunk(sound->streamed.mainData, sound->streamed.chunks + APP_STREAMED_AUDIO_CHUNK_SIZE + sound->streamed.chunkEnd[1], APP_STREAMED_AUDIO_CHUNK_SIZE - sound->streamed.chunkEnd[1], &mainSize, &sound->streamed.mainFinished, sound->looping)) {
				abort();
			}
			sound->streamed.chunkEnd[1] += mainSize;
			sound->streamed.lastChunk[1] = sound->streamed.mainFinished && !sound->looping;
		}
	}
	SDL_MemoryBarrierRelease();
	SDL_SetAtomicInt(&sound->streamed.ready[1], 1);

	SDL_MemoryBarrierRelease();
	SDL_SetAtomicInt(&sound->streamed.chunkID, 0);
}

static void APP_StartStreamedSound(APP_Sound* sound, bool resume)
{
	if (resume && !sound->paused) {
		return;
	}

	if (!SDL_LockAudioStream(sound->stream)) {
		APP_Exit(EXIT_FAILURE);
	}
	if (!resume) {
		APP_ReadyStreamedSound(sound);
	}
	sound->paused = false;
	sound->playing = true;
	if (!SDL_UnlockAudioStream(sound->stream)) {
		APP_Exit(EXIT_FAILURE);
	}
}

static void APP_StopStreamedSound(APP_Sound* sound)
{
	if (!SDL_LockAudioStream(sound->stream)) {
		APP_Exit(EXIT_FAILURE);
	}
	APP_ReadyStreamedSound(sound);
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
		sound->playing = false;
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

void APP_LoadWave(int num, const char* leadinFilename, const char* mainFilename, bool looping, bool streaming)
{
	if (num < 0 || num >= APP_WavesCount) {
		return;
	}

	if (streaming) {
		APP_LoadStreamedSound(&APP_Waves[num], leadinFilename, mainFilename, looping);
	}
	else if (APP_LoadPreloadedSound(&APP_Waves[num], leadinFilename, true) || APP_LoadPreloadedSound(&APP_Waves[num], mainFilename, false)) {
		APP_SetSoundLooping(&APP_Waves[num], looping);
	}
}

static void APP_StartWave(int num, bool resume)
{
	if (num >= 0) {
		if (!APP_Waves[num].loaded) {
			return;
		}
		if (APP_Waves[num].streaming) {
			APP_StartStreamedSound(&APP_Waves[num], resume);
		}
		else {
			APP_StartPreloadedSound(&APP_Waves[num], resume);
		}
	}
	else {
		for (int i = 0; i < APP_WavesCount; i++) {
			if (!APP_Waves[i].loaded) {
				continue;
			}
			if (APP_Waves[i].streaming) {
				APP_StartStreamedSound(&APP_Waves[i], resume);
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
		if (!APP_Waves[num].loaded) {
			return;
		}
		if (APP_Waves[num].streaming) {
			APP_StopStreamedSound(&APP_Waves[num]);
		}
		else {
			APP_StopPreloadedSound(&APP_Waves[num]);
		}
	}
	else {
		for (int i = 0; i < APP_WavesCount; i++) {
			if (!APP_Waves[i].loaded) {
				continue;
			}
			if (APP_Waves[i].streaming) {
				APP_StopStreamedSound(&APP_Waves[i]);
			}
			else {
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
		if (!APP_Waves[num].loaded) {
			return;
		}
		APP_PauseSound(&APP_Waves[num]);
	}
	else {
		for (int i = 0; i < APP_WavesCount; i++) {
			if (APP_Waves[i].loaded) {
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
		return APP_Waves[num].loaded && APP_IsSoundPlaying(&APP_Waves[num]);
	}
	else {
		for (int i = 0; i < APP_WavesCount; i++) {
			if (APP_Waves[i].loaded && APP_IsSoundPlaying(&APP_Waves[i])) {
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
		if (APP_Waves[num].loaded) {
			APP_SetSoundVolume(&APP_Waves[num], volume);
		}
	}
	else {
		for (int i = 0; i < APP_WavesCount; i++) {
			if (APP_Waves[i].loaded) {
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
		if (APP_Waves[num].loaded) {
			APP_SetSoundLooping(&APP_Waves[num], looping);
		}
	}
	else {
		for (int i = 0; i < APP_WavesCount; i++) {
			if (APP_Waves[num].loaded) {
				APP_SetSoundLooping(&APP_Waves[i], looping);
			}
		}
	}
}

void APP_LoadMusic(const char* leadinFilename, const char* mainFilename)
{
	APP_LoadStreamedSound(&APP_Music, leadinFilename, mainFilename, true);
}

void APP_PlayMusic(void)
{
	if (APP_Music.loaded) {
		APP_StartStreamedSound(&APP_Music, false);
	}
}

void APP_ResumeMusic(void)
{
	if (APP_Music.loaded) {
		APP_StartStreamedSound(&APP_Music, true);
	}
}

void APP_StopMusic(void)
{
	if (APP_Music.loaded) {
		APP_StopStreamedSound(&APP_Music);
	}
}

void APP_PauseMusic(void)
{
	if (APP_Music.loaded) {
		APP_PauseSound(&APP_Music);
	}
}

bool APP_IsMusicPlaying(void)
{
	return APP_Music.loaded && APP_IsSoundPlaying(&APP_Music);
}

void APP_SetMusicVolume(int volume)
{
	if (APP_Music.loaded) {
		APP_SetSoundVolume(&APP_Music, volume);
	}
}
