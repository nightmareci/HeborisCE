#include "main_sdl/include.h"
#include "SDL_kanji.h"
#include "ygs2kfunc.h"
#include "ygs2kprivate.h"
#include "filesystem.h"
#include "nanotime.h"

#define		YGS_GAME_CAPTION		"HEBORIS C7-EX SDL2"

typedef struct YGS2kSTextLayer
{
	bool	enable;
	int	x;
	int	y;
	int	r, g, b;
	int	size;
	char	string[256];
} YGS2kSTextLayer;

typedef enum YGS2kESoundType
{
	YGS_SOUNDTYPE_NONE,
	YGS_SOUNDTYPE_WAV,
	YGS_SOUNDTYPE_MUS,
} YGS2kESoundType;

static bool			s_bInitFast = false;

static SDL_Window		*s_pScreenWindow = NULL;
static SDL_Renderer		*s_pScreenRenderer = NULL;
static SDL_Texture		*s_pScreenRenderTarget = NULL;
static float			s_fScreenSubpixelOffset = 0.0f;

static SDL_Texture		*s_pYGSTexture[YGS_TEXTURE_MAX];

static int			s_iYGSSoundType[YGS_SOUND_MAX];
static Mix_Chunk		*s_pYGSSound[YGS_SOUND_MAX];
static int			s_iYGSSoundVolume[YGS_SOUND_MAX];
static Mix_Music		*s_pYGSExMusic[YGS_SOUND_MAX];
static Mix_Music		*s_pYGSMusic;
static int			s_iYGSMusicVolume;
static bool s_bWaveFormatSupported[YGS_WAVE_MAXFORMAT];

#define		YGS_KANJIFONTFILE_MAX	3
static Kanji_Font		*s_pKanjiFont[YGS_KANJIFONTFILE_MAX];

static int			s_iLogicalWidth;
static int			s_iLogicalHeight;

static bool			s_bNoFrameskip;
static nanotime_step_data		s_StepData = { 0 };
static bool		s_bLastFrameSkipped;
static uint64_t		s_uFPSCount;
static unsigned int		s_uFPSCnting;
static unsigned int		s_uFPS;
static unsigned int		s_uNowFPS;

static YGS2kSTextLayer		s_TextLayer[YGS_TEXTLAYER_MAX];
static int32_t			s_iScreenMode;
static int32_t			s_iScreenIndex;

static int			s_iNewOffsetX = 0, s_iNewOffsetY = 0;
static int			s_iOffsetX = 0, s_iOffsetY = 0;

static int quitLevel;

static void YGS2kPrivateTextBlt(int x, int y, const char* text, int r, int g, int b, int size);
static void YGS2kPrivateKanjiDraw(int x, int y, int r, int g, int b, int size, const char *str);
static void YGS2kPrivateKanjiFontFinalize();
static void YGS2kPrivateKanjiFontInitialize();

static float YGS2kGetScreenSubpixelOffset()
{
	// The returned subpixel offset nudges all draws to have pixel coordinates
	// that end up centered in the floating point coordinate space. Without
	// this offset, pixel coordinates are at the upper left of the intended
	// pixels, resulting in off-by-one drawing errors sometimes.
	//
	// The numerator of the return value is the fraction of a pixel to adjust
	// draw position rightwards and downwards, and the division by the current
	// scale converts that subpixel amount to the amount to adjust by within
	// the currently set logical resolution (320x240 or 640x480), so it adjusts
	// by less than 0.375 if the render resolution is above logical resolution,
	// or greater than 0.375 if below logical resolution. If the numerator is
	// exactly 0.5f, then system-dependent rounding errors can occur, because
	// the coordinate is located exactly in the pixel center, producing
	// sampling artifacts on some systems, but not on others. By using a
	// numerator less than 0.5f, the rounding of draw coordinates by the
	// graphics implementation should place drawn pixels in the center. I also
	// saw scaling artifacts happen with a numerator of 0.25f, so maybe the
	// numerator needs to be somewhere in the open range (0.25f, 0.5f) to
	// likely work everywhere; for now, the midpoint, 0.375f, is used, and
	// seems to work correctly on some systems I've tested.
	//
	// Getting the scale from SDL_RenderGetScale is always appropriate here,
	// being an integer value when integer scaling is in effect, or a
	// non-integer scale when fill-screen scaling is in effect. Even if the
	// scale value is below 1.0f, the formula is still correct.
	//
	// -Brandon McGriff <nightmareci@gmail.com>
	if ( s_pScreenRenderer )
	{
		float scale;
		SDL_RenderGetScale(s_pScreenRenderer, &scale, NULL);
		return 0.375f / scale;
	}
	else
	{
		return 0.0f;
	}
}

bool YGS2kInit()
{
	quitLevel = 0;

	/* SDLの初期化 || SDL initialization */
	if ( SDL_Init(
		SDL_INIT_AUDIO | SDL_INIT_VIDEO
		#ifdef ENABLE_JOYSTICK
		| SDL_INIT_JOYSTICK
		#endif
		#ifdef ENABLE_GAME_CONTROLLER
		| SDL_INIT_GAMECONTROLLER
		#endif
	) < 0 )
	{
		fprintf(stderr, "Couldn't initialize SDL: %s\n", SDL_GetError());
		YGS2kExit(EXIT_FAILURE);
	}

	// If this fails, it doesn't matter, the game will still work. But it's
	// called because if it works, the game might perform better.
	SDL_SetThreadPriority(SDL_THREAD_PRIORITY_HIGH);

	quitLevel++;
	SDL_SetHint(SDL_HINT_RENDER_BATCHING, "1");

	/* 画像の初期化 || Image initialization */
	if ( IMG_Init(IMG_INIT_PNG) != IMG_INIT_PNG )
	{
		fprintf(stderr, "Couldn't initialize image support: %s\n", IMG_GetError());
		YGS2kExit(EXIT_FAILURE);
	}
	quitLevel++;

	/* サウンドの初期化 || Sound initialization */
	const int formatsInitialized = Mix_Init(
		MIX_INIT_MID |
		MIX_INIT_OGG |
		MIX_INIT_MP3 |
		MIX_INIT_FLAC |
		MIX_INIT_OPUS |
		MIX_INIT_MOD
	);
	if ( !formatsInitialized )
	{
		fprintf(stderr, "Couldn't initialize sound support: %s\n", Mix_GetError());
		YGS2kExit(EXIT_FAILURE);
	}
	quitLevel++;

	// TODO: Refactor so this source file doesn't access anything in the
	// game code.
	s_bWaveFormatSupported[YGS_WAVE_MID] = !!(formatsInitialized & MIX_INIT_MID);
	s_bWaveFormatSupported[YGS_WAVE_WAV] = 1; // WAVEはいつでも利用可能 || WAVE is always supported
	s_bWaveFormatSupported[YGS_WAVE_OGG] = !!(formatsInitialized & MIX_INIT_OGG);
	s_bWaveFormatSupported[YGS_WAVE_MP3] = !!(formatsInitialized & MIX_INIT_MP3);
	s_bWaveFormatSupported[YGS_WAVE_FLAC] = !!(formatsInitialized & MIX_INIT_FLAC);
	s_bWaveFormatSupported[YGS_WAVE_OPUS] = !!(formatsInitialized & MIX_INIT_OPUS);
	s_bWaveFormatSupported[YGS_WAVE_MOD] = !!(formatsInitialized & MIX_INIT_MOD);
	s_bWaveFormatSupported[YGS_WAVE_IT] = !!(formatsInitialized & MIX_INIT_MOD);
	s_bWaveFormatSupported[YGS_WAVE_XM] = !!(formatsInitialized & MIX_INIT_MOD);
	s_bWaveFormatSupported[YGS_WAVE_S3M] = !!(formatsInitialized & MIX_INIT_MOD);

#ifdef __EMSCRIPTEN__
	// In testing on desktop Linux, Firefox seems to cope with a 1024 byte
	// buffer fine, but Chrome produces a fair bit of audio breakup. 2048
	// seems to work fine in both, though.
	if ( Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0 )
#else
	// A 1024 byte buffer seems to be a good choice for all native code
	// ports. It's reasonably low latency but doesn't result in audio
	// breakup.
	if ( Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024) < 0 )
#endif
	{
		fprintf(stderr, "Couldn't open audio: %s\n", Mix_GetError());
		YGS2kExit(EXIT_FAILURE);
	}
	quitLevel++;

	Mix_AllocateChannels(100);

	int		configChanged = 0;

	s_iNewOffsetX = 0;	s_iNewOffsetY = 0;
	s_iOffsetX = 0;		s_iOffsetY = 0;

	YGS2kSetFPS(60);

	if (!s_bInitFast) YGS2kInputOpen();

    /* テクスチャ領域の初期化 */
    if (!s_bInitFast) memset(s_pYGSTexture, 0, sizeof(s_pYGSTexture));

	/* サウンドの初期化 */
	for ( int i = 0 ; i < YGS_SOUND_MAX ; i ++ )
	{
		if (!s_bInitFast) s_iYGSSoundType[i] = YGS_SOUNDTYPE_NONE;
		s_iYGSSoundVolume[i] = 0;
		if (!s_bInitFast) s_pYGSSound[i] = NULL;
		if (!s_bInitFast) s_pYGSExMusic[i] = NULL;
	}

	if (!s_bInitFast) s_pYGSMusic = NULL;
	s_iYGSMusicVolume = 0;

	/* テキストレイヤーの初期化 */
	for ( int i = 0 ; i < YGS_TEXTLAYER_MAX ; i ++ )
	{
		memset(&s_TextLayer[i], 0, sizeof(YGS2kSTextLayer));
		s_TextLayer[i].r = s_TextLayer[i].g = s_TextLayer[i].b = 255;
		s_TextLayer[i].size = 16;
	}

	if (!s_bInitFast) YGS2kPrivateKanjiFontInitialize();

	s_bLastFrameSkipped = false;
	s_uFPSCount = 0u;
	s_uFPS			= 0;
	s_bNoFrameskip		= false;

	srand((unsigned)time(NULL));

	s_bInitFast = true;
	
	return true;
}

void YGS2kExit(int exitStatus)
{
	YGS2kInputClose();

	/* サウンドの解放 */
	for ( int i = 0 ; i < YGS_SOUND_MAX ; i ++ )
	{
		s_iYGSSoundType[i] = YGS_SOUNDTYPE_NONE;

		if ( s_pYGSSound[i] )
		{
			Mix_FreeChunk(s_pYGSSound[i]);
			s_pYGSSound[i] = NULL;
		}
		if ( s_pYGSExMusic[i] )
		{
			Mix_FreeMusic(s_pYGSExMusic[i]);
			s_pYGSExMusic[i] = NULL;
		}
	}
	if ( s_pYGSMusic )
	{
		Mix_FreeMusic(s_pYGSMusic);
		s_pYGSMusic = NULL;
	}

	/* テクスチャ領域の解放 */
	for ( int i = 0 ; i < YGS_TEXTURE_MAX ; i ++ )
	{
		if ( s_pYGSTexture[i] )
		{
			SDL_DestroyTexture(s_pYGSTexture[i]);
			s_pYGSTexture[i] = NULL;
		}
	}
	if ( s_pScreenRenderer ) {
		SDL_SetRenderTarget( s_pScreenRenderer, NULL );
	}
	if ( s_pScreenRenderTarget ) {
		SDL_DestroyTexture( s_pScreenRenderTarget );
		s_pScreenRenderTarget = NULL;
	}
	if ( s_pScreenRenderer ) {
		SDL_DestroyRenderer( s_pScreenRenderer );
		s_pScreenRenderer = NULL;
	}

	if ( s_pScreenWindow ) {
		SDL_DestroyWindow( s_pScreenWindow );
		s_pScreenWindow = NULL;
	}


	YGS2kPrivateKanjiFontFinalize();

	switch ( quitLevel )
	{
	case 5: FSDeInit();
	case 4: Mix_CloseAudio();
	case 3: Mix_Quit();
	case 2: IMG_Quit();
	case 1: SDL_Quit();
	default: break;
	}
	exit(exitStatus);
}

bool YGS2kHalt()
{
	SDL_Event	ev;

	if ( s_pScreenRenderer )
	{
		SDL_RenderFlush( s_pScreenRenderer );

		#ifndef NDEBUG
		s_bNoFrameskip = true;
		#endif
		if ( s_bNoFrameskip )
		{
			/* バックサーフェスをフロントに転送 */
			if ( s_pScreenRenderTarget )
			{
				SDL_SetRenderTarget(s_pScreenRenderer, NULL);
				SDL_RenderClear( s_pScreenRenderer );
				SDL_RenderCopy(s_pScreenRenderer, s_pScreenRenderTarget, NULL, NULL);
				SDL_RenderPresent(s_pScreenRenderer);
				SDL_SetRenderTarget(s_pScreenRenderer, s_pScreenRenderTarget);
			}
			else {
				SDL_RenderPresent(s_pScreenRenderer);
			}

			/* フレームレート待ち */
			s_bLastFrameSkipped = !nanotime_step(&s_StepData);

			/* 画面塗りつぶし */
			SDL_RenderClear( s_pScreenRenderer );
		}
		else
		{
			if ( !s_bLastFrameSkipped )
			{
				/* バックサーフェスをフロントに転送 */
				if ( s_pScreenRenderTarget )
				{
					SDL_SetRenderTarget(s_pScreenRenderer, NULL);
					SDL_RenderClear( s_pScreenRenderer );
					SDL_RenderCopy(s_pScreenRenderer, s_pScreenRenderTarget, NULL, NULL);
					SDL_RenderPresent(s_pScreenRenderer);
					SDL_SetRenderTarget(s_pScreenRenderer, s_pScreenRenderTarget);
				}
				else {
					SDL_RenderPresent(s_pScreenRenderer);
				}

				/* 画面塗りつぶし */
				SDL_RenderClear( s_pScreenRenderer );
			}

			/* フレームレート待ち */
			s_bLastFrameSkipped = !nanotime_step(&s_StepData);
		}
	}

	/* フレームレート計算 */
	s_uFPSCnting ++;

	if ( nanotime_interval(s_uFPSCount, nanotime_now(), s_StepData.now_max) >= NANOTIME_NSEC_PER_SEC )
	{
		s_uFPS = s_uFPSCnting;
		s_uFPSCnting = 0;
		s_uFPSCount = nanotime_now();
	}

	/* イベント処理 */
	SDL_PumpEvents();
	#ifdef ENABLE_JOYSTICK
	bool joyChanged = false;
	#endif
	#ifdef ENABLE_GAME_CONTROLLER
	bool conChanged = false;
	#endif
	while (SDL_PeepEvents(&ev, 1, SDL_GETEVENT, 0, SDL_LASTEVENT) == 1)
	{
		switch(ev.type){
			case SDL_QUIT:						// ウィンドウの×ボタンが押された時など
				return false;

			case SDL_WINDOWEVENT:
				if (ev.window.event == SDL_WINDOWEVENT_SIZE_CHANGED) {
					s_fScreenSubpixelOffset = SCREEN_SUBPIXEL_OFFSET;
				}
				break;

			#ifdef ENABLE_JOYSTICK
			case SDL_JOYDEVICEADDED:
			case SDL_JOYDEVICEREMOVED:
				joyChanged = true;
			#endif

			#ifdef ENABLE_GAME_CONTROLLER
			case SDL_CONTROLLERDEVICEADDED:
			case SDL_CONTROLLERDEVICEREMOVED:
				conChanged = true;
				break;
			#endif

			default:
				break;
		}
	}

	#ifdef ENABLE_JOYSTICK
	if (joyChanged)
	{
		YGS2kJoyClose();
		YGS2kJoyOpen();
	}
	#endif
		
	#ifdef ENABLE_GAME_CONTROLLER
	if (conChanged)
	{
		YGS2kConClose();
		YGS2kConOpen();
	}
	#endif

	/* 画面ずらし量の反映 */
	s_iOffsetX = s_iNewOffsetX;
	s_iOffsetY = s_iNewOffsetY;

	return true;
}

static void YGS2kPrivateTextBlt(int x, int y, const char* text, int r, int g, int b, int size)
{
	YGS2kPrivateKanjiDraw(x, y, r, g, b, size, text);
}

int YGS2kIsPlayMusic()
{
	return Mix_PlayingMusic();
}

#ifdef __EMSCRIPTEN__
static EM_BOOL YGS2kEmscriptenResizeCallback(int eventType, const EmscriptenUiEvent* uiEvent, void* userData)
{
	if (
		eventType != EMSCRIPTEN_EVENT_RESIZE ||
		!s_pScreenWindow
	) {
		return false;
	}

	SDL_SetWindowSize(s_pScreenWindow, uiEvent->windowInnerWidth, uiEvent->windowInnerHeight);
	return true;
}
#endif

int YGS2kSetScreen(int32_t *screenMode, int32_t *screenIndex)
{
	Uint32		windowFlags = SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE;
	int		windowX, windowY;
	int		logicalWidth, logicalHeight;

	/* 画面の設定 */
	YGS2kEScreenModeFlag windowType = *screenMode & YGS_SCREENMODE_WINDOWTYPE;
	if (windowType < 0 || windowType >= YGS_SCREENMODE_NUMWINDOWTYPES) {
		s_pScreenRenderTarget = NULL;
		s_pScreenRenderer = NULL;
		s_pScreenWindow = NULL;
		return 0;
	}
	int displayIndex = YGS_SCREENINDEX_DISPLAY_TOVALUE(*screenIndex);
	int modeIndex = YGS_SCREENINDEX_MODE_TOVALUE(*screenIndex);
	int numVideoDisplays = SDL_GetNumVideoDisplays();
	int numDisplayModes = SDL_GetNumDisplayModes(displayIndex);
	if (numVideoDisplays < 0 || numDisplayModes < 0) {
		s_pScreenRenderTarget = NULL;
		s_pScreenRenderer = NULL;
		s_pScreenWindow = NULL;
		return 0;
	}

	if (
			displayIndex >= numVideoDisplays ||
			((windowType == YGS_SCREENMODE_FULLSCREEN || windowType == YGS_SCREENMODE_FULLSCREEN_DESKTOP) && modeIndex >= numDisplayModes)
	) {
		*screenMode = DEFAULT_SCREEN_MODE;
		*screenIndex = 0;
		windowX = SDL_WINDOWPOS_CENTERED_DISPLAY(*screenIndex);
		windowY = SDL_WINDOWPOS_CENTERED_DISPLAY(*screenIndex);
	}
	else
	{
		windowX = SDL_WINDOWPOS_CENTERED_DISPLAY(displayIndex);
		windowY = SDL_WINDOWPOS_CENTERED_DISPLAY(displayIndex);
		switch ( windowType & YGS_SCREENMODE_WINDOWTYPE )
		{
		case YGS_SCREENMODE_WINDOW_MAXIMIZED:
			windowFlags |= SDL_WINDOW_MAXIMIZED;
			break;

		case YGS_SCREENMODE_FULLSCREEN_DESKTOP:
			windowFlags |= SDL_WINDOW_FULLSCREEN_DESKTOP;
			break;

		case YGS_SCREENMODE_FULLSCREEN: {
			windowFlags |= SDL_WINDOW_FULLSCREEN;
			break;
		}
		}
	}

	if ( *screenMode & YGS_SCREENMODE_DETAILLEVEL )
	{
		logicalWidth  = 640;
		logicalHeight = 480;
	}
	else
	{
		logicalWidth  = 320;
		logicalHeight = 240;
	}

	s_iLogicalWidth  = logicalWidth;
	s_iLogicalHeight = logicalHeight;

	/* ウィンドウの作成 */
	if (
		windowType == YGS_SCREENMODE_FULLSCREEN ||
		windowType == YGS_SCREENMODE_FULLSCREEN_DESKTOP
	)
	{
		SDL_DisplayMode displayMode;
		int status;
		if ( windowType == YGS_SCREENMODE_FULLSCREEN )
		{
			status = SDL_GetDisplayMode(displayIndex, modeIndex, &displayMode);
		}
		else
		{
			status = SDL_GetDesktopDisplayMode(displayIndex, &displayMode);
		}
		if ( status < 0)
		{
			*screenMode &= ~YGS_SCREENMODE_WINDOWTYPE;
			*screenMode |= YGS_SCREENMODE_WINDOW;
			*screenIndex = 0;
			if ( !s_pScreenWindow )
			{
				s_pScreenWindow = SDL_CreateWindow(YGS_GAME_CAPTION, SDL_WINDOWPOS_CENTERED_DISPLAY(displayIndex), SDL_WINDOWPOS_CENTERED_DISPLAY(displayIndex), logicalWidth, logicalHeight, SDL_WINDOW_HIDDEN |
				SDL_WINDOW_RESIZABLE
				);
				if ( !s_pScreenWindow )
				{
					s_pScreenRenderTarget = NULL;
					s_pScreenRenderer = NULL;
					s_pScreenWindow = NULL;
					return 0;
				}
			}
			else
			{
				SDL_RestoreWindow(s_pScreenWindow);
				SDL_SetWindowSize(s_pScreenWindow, logicalWidth, logicalHeight);
				SDL_SetWindowResizable(s_pScreenWindow, SDL_TRUE);
				SDL_SetWindowPosition(s_pScreenWindow, SDL_WINDOWPOS_CENTERED_DISPLAY(displayIndex), SDL_WINDOWPOS_CENTERED_DISPLAY(displayIndex));
				if (SDL_SetWindowFullscreen(s_pScreenWindow, 0u) < 0)
				{
					SDL_DestroyWindow(s_pScreenWindow);
					s_pScreenRenderTarget = NULL;
					s_pScreenRenderer = NULL;
					s_pScreenWindow = NULL;
					return 0;
				}
			}
		}
		else {
			if ( !s_pScreenWindow )
			{
				s_pScreenWindow = SDL_CreateWindow(YGS_GAME_CAPTION, windowX, windowY, displayMode.w, displayMode.h, windowFlags);
				if ( !s_pScreenWindow )
				{
					s_pScreenRenderTarget = NULL;
					s_pScreenRenderer = NULL;
					s_pScreenWindow = NULL;
					return 0;
				}
			}
			else
			{
				SDL_SetWindowPosition(s_pScreenWindow, windowX, windowY);
				int fullscreenError = -1;
				if ((windowFlags & SDL_WINDOW_FULLSCREEN_DESKTOP) == SDL_WINDOW_FULLSCREEN_DESKTOP)
				{
					fullscreenError = SDL_SetWindowFullscreen(s_pScreenWindow, SDL_WINDOW_FULLSCREEN_DESKTOP);
				}
				else if ((windowFlags & SDL_WINDOW_FULLSCREEN) == SDL_WINDOW_FULLSCREEN)
				{
					fullscreenError = SDL_SetWindowFullscreen(s_pScreenWindow, SDL_WINDOW_FULLSCREEN);
				}
				if (fullscreenError < 0)
				{
					if (s_pScreenRenderTarget) SDL_DestroyTexture(s_pScreenRenderTarget);
					SDL_DestroyRenderer(s_pScreenRenderer);
					SDL_DestroyWindow(s_pScreenWindow);

					s_pScreenRenderTarget = NULL;
					s_pScreenRenderer = NULL;
					s_pScreenWindow = NULL;

					return 0;
				}
			}
			if ( SDL_SetWindowDisplayMode(s_pScreenWindow, &displayMode) < 0 )
			{
				if (s_pScreenRenderTarget) SDL_DestroyTexture(s_pScreenRenderTarget);
				if (s_pScreenRenderer) SDL_DestroyRenderer(s_pScreenRenderer);
				SDL_DestroyWindow(s_pScreenWindow);

				s_pScreenRenderTarget = NULL;
				s_pScreenRenderer = NULL;
				s_pScreenWindow = NULL;

				return 0;
			}
		}
	}
	else if (
		windowType == YGS_SCREENMODE_WINDOW ||
		windowType == YGS_SCREENMODE_WINDOW_MAXIMIZED
	)
	{
		SDL_DisplayMode displayMode;
		if ( SDL_GetDesktopDisplayMode(displayIndex, &displayMode) < 0 )
		{
			s_pScreenRenderTarget = NULL;
			s_pScreenRenderer = NULL;
			s_pScreenWindow = NULL;
			return 0;
		}

		int maxScale;
		if(displayMode.w <= logicalWidth || displayMode.h <= logicalHeight)
		{
			maxScale = 1;
		}
		else if(displayMode.w > displayMode.h)
		{
			maxScale = (displayMode.h / logicalHeight) - (displayMode.h % logicalHeight == 0);
		}
		else
		{
			maxScale = (displayMode.w / logicalWidth) - (displayMode.w % logicalWidth == 0);
		}
		int maxWidth = maxScale * logicalWidth;
		int maxHeight = maxScale * logicalHeight;
		int scale = modeIndex + 1;
		int windowW = scale * logicalWidth;
		int windowH = scale * logicalHeight;
		if ( scale > maxScale) {
			windowW = logicalWidth;
			windowH = logicalHeight;
			*screenIndex = YGS_SCREENINDEX_TOSETTING(displayIndex, 0);
		}
		if ( !s_pScreenWindow )
		{
			s_pScreenWindow = SDL_CreateWindow(YGS_GAME_CAPTION, windowX, windowY, windowW, windowH, windowFlags);
			if ( !s_pScreenWindow )
			{
				s_pScreenRenderTarget = NULL;
				s_pScreenRenderer = NULL;
				s_pScreenWindow = NULL;
				return 0;
			}
		}
		else
		{
			if (windowFlags & SDL_WINDOW_MAXIMIZED) {
				SDL_MaximizeWindow(s_pScreenWindow);
			}
			else {
				SDL_RestoreWindow(s_pScreenWindow);
			}
			SDL_SetWindowResizable(s_pScreenWindow, SDL_TRUE);
			int fullscreenError = SDL_SetWindowFullscreen(s_pScreenWindow, 0);
			if (fullscreenError < 0)
			{
				SDL_DestroyWindow(s_pScreenWindow);
				s_pScreenRenderTarget = NULL;
				s_pScreenRenderer = NULL;
				s_pScreenWindow = NULL;
				return 0;
			}
			SDL_SetWindowSize(s_pScreenWindow, windowW, windowH);
			SDL_SetWindowPosition(s_pScreenWindow, windowX, windowY);
		}
	}

	// fix to allow rendering to the texture.
	if (!s_pScreenRenderer)
	{
		s_pScreenRenderer = SDL_CreateRenderer(s_pScreenWindow, -1, *screenMode & YGS_SCREENMODE_VSYNC ? SDL_RENDERER_PRESENTVSYNC : 0);
		if ( !s_pScreenRenderer )
		{
			SDL_DestroyWindow(s_pScreenWindow);
			s_pScreenRenderTarget = NULL;
			s_pScreenRenderer = NULL;
			s_pScreenWindow = NULL;
			return 0;
		}
	}
	else
	{
		if (SDL_RenderSetVSync(s_pScreenRenderer, !!(*screenMode & YGS_SCREENMODE_VSYNC))) {
			SDL_DestroyWindow(s_pScreenWindow);
			s_pScreenRenderTarget = NULL;
			s_pScreenRenderer = NULL;
			s_pScreenWindow = NULL;
			return 0;
		}
	}

	if ( *screenMode & YGS_SCREENMODE_SCALEMODE )
	{
		if ( SDL_RenderSetIntegerScale(s_pScreenRenderer, SDL_TRUE) < 0 )
		{
			SDL_DestroyRenderer(s_pScreenRenderer);
			SDL_DestroyWindow(s_pScreenWindow);
			s_pScreenRenderTarget = NULL;
			s_pScreenRenderer = NULL;
			s_pScreenWindow = NULL;
			return 0;
		}
		SDL_SetWindowMinimumSize(s_pScreenWindow, logicalWidth, logicalHeight);
	}
	else {
		if ( SDL_RenderSetIntegerScale(s_pScreenRenderer, SDL_FALSE) < 0 )
		{
			SDL_DestroyRenderer(s_pScreenRenderer);
			SDL_DestroyWindow(s_pScreenWindow);
			s_pScreenRenderTarget = NULL;
			s_pScreenRenderer = NULL;
			s_pScreenWindow = NULL;
			return 0;
		}
		SDL_SetWindowMinimumSize(s_pScreenWindow, 1, 1);
	}

	if ( SDL_RenderSetLogicalSize(s_pScreenRenderer, logicalWidth, logicalHeight) < 0 )
	{
		if (s_pScreenRenderTarget)
		{
			SDL_DestroyTexture(s_pScreenRenderTarget);
		}
		SDL_DestroyRenderer(s_pScreenRenderer);
		SDL_DestroyWindow(s_pScreenWindow);
		s_pScreenRenderTarget = NULL;
		s_pScreenRenderer = NULL;
		s_pScreenWindow = NULL;
		return 0;
	}

	s_fScreenSubpixelOffset = SCREEN_SUBPIXEL_OFFSET;
    // TODO: Figure out how to get render targets in Emscripten working with
	// SDL_BLENDMODE_BLEND textures. This #ifdef is a workaround for now. It's
	// probably a bug in the Emscripten SDL 2 port.
    #ifndef __EMSCRIPTEN__
	if ( SDL_RenderTargetSupported(s_pScreenRenderer) )
	{
		if ( !(*screenMode & YGS_SCREENMODE_RENDERLEVEL) )
		{
			if (s_pScreenRenderTarget)
			{
				SDL_DestroyTexture(s_pScreenRenderTarget);
			}
			s_pScreenRenderTarget = SDL_CreateTexture(s_pScreenRenderer, SDL_PIXELFORMAT_RGBX8888, SDL_TEXTUREACCESS_TARGET, logicalWidth, logicalHeight);
			if ( !s_pScreenRenderTarget )
			{
				SDL_DestroyRenderer(s_pScreenRenderer);
				SDL_DestroyWindow(s_pScreenWindow);
				s_pScreenRenderTarget = NULL;
				s_pScreenRenderer = NULL;
				s_pScreenWindow = NULL;
				 return 0;
			}
			if (
				SDL_RenderClear(s_pScreenRenderer) < 0 ||
				SDL_SetRenderTarget(s_pScreenRenderer, s_pScreenRenderTarget) < 0 ||
				SDL_RenderClear(s_pScreenRenderer) < 0
			)
			{
				SDL_DestroyTexture(s_pScreenRenderTarget);
				SDL_DestroyRenderer(s_pScreenRenderer);
				SDL_DestroyWindow(s_pScreenWindow);
				s_pScreenRenderTarget = NULL;
				s_pScreenRenderer = NULL;
				s_pScreenWindow = NULL;
				return 0;
			}
		}
		else if ( s_pScreenRenderTarget )
		{
			SDL_DestroyTexture(s_pScreenRenderTarget);
			s_pScreenRenderTarget = NULL;
		}
	}
	else
    #endif
    {
		s_pScreenRenderTarget = NULL;
		*screenMode |= YGS_SCREENMODE_RENDERLEVEL;
	}

	/* マウスカーソルを消す場合は */
	if ( SDL_ShowCursor( !(windowFlags & SDL_WINDOW_FULLSCREEN) ) < 0)
	{
		if (s_pScreenRenderTarget)
		{
			SDL_DestroyTexture(s_pScreenRenderTarget);		
		}
		SDL_DestroyRenderer(s_pScreenRenderer);
		SDL_DestroyWindow(s_pScreenWindow);
		s_pScreenRenderTarget = NULL;
		s_pScreenRenderer = NULL;
		s_pScreenWindow = NULL;
		return 0;
	}
	
	s_iScreenMode = *screenMode;
	s_iScreenIndex = *screenIndex;

	#ifdef __EMSCRIPTEN__
	if ( emscripten_set_resize_callback(EMSCRIPTEN_EVENT_TARGET_WINDOW, NULL, false, YGS2kEmscriptenResizeCallback) < 0 )
	{
		if (s_pScreenRenderTarget)
		{
			SDL_DestroyTexture(s_pScreenRenderTarget);
		}
		SDL_DestroyRenderer(s_pScreenRenderer);
		SDL_DestroyWindow(s_pScreenWindow);
		s_pScreenRenderTarget = NULL;
		s_pScreenRenderer = NULL;
		s_pScreenWindow = NULL;
		return 0;
	}

	{
		int width = -1, height = -1;
		width = EM_ASM_INT({ return window.innerWidth; });
		height = EM_ASM_INT({ return window.innerHeight; });
		if ( width > 0 && height > 0 )
		{
			SDL_SetWindowSize(s_pScreenWindow, width, height);
		}
	}
	#endif

	return 1;
}

int YGS2kGetMaxDisplayIndex()
{
	return SDL_GetNumVideoDisplays();
}

int YGS2kGetMaxDisplayMode( int displayIndex )
{
	return SDL_GetNumDisplayModes(displayIndex);
}

int YGS2kRenderLevelLowSupported()
{
	return s_pScreenRenderer ? SDL_RenderTargetSupported(s_pScreenRenderer) : 0;
}

void YGS2kSetConstParam ( const char *param, int value )
{
	// TODO
}

int YGS2kRand ( int max )
{
	return rand() % max;
}

void YGS2kPauseMusic()
{
	Mix_PauseMusic();
}

void YGS2kReplayMusic()
{
	Mix_ResumeMusic();
}

void YGS2kPlayWave ( int num )
{
	switch ( s_iYGSSoundType[num] )
	{
	case YGS_SOUNDTYPE_WAV:
		Mix_PlayChannel(num, s_pYGSSound[num], 0);
		Mix_Volume(num, s_iYGSSoundVolume[num]);
		break;

	case YGS_SOUNDTYPE_MUS:
		if ((num == 56) || (num == 57)) {
			Mix_PlayMusic(s_pYGSExMusic[num], 0);
		} else {
			Mix_PlayMusic(s_pYGSExMusic[num], -1);
		}
		Mix_VolumeMusic(s_iYGSSoundVolume[num]);
		break;

	default:
		break;
	}
}

void YGS2kReplayWave ( int num )
{
	switch ( s_iYGSSoundType[num] )
	{
	case YGS_SOUNDTYPE_WAV:
		Mix_Resume(num);
		break;

	case YGS_SOUNDTYPE_MUS:
		Mix_RewindMusic();
		break;
	
	default:
		break;
	}
}

void YGS2kStopWave ( int num )
{
	switch ( s_iYGSSoundType[num] )
	{
	case YGS_SOUNDTYPE_WAV:
		Mix_HaltChannel(num);
		break;

	case YGS_SOUNDTYPE_MUS:
		Mix_HaltMusic();
		break;
		
	default:
		break;
	}
}

void YGS2kPauseWave ( int num )
{
	switch ( s_iYGSSoundType[num] )
	{
	case YGS_SOUNDTYPE_WAV:
		Mix_Pause(num);
		break;

	case YGS_SOUNDTYPE_MUS:
		Mix_PauseMusic();
		break;

	default:
		break;
	}
}

void YGS2kSetVolumeWave( int num, int vol )
{
	int volume = (int)((vol / 100.0f) * YGS_VOLUME_MAX);
	if ( volume > YGS_VOLUME_MAX ) { volume = YGS_VOLUME_MAX; }
	if ( volume < 0 )   { volume = 0; }
	s_iYGSSoundVolume[num] = volume;

	switch ( s_iYGSSoundType[num] )
	{
	case YGS_SOUNDTYPE_WAV:
		Mix_Volume(num, volume);
		break;

	case YGS_SOUNDTYPE_MUS:
		Mix_VolumeMusic(volume);
		break;

	default:
		break;
	}
}

int YGS2kIsPlayWave( int num )
{
	switch ( s_iYGSSoundType[num] )
	{
	case YGS_SOUNDTYPE_WAV:
		/* なぜかここを実行すると落ちる… */
		return Mix_Playing(num);

	case YGS_SOUNDTYPE_MUS:
		return Mix_PlayingMusic();
		
	default:
		return 0;
	}
}

void YGS2kLoadWave( const char* filename, int num )
{
	int		len = strlen(filename);
	if ( len < 4 ) { return; }

	s_iYGSSoundType[num] = YGS_SOUNDTYPE_NONE;

	// 拡張子、または番号(50番以降がBGM)によって読み込み方法を変える
	// Change the loading method depending on the extension or number (numbers after 50 are BGM)
	SDL_RWops *src;
	src = FSOpenRead(filename);
	if ( !src ) return;
	if ( SDL_strcasecmp(&filename[len - 4], ".wav") || num >= 50 )
	{
		if ( s_pYGSExMusic[num] != NULL )
		{
			Mix_FreeMusic(s_pYGSExMusic[num]);
			s_pYGSExMusic[num] = NULL;
		}
		s_pYGSExMusic[num] = Mix_LoadMUS_RW(src, SDL_TRUE);
		s_iYGSSoundType[num] = YGS_SOUNDTYPE_MUS;
		s_iYGSSoundVolume[num] = YGS_VOLUME_MAX;
	}
	else
	{
		if ( s_pYGSSound[num] != NULL )
		{
			Mix_FreeChunk(s_pYGSSound[num]);
			s_pYGSSound[num] = NULL;
		}
		s_pYGSSound[num] = Mix_LoadWAV_RW(src, SDL_TRUE);
		s_iYGSSoundType[num] = YGS_SOUNDTYPE_WAV;
		s_iYGSSoundVolume[num] = YGS_VOLUME_MAX;
	}
}

void YGS2kSetLoopModeWave( int num, int mode )
{
   // true=loop, false=no loop.  since this is only used on BGMs, and all BGMs already loop, this is a no-op.
}

void YGS2kLoadMusic( const char* filename )
{
	if ( s_pYGSMusic )
	{
		Mix_FreeMusic(s_pYGSMusic);
		s_pYGSMusic = NULL;
	}

	SDL_RWops* src;
	if (!(src = FSOpenRead(filename))) return;
	s_pYGSMusic = Mix_LoadMUS_RW(src, SDL_TRUE);
	s_iYGSMusicVolume = YGS_VOLUME_MAX;
}

void YGS2kLoadBitmap( const char* filename, int plane, int val )
{
	if ( !s_pScreenRenderer )
	{
		return;
    }

	if ( s_pYGSTexture[plane] )
	{
		SDL_DestroyTexture(s_pYGSTexture[plane]);
		s_pYGSTexture[plane] = NULL;
	}

	SDL_RWops* src;
	if (!(src = FSOpenRead(filename))) return;
	if (!(s_pYGSTexture[plane] = IMG_LoadTexture_RW(s_pScreenRenderer, src, SDL_TRUE))) return;

	SDL_SetTextureBlendMode(s_pYGSTexture[plane], SDL_BLENDMODE_BLEND);
}

void YGS2kPlayMusic()
{
	if ( s_pYGSMusic )
	{
		Mix_PlayMusic(s_pYGSMusic, -1);
	}
}

void YGS2kStopMusic()
{
	Mix_HaltMusic();
}

void YGS2kSetVolumeMusic(int vol)
{
	int volume = (int)((vol / 100.0f) * YGS_VOLUME_MAX);
	if ( volume > YGS_VOLUME_MAX ) { volume = YGS_VOLUME_MAX; }
	if ( volume < 0 )   { volume = 0; }
	s_iYGSMusicVolume = volume;
	Mix_VolumeMusic(volume);
}

bool YGS2kWaveFormatSupported(YGS2kEWaveFormat format) {
	return s_bWaveFormatSupported[format & YGS_WAVE_FORMAT];
}

void YGS2kSetColorKeyPos(int plane, int x, int y)
{
	// TODO
	// sets transparent color to the specified pixel.  Since we use actual alpha channel in our assets, this is a no-oop
}

void YGS2kEnableBlendColorKey(int plane, int key)
{
	// TODO
	// alows for parial transparency.   again, because we use real transparency, it's a no-op.
}

void YGS2kCreateSurface(int surf, int w, int h)
{
	// TODO
	// required for orignal YGS2K engine. not needed at all for SDL2 renderer.
}

void YGS2kClearSecondary()
{
	// TODO
	// used to write the listed color to all pixels of the rendering area.
	// with SDL2 renderer, we never need to do this, so it's a no-op
}

void YGS2kSetFillColor(int col)
{
	// TODO
	// sets the color that YGS2kClearSecondary uses to fill the render target. since YGS2kClearSecondary is a no-op, so is this.
}

void YGS2kLoadFile( const char* filename, void* buf, size_t size )
{
	SDL_RWops	*src = FSOpenRead(filename);

	if ( src )
	{
		SDL_RWread(src, buf, size, 1);
		SDL_RWclose(src);

		/* エンディアン変換 */
		int32_t* buf2 = (int32_t*)buf;
		for ( size_t i = 0 ; i < size / sizeof(int32_t) ; i ++ )
		{
			buf2[i] = SWAP32(buf2[i]);
		}
	}
}

void YGS2kReadFile( const char* filename, void* buf, size_t size, size_t offset )
{
	SDL_RWops	*src = FSOpenRead(filename);

	if ( src )
	{
		if (SDL_RWseek(src, offset, RW_SEEK_SET) < 0) return;
		SDL_RWread(src, buf, size, 1);
		SDL_RWclose(src);

		/* エンディアン変換 */
		int32_t* buf2 = (int32_t*)buf;
		for ( size_t i = 0 ; i < size / sizeof(int32_t) ; i ++ )
		{
			buf2[i] = SWAP32(buf2[i]);
		}
	}
}


void YGS2kSaveFile( const char* filename, void* buf, size_t size )
{
	/* エンディアン変換 */
	int32_t* buf2 = (int32_t*)buf;
	for ( size_t i = 0 ; i < size / sizeof(int32_t) ; i ++ )
	{
		buf2[i] = SWAP32(buf2[i]);
	}

	SDL_RWops	*dst = FSOpenWrite(filename);

	if ( dst )
	{
		SDL_RWwrite(dst, buf, size, 1);
		if ( SDL_RWclose(dst) < 0 )
		{
			fprintf(stderr, "Error closing: %s\n", SDL_GetError());
		}
		// TODO: Create a custom SDL_RWops object type for Emscripten and put this in that.
		#ifdef __EMSCRIPTEN__
		EM_ASM({
			FS.syncfs(function (err) {
				assert(!err);
			});
		});
		#endif
	}

	/* もどす */
	for ( size_t i = 0 ; i < size / sizeof(int32_t) ; i ++ )
	{
		buf2[i] = SWAP32(buf2[i]);
	}
}

void YGS2kAppendFile( const char* filename, void* buf, size_t size ) {
	/* エンディアン変換 */
	int32_t* buf2 = (int32_t*)buf;
	for ( size_t i = 0 ; i < size / sizeof(int32_t) ; i ++ )
	{
		buf2[i] = SWAP32(buf2[i]);
	}

	SDL_RWops	*dst = FSOpenAppend(filename);

	if ( dst )
	{
		SDL_RWwrite(dst, buf, size, 1);
		// TODO: Create a custom SDL_RWops object type for Emscripten and put this in that.
		SDL_RWclose(dst);
		#ifdef __EMSCRIPTEN__
		EM_ASM({
			FS.syncfs(function (err) {
				assert(!err);
			});
		});
		#endif
	}

	/* もどす */
	for ( size_t i = 0 ; i < size / sizeof(int32_t) ; i ++ )
	{
		buf2[i] = SWAP32(buf2[i]);
	}
}

void YGS2kTextLayerOn ( int layer, int x, int y )
{
	s_TextLayer[layer].enable = true;
	s_TextLayer[layer].x = x;
	s_TextLayer[layer].y = y;
}

void YGS2kTextMove ( int layer, int x, int y )
{
	s_TextLayer[layer].x = x;
	s_TextLayer[layer].y = y;
}

void YGS2kTextColor ( int layer, int r, int g, int b )
{
	s_TextLayer[layer].r = r;
	s_TextLayer[layer].g = g;
	s_TextLayer[layer].b = b;
}

void YGS2kTextBackColorDisable ( int layer )
{
	// TODO
	// turns off the shadow effect for text in the listed layer. since we don't even use said shadow effect to begin with, it's a no-op.
}

void YGS2kTextSize ( int layer, int size )
{
	// TODO
	s_TextLayer[layer].size = size;
}

void YGS2kTextHeight ( int layer, int height )
{
	// TODO
	// only used in flexdraw.c for ExTextHeight. But since ExTextHeight is unused, we don't need to bother implementing it. 
}

void YGS2kTextOut ( int layer, const char* text )
{
	strcpy(s_TextLayer[layer].string, text);
}

void YGS2kTextBlt ( int layer )
{
	if ( s_TextLayer[layer].enable )
	{
		YGS2kPrivateTextBlt(s_TextLayer[layer].x + s_iOffsetX, s_TextLayer[layer].y + s_iOffsetY, s_TextLayer[layer].string, s_TextLayer[layer].r, s_TextLayer[layer].g, s_TextLayer[layer].b, s_TextLayer[layer].size);
	}
}

void YGS2kTextLayerOff ( int layer )
{
	s_TextLayer[layer].enable = false;
}

void YGS2kBltAlways(bool always)
{
	s_bNoFrameskip = always;
}

void YGS2kBlt(int pno, int dx, int dy)
{
    if ( s_pYGSTexture[pno] == NULL ) { return; }
	int w, h;
	SDL_QueryTexture(s_pYGSTexture[pno], NULL, NULL, &w, &h);
	YGS2kBltRect(pno, dx, dy, 0, 0, w, h);
}

void YGS2kBltRect(int pno, int dx, int dy, int sx, int sy, int hx, int hy)
{
	if ( !s_pScreenRenderer )
	{
		return;
    }

    if ((pno > 99)&& s_pScreenRenderTarget) //  hack to use screen render target as source
	{
		SDL_Rect	src = { 0 };
		SDL_Rect	dst = { 0 };

		src.x = sx;			src.y = sy;
		src.w = hx;			src.h = hy;
		dst.x = dx + s_iOffsetX;	dst.y = dy + s_iOffsetY;
		dst.w = hx;			dst.h = hy;

		SDL_RenderCopy(s_pScreenRenderer, s_pScreenRenderTarget, &src, &dst);
		return;
	}
    if (pno > 99) return; // give up so check below isn't ran if we use the hack.
    if ( s_pYGSTexture[pno] == NULL ) return;

	if ( s_pScreenRenderTarget )
	{
		SDL_Rect	src = { 0 };
		SDL_Rect	dst = { 0 };

		src.x = sx;			src.y = sy;
		src.w = hx;			src.h = hy;
		dst.x = dx + s_iOffsetX;	dst.y = dy + s_iOffsetY;
		dst.w = hx;			dst.h = hy;

		SDL_RenderCopy(s_pScreenRenderer, s_pYGSTexture[pno], &src, &dst);
	}
	else
	{
		SDL_Rect	src = { 0 };
		SDL_FRect	dst = { s_fScreenSubpixelOffset, s_fScreenSubpixelOffset };

		src.x  = sx;			src.y  = sy;
		src.w  = hx;			src.h  = hy;
		dst.x += dx + s_iOffsetX;	dst.y += dy + s_iOffsetY;
		dst.w  = hx;			dst.h  = hy;

		SDL_RenderCopyF(s_pScreenRenderer, s_pYGSTexture[pno], &src, &dst);
	}
}

void YGS2kBltFast(int pno, int dx, int dy)
{
	YGS2kBlt(pno, dx, dy);
}

void YGS2kBltFastRect(int pno, int dx, int dy, int sx, int sy, int hx, int hy)
{
	YGS2kBltRect(pno, dx, dy, sx, sy, hx, hy);
}

void YGS2kBlendBlt(int pno, int dx, int dy, int ar, int ag, int ab, int br, int bg, int bb)
{
    if ( s_pYGSTexture[pno] == NULL ) return;

	SDL_SetTextureAlphaMod(s_pYGSTexture[pno], ar);
	YGS2kBlt(pno, dx, dy);
	SDL_SetTextureAlphaMod(s_pYGSTexture[pno], SDL_ALPHA_OPAQUE);
}

void YGS2kBlendBltRect(int pno, int dx, int dy, int sx, int sy, int hx, int hy, int ar, int ag, int ab, int br, int bg, int bb)
{
    if ( s_pYGSTexture[pno] == NULL ) return;

	SDL_SetTextureAlphaMod(s_pYGSTexture[pno], ar);
	YGS2kBltRect(pno, dx, dy, sx, sy, hx, hy);
	SDL_SetTextureAlphaMod(s_pYGSTexture[pno], SDL_ALPHA_OPAQUE);
}

void YGS2kBltR(int pno, int dx, int dy, int scx, int scy)
{
	YGS2kBltRectR(pno, dx, dy, 0, 0, s_iLogicalWidth, s_iLogicalHeight, scx, scy);
}

void YGS2kBltRectR(int pno, int dx, int dy, int sx, int sy, int hx, int hy, int scx, int scy)
{
	if ( !s_pScreenRenderer )
	{
		return;
	}

	if ((pno > 99)&& s_pScreenRenderTarget) //  hack to use screen render target as source
	{
		SDL_Rect	src = { 0 };
		SDL_Rect	dst = { 0 };

		src.x = sx;					src.y = sy;
		src.w = hx;					src.h = hy;
		dst.x = dx + s_iOffsetX;	dst.y = dy + s_iOffsetY;
		dst.w = hx * (scx / 65536.0f);
		dst.h = hy * (scy / 65536.0f);

		if ( src.w == 0 || src.h == 0 || dst.w == 0 || dst.h == 0 ) { return; }

		SDL_RenderCopy(s_pScreenRenderer, s_pScreenRenderTarget, &src, &dst);
		return;
	}
    if (pno > 99) return; // give up so check below isn't ran if we use the hack.
    if ( s_pYGSTexture[pno] == NULL ) return;

	// ちゃんと拡大して描画する
	if ( s_pScreenRenderTarget )
	{
		SDL_Rect	src = { 0 };
		SDL_Rect	dst = { 0 };

		src.x = sx;					src.y = sy;
		src.w = hx;					src.h = hy;
		dst.x = dx + s_iOffsetX;	dst.y = dy + s_iOffsetY;
		dst.w = hx * (scx / 65536.0f);
		dst.h = hy * (scy / 65536.0f);

		if ( src.w == 0 || src.h == 0 || dst.w == 0 || dst.h == 0 ) { return; }

		SDL_RenderCopy( s_pScreenRenderer, s_pYGSTexture[pno], &src, &dst );
	}
	else
	{
		SDL_Rect	src = { 0 };
		SDL_FRect	dst = { s_fScreenSubpixelOffset, s_fScreenSubpixelOffset };

		src.x  = sx;			src.y  = sy;
		src.w  = hx;			src.h  = hy;
		dst.x += dx + s_iOffsetX;	dst.y += dy + s_iOffsetY;
		dst.w  = (int)(hx * (scx / 65536.0f));
		dst.h  = (int)(hy * (scy / 65536.0f));

		if ( src.w == 0 || src.h == 0 || dst.w == 0 || dst.h == 0 ) { return; }

		SDL_RenderCopyF( s_pScreenRenderer, s_pYGSTexture[pno], &src, &dst );
	}
}

void YGS2kBltFastR(int pno, int dx, int dy, int scx, int scy)
{
	YGS2kBltR(pno, dx, dy, scx, scy);
}

void YGS2kBltFastRectR(int pno, int dx, int dy, int sx, int sy, int hx, int hy, int scx, int scy)
{
	YGS2kBltRectR(pno, dx, dy, sx, sy, hx, hy, scx, scy);
}

void YGS2kBltTrans(int pno, int dx, int dy)
{
	// TODO
	// completely unused.  so we don't need to care what it even does.
}

void YGS2kBlendBltR(int pno, int dx, int dy, int ar, int ag, int ab, int br, int bg, int bb, int scx, int scy)
{
	YGS2kBlendBltRectR(pno, dx, dy, 0, 0, s_iLogicalWidth, s_iLogicalHeight, ar, ag, ab, br, bg, bb, scx, scy);
}

void YGS2kBlendBltRectR(int pno, int dx, int dy, int sx, int sy, int hx, int hy, int ar, int ag, int ab, int br, int bg, int bb, int scx, int scy)
{
	if ( !s_pScreenRenderer )
	{
		return;
	}

	if ((pno > 99)&& s_pScreenRenderTarget) //  hack to use screen render target as source
	{
		SDL_Rect	src = { 0 };
		SDL_Rect	dst = { 0 };

		src.x = sx;					src.y = sy;
		src.w = hx;					src.h = hy;
		dst.x = dx + s_iOffsetX;	dst.y = dy + s_iOffsetY;
		dst.w = hx * (scx / 65536.0f);
		dst.h = hy * (scy / 65536.0f);

		if ( src.w == 0 || src.h == 0 || dst.w == 0 || dst.h == 0 ) { return; }

		SDL_SetTextureAlphaMod(s_pScreenRenderTarget, ar);
		SDL_RenderCopy(s_pScreenRenderer, s_pScreenRenderTarget, &src, &dst);
		SDL_SetTextureAlphaMod(s_pScreenRenderTarget, SDL_ALPHA_OPAQUE);		return;
	}
    if (pno > 99) return; // give up so check below isn't ran if we use the hack.
    if ( s_pYGSTexture[pno] == NULL ) return;

	// ちゃんと拡大して描画する
	if ( s_pScreenRenderTarget )
	{
		SDL_Rect	src = { 0 };
		SDL_Rect	dst = { 0 };

		src.x = sx;					src.y = sy;
		src.w = hx;					src.h = hy;
		dst.x = dx + s_iOffsetX;	dst.y = dy + s_iOffsetY;
		dst.w = hx * (scx / 65536.0f);
		dst.h = hy * (scy / 65536.0f);

		if ( src.w == 0 || src.h == 0 || dst.w == 0 || dst.h == 0 ) { return; }

		SDL_SetTextureAlphaMod(s_pYGSTexture[pno], ar);
		SDL_RenderCopy( s_pScreenRenderer, s_pYGSTexture[pno], &src, &dst );
		SDL_SetTextureAlphaMod(s_pYGSTexture[pno], SDL_ALPHA_OPAQUE);
	}
	else
	{
		SDL_Rect	src = { 0 };
		SDL_FRect	dst = { s_fScreenSubpixelOffset, s_fScreenSubpixelOffset };

		src.x  = sx;			src.y  = sy;
		src.w  = hx;			src.h  = hy;
		dst.x += dx + s_iOffsetX;	dst.y += dy + s_iOffsetY;
		dst.w  = hx * (scx / 65536.0f);
		dst.h  = hy * (scy / 65536.0f);

		if ( src.w == 0 || src.h == 0 || dst.w == 0 || dst.h == 0 ) { return; }

		SDL_SetTextureAlphaMod(s_pYGSTexture[pno], ar);
		SDL_RenderCopyF( s_pScreenRenderer, s_pYGSTexture[pno], &src, &dst );
		SDL_SetTextureAlphaMod(s_pYGSTexture[pno], SDL_ALPHA_OPAQUE);
	}
}

void YGS2kSetSecondaryOffset(int x, int y)
{
	s_iNewOffsetX = x;
	s_iNewOffsetY = y;
}

void YGS2kSetColorKeyRGB(int pno, int r, int g, int b)
{
	// TODO
	//  again because we have actual transparency in our assets, this is a no-op.
}

void YGS2kSwapToSecondary(int pno)
{
	 // swaps the rendering target with a layer.  not implemented because no one can figuer out HOW in SDL.
	 // Only required for EH-Final gimmick, which currently has a workaround.
	 // it's also used in the graphic loader, and the backgroud loader, but doesn't seem needed.
}

void YGS2kSetFPS(unsigned fps)
{
	if (fps == 0) {
		s_uNowFPS = 1u;
	}
	else {
		s_uNowFPS = fps;
	}
	nanotime_step_init(&s_StepData, NANOTIME_NSEC_PER_SEC / s_uNowFPS, nanotime_now_max(), nanotime_now, nanotime_sleep);
}

int YGS2kGetFPS()
{
	return s_uNowFPS;
}

int YGS2kGetRealFPS()
{
	return s_uFPS;
}

void YGS2kStrCpy(char *dest, const char *src)
{
	strcpy(dest, src);
}

void YGS2kStrCat(char *str1, const char *str2)
{
	strcat(str1, str2);
}

int YGS2kStrLen(const char *stri)
{
	return strlen(stri);
}

void YGS2kMidStr(const char *src, int start, int len, char *dest)
{
	int		i;
	for ( i = 0 ; i < len ; i ++ )
	{
		dest[i] = src[start - 1 + i];
	}
	dest[len] = '\0';
}

void YGS2kLeftStr(const char *src, int len, char *dest)
{
	YGS2kMidStr(src, 1, len, dest);
}

char YGS2kCharAt(const char *stri, int pos)
{
	return stri[pos];
}

int YGS2kValLong(const char *stri)
{
	return atoi(stri);
}

void YGS2kFillMemory(void* buf, int size, int val)
{
	memset(buf, val, size);
}

////////////////////////////////////////////////////

static void YGS2kPrivateKanjiFontInitialize()
{
	SDL_RWops *src;

	/* 10pxフォント読み込み */
	src = FSOpenRead("res/font/knj10.bdf");
	if ( src )
	{
		s_pKanjiFont[0] = Kanji_OpenFont(src, 10);
		SDL_RWclose(src);
	}
	else
	{
		s_pKanjiFont[0] = NULL;
	}
	if ( s_pKanjiFont[0] )
	{
		src = FSOpenRead("res/font/5x10a.bdf");
		if ( src ) {
			Kanji_AddFont(s_pKanjiFont[0], src);
			SDL_RWclose(src);
		}
	}
	else
	{
		/* フォントがない場合代替を使う */
		src = FSOpenRead("res/font/knj12.bdf");
		if ( src )
		{
			s_pKanjiFont[0] = Kanji_OpenFont(src, 10);
			SDL_RWclose(src);
		}
		else {
			s_pKanjiFont[0] = NULL;
		}
		if ( s_pKanjiFont[0] )
		{
			src = FSOpenRead("res/font/6x12a.bdf");
			if ( src ) {
				Kanji_AddFont(s_pKanjiFont[0], src);
				SDL_RWclose(src);
			}
		}
	}

	if ( s_pKanjiFont[0] )
	{
		Kanji_SetCodingSystem(s_pKanjiFont[0], KANJI_SJIS);
	}

	/* 12pxフォント読み込み */
	src = FSOpenRead("res/font/knj12.bdf");
	if ( src ) {
		s_pKanjiFont[1] = Kanji_OpenFont(src, 12);
		SDL_RWclose(src);
	}
	else
	{
		s_pKanjiFont[1] = NULL;
	}
	if ( s_pKanjiFont[1] )
	{
		src = FSOpenRead("res/font/6x12a.bdf");
		if ( src )
		{
			Kanji_AddFont(s_pKanjiFont[1], src);
			SDL_RWclose(src);
			Kanji_SetCodingSystem(s_pKanjiFont[1], KANJI_SJIS);
		}
	}

	/* 16pxフォント読み込み */
	src = FSOpenRead("res/font/knj16.bdf");
	if ( src )
	{
		s_pKanjiFont[2] = Kanji_OpenFont(src, 16);
		SDL_RWclose(src);
	}
	else
	{
		s_pKanjiFont[2] = NULL;
	}
	if ( s_pKanjiFont[2] )
	{
		src = FSOpenRead("res/font/8x16a.bdf");
		if ( src )
		{
			Kanji_AddFont(s_pKanjiFont[2], src);
			SDL_RWclose(src);
			Kanji_SetCodingSystem(s_pKanjiFont[2], KANJI_SJIS);
		}
	}
}

static void YGS2kPrivateKanjiFontFinalize()
{
	if ( s_pKanjiFont[0] ) { Kanji_CloseFont(s_pKanjiFont[0]); }
	if ( s_pKanjiFont[1] ) { Kanji_CloseFont(s_pKanjiFont[1]); }
	if ( s_pKanjiFont[2] ) { Kanji_CloseFont(s_pKanjiFont[2]); }
}

static void YGS2kPrivateKanjiDraw(int x, int y, int r, int g, int b, int size, const char *str)
{
	if ( !s_pScreenRenderer )
	{
		return;
	}

	SDL_Color col = { 0 };
	int		font = 0;

	if ( size >= 12 )
	{
		font ++;
	}
	if ( size >= 16 )
	{
		font ++;
	}

	col.r = r;
	col.g = g;
	col.b = b;

	if ( s_pKanjiFont[font] )
	{
		if ( s_pScreenRenderTarget )
		{
			Kanji_PutTextRenderer(s_pKanjiFont[font], x, y, 0.0f, 0.0f, s_pScreenRenderer, str, col);
		}
		else {
			Kanji_PutTextRenderer(s_pKanjiFont[font], x, y, s_fScreenSubpixelOffset, s_fScreenSubpixelOffset, s_pScreenRenderer, str, col);
		}
	}
}
