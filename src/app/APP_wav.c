#include "APP_wav.h"

#define DR_WAV_IMPLEMENTATION
#define DR_WAV_NO_STDIO
#define DRWAV_API static
#define DRWAV_PRIVATE static
#define DRWAV_ASSERT(expression) SDL_assert((expression))
#define DRWAV_MALLOC(sz) SDL_malloc((sz))
#define DRWAV_REALLOC(p, sz) SDL_realloc((p), (sz))
#define DRWAV_FREE(p) SDL_free((p))
#define DRWAV_COPY_MEMORY(dst, src, sz) SDL_memcpy((dst), (src), (sz))
#define DRWAV_ZERO_MEMORY(p, sz) SDL_memset((p), 0, (sz))
#include "dr_wav.h"

typedef struct APP_StreamingWAVAudioData
{
	APP_StreamingAudioData callbacks;
	drwav data;
	drwav_uint64 frames;
	uint64_t framesMax;
	SDL_Mutex* lock;
	SDL_AudioStream* converter;
	SDL_IOStream* file;
	float* buffer;
	SDL_AudioSpec srcSpec;
	SDL_AudioSpec dstSpec;
	bool finished;
} APP_StreamingWAVAudioData;

static bool APP_GetWAVStreamingAudioDataChunk(APP_StreamingAudioData* streamingAudioData, uint8_t* chunk, int chunkTotalSize, int* chunkOutSize, bool* finished, bool looping);
static bool APP_RestartWAVStreamingAudioData(APP_StreamingAudioData* streamingAudioData);
static void APP_DestroyWAVStreamingAudioData(APP_StreamingAudioData* streamingAudioData);

static size_t APP_DRWAV_Read(void* pUserData, void* pBufferOut, size_t bytesToRead)
{
	return SDL_ReadIO((SDL_IOStream*)pUserData, pBufferOut, bytesToRead);
}

static drwav_bool32 APP_DRWAV_Seek(void* pUserData, int offset, drwav_seek_origin origin)
{
	SDL_IOStream* const file = pUserData;
	switch (origin) {
	default:
	case DRWAV_SEEK_SET:
		return SDL_SeekIO(file, offset, SDL_IO_SEEK_SET) >= 0;

	case DRWAV_SEEK_CUR:
		return SDL_SeekIO(file, offset, SDL_IO_SEEK_CUR) >= 0;

	case DRWAV_SEEK_END:
		return SDL_SeekIO(file, offset, SDL_IO_SEEK_END) >= 0;
	}
}

static drwav_bool32 APP_DRWAV_Tell(void* pUserData, drwav_int64* pCursor)
{
	*pCursor = SDL_TellIO((SDL_IOStream*)pUserData);
	return *pCursor >= 0;
}

static void* APP_DRWAV_Malloc(size_t sz, void* pUserData)
{
	(void)pUserData;
	return SDL_malloc(sz);
}

static void* APP_DRWAV_Realloc(void* p, size_t sz, void* pUserData)
{
	(void)pUserData;
	return SDL_realloc(p, sz);
}

static void APP_DRWAV_Free(void* p, void* pUserData)
{
	(void)pUserData;
	SDL_free(p);
}

bool APP_PreloadWAV(SDL_IOStream* file, const SDL_AudioSpec* format, uint8_t** data, uint32_t* size)
{
	unsigned channels, sampleRate;
	drwav_uint64 totalFrameCount;
	const drwav_allocation_callbacks allocationCallbacks = { NULL, APP_DRWAV_Malloc, APP_DRWAV_Realloc, APP_DRWAV_Free };
	float** const floatData = (float**)data;
	*floatData = drwav_open_and_read_pcm_frames_f32(APP_DRWAV_Read, APP_DRWAV_Seek, APP_DRWAV_Tell, file, &channels, &sampleRate, &totalFrameCount, &allocationCallbacks);
	SDL_CloseIO(file);
	if (!*data) {
		return false;
	}
	*size = (uint32_t)(sizeof(float) * channels * totalFrameCount);
	const SDL_AudioSpec waveFormat = { SDL_AUDIO_F32, channels, sampleRate };
	if (
		waveFormat.format != format->format ||
		waveFormat.channels != format->channels ||
		waveFormat.freq != format->freq
	) {
		if (totalFrameCount > INT_MAX / (sizeof(float) * waveFormat.channels)) {
			SDL_free(*data);
			*data = NULL;
			return false;
		}
		uint8_t* convertedData;
		int convertedSize;
		if (!SDL_ConvertAudioSamples(&waveFormat, *data, (int)*size, format, &convertedData, &convertedSize)) {
			SDL_free(*data);
			*data = NULL;
			return false;
		}
		SDL_free(*data);
		*data = convertedData;
		*size = convertedSize;
	}
	else if (totalFrameCount > UINT32_MAX / (sizeof(float) * waveFormat.channels)) {
		SDL_free(*data);
		*data = NULL;
		return false;
	}
	return true;
}

APP_StreamingAudioData* APP_CreateStreamingWAVAudioData(SDL_IOStream* file, SDL_AudioSpec* dstSpec)
{
	APP_StreamingWAVAudioData* wav = SDL_calloc(1, sizeof(APP_StreamingWAVAudioData));
	if (!wav) {
		SDL_CloseIO(file);
		return NULL;
	}
	wav->callbacks.getChunk = APP_GetWAVStreamingAudioDataChunk;
	wav->callbacks.restart = APP_RestartWAVStreamingAudioData;
	wav->callbacks.destroy = APP_DestroyWAVStreamingAudioData;

	wav->lock = SDL_CreateMutex();
	if (!wav->lock) {
		SDL_free(wav);
		SDL_CloseIO(file);
		return NULL;
	}

	const drwav_allocation_callbacks allocationCallbacks = { NULL, APP_DRWAV_Malloc, APP_DRWAV_Realloc, APP_DRWAV_Free };
	if (!drwav_init(&wav->data, APP_DRWAV_Read, APP_DRWAV_Seek, APP_DRWAV_Tell, file, &allocationCallbacks)) {
		SDL_DestroyMutex(wav->lock);
		SDL_free(wav);
		SDL_CloseIO(file);
		return NULL;
	}
	if (drwav_get_length_in_pcm_frames(&wav->data, &wav->frames) != DRWAV_SUCCESS || wav->frames == 0) {
		drwav_uninit(&wav->data);
		SDL_DestroyMutex(wav->lock);
		SDL_free(wav);
		SDL_CloseIO(file);
		return NULL;
	}

	wav->srcSpec.channels = wav->data.channels;
	wav->srcSpec.freq = wav->data.sampleRate;
	wav->dstSpec = *dstSpec;

	if (
		dstSpec->format != SDL_AUDIO_S16 ||
		dstSpec->channels != wav->srcSpec.channels ||
		dstSpec->freq != wav->srcSpec.freq
	) {
		wav->srcSpec.format = SDL_AUDIO_F32;
		wav->converter = SDL_CreateAudioStream(&wav->srcSpec, dstSpec);
		if (!wav->converter) {
			drwav_uninit(&wav->data);
			SDL_DestroyMutex(wav->lock);
			SDL_free(wav);
			SDL_CloseIO(file);
			return NULL;
		}
		wav->framesMax = APP_STREAMED_AUDIO_CHUNK_FLOAT_SAMPLES_MAX(wav->srcSpec.channels);
		wav->buffer = SDL_malloc(sizeof(float) * wav->srcSpec.channels * wav->framesMax);
		if (!wav->buffer) {
			SDL_DestroyAudioStream(wav->converter);
			drwav_uninit(&wav->data);
			SDL_DestroyMutex(wav->lock);
			SDL_free(wav);
			SDL_CloseIO(file);
			return NULL;
		}
	}
	wav->file = file;

	return (APP_StreamingAudioData*)wav;
}

static bool APP_GetWAVStreamingAudioDataChunk(APP_StreamingAudioData* streamingAudioData, uint8_t* chunk, int chunkTotalSize, int* chunkOutSize, bool* finished, bool looping)
{
	*finished = false;
	*chunkOutSize = 0;
	if (chunkTotalSize == 0) {
		return true;
	}

	APP_StreamingWAVAudioData* wav = (APP_StreamingWAVAudioData*)streamingAudioData;
	SDL_LockMutex(wav->lock);
	int chunkRemaining = APP_STREAMED_AUDIO_CHUNK_SPEC_SIZE_MAX(wav->dstSpec, chunkTotalSize);
	uint8_t* pos = chunk;

	if (wav->converter) {
		while (true) {
			const int gotBytes = SDL_GetAudioStreamData(wav->converter, pos, chunkRemaining);
			if (gotBytes < 0) {
				SDL_UnlockMutex(wav->lock);
				return false;
			}
			*chunkOutSize += gotBytes;
			if (SDL_GetAudioStreamAvailable(wav->converter) == 0 && wav->finished) {
				*finished = true;
				SDL_UnlockMutex(wav->lock);
				return true;
			}
			pos += gotBytes;
			chunkRemaining -= gotBytes;
			if (chunkRemaining == 0) {
				SDL_UnlockMutex(wav->lock);
				return true;
			}

			if (looping || !wav->finished) {
				uint64_t totalSamples = 0;
				do {
					const uint64_t gotSamples = drwav_read_pcm_frames_f32(&wav->data, wav->framesMax, wav->buffer);
					switch (SDL_GetIOStatus(wav->file)) {
					case SDL_IO_STATUS_ERROR:
					case SDL_IO_STATUS_NOT_READY:
					case SDL_IO_STATUS_WRITEONLY:
						SDL_UnlockMutex(wav->lock);
						return false;

					default:
						break;
					}
					if (!SDL_PutAudioStreamData(wav->converter, wav->buffer, (int)(sizeof(float) * wav->srcSpec.channels * gotSamples))) {
						SDL_UnlockMutex(wav->lock);
						return false;
					}
					drwav_uint64 cursor;
					if (drwav_get_cursor_in_pcm_frames(&wav->data, &cursor) != DRWAV_SUCCESS) {
						SDL_UnlockMutex(wav->lock);
						return false;
					}
					if (cursor == wav->frames) {
						if (looping) {
							if (!drwav_seek_to_pcm_frame(&wav->data, 0)) {
								SDL_UnlockMutex(wav->lock);
								return false;
							}
						}
						else {
							wav->finished = true;
							break;
						}
					}
					totalSamples += gotSamples;
				} while (totalSamples < wav->framesMax);
			}
		}
	}
	else {
		do {
			const uint64_t gotSamples = drwav_read_pcm_frames_s16(&wav->data, chunkRemaining / (sizeof(int16_t) * wav->dstSpec.channels), (int16_t*)pos);
			switch (SDL_GetIOStatus(wav->file)) {
			case SDL_IO_STATUS_ERROR:
			case SDL_IO_STATUS_NOT_READY:
			case SDL_IO_STATUS_WRITEONLY:
				SDL_UnlockMutex(wav->lock);
				return false;

			default:
				break;
			}
			if (gotSamples > INT_MAX / (sizeof(int16_t) * wav->dstSpec.channels)) {
				SDL_UnlockMutex(wav->lock);
				return false;
			}
			if (wav->data.readCursorInPCMFrames == wav->data.totalPCMFrameCount) {
				if (looping) {
					if (!drwav_seek_to_pcm_frame(&wav->data, 0)) {
						SDL_UnlockMutex(wav->lock);
						return false;
					}
				}
				else {
					wav->finished = true;
					*finished = true;
				}
			}
			const int gotBytes = (int)(sizeof(int16_t) * wav->dstSpec.channels * gotSamples);
			pos += gotBytes;
			*chunkOutSize += gotBytes;
			chunkRemaining -= gotBytes;
		} while (chunkRemaining > 0 && !*finished);
		SDL_UnlockMutex(wav->lock);
		return true;
	}
}

static bool APP_RestartWAVStreamingAudioData(APP_StreamingAudioData* streamingAudioData)
{
	APP_StreamingWAVAudioData* wav = (APP_StreamingWAVAudioData*)streamingAudioData;
	SDL_LockMutex(wav->lock);

	wav->finished = false;
	if (wav->converter) {
		if (!SDL_ClearAudioStream(wav->converter)) {
			SDL_UnlockMutex(wav->lock);
			return false;
		}
	}
	const bool success = !!drwav_seek_to_pcm_frame(&wav->data, 0);
	SDL_UnlockMutex(wav->lock);
	return success;
}

static void APP_DestroyWAVStreamingAudioData(APP_StreamingAudioData* streamingAudioData)
{
	APP_StreamingWAVAudioData* wav = (APP_StreamingWAVAudioData*)streamingAudioData;

	SDL_DestroyAudioStream(wav->converter);
	SDL_free(wav->buffer);
	drwav_uninit(&wav->data);
	SDL_DestroyMutex(wav->lock);
	SDL_free(wav);
}
