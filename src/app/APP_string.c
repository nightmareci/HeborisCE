#include "APP_string.h"

void APP_MidStr(const char* src, size_t start, size_t len, char* dest)
{
	for (size_t i = 0; i < len; i++) {
		dest[i] = src[start - 1 + i];
	}
	dest[len] = '\0';
}

void APP_LeftStr(const char* src, size_t len, char* dest)
{
	APP_MidStr(src, 1, len, dest);
}
