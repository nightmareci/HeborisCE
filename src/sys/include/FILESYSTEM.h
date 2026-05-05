#ifndef FILESYSTEM_h_
#define FILESYSTEM_h_

#include <STD.h>

bool FILESYSTEM_Init(int argc, char** argv);

bool FILESYSTEM_CreateDirectory(const char* directory);

SDL_IOStream* FILESYSTEM_OpenRead(const char* filename);
SDL_IOStream* FILESYSTEM_OpenWrite(const char* filename);
SDL_IOStream* FILESYSTEM_OpenAppend(const char* filename);

bool FILESYSTEM_FileExists(const char* filename);

void FILESYSTEM_ReadFile32(const char* filename, int32_t* values, size_t count, size_t start);
void FILESYSTEM_WriteFile32(const char* filename, int32_t* values, size_t count);
void FILESYSTEM_AppendFile32(const char* filename, int32_t* values, size_t count);

void FILESYSTEM_Quit(void);

#endif
