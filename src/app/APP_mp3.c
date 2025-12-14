#include "APP_mp3.h"
#include "app/APP_audio_private.h"
#include <SDL3/SDL_audio.h>

#define DR_MP3_IMPLEMENTATION
#define DRMP3_API static
#define DRMP3_PRIVATE static
#define DR_MP3_FLOAT_OUTPUT
#define DR_MP3_NO_STDIO
#define DRMP3_ASSERT(expression) SDL_assert((expression))
#define DRMP3_COPY_MEMORY(dst, src, sz) SDL_memcpy((dst), (src), (sz))
#define DRMP3_MOVE_MEMORY(dst, src, sz) SDL_memmove((dst), (src), (sz))
#define DRMP3_ZERO_MEMORY(p, sz) SDL_memset((p), 0, (sz))
#define DRMP3_MALLOC(sz) SDL_malloc((sz))
#define DRMP3_REALLOC(p, sz) SDL_realloc((p), (sz))
#define DRMP3_FREE(p) SDL_free((p))
#include "dr_mp3.h"

typedef struct APP_StreamingMP3AudioData
{
	APP_StreamingAudioData callbacks;
	drmp3 data;
	uint64_t frames;
	uint64_t framesMax;
	SDL_Mutex* lock;
	SDL_AudioStream* converter;
	SDL_IOStream* file;
	float* buffer;
	SDL_AudioSpec srcSpec;
	SDL_AudioSpec dstSpec;
	bool finished;
} APP_StreamingMP3AudioData;

static bool APP_GetMP3StreamingAudioDataChunk(APP_StreamingAudioData* streamingAudioData, uint8_t* chunk, int chunkTotalSize, int* chunkOutSize, bool* finished, bool looping);
static bool APP_RestartMP3StreamingAudioData(APP_StreamingAudioData* streamingAudioData);
static void APP_DestroyMP3StreamingAudioData(APP_StreamingAudioData* streamingAudioData);

static size_t APP_DRMP3_Read(void* pUserData, void* pBufferOut, size_t bytesToRead)
{
	return SDL_ReadIO((SDL_IOStream*)pUserData, pBufferOut, bytesToRead);
}

static drmp3_bool32 APP_DRMP3_Seek(void* pUserData, int offset, drmp3_seek_origin origin)
{
	SDL_IOStream* const file = pUserData;
	switch (origin) {
	default:
	case DRMP3_SEEK_SET:
		return SDL_SeekIO(file, offset, SDL_IO_SEEK_SET) >= 0;

	case DRMP3_SEEK_CUR:
		return SDL_SeekIO(file, offset, SDL_IO_SEEK_CUR) >= 0;

	case DRMP3_SEEK_END:
		return SDL_SeekIO(file, offset, SDL_IO_SEEK_END) >= 0;
	}
}

static drmp3_bool32 APP_DRMP3_Tell(void* pUserData, drmp3_int64* pCursor)
{
	*pCursor = SDL_TellIO((SDL_IOStream*)pUserData);
	return *pCursor >= 0;
}

static void* APP_DRMP3_Malloc(size_t sz, void* pUserData)
{
	(void)pUserData;
	return SDL_malloc(sz);
}

static void* APP_DRMP3_Realloc(void* p, size_t sz, void* pUserData)
{
	(void)pUserData;
	return SDL_realloc(p, sz);
}

static void APP_DRMP3_Free(void* p, void* pUserData)
{
	(void)pUserData;
	SDL_free(p);
}

bool APP_PreloadMP3(SDL_IOStream* file, const SDL_AudioSpec* format, uint8_t** data, uint32_t* size)
{
	drmp3_config config;
	drmp3_uint64 totalFrameCount;
	const drmp3_allocation_callbacks allocationCallbacks = { NULL, APP_DRMP3_Malloc, APP_DRMP3_Realloc, APP_DRMP3_Free };
	float** const floatData = (float**)data;
	*floatData = drmp3_open_and_read_pcm_frames_f32(APP_DRMP3_Read, APP_DRMP3_Seek, APP_DRMP3_Tell, file, &config, &totalFrameCount, &allocationCallbacks);
	SDL_CloseIO(file);
	if (!*data) {
		return false;
	}
	*size = (uint32_t)(sizeof(float) * config.channels * totalFrameCount);
	const SDL_AudioSpec mp3Format = { SDL_AUDIO_F32, config.channels, config.sampleRate };
	if (
		mp3Format.format != format->format ||
		mp3Format.channels != format->channels ||
		mp3Format.freq != format->freq
	) {
		if (totalFrameCount > INT_MAX / (sizeof(float) * mp3Format.channels)) {
			SDL_free(*data);
			*data = NULL;
			return false;
		}
		uint8_t* convertedData;
		int convertedSize;
		if (!SDL_ConvertAudioSamples(&mp3Format, *data, (int)*size, format, &convertedData, &convertedSize)) {
			SDL_free(*data);
			*data = NULL;
			return false;
		}
		SDL_free(*data);
		*data = convertedData;
		*size = convertedSize;
	}
	else if (totalFrameCount > UINT32_MAX / (sizeof(float) * mp3Format.channels)) {
		SDL_free(*data);
		*data = NULL;
		return false;
	}
	return true;
}

APP_StreamingAudioData* APP_CreateStreamingMP3AudioData(SDL_IOStream* file, SDL_AudioSpec* dstSpec)
{
	APP_StreamingMP3AudioData* mp3 = SDL_calloc(1, sizeof(APP_StreamingMP3AudioData));
	if (!mp3) {
		SDL_CloseIO(file);
		return NULL;
	}
	mp3->callbacks.getChunk = APP_GetMP3StreamingAudioDataChunk;
	mp3->callbacks.restart = APP_RestartMP3StreamingAudioData;
	mp3->callbacks.destroy = APP_DestroyMP3StreamingAudioData;

	mp3->lock = SDL_CreateMutex();
	if (!mp3->lock) {
		SDL_free(mp3);
		SDL_CloseIO(file);
		return NULL;
	}

	const drmp3_allocation_callbacks allocationCallbacks = { NULL, APP_DRMP3_Malloc, APP_DRMP3_Realloc, APP_DRMP3_Free };
	if (!drmp3_init(&mp3->data, APP_DRMP3_Read, APP_DRMP3_Seek, APP_DRMP3_Tell, NULL, file, &allocationCallbacks)) {
		SDL_DestroyMutex(mp3->lock);
		SDL_free(mp3);
		SDL_CloseIO(file);
		return NULL;
	}
	mp3->frames = drmp3_get_pcm_frame_count(&mp3->data);
	if (mp3->frames == 0) {
		drmp3_uninit(&mp3->data);
		SDL_DestroyMutex(mp3->lock);
		SDL_free(mp3);
		SDL_CloseIO(file);
		return NULL;
	}

	mp3->srcSpec.channels = mp3->data.channels;
	mp3->srcSpec.freq = mp3->data.sampleRate;
	mp3->dstSpec = *dstSpec;

	if (
		dstSpec->format != SDL_AUDIO_S16 ||
		dstSpec->channels != mp3->srcSpec.channels ||
		dstSpec->freq != mp3->srcSpec.freq
	) {
		mp3->srcSpec.format = SDL_AUDIO_F32;
		mp3->converter = SDL_CreateAudioStream(&mp3->srcSpec, dstSpec);
		if (!mp3->converter) {
			drmp3_uninit(&mp3->data);
			SDL_DestroyMutex(mp3->lock);
			SDL_free(mp3);
			SDL_CloseIO(file);
			return NULL;
		}
		mp3->framesMax = APP_STREAMED_AUDIO_CHUNK_FLOAT_SAMPLES_MAX(mp3->srcSpec.channels);
		mp3->buffer = SDL_malloc(sizeof(float) * mp3->srcSpec.channels * mp3->framesMax);
		if (!mp3->buffer) {
			SDL_DestroyAudioStream(mp3->converter);
			drmp3_uninit(&mp3->data);
			SDL_DestroyMutex(mp3->lock);
			SDL_free(mp3);
			SDL_CloseIO(file);
			return NULL;
		}
	}
	mp3->file = file;

	return (APP_StreamingAudioData*)mp3;
}

static bool APP_GetMP3StreamingAudioDataChunk(APP_StreamingAudioData* streamingAudioData, uint8_t* chunk, int chunkTotalSize, int* chunkOutSize, bool* finished, bool looping)
{
	*finished = false;
	*chunkOutSize = 0;
	if (chunkTotalSize == 0) {
		return true;
	}

	APP_StreamingMP3AudioData* mp3 = (APP_StreamingMP3AudioData*)streamingAudioData;
	SDL_LockMutex(mp3->lock);
	int chunkRemaining = APP_STREAMED_AUDIO_CHUNK_SPEC_SIZE_MAX(mp3->dstSpec, chunkTotalSize);
	uint8_t* pos = chunk;

	if (mp3->converter) {
		while (true) {
			const int gotBytes = SDL_GetAudioStreamData(mp3->converter, pos, chunkRemaining);
			if (gotBytes < 0) {
				SDL_UnlockMutex(mp3->lock);
				return false;
			}
			*chunkOutSize += gotBytes;
			if (SDL_GetAudioStreamAvailable(mp3->converter) == 0 && mp3->finished) {
				*finished = true;
				SDL_UnlockMutex(mp3->lock);
				return true;
			}
			pos += gotBytes;
			chunkRemaining -= gotBytes;
			if (chunkRemaining == 0) {
				SDL_UnlockMutex(mp3->lock);
				return true;
			}

			if (looping || !mp3->finished) {
				uint64_t totalSamples = 0;
				do {
					const uint64_t gotSamples = drmp3_read_pcm_frames_f32(&mp3->data, mp3->framesMax, mp3->buffer);
					switch (SDL_GetIOStatus(mp3->file)) {
					case SDL_IO_STATUS_ERROR:
					case SDL_IO_STATUS_NOT_READY:
					case SDL_IO_STATUS_WRITEONLY:
						SDL_UnlockMutex(mp3->lock);
						return false;

					default:
						break;
					}
					if (!SDL_PutAudioStreamData(mp3->converter, mp3->buffer, (int)(sizeof(float) * mp3->srcSpec.channels * gotSamples))) {
						SDL_UnlockMutex(mp3->lock);
						return false;
					}
					if (mp3->data.currentPCMFrame == mp3->frames) {
						if (looping) {
							if (!drmp3_seek_to_pcm_frame(&mp3->data, 0)) {
								SDL_UnlockMutex(mp3->lock);
								return false;
							}
						}
						else {
							mp3->finished = true;
							break;
						}
					}
					totalSamples += gotSamples;
				} while (totalSamples < mp3->framesMax);
			}
		}
	}
	else {
		do {
			const uint64_t gotSamples = drmp3_read_pcm_frames_s16(&mp3->data, chunkRemaining / (sizeof(int16_t) * mp3->dstSpec.channels), (int16_t*)pos);
			switch (SDL_GetIOStatus(mp3->file)) {
			case SDL_IO_STATUS_ERROR:
			case SDL_IO_STATUS_NOT_READY:
			case SDL_IO_STATUS_WRITEONLY:
				SDL_UnlockMutex(mp3->lock);
				return false;

			default:
				break;
			}
			if (gotSamples > INT_MAX / (sizeof(int16_t) * mp3->dstSpec.channels)) {
				SDL_UnlockMutex(mp3->lock);
				return false;
			}
			if (mp3->data.currentPCMFrame == mp3->frames) {
				if (looping) {
					if (!drmp3_seek_to_pcm_frame(&mp3->data, 0)) {
						SDL_UnlockMutex(mp3->lock);
						return false;
					}
				}
				else {
					mp3->finished = true;
					*finished = true;
				}
			}
			const int gotBytes = (int)(sizeof(int16_t) * mp3->dstSpec.channels * gotSamples);
			pos += gotBytes;
			*chunkOutSize += gotBytes;
			chunkRemaining -= gotBytes;
		} while (chunkRemaining > 0 && !*finished);
		SDL_UnlockMutex(mp3->lock);
		return true;
	}
}

static bool APP_RestartMP3StreamingAudioData(APP_StreamingAudioData* streamingAudioData)
{
	APP_StreamingMP3AudioData* mp3 = (APP_StreamingMP3AudioData*)streamingAudioData;
	SDL_LockMutex(mp3->lock);

	mp3->finished = false;
	if (mp3->converter) {
		if (!SDL_ClearAudioStream(mp3->converter)) {
			SDL_UnlockMutex(mp3->lock);
			return false;
		}
	}
	const bool success = !!drmp3_seek_to_pcm_frame(&mp3->data, 0);
	SDL_UnlockMutex(mp3->lock);
	return success;
}

static void APP_DestroyMP3StreamingAudioData(APP_StreamingAudioData* streamingAudioData)
{
	APP_StreamingMP3AudioData* mp3 = (APP_StreamingMP3AudioData*)streamingAudioData;

	SDL_DestroyAudioStream(mp3->converter);
	SDL_free(mp3->buffer);
	drmp3_uninit(&mp3->data);
	SDL_DestroyMutex(mp3->lock);
	SDL_free(mp3);
}
