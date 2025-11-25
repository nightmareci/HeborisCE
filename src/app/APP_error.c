#include "APP_error.h"
#include <SDL3/SDL_error.h>

bool APP_SetErrorFunctionLine(const char* function, int line, const char* fmt, ...)
{
	va_list args;
	char* fullFmt;
	if (SDL_asprintf(&fullFmt, "%s(): Line %d\n%s", function, line, fmt) < 0) {
		return false;
	}
	char* message;
	va_start(args, fmt);
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
