#include "APP_mp3.h"

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
	return SDL_malloc(sz);
}

static void* APP_DRMP3_Realloc(void* p, size_t sz, void* pUserData)
{
	return SDL_realloc(p, sz);
}

static void APP_DRMP3_Free(void* p, void* pUserData)
{
	SDL_free(p);
}

bool APP_LoadMP3(SDL_IOStream* file, const SDL_AudioSpec* format, uint8_t** data, uint32_t* size)
{
	drmp3_config config;
	drmp3_uint64 totalFrameCount;
	const drmp3_allocation_callbacks allocationCallbacks = { NULL, APP_DRMP3_Malloc, APP_DRMP3_Realloc, APP_DRMP3_Free };
	(float*)*data = drmp3_open_and_read_pcm_frames_f32(APP_DRMP3_Read, APP_DRMP3_Seek, APP_DRMP3_Tell, file, &config, &totalFrameCount, &allocationCallbacks);
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