#ifndef APP_global_h_
#define APP_global_h_

#include "APP_worker.h"
#include <SDL3/SDL_video.h>
#include <SDL3/SDL_render.h>

extern float APP_ScreenSubpixelOffset;
extern SDL_Window* APP_ScreenWindow;
extern SDL_Renderer* APP_ScreenRenderer;
extern SDL_Texture* APP_ScreenRenderTarget;
extern APP_Worker* APP_LoadingWorker;

#endif
