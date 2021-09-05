#include "main_sdl/include.h"
#include "heboris_ini.h"

int32_t atoi_special(const char *chr)
{
	if ( strlen(chr) > 2 && chr[0] == '0' && chr[1] == 'x' )
	{
		return strtol(chr, NULL, 16);
	}
	else
	{
		return atoi(chr);
	}
}

#define		INI_SECTION_DEF(sec)		if ( strcasecmp(section, #sec) == 0 ) { sec = atoi_special(value); return; }
#define		INI_SECTION_DEF_STRING(sec)	{}

void ReadIniSection(const char *section, const char *value)
{
#include "inireader_heboris_ini.inc"
}

#undef		INI_SECTION_DEF
#undef		INI_SECTION_DEF_STRING
#define		INI_SECTION_DEF(sec)		{}
#define		INI_SECTION_DEF_STRING(sec)	if ( strcasecmp(section, #sec) == 0 ) { strcpy(sec, value); return; }

void ReadIniSectionString(const char *section, const char *value)
{
#include "inireader_heboris_ini.inc"
}

#undef		INI_SECTION_DEF
