#ifndef error_h_
#define error_h_

#include <framework/stdinc.h>

bool ERROR_FunctionLineSet(const char* function, int line, const char* fmt, ...);

#define ERROR_Set(...) ERROR_FunctionLineSet(__func__, __LINE__, __VA_ARGS__)

#endif
