#ifndef ERROR_h_
#define ERROR_h_

#include <stdbool.h>

bool ERROR_FunctionLineSet(const char* function, int line, const char* fmt, ...);

#define ERROR_Set(...) ERROR_FunctionLineSet(__func__, __LINE__, __VA_ARGS__)

#endif
