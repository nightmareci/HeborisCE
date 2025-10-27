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
	return SDL_malloc(sz);
}

static void* APP_DRWAV_Realloc(void* p, size_t sz, void* pUserData)
{
	return SDL_realloc(p, sz);
}

static void APP_DRWAV_Free(void* p, void* pUserData)
{
	SDL_free(p);
}

bool APP_LoadWAV(SDL_IOStream* file, const SDL_AudioSpec* format, uint8_t** data, uint32_t* size)
{
	unsigned channels, sampleRate;
	drwav_uint64 totalFrameCount;
	const drwav_allocation_callbacks allocationCallbacks = { NULL, APP_DRWAV_Malloc, APP_DRWAV_Realloc, APP_DRWAV_Free };
	(float*)*data = drwav_open_and_read_pcm_frames_f32(APP_DRWAV_Read, APP_DRWAV_Seek, APP_DRWAV_Tell, file, &channels, &sampleRate, &totalFrameCount, &allocationCallbacks);
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
