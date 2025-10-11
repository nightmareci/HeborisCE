#ifndef filesystem_h_
#define filesystem_h_

#include "APP_common.h"

bool APP_InitFilesystem(int argc, char** argv);
void APP_QuitFilesystem(void);

bool APP_CreateDirectory(const char* const directory);

SDL_IOStream* APP_OpenRead(const char* const filename);
SDL_IOStream* APP_OpenWrite(const char* const filename);
SDL_IOStream* APP_OpenAppend(const char* const filename);

bool APP_GetPathInfo(const char* const filename, SDL_PathInfo* info);
void* APP_GetFileBuffer(const char* const filename, size_t* size);

#endif
