#ifndef APP_main_h_
#define APP_main_h_

#include "APP_stdinc.h"

void APP_ResetFrameStep(void);

bool APP_Update(void);

void APP_SetFPS(uint32_t fps);

int APP_GetFPS(void);

int APP_GetRealFPS(void);

void APP_SetRenderWhileSkippingFrames(bool render);

bool APP_RenderThisFrame(void);

void APP_SetResourceSettings(size_t waveCount, const char* const* writeDirectories, size_t writeDirectoryCount, int planeCount, int textLayerCount);

void APP_Init(void);

void APP_Quit(void);

SDL_NORETURN void APP_Exit(SDL_AppResult result);

#endif
