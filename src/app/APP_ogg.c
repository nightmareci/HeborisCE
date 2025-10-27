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

bool APP_LoadOGG(SDL_IOStream* file, const SDL_AudioSpec* format, uint8_t** data, uint32_t* size)
{
	*data = NULL;
	int error;
	stb_vorbis* vorbis = stb_vorbis_open_io(file, 1, &error, NULL);
	if (!vorbis) {
		SDL_CloseIO(file);
		return false;
	}
	const SDL_AudioSpec vorbisFormat = { SDL_AUDIO_F32, vorbis->channels, vorbis->sample_rate };
	unsigned samples = stb_vorbis_stream_length_in_samples(vorbis);
	if (!samples) {
		stb_vorbis_close(vorbis);
		return false;
	}
	uint8_t* const vorbisData = SDL_malloc(sizeof(float) * vorbisFormat.channels * samples);
	if (!vorbisData) {
		stb_vorbis_close(vorbis);
		return false;
	}
	int gotSamples = stb_vorbis_get_samples_float_interleaved(vorbis, vorbisFormat.channels, (float*)vorbisData, vorbisFormat.channels * samples);
	error = stb_vorbis_get_error(vorbis);
	if (gotSamples == 0 || error) {
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
