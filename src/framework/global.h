#ifndef global_h_
#define global_h_

#include <SDL3/SDL_video.h>
#include <SDL3/SDL_render.h>
#include "worker.h"

extern float GLOBAL_ScreenSubpixelOffset;
extern SDL_Window* GLOBAL_ScreenWindow;
extern SDL_Renderer* GLOBAL_ScreenRenderer;
extern SDL_Texture* GLOBAL_ScreenRenderTarget;
extern WORKER_Object* GLOBAL_LoadingWorker;

#endif
