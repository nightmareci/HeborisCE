#ifndef filesystem_h_
#define filesystem_h_

#include <framework/stdinc.h>

void FILESYSTEM_ReadFile32(const char* filename, int32_t* values, size_t count, size_t start);
void FILESYSTEM_WriteFile32(const char* filename, int32_t* values, size_t count);
void FILESYSTEM_AppendFile32(const char* filename, int32_t* values, size_t count);

#endif
