#include "APP_audio.h"
#include "APP_filesystem.h"
#include "APP_main.h"
#include "APP_error.h"
#include <SDL3_mixer/SDL_mixer.h>

typedef struct APP_Wave
{
	MIX_Track* track;
	bool looping;
} APP_Wave;

static MIX_Mixer* APP_Mixer = NULL;
static int APP_WavesCount = 0;
static APP_Wave* APP_Waves = NULL;
static MIX_Track* APP_Music = NULL;
static SDL_PropertiesID APP_LoopTrack = 0;

bool APP_InitAudio(int wavesCount)
{
	if (wavesCount < 0) {
		return APP_SetError("\"wavesCount\" is invalid; must be at least 0");
	}
	if (APP_Mixer) {
		APP_QuitAudio();
	}

	// Initialize mixer
	if (!MIX_Init()) {
		return APP_SetError("Couldn't init mixer: %s", SDL_GetError());
	}

	// Initialize mixer device
	APP_Mixer = MIX_CreateMixerDevice(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, NULL);
	if (!APP_Mixer) {
		return APP_SetError("Couldn't create mixer device: %s", SDL_GetError());
	}

	// サウンドの初期化
	// Initialize sounds
	if (wavesCount > 0) {
		APP_Waves = SDL_calloc(wavesCount, sizeof(APP_Wave));
		if (!APP_Waves) {
			return APP_SetError("Couldn't allocate memory for waves");
		}
	}
	APP_WavesCount = wavesCount;
	APP_Music = NULL;
	APP_LoopTrack = SDL_CreateProperties();
	if (!APP_LoopTrack) {
		return APP_SetError("Failed to create properties for looping audio tracks: %s", SDL_GetError());
	}
	else if (!SDL_SetNumberProperty(APP_LoopTrack, MIX_PROP_PLAY_LOOPS_NUMBER, -1)) {
		return APP_SetError("Failed to set audio track loop properties to have infinite loops number: %s", SDL_GetError());
	}

	return true;
}

void APP_QuitAudio(void)
{
	if (!APP_Mixer) {
		return;
	}

	// サウンドの解放
	// Free sounds
	for (int i = 0; i < APP_WavesCount; i++) {
		MIX_DestroyTrack(APP_Waves[i].track);
	}
	SDL_free(APP_Waves);
	APP_Waves = NULL;
	APP_WavesCount = 0;
	MIX_DestroyTrack(APP_Music);
	APP_Music = NULL;
	SDL_DestroyProperties(APP_LoopTrack);
	APP_LoopTrack = 0;
	MIX_DestroyMixer(APP_Mixer);
	APP_Mixer = NULL;
	MIX_Quit();
}

// Loads a track. If no file for the track was found, *track is set NULL.
//
// Returns true if no error occurred (which could mean no file was found).
// Returns false in case of an error; call SDL_GetError() for more information.
static bool APP_LoadTrack(MIX_Track** track, const char* filename)
{
        *track = NULL;

	const char *const exts[] = {
		".wav",
		".ogg",
		".mp3",
		".flac",
		".opus",
		".mid",
		".midi",

		// TODO: Add remaining format file extensions supported by libxmp
		".it",
		".s3m",
		".mod",
		".xm",

		".ay",
		".gbs",
		".gym",
		".hes",
		".kss",
		".nsf",
		".nsfe",
		".sap",
		".spc",
		".vgm",
		".vgz",
		".aiff",
		".aif",
		".aifc",
		".voc",
		".au",
		//".wv", TODO: Enable this once wavpack builds for all platforms
		""
	};

	for (size_t i = 0; i < SDL_arraysize(exts); i++) {
		char* filenameExt;
		if (SDL_asprintf(&filenameExt, "%s%s", filename, exts[i]) < 0) {
			return false;
		}
		if (!APP_FileExists(filenameExt)) {
			SDL_free(filenameExt);
			continue;
		}
		SDL_IOStream* const io = APP_OpenRead(filenameExt);
		if (!io) {
			SDL_free(filenameExt);
			return false;
		}
		SDL_free(filenameExt);
		MIX_Audio* const audio = MIX_LoadAudio_IO(APP_Mixer, io, false, true);
		if (!audio) {
			return false;
		}
		*track = MIX_CreateTrack(APP_Mixer);
		if (!*track) {
			MIX_DestroyAudio(audio);
			return false;
		}
		if (!MIX_SetTrackAudio(*track, audio)) {
		        MIX_DestroyTrack(*track);
			*track = NULL;
		        MIX_DestroyAudio(audio);
			return false;
		}
		// Doesn't actually destroy the audio here, just leaves it only
		// referenced by the track, so destroying the track later also destroys
		// the audio.
		MIX_DestroyAudio(audio);
		return true;
	}
	return true;
}

void APP_LoadWave(int num, const char* filename, bool looping)
{
	if (num < 0 || num >= APP_WavesCount || !filename || !*filename) {
		return;
	}

	if (APP_Waves[num].track) {
	        MIX_DestroyTrack(APP_Waves[num].track);
		APP_Waves[num] = (APP_Wave) { 0 };
	}

	if (!APP_LoadTrack(&APP_Waves[num].track, filename)) {
	        APP_Exit(SDL_APP_FAILURE);
	}
	if (APP_Waves[num].track) {
        	APP_Waves[num].looping = looping;
	}
}

void APP_PlayWave(int num)
{
	if (APP_WavesCount == 0 || num >= APP_WavesCount) {
		return;
	}

	if (num >= 0) {
		if (APP_Waves[num].track && !MIX_PlayTrack(APP_Waves[num].track, APP_Waves[num].looping ? APP_LoopTrack : 0)) {
     			APP_SetError("Failed playing wave: %s", SDL_GetError());
     			APP_Exit(SDL_APP_FAILURE);
		}
	}
	else {
		for (int i = 0; i < APP_WavesCount; i++) {
			if (APP_Waves[i].track && !MIX_PlayTrack(APP_Waves[i].track, APP_Waves[i].looping ? APP_LoopTrack : 0)) {
		   		APP_SetError("Failed playing wave: %s", SDL_GetError());
		  		APP_Exit(SDL_APP_FAILURE);
		   	}
		}
	}
}

void APP_ResumeWave(int num)
{
	if (APP_WavesCount == 0 || num >= APP_WavesCount) {
		return;
	}

	if (num >= 0) {
		if (APP_Waves[num].track && !MIX_ResumeTrack(APP_Waves[num].track)) {
			APP_SetError("Failed resuming wave: %s", SDL_GetError());
			APP_Exit(SDL_APP_FAILURE);
		}
	}
	else if (!MIX_ResumeAllTracks(APP_Mixer)) {
		APP_SetError("Failed resuming all waves: %s", SDL_GetError());
		APP_Exit(SDL_APP_FAILURE);
	}
}

void APP_StopWave(int num)
{
	if (APP_WavesCount == 0 || num >= APP_WavesCount) {
		return;
	}

	if (num >= 0) {
		if (APP_Waves[num].track && !MIX_StopTrack(APP_Waves[num].track, 0)) {
			APP_SetError("Failed stopping wave: %s", SDL_GetError());
			APP_Exit(SDL_APP_FAILURE);
		}
	}
	else if (!MIX_StopAllTracks(APP_Mixer, 0)) {
		APP_SetError("Failed stopping all waves: %s", SDL_GetError());
		APP_Exit(SDL_APP_FAILURE);
	}
}

void APP_PauseWave(int num)
{
	if (APP_WavesCount == 0 || num >= APP_WavesCount) {
		return;
	}

	if (num >= 0) {
		if (APP_Waves[num].track && !MIX_PauseTrack(APP_Waves[num].track)) {
			APP_SetError("Failed pausing wave: %s", SDL_GetError());
			APP_Exit(SDL_APP_FAILURE);
		}
	}
	else if (!MIX_PauseAllTracks(APP_Mixer)) {
		APP_SetError("Failed pausing all waves: %s", SDL_GetError());
		APP_Exit(SDL_APP_FAILURE);
	}
}

bool APP_IsWavePlaying(int num)
{
	if (APP_WavesCount == 0 || num >= APP_WavesCount) {
		return false;
	}

	if (num >= 0) {
		return APP_Waves[num].track != NULL && MIX_TrackPlaying(APP_Waves[num].track);
	}
	else {
		for (int i = 0; i < APP_WavesCount; i++) {
			if (APP_Waves[num].track && MIX_TrackPlaying(APP_Waves[num].track)) {
				return true;
			}
		}
		return false;
	}
}

void APP_SetWaveVolume(int num, int volume)
{
	if (APP_WavesCount == 0 || num >= APP_WavesCount) {
		return;
	}

	const float gain = SDL_clamp(volume, 0, 100) / 100.0f;
	if (num >= 0) {
		if (APP_Waves[num].track && !MIX_SetTrackGain(APP_Waves[num].track, gain)) {
			APP_SetError("Failed setting wave volume: %s", SDL_GetError());
			APP_Exit(SDL_APP_FAILURE);
		}
	}
	else {
		for (int i = 0; i < APP_WavesCount; i++) {
			if (APP_Waves[num].track && !MIX_SetTrackGain(APP_Waves[num].track, gain)) {
				APP_SetError("Failed setting waves volume: %s", SDL_GetError());
				APP_Exit(SDL_APP_FAILURE);
			}
		}
	}
}

void APP_SetWaveLooping(int num, bool looping)
{
	if (APP_WavesCount == 0 || num >= APP_WavesCount) {
		return;
	}

	if (num >= 0) {
	        if (!APP_Waves[num].track || APP_Waves[num].looping == looping) {
                        return;
                }

		APP_Waves[num].looping = looping;
		if (!MIX_SetTrackLoops(APP_Waves[num].track, looping ? -1 : 0)) {
			APP_SetError("Failed setting wave loop mode: %s", SDL_GetError());
			APP_Exit(SDL_APP_FAILURE);
		}
	}
	else {
		for (int i = 0; i < APP_WavesCount; i++) {
		        if (!APP_Waves[i].track || APP_Waves[i].looping == looping) {
			        continue;
			}

			APP_Waves[i].looping = looping;
			if (!MIX_SetTrackLoops(APP_Waves[i].track, looping ? -1 : 0)) {
				APP_SetError("Failed setting waves loop mode: %s", SDL_GetError());
				APP_Exit(SDL_APP_FAILURE);
			}
		}
	}
}

void APP_LoadMusic(const char* filename)
{
        if (APP_Music) {
                MIX_DestroyTrack(APP_Music);
                APP_Music = NULL;
        }

	if (!APP_LoadTrack(&APP_Music, filename)) {
	        APP_Exit(SDL_APP_FAILURE);
	}
}

void APP_PlayMusic(void)
{
        if (APP_Music && !MIX_PlayTrack(APP_Music, APP_LoopTrack)) {
                APP_Exit(SDL_APP_FAILURE);
        }
}

void APP_ResumeMusic(void)
{
        if (APP_Music && !MIX_ResumeTrack(APP_Music)) {
                APP_Exit(SDL_APP_FAILURE);
        }
}

void APP_StopMusic(void)
{
        if (APP_Music && !MIX_StopTrack(APP_Music, 0)) {
                APP_Exit(SDL_APP_FAILURE);
        }
}

void APP_PauseMusic(void)
{
        if (APP_Music && !MIX_PauseTrack(APP_Music)) {
                APP_Exit(SDL_APP_FAILURE);
        }
}

bool APP_IsMusicPlaying(void)
{
        return APP_Music != NULL && MIX_TrackPlaying(APP_Music);
}

void APP_SetMusicVolume(int volume)
{
        if (!APP_Music) {
                return;
        }

        const float gain = SDL_clamp(volume, 0, 100) / 100.0f;
        if (!MIX_SetTrackGain(APP_Music, gain)) {
                APP_Exit(SDL_APP_FAILURE);
        }
}
