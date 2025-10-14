#include "APP_stdlib.h"

Sint32 APP_Rand(Sint32 max)
{
	return SDL_rand(max);
}

void APP_StrCpy(char *dest, const char *src)
{
	// TODO: Use SDL_strlcpy() instead, but that requires adding a parameter of the size of the dest argument
	strcpy(dest, src);
}

void APP_StrCat(char *str1, const char *str2)
{
	// TODO: Use SDL_strlcat() instead, but that requires adding a parameter of the size of the str1 argument
	strcat(str1, str2);
}

size_t APP_StrLen(const char *stri)
{
	return SDL_strlen(stri);
}

void APP_MidStr(const char *src, size_t start, size_t len, char *dest)
{
	int		i;
	for ( i = 0 ; i < len ; i ++ )
	{
		dest[i] = src[start - 1 + i];
	}
	dest[len] = '\0';
}

void APP_LeftStr(const char *src, size_t len, char *dest)
{
	APP_MidStr(src, 1, len, dest);
}

char APP_CharAt(const char *stri, size_t pos)
{
	return stri[pos];
}

int APP_ValLong(const char *stri)
{
	return SDL_atoi(stri);
}

void APP_FillMemory(void* buf, size_t size, int val)
{
	SDL_memset(buf, val, size);
}
