#include "include.h"
#include "prototypes.h"

enum
{
	INI_READ_NONE,
	INI_READ_READSECTION_READY,
	INI_READ_READSECTION,
	INI_READ_READSECTION_END,
	INI_READ_READVALUE_READY,
	INI_READ_READVALUE,
	INI_READ_READVALUE_END,
	INI_READ_READSTRING,
	INI_READ_MAX,
};

#define		INIREAD_READNAME_MAX	512
#define		INIREAD_READVALUE_MAX	2048
#define		INIREAD_READBUFFER_MAX	2048

static char	s_szIniReadName [INIREAD_READNAME_MAX];
static char	s_szIniReadValue[INIREAD_READVALUE_MAX];
static char s_cLastChar;
static int  s_iIniReadMode;
static bool s_bInComment = false;
static bool s_bInCommentBlock = false;
static bool s_bInQuote = false;
static bool s_bBeforeYen = false;
static bool s_bValueRead = false;

inline bool IsValidChar(char chr, bool num_enable)
{
	return ( ( chr >= 'a' && chr <= 'z' ) || ( chr >= 'A' && chr <= 'Z' ) || chr == '_' || chr == '-' || chr == '[' || chr == ']' ) || ( ( chr >= '0' && chr <= '9' ) && num_enable );
}

inline bool IsEqualChar(char chr)
{
	return chr == '=';
}

inline bool IsSemiColonChar(char chr)
{
	return chr == ';';
}

inline bool IsDoubleQuoteChar(char chr)
{
	return chr == '"';
}

inline bool IsYenChar(char chr)
{
	return chr == '\\';
}

inline bool IsSpaceChar(char chr)
{
	return chr == ' ' || chr == '\t' || chr == '\r' || chr == '\n';
}

static void IniReadProcess(const char* buf, int size)
{
	for ( int i = 0 ; i < size ; i ++ )
	{
		char	chr = buf[i];

		if ( s_bInComment )
		{
			if ( chr == '\n' || chr == '\r' )
			{
				s_bInComment = false;
			}
		}

		else if ( s_bInCommentBlock )
		{
			if ( s_cLastChar == '*' && chr == '/' )
			{
				s_bInCommentBlock = false;
			}
		}

		else
		{
			if ( s_cLastChar == '/' && chr == '/' )
			{
				s_bInComment = true;
			}
			else if ( s_cLastChar == '/' && chr == '*' )
			{
				s_bInCommentBlock = true;
			}
			else
			{
				switch ( s_iIniReadMode )
				{
				case INI_READ_READSECTION_READY:
					if ( IsValidChar(chr, false) )
					{
						s_iIniReadMode = INI_READ_READSECTION;
						s_szIniReadName[0] = chr;
						s_szIniReadName[1] = '\0';
					}
					break;

				case INI_READ_READSECTION:
					if ( IsValidChar(chr, true) )
					{
						if ( strlen(s_szIniReadName) < INIREAD_READNAME_MAX - 1 )
						{
							char	str[2];
							str[0] = chr;
							str[1] = '\0';
							strcat(s_szIniReadName, str);
						}
					}
					else if ( IsEqualChar(chr) )
					{
						s_iIniReadMode = INI_READ_READVALUE_READY;
					}
					else if ( !IsSpaceChar(chr) )
					{
						s_iIniReadMode = INI_READ_READSECTION_END;
					}
					break;

				case INI_READ_READSECTION_END:
					if ( IsEqualChar(chr) )
					{
						s_iIniReadMode = INI_READ_READVALUE_READY;
					}
					break;

				case INI_READ_READVALUE_READY:
					if ( IsValidChar(chr, true) )
					{
						s_iIniReadMode = INI_READ_READVALUE;
						s_szIniReadValue[0] = chr;
						s_szIniReadValue[1] = '\0';
					}

					if ( IsDoubleQuoteChar(chr) )
					{
						s_iIniReadMode = INI_READ_READSTRING;
						s_bInQuote = true;
						s_bBeforeYen = false;
						s_szIniReadValue[0] = '\0';
					}
					break;

				case INI_READ_READVALUE:
					if ( IsValidChar(chr, true) )
					{
						if ( strlen(s_szIniReadValue) < INIREAD_READVALUE_MAX - 1)
						{
							char	str[2];
							str[0] = chr;
							str[1] = '\0';
							strcat(s_szIniReadValue, str);
						}
					}
					else if ( IsSemiColonChar(chr) )
					{
						ReadIniSection(s_szIniReadName, s_szIniReadValue);
						s_iIniReadMode = INI_READ_READSECTION_READY;
					}
					else if ( !IsSpaceChar(chr) )
					{
						s_iIniReadMode = INI_READ_READVALUE_END;
					}
					break;

				case INI_READ_READVALUE_END:
					if ( IsEqualChar(chr) )
					{
						ReadIniSection(s_szIniReadName, s_szIniReadValue);
						s_iIniReadMode = INI_READ_READSECTION_READY;
					}
					break;

				case INI_READ_READSTRING:
					if ( !s_bBeforeYen )
					{
						if ( IsDoubleQuoteChar(chr) )
						{
							s_bInQuote = !s_bInQuote;
							break;
						}
						else if ( IsYenChar(chr) )
						{
							s_bBeforeYen = true;
							break;
						}
					}

					if ( s_bInQuote )
					{
						if ( strlen(s_szIniReadValue) < INIREAD_READVALUE_MAX - 1)
						{
							char	str[2];
							str[0] = chr;
							str[1] = '\0';
							strcat(s_szIniReadValue, str);
						}
					}
					else
					{
						if ( IsSemiColonChar(chr) )
						{
							ReadIniSectionString(s_szIniReadName, s_szIniReadValue);
							// MessageBox(0, s_szIniReadValue, s_szIniReadName, MB_OK);
							s_iIniReadMode = INI_READ_READSECTION_READY;
						}
					}
					break;
				}
			}
		}

		s_cLastChar = chr;
	}
}

void LoadIniFile(const char *filename)
{
	s_szIniReadName [0] = '\0';
	s_szIniReadValue[0] = '\0';
	s_cLastChar = 0;
	s_iIniReadMode = INI_READ_READSECTION_READY;
	s_bInComment = false;
	s_bInCommentBlock = false;
	s_bValueRead = false;

	FILE	*file = fopen(filename, "rb");
	if ( file != NULL )
	{
		char	buf[INIREAD_READBUFFER_MAX];
		while ( !feof(file) )
		{
			int readsize = 0;
			readsize = fread(buf, 1, INIREAD_READBUFFER_MAX, file);
			IniReadProcess(buf, readsize);
		}
		fclose(file);
	}
}
