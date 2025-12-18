#ifndef APP_filesystem_h_
#define APP_filesystem_h_

#include "APP_stdinc.h"

bool APP_InitFilesystem(int argc, char** argv);

bool APP_CreateDirectory(const char* directory);

SDL_IOStream* APP_OpenRead(const char* filename);
SDL_IOStream* APP_OpenWrite(const char* filename);
SDL_IOStream* APP_OpenAppend(const char* filename);

bool APP_FileExists(const char* filename);

void APP_ReadFile32(const char* filename, int32_t* values, size_t count, size_t start);
void APP_WriteFile32(const char* filename, int32_t* values, size_t count);
void APP_AppendFile32(const char* filename, int32_t* values, size_t count);

void APP_QuitFilesystem(void);

#endif
