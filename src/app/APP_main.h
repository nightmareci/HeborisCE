#ifndef APP_main_h_
#define APP_main_h_

#include "APP_stdinc.h"

void APP_Init(size_t wavesCount, const char* const* writeDirectories, size_t writeDirectoriesCount);

void APP_Quit(void);

void APP_Exit(int status);

#ifndef NDEBUG
#define APP_Exit(exitStatus) (SDL_Log("Exited in file %s, line %d, status %s", __FILE__, __LINE__, (exitStatus) ? "EXIT_FAILURE" : "EXIT_SUCCESS"), APP_Exit((exitStatus)))
#endif

#endif
