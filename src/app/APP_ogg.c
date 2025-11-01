#include "APP_ogg.h"

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
	stb_vorbis* vorbis;
	SDL_AudioStream* converter;
	float* buffer;
	SDL_AudioSpec srcSpec;
	SDL_AudioSpec dstSpec;
	int floatsMax;
} APP_StreamingOGGAudioData;

static bool APP_GetOGGStreamingAudioDataChunk(APP_StreamingAudioData* streamingAudioData, uint8_t* chunk, int chunkTotalSize, int* chunkOutSize, bool looping);
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
		return NULL;
	}
	ogg->callbacks.getChunk = APP_GetOGGStreamingAudioDataChunk;
	ogg->callbacks.restart = APP_RestartOGGStreamingAudioData;
	ogg->callbacks.destroy = APP_DestroyOGGStreamingAudioData;

	int error;
	ogg->vorbis = stb_vorbis_open_io(file, 0, &error, NULL);
	if (!ogg->vorbis) {
		SDL_free(ogg);
		return NULL;
	}
	if (stb_vorbis_stream_length_in_samples(ogg->vorbis) == 0) {
		stb_vorbis_close(ogg->vorbis);
		SDL_free(ogg);
		return false;
	}

	ogg->srcSpec.channels = ogg->vorbis->channels;
	ogg->srcSpec.freq = ogg->vorbis->sample_rate;
	ogg->dstSpec = *dstSpec;

	if (
		dstSpec->format != SDL_AUDIO_S16 ||
		dstSpec->channels != ogg->srcSpec.channels ||
		dstSpec->freq != ogg->srcSpec.freq
	) {
		ogg->srcSpec.format = SDL_AUDIO_F32;
		ogg->converter = SDL_CreateAudioStream(&ogg->srcSpec, dstSpec);
		if (!ogg->converter) {
			stb_vorbis_close(ogg->vorbis);
			SDL_free(ogg);
			return false;
		}
		ogg->floatsMax = APP_STREAMED_AUDIO_CHUNK_FLOATS_MAX(ogg->srcSpec.channels);
		ogg->buffer = SDL_malloc(sizeof(float) * ogg->floatsMax);
		if (!ogg->buffer) {
			stb_vorbis_close(ogg->vorbis);
			SDL_free(ogg);
			return false;
		}
	}

	return (APP_StreamingAudioData*)ogg;
}

static bool APP_GetOGGStreamingAudioDataChunk(APP_StreamingAudioData* streamingAudioData, uint8_t* chunk, int chunkTotalSize, int* chunkOutSize, bool looping)
{
	*chunkOutSize = 0;
	if (chunkTotalSize == 0) {
		return true;
	}

	APP_StreamingOGGAudioData* ogg = (APP_StreamingOGGAudioData*)streamingAudioData;
	uint8_t* pos = chunk;
	bool finished = false;

	if (ogg->converter) {
		do {
			const int gotBytes = SDL_GetAudioStreamData(ogg->converter, pos, chunkTotalSize);
			if (gotBytes < 0) {
				return false;
			}
			pos += gotBytes;
			*chunkOutSize += gotBytes;
			chunkTotalSize -= gotBytes;
			if (chunkTotalSize == 0 || finished) {
				return true;
			}

			int totalSamples = 0;
			do {
				const int gotSamples = stb_vorbis_get_samples_float_interleaved(ogg->vorbis, ogg->srcSpec.channels, ogg->buffer, ogg->floatsMax);
				if (stb_vorbis_get_error(ogg->vorbis)) {
					return false;
				}
				if (!SDL_PutAudioStreamData(ogg->converter, ogg->buffer, sizeof(float) * ogg->srcSpec.channels * gotSamples)) {
					return false;
				}
				if (ogg->srcSpec.channels * gotSamples < ogg->floatsMax) {
					if (looping) {
						if (!stb_vorbis_seek_start(ogg->vorbis)) {
							return false;
						}
					}
					else {
						finished = true;
						break;
					}
				}
				totalSamples += gotSamples;
			} while (totalSamples * ogg->srcSpec.channels < ogg->floatsMax);
		} while (true);
	}
	else {
		do {
			const int gotSamples = stb_vorbis_get_samples_short_interleaved(ogg->vorbis, ogg->dstSpec.channels, (int16_t*)pos, chunkTotalSize / sizeof(int16_t));
			if (stb_vorbis_get_error(ogg->vorbis)) {
				return false;
			}
			if (gotSamples < chunkTotalSize / (sizeof(int16_t) * ogg->dstSpec.channels)) {
				if (looping) {
					if (!stb_vorbis_seek_start(ogg->vorbis)) {
						return false;
					}
				}
				else {
					finished = true;
				}
			}
			const int gotBytes = sizeof(int16_t) * ogg->dstSpec.channels * gotSamples;
			pos += gotBytes;
			*chunkOutSize += gotBytes;
			chunkTotalSize -= gotBytes;
		} while (chunkTotalSize > 0 && !finished);
		return true;
	}
}

static bool APP_RestartOGGStreamingAudioData(APP_StreamingAudioData* streamingAudioData)
{
	APP_StreamingOGGAudioData* ogg = (APP_StreamingOGGAudioData*)streamingAudioData;

	return !!stb_vorbis_seek_start(ogg->vorbis);
}

static void APP_DestroyOGGStreamingAudioData(APP_StreamingAudioData* streamingAudioData)
{
	APP_StreamingOGGAudioData* ogg = (APP_StreamingOGGAudioData*)streamingAudioData;

	SDL_DestroyAudioStream(ogg->converter);
	SDL_free(ogg->buffer);
	stb_vorbis_close(ogg->vorbis);
	SDL_free(ogg);
}
