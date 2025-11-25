#ifndef APP_error_h_
#define APP_error_h_

#include <stdbool.h>

bool APP_SetFunctionLineError(const char* function, int line, const char* fmt, ...);

#define APP_SetError(...) APP_SetFunctionLineError(__func__, __LINE__, __VA_ARGS__)

#endif
