#ifndef APP_main_h_
#define APP_main_h_

#include "APP_stdinc.h"

void APP_Init(size_t wavesCount, const char* const* writeDirectories, size_t writeDirectoriesCount);

void APP_Quit(void);

SDL_NORETURN void APP_Exit(const char* function, int line, const char* format, ...);

#endif
