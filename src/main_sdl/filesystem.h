#ifndef filesystem_h_
#define filesystem_h_

#include "include.h"

bool FS_Init(int argc, char** argv);
void FS_Quit(void);

bool FS_CreateDirectory(const char* const directory);

SDL_RWops* FS_OpenRead(const char* const filename);
SDL_RWops* FS_OpenWrite(const char* const filename);
SDL_RWops* FS_OpenAppend(const char* const filename);

#endif
