#ifndef MAIN_h_
#define MAIN_h_

#include <STD.h>

void MAIN_ResetFrameStep(void);

bool MAIN_Update(void);

void MAIN_SetFPS(uint32_t fps);

int MAIN_GetFPS(void);

int MAIN_GetRealFPS(void);

void MAIN_SetRenderWhileSkippingFrames(bool render);

bool MAIN_RenderThisFrame(void);

void MAIN_SetResourceSettings(
	int waveCount,
	const char* const* writeDirectories,
	size_t writeDirectoryCount,
	int planeCount,
	int textLayerCount
);

void MAIN_WaitToFinishLoading(void);

void MAIN_Init(void);

void MAIN_Quit(void);

SDL_NORETURN void MAIN_Exit(SDL_AppResult result);

#endif
