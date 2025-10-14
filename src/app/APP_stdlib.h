#ifndef APP_stdlib_h_
#define APP_stdlib_h_

#include "APP_stdinc.h"

// TODO: Many of these functions should be removed and their SDL equivalents used instead

Sint32 APP_Rand(Sint32 max);

void APP_StrCpy(char *dest, const char *src);
void APP_StrCat(char *str1, const char *str2);
size_t APP_StrLen(const char *stri);
void APP_MidStr(const char *src, size_t start, size_t len, char *dest);
void APP_LeftStr(const char *src, size_t len, char *dest);
char APP_CharAt(const char *stri, size_t pos);
int APP_ValLong(const char *stri);
void APP_FillMemory(void* buf, size_t size, int val);

#endif
