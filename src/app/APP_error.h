#ifndef APP_error_h_
#define APP_error_h_

#include <SDL3/SDL_error.h>

static inline bool APP_SetErrorFunctionLine(const char* function, int line, const char* fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	char* fullFmt;
	if (SDL_asprintf(&fullFmt, "%s(): Line %d\n%s", function, line, fmt) < 0) {
		return false;
	}
	char* message;
	if (SDL_vasprintf(&message, fullFmt, args) < 0) {
		va_end(args);
		SDL_free(fullFmt);
		return false;
	}
	va_end(args);
	SDL_SetError("%s", message);
	SDL_free(message);
	SDL_free(fullFmt);
	return false;
}

#define APP_SetError(...) APP_SetErrorFunctionLine(__FUNCTION__, __LINE__, __VA_ARGS__)

#endif
