#include "include.h"
#include "gamestart.h"

bool PollEvent(); // イベント処理

int main(int argc, char* argv[])
{
	/* SDLの初期化 */
	if ( SDL_Init(SDL_INIT_AUDIO | SDL_INIT_VIDEO | SDL_INIT_JOYSTICK) < 0 )
	{
		fprintf(stderr, "Couldn't initialize SDL: %s\n", SDL_GetError());
		return 1;
	}

	/* サウンドの初期化 */
	Mix_Init(
		MIX_INIT_FLAC |
		MIX_INIT_MOD |
		MIX_INIT_MP3 |
		MIX_INIT_OGG |
		MIX_INIT_MID |
		MIX_INIT_OPUS
	);
	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 800);
	Mix_AllocateChannels(100);

	gameMain();

	Mix_CloseAudio();

	SDL_Quit();

	return 0;
}
