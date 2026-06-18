#ifndef filesystem_private_h_
#define filesystem_private_h_

#include <stdinc.h>

bool FILESYSTEM_Init(int argc, char** argv);

SDL_IOStream* FILESYSTEM_OpenRead(const char* filename);
SDL_IOStream* FILESYSTEM_OpenWrite(const char* filename);
SDL_IOStream* FILESYSTEM_OpenAppend(const char* filename);

bool FILESYSTEM_FileExists(const char* filename);

bool FILESYSTEM_CreateDirectory(const char* directory);

void FILESYSTEM_Quit(void);

#endif
