#include "APP_ogg.h"
#include <SDL3/SDL_audio.h>

#define STB_VORBIS_SDL 1
#define STB_VORBIS_NO_STDIO 1
#define STB_VORBIS_NO_CRT 1
#define STB_VORBIS_NO_PUSHDATA_API 1
#define STB_VORBIS_MAX_CHANNELS 8
#define STB_FORCEINLINE SDL_FORCE_INLINE
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
#define STB_VORBIS_BIG_ENDIAN 1
#endif
#define STBV_CDECL SDLCALL
#ifdef assert
#undef assert
#endif
#ifdef memset
#undef memset
#endif
#ifdef memcpy
#undef memcpy
#endif
#define assert SDL_assert
#define memset SDL_memset
#define memcmp SDL_memcmp
#define memcpy SDL_memcpy
#define qsort SDL_qsort
#define malloc SDL_malloc
#define realloc SDL_realloc
#define free SDL_free
#define pow SDL_pow
#define floor SDL_floor
#define ldexp(v, e) SDL_scalbn((v), (e))
#define abs(x) SDL_abs(x)
#define cos(x) SDL_cos(x)
#define sin(x) SDL_sin(x)
#define log(x) SDL_log(x)
#define exp(x) SDL_exp(x)
#define STB_VORBIS_STDINT_DEFINED 1
typedef Uint8 uint8;
typedef Sint8 int8;
typedef Uint16 uint16;
typedef Sint16 int16;
typedef Uint32 uint32;
typedef Sint32 int32;
#include "stb_vorbis.h"

typedef struct APP_StreamingOGGAudioData
{
	APP_StreamingAudioData callbacks;
	stb_vorbis* data;
	SDL_AudioStream* converter;
	float* buffer;
	SDL_AudioSpec srcSpec;
	SDL_AudioSpec dstSpec;
	unsigned samples;
	int floatsMax;
	bool finished;
} APP_StreamingOGGAudioData;

static bool APP_GetOGGStreamingAudioDataChunk(APP_StreamingAudioData* streamingAudioData, uint8_t* chunk, int chunkTotalSize, int* chunkOutSize, bool* finished, bool looping);
static bool APP_RestartOGGStreamingAudioData(APP_StreamingAudioData* streamingAudioData);
static void APP_DestroyOGGStreamingAudioData(APP_StreamingAudioData* streamingAudioData);

bool APP_PreloadOGG(SDL_IOStream* file, const SDL_AudioSpec* format, uint8_t** data, uint32_t* size)
{
	*data = NULL;
	int error;
	stb_vorbis* vorbis = stb_vorbis_open_io(file, 1, &error, NULL);
	if (!vorbis) {
		SDL_CloseIO(file);
		return false;
	}
	const SDL_AudioSpec vorbisFormat = { SDL_AUDIO_F32, vorbis->channels, vorbis->sample_rate };
	const unsigned samples = stb_vorbis_stream_length_in_samples(vorbis);
	if (!samples) {
		stb_vorbis_close(vorbis);
		return false;
	}
	uint8_t* const vorbisData = SDL_malloc(sizeof(float) * vorbisFormat.channels * samples);
	if (!vorbisData) {
		stb_vorbis_close(vorbis);
		return false;
	}
	const int gotSamples = stb_vorbis_get_samples_float_interleaved(vorbis, vorbisFormat.channels, (float*)vorbisData, vorbisFormat.channels * samples);
	if (gotSamples == 0 || stb_vorbis_get_error(vorbis)) {
		stb_vorbis_close(vorbis);
		SDL_free(vorbisData);
		return false;
	}
	stb_vorbis_close(vorbis);
	size_t vorbisSize = sizeof(float) * vorbisFormat.channels * gotSamples;
	if (
		vorbisFormat.format != format->format ||
		vorbisFormat.channels != format->channels ||
		vorbisFormat.freq != format->freq
	) {
		if (vorbisSize > INT_MAX) {
			SDL_free(vorbisData);
			return false;
		}
		int convertedSize;
		if (!SDL_ConvertAudioSamples(&vorbisFormat, (uint8_t*)vorbisData, (int)vorbisSize, format, data, &convertedSize)) {
			SDL_free(vorbisData);
			return false;
		}
		SDL_free(vorbisData);
		*size = convertedSize;
	}
	else {
		if (vorbisSize > UINT32_MAX) {
			SDL_free(vorbisData);
			return false;
		}
		*data = (uint8_t*)vorbisData;
		*size = (uint32_t)vorbisSize;
	}
	return true;
}

APP_StreamingAudioData* APP_CreateStreamingOGGAudioData(SDL_IOStream* file, SDL_AudioSpec* dstSpec)
{
	APP_StreamingOGGAudioData* ogg = SDL_calloc(1, sizeof(APP_StreamingOGGAudioData));
	if (!ogg) {
		SDL_CloseIO(file);
		return NULL;
	}
	ogg->callbacks.getChunk = APP_GetOGGStreamingAudioDataChunk;
	ogg->callbacks.restart = APP_RestartOGGStreamingAudioData;
	ogg->callbacks.destroy = APP_DestroyOGGStreamingAudioData;

	int error;
	ogg->data = stb_vorbis_open_io(file, 1, &error, NULL);
	if (!ogg->data) {
		SDL_free(ogg);
		return NULL;
	}
	ogg->samples = stb_vorbis_stream_length_in_samples(ogg->data);
	if (ogg->samples == 0) {
		stb_vorbis_close(ogg->data);
		SDL_free(ogg);
		return NULL;
	}

	ogg->srcSpec.channels = ogg->data->channels;
	ogg->srcSpec.freq = ogg->data->sample_rate;
	ogg->dstSpec = *dstSpec;

	if (
		dstSpec->format != SDL_AUDIO_S16 ||
		dstSpec->channels != ogg->srcSpec.channels ||
		dstSpec->freq != ogg->srcSpec.freq
	) {
		ogg->srcSpec.format = SDL_AUDIO_F32;
		ogg->converter = SDL_CreateAudioStream(&ogg->srcSpec, dstSpec);
		if (!ogg->converter) {
			stb_vorbis_close(ogg->data);
			SDL_free(ogg);
			return NULL;
		}
		ogg->floatsMax = APP_STREAMED_AUDIO_CHUNK_FLOATS_MAX(ogg->srcSpec.channels);
		ogg->buffer = SDL_malloc(sizeof(float) * ogg->floatsMax);
		if (!ogg->buffer) {
			SDL_DestroyAudioStream(ogg->converter);
			stb_vorbis_close(ogg->data);
			SDL_free(ogg);
			return NULL;
		}
	}

	return (APP_StreamingAudioData*)ogg;
}

static bool APP_GetOGGStreamingAudioDataChunk(APP_StreamingAudioData* streamingAudioData, uint8_t* chunk, int chunkTotalSize, int* chunkOutSize, bool* finished, bool looping)
{
	*finished = false;
	*chunkOutSize = 0;
	if (chunkTotalSize == 0) {
		return true;
	}

	APP_StreamingOGGAudioData* ogg = (APP_StreamingOGGAudioData*)streamingAudioData;
	int chunkRemaining = APP_STREAMED_AUDIO_CHUNK_SPEC_SIZE_MAX(ogg->dstSpec, chunkTotalSize);
	uint8_t* pos = chunk;

	if (ogg->converter) {
		while (true) {
			const int gotBytes = SDL_GetAudioStreamData(ogg->converter, pos, chunkRemaining);
			if (gotBytes < 0) {
				return false;
			}
			*chunkOutSize += gotBytes;
			if (SDL_GetAudioStreamAvailable(ogg->converter) == 0 && ogg->finished) {
				*finished = true;
				return true;
			}
			pos += gotBytes;
			chunkRemaining -= gotBytes;
			if (chunkRemaining == 0) {
				return true;
			}

			if (looping || !ogg->finished) {
				int totalSamples = 0;
				do {
					const int gotSamples = stb_vorbis_get_samples_float_interleaved(ogg->data, ogg->srcSpec.channels, ogg->buffer, ogg->floatsMax);
					if (stb_vorbis_get_error(ogg->data)) {
						return false;
					}
					if (gotSamples > INT_MAX / (sizeof(float) * ogg->srcSpec.channels)) {
						return false;
					}
					if (!SDL_PutAudioStreamData(ogg->converter, ogg->buffer, sizeof(float) * ogg->srcSpec.channels * gotSamples)) {
						return false;
					}
					if (stb_vorbis_get_playback_sample_offset(ogg->data) == ogg->samples) {
						if (looping) {
							if (!stb_vorbis_seek_start(ogg->data)) {
								return false;
							}
						}
						else {
							ogg->finished = true;
							break;
						}
					}
					totalSamples += gotSamples;
				} while (totalSamples * ogg->srcSpec.channels < ogg->floatsMax);
			}
		}
	}
	else {
		do {
			const int gotSamples = stb_vorbis_get_samples_short_interleaved(ogg->data, ogg->dstSpec.channels, (int16_t*)pos, chunkRemaining / sizeof(int16_t));
			if (stb_vorbis_get_error(ogg->data)) {
				return false;
			}
			if (gotSamples > INT_MAX / (sizeof(int16_t) * ogg->dstSpec.channels)) {
				return false;
			}
			if (stb_vorbis_get_playback_sample_offset(ogg->data) == ogg->samples) {
				if (looping) {
					if (!stb_vorbis_seek_start(ogg->data)) {
						return false;
					}
				}
				else {
					ogg->finished = true;
					*finished = true;
				}
			}
			const int gotBytes = sizeof(int16_t) * ogg->dstSpec.channels * gotSamples;
			pos += gotBytes;
			*chunkOutSize += gotBytes;
			chunkRemaining -= gotBytes;
		} while (chunkRemaining > 0 && !*finished);
		return true;
	}
}

static bool APP_RestartOGGStreamingAudioData(APP_StreamingAudioData* streamingAudioData)
{
	APP_StreamingOGGAudioData* ogg = (APP_StreamingOGGAudioData*)streamingAudioData;

	ogg->finished = false;
	if (ogg->converter) {
		if (!SDL_ClearAudioStream(ogg->converter)) {
			return false;
		}
	}
	return !!stb_vorbis_seek_start(ogg->data);
}

static void APP_DestroyOGGStreamingAudioData(APP_StreamingAudioData* streamingAudioData)
{
	APP_StreamingOGGAudioData* ogg = (APP_StreamingOGGAudioData*)streamingAudioData;

	SDL_DestroyAudioStream(ogg->converter);
	SDL_free(ogg->buffer);
	stb_vorbis_close(ogg->data);
	SDL_free(ogg);
}
