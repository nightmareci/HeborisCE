#include "APP_global.h"

float APP_ScreenSubpixelOffset = 0.0f;
SDL_Window* APP_ScreenWindow = NULL;
SDL_Renderer* APP_ScreenRenderer = NULL;
SDL_Texture* APP_ScreenRenderTarget = NULL;
APP_Worker* APP_LoadingWorker = NULL;
