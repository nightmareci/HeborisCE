#include <AUDIO.h>
#include <FILESYSTEM.h>
#include <MAIN.h>
#include <ERROR.h>
#include <SDL3_mixer/SDL_mixer.h>

typedef struct AUDIO_Wave
{
	MIX_Track* track;
	bool looping;
} AUDIO_Wave;

static MIX_Mixer* AUDIO_Mixer = NULL;
static int AUDIO_WavesCount = 0;
static AUDIO_Wave* AUDIO_Waves = NULL;
static MIX_Track* AUDIO_Music = NULL;
static SDL_PropertiesID AUDIO_LoopTrack = 0;

bool AUDIO_Init(int wavesCount)
{
	if (wavesCount < 0) {
		return ERROR_Set("\"wavesCount\" is invalid; must be at least 0");
	}
	if (AUDIO_Mixer) {
		AUDIO_Quit();
	}

	if (!MIX_Init()) {
		return ERROR_Set("Couldn't init mixer: %s", SDL_GetError());
	}

	AUDIO_Mixer = MIX_CreateMixerDevice(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, NULL);
	if (!AUDIO_Mixer) {
		return ERROR_Set("Couldn't create mixer device: %s", SDL_GetError());
	}

	if (wavesCount > 0) {
		AUDIO_Waves = SDL_calloc(wavesCount, sizeof(AUDIO_Wave));
		if (!AUDIO_Waves) {
			return ERROR_Set("Couldn't allocate memory for waves");
		}
	}
	AUDIO_WavesCount = wavesCount;
	AUDIO_Music = NULL;
	AUDIO_LoopTrack = SDL_CreateProperties();
	if (!AUDIO_LoopTrack) {
		return ERROR_Set("Failed to create properties for looping audio tracks: %s", SDL_GetError());
	}
	else if (!SDL_SetNumberProperty(AUDIO_LoopTrack, MIX_PROP_PLAY_LOOPS_NUMBER, -1)) {
		return ERROR_Set("Failed to set audio track loop properties to have infinite loops number: %s", SDL_GetError());
	}

	return true;
}

void AUDIO_Quit(void)
{
	if (!AUDIO_Mixer) {
		return;
	}

	for (int i = 0; i < AUDIO_WavesCount; i++) {
		MIX_DestroyTrack(AUDIO_Waves[i].track);
	}
	SDL_free(AUDIO_Waves);
	AUDIO_Waves = NULL;
	AUDIO_WavesCount = 0;
	MIX_DestroyTrack(AUDIO_Music);
	AUDIO_Music = NULL;
	SDL_DestroyProperties(AUDIO_LoopTrack);
	AUDIO_LoopTrack = 0;
	MIX_DestroyMixer(AUDIO_Mixer);
	AUDIO_Mixer = NULL;
	MIX_Quit();
}

// Loads a track. If no file for the track was found, *track is set NULL.
//
// Returns true if no error occurred (which could mean no file was found).
// Returns false in case of an error; call SDL_GetError() for more information.
static bool AUDIO_LoadTrack(MIX_Track** track, const char* filename)
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
		if (!FILESYSTEM_FileExists(filenameExt)) {
			SDL_free(filenameExt);
			continue;
		}
		SDL_IOStream* const io = FILESYSTEM_OpenRead(filenameExt);
		if (!io) {
			SDL_free(filenameExt);
			return false;
		}
		SDL_free(filenameExt);
		MIX_Audio* const audio = MIX_LoadAudio_IO(AUDIO_Mixer, io, false, true);
		if (!audio) {
			return false;
		}
		*track = MIX_CreateTrack(AUDIO_Mixer);
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
	if (!*track) {
		SDL_Log("Missing audio file %s.*", filename);
	}
	return true;
}

void AUDIO_LoadWave(int num, const char* filename, bool looping)
{
	if (num < 0 || num >= AUDIO_WavesCount || !filename || !*filename) {
		return;
	}

	if (AUDIO_Waves[num].track) {
	        MIX_DestroyTrack(AUDIO_Waves[num].track);
		AUDIO_Waves[num] = (AUDIO_Wave) { 0 };
	}

	if (!AUDIO_LoadTrack(&AUDIO_Waves[num].track, filename)) {
	        MAIN_Exit(SDL_APP_FAILURE);
	}
	if (AUDIO_Waves[num].track) {
        	AUDIO_Waves[num].looping = looping;
	}
}

void AUDIO_PlayWave(int num)
{
	if (AUDIO_WavesCount == 0 || num >= AUDIO_WavesCount) {
		return;
	}

	if (num >= 0) {
		if (AUDIO_Waves[num].track && !MIX_PlayTrack(AUDIO_Waves[num].track, AUDIO_Waves[num].looping ? AUDIO_LoopTrack : 0)) {
     			ERROR_Set("Failed playing wave: %s", SDL_GetError());
     			MAIN_Exit(SDL_APP_FAILURE);
		}
	}
	else {
		for (int i = 0; i < AUDIO_WavesCount; i++) {
			if (AUDIO_Waves[i].track && !MIX_PlayTrack(AUDIO_Waves[i].track, AUDIO_Waves[i].looping ? AUDIO_LoopTrack : 0)) {
		   		ERROR_Set("Failed playing wave: %s", SDL_GetError());
		  		MAIN_Exit(SDL_APP_FAILURE);
		   	}
		}
	}
}

void AUDIO_ResumeWave(int num)
{
	if (AUDIO_WavesCount == 0 || num >= AUDIO_WavesCount) {
		return;
	}

	if (num >= 0) {
		if (AUDIO_Waves[num].track && !MIX_ResumeTrack(AUDIO_Waves[num].track)) {
			ERROR_Set("Failed resuming wave: %s", SDL_GetError());
			MAIN_Exit(SDL_APP_FAILURE);
		}
	}
	else if (!MIX_ResumeAllTracks(AUDIO_Mixer)) {
		ERROR_Set("Failed resuming all waves: %s", SDL_GetError());
		MAIN_Exit(SDL_APP_FAILURE);
	}
}

void AUDIO_StopWave(int num)
{
	if (AUDIO_WavesCount == 0 || num >= AUDIO_WavesCount) {
		return;
	}

	if (num >= 0) {
		if (AUDIO_Waves[num].track && !MIX_StopTrack(AUDIO_Waves[num].track, 0)) {
			ERROR_Set("Failed stopping wave: %s", SDL_GetError());
			MAIN_Exit(SDL_APP_FAILURE);
		}
	}
	else if (!MIX_StopAllTracks(AUDIO_Mixer, 0)) {
		ERROR_Set("Failed stopping all waves: %s", SDL_GetError());
		MAIN_Exit(SDL_APP_FAILURE);
	}
}

void AUDIO_PauseWave(int num)
{
	if (AUDIO_WavesCount == 0 || num >= AUDIO_WavesCount) {
		return;
	}

	if (num >= 0) {
		if (AUDIO_Waves[num].track && !MIX_PauseTrack(AUDIO_Waves[num].track)) {
			ERROR_Set("Failed pausing wave: %s", SDL_GetError());
			MAIN_Exit(SDL_APP_FAILURE);
		}
	}
	else if (!MIX_PauseAllTracks(AUDIO_Mixer)) {
		ERROR_Set("Failed pausing all waves: %s", SDL_GetError());
		MAIN_Exit(SDL_APP_FAILURE);
	}
}

bool AUDIO_IsWavePlaying(int num)
{
	if (AUDIO_WavesCount == 0 || num >= AUDIO_WavesCount) {
		return false;
	}

	if (num >= 0) {
		return AUDIO_Waves[num].track != NULL && MIX_TrackPlaying(AUDIO_Waves[num].track);
	}
	else {
		for (int i = 0; i < AUDIO_WavesCount; i++) {
			if (AUDIO_Waves[num].track && MIX_TrackPlaying(AUDIO_Waves[num].track)) {
				return true;
			}
		}
		return false;
	}
}

void AUDIO_SetWaveVolume(int num, int volume)
{
	if (AUDIO_WavesCount == 0 || num >= AUDIO_WavesCount) {
		return;
	}

	const float gain = SDL_clamp(volume, 0, 100) / 100.0f;
	if (num >= 0) {
		if (AUDIO_Waves[num].track && !MIX_SetTrackGain(AUDIO_Waves[num].track, gain)) {
			ERROR_Set("Failed setting wave volume: %s", SDL_GetError());
			MAIN_Exit(SDL_APP_FAILURE);
		}
	}
	else {
		for (int i = 0; i < AUDIO_WavesCount; i++) {
			if (AUDIO_Waves[num].track && !MIX_SetTrackGain(AUDIO_Waves[num].track, gain)) {
				ERROR_Set("Failed setting waves volume: %s", SDL_GetError());
				MAIN_Exit(SDL_APP_FAILURE);
			}
		}
	}
}

void AUDIO_SetWaveLooping(int num, bool looping)
{
	if (AUDIO_WavesCount == 0 || num >= AUDIO_WavesCount) {
		return;
	}

	if (num >= 0) {
	        if (!AUDIO_Waves[num].track || AUDIO_Waves[num].looping == looping) {
                        return;
                }

		AUDIO_Waves[num].looping = looping;
		if (!MIX_SetTrackLoops(AUDIO_Waves[num].track, looping ? -1 : 0)) {
			ERROR_Set("Failed setting wave loop mode: %s", SDL_GetError());
			MAIN_Exit(SDL_APP_FAILURE);
		}
	}
	else {
		for (int i = 0; i < AUDIO_WavesCount; i++) {
		        if (!AUDIO_Waves[i].track || AUDIO_Waves[i].looping == looping) {
			        continue;
			}

			AUDIO_Waves[i].looping = looping;
			if (!MIX_SetTrackLoops(AUDIO_Waves[i].track, looping ? -1 : 0)) {
				ERROR_Set("Failed setting waves loop mode: %s", SDL_GetError());
				MAIN_Exit(SDL_APP_FAILURE);
			}
		}
	}
}

void AUDIO_LoadMusic(const char* filename)
{
        if (AUDIO_Music) {
                MIX_DestroyTrack(AUDIO_Music);
                AUDIO_Music = NULL;
        }

	if (!AUDIO_LoadTrack(&AUDIO_Music, filename)) {
	        MAIN_Exit(SDL_APP_FAILURE);
	}
}

void AUDIO_PlayMusic(void)
{
        if (AUDIO_Music && !MIX_PlayTrack(AUDIO_Music, AUDIO_LoopTrack)) {
                MAIN_Exit(SDL_APP_FAILURE);
        }
}

void AUDIO_ResumeMusic(void)
{
        if (AUDIO_Music && !MIX_ResumeTrack(AUDIO_Music)) {
                MAIN_Exit(SDL_APP_FAILURE);
        }
}

void AUDIO_StopMusic(void)
{
        if (AUDIO_Music && !MIX_StopTrack(AUDIO_Music, 0)) {
                MAIN_Exit(SDL_APP_FAILURE);
        }
}

void AUDIO_PauseMusic(void)
{
        if (AUDIO_Music && !MIX_PauseTrack(AUDIO_Music)) {
                MAIN_Exit(SDL_APP_FAILURE);
        }
}

bool AUDIO_IsMusicPlaying(void)
{
        return AUDIO_Music != NULL && MIX_TrackPlaying(AUDIO_Music);
}

void AUDIO_SetMusicVolume(int volume)
{
        if (!AUDIO_Music) {
                return;
        }

        const float gain = SDL_clamp(volume, 0, 100) / 100.0f;
        if (!MIX_SetTrackGain(AUDIO_Music, gain)) {
                MAIN_Exit(SDL_APP_FAILURE);
        }
}
