#include "main_sdl/include.h"
#include "gamestart.h"

bool PollEvent(); // イベント処理

int main(int argc, char* argv[])
{
	/* SDLの初期化 */
	if ( SDL_Init(SDL_INIT_AUDIO | SDL_INIT_VIDEO | SDL_INIT_JOYSTICK) < 0 )
	{
		fprintf(stderr, "Couldn't initialize SDL: %s\n", SDL_GetError());
		return EXIT_FAILURE;
	}
	SDL_SetHint(SDL_HINT_RENDER_BATCHING, "1");

	/* 画像の初期化 */
	IMG_Init(
		IMG_INIT_JPG |
		IMG_INIT_PNG |
		IMG_INIT_TIF |
		IMG_INIT_WEBP
	);

	/* サウンドの初期化 */
	Mix_Init(
		MIX_INIT_FLAC |
		MIX_INIT_MOD |
		MIX_INIT_MP3 |
		MIX_INIT_OGG |
		MIX_INIT_MID |
		MIX_INIT_OPUS
	);
	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024);
	Mix_AllocateChannels(100);

	gameMain();

	/* 辞める */
	Mix_CloseAudio();
	Mix_Quit();
	IMG_Quit();
	SDL_Quit();
	return EXIT_SUCCESS;
}
