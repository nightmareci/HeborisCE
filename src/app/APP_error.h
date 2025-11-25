#ifndef APP_error_h_
#define APP_error_h_

#include <stdbool.h>

bool APP_SetErrorFunctionLine(const char* function, int line, const char* fmt, ...);

#define APP_SetError(...) APP_SetErrorFunctionLine(__func__, __LINE__, __VA_ARGS__)

#endif
