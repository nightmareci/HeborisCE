#ifndef APP_filesystem_h_
#define APP_filesystem_h_

#include "APP_stdinc.h"

bool APP_InitFilesystem(int argc, char** argv);

bool APP_CreateDirectory(const char* directory);

SDL_IOStream* APP_OpenRead(const char* filename);
SDL_IOStream* APP_OpenWrite(const char* filename);
SDL_IOStream* APP_OpenAppend(const char* filename);

bool APP_FileExists(const char* filename);

// TODO: Refactor these file functions, e.g.:
// bool APP_ReadFile32(const char* filename, int32_t* values, size_t start, size_t count);
// bool APP_WriteFile32(const char* filename, int32_t* values, size_t start, size_t count);
// bool APP_AppendFile32(const char* filename, int32_t* values, size_t count);
// They shouldn't exit when errors occur, instead returning false

void APP_LoadFile(const char* filename, void* buf, size_t size);
void APP_ReadFile(const char* filename, void* buf, size_t size, size_t offset);
void APP_SaveFile(const char* filename, void* buf, size_t size);
void APP_AppendFile(const char* filename, void* buf, size_t size);

void APP_QuitFilesystem(void);

#endif
