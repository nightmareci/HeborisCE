#ifndef filesystem_private_h_
#define filesystem_private_h_

#include <stdinc.h>

bool FILESYSTEM_CreateDirectory(const char* directory);
bool FILESYSTEM_Init(int argc, char** argv);
void FILESYSTEM_Quit(void);

#endif
