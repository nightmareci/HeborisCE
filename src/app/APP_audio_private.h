#ifndef APP_audio_private_h_
#define APP_audio_private_h_

#include "APP_stdinc.h"

typedef bool (* APP_PreloadAudioDataFunction)(SDL_IOStream* file, const SDL_AudioSpec* format, uint8_t** data, uint32_t* size);

#define APP_STREAMED_AUDIO_CHUNK_SIZE 0x8000

#define APP_STREAMED_AUDIO_CHUNK_FLOATS_MAX(channels) (APP_STREAMED_AUDIO_CHUNK_SIZE / sizeof(float)) - ((APP_STREAMED_AUDIO_CHUNK_SIZE / sizeof(float)) % (channels));

typedef struct APP_StreamingAudioData APP_StreamingAudioData;

typedef APP_StreamingAudioData* (* APP_CreateStreamingAudioDataFunction)(SDL_IOStream* file, SDL_AudioSpec* dstSpec);

struct APP_StreamingAudioData {
	bool (* getChunk)(APP_StreamingAudioData* streamingAudioData, uint8_t* chunk, int chunkTotalSize, int* chunkOutSize, bool* finished, bool looping);
	bool (* restart)(APP_StreamingAudioData* streamingAudioData);
	void (* destroy)(APP_StreamingAudioData* streamingAudioData);
};

#define APP_GetStreamingAudioDataChunk(streamingAudioData, chunk, chunkTotalSize, chunkOutSize, finished, looping) \
	(streamingAudioData)->getChunk((streamingAudioData), (chunk), (chunkTotalSize), (chunkOutSize), (finished), (looping))

#define APP_RestartStreamingAudioData(streamingAudioData) \
	(streamingAudioData)->restart((streamingAudioData))

#define APP_DestroyStreamingAudioData(streamingAudioData) \
	(streamingAudioData)->destroy((streamingAudioData))

#endif
