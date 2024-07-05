/*
  Adobe BDF font library for software using SDL
  Copyright (C) 2024 Brandon McGriff <nightmareci@gmail.com>

  This software is provided 'as-is', without any express or implied
  warranty.  In no event will the authors be held liable for any damages
  arising from the use of this software.

  Permission is granted to anyone to use this software for any purpose,
  including commercial applications, and to alter it and redistribute it
  freely, subject to the following restrictions:

  1. The origin of this software must not be misrepresented; you must not
     claim that you wrote the original software. If you use this software
     in a product, an acknowledgment in the product documentation would be
     appreciated but is not required.
  2. Altered source versions must be plainly marked as such, and must not be
     misrepresented as being the original software.
  3. This notice may not be removed or altered from any source distribution.
*/
#include "BDF_font.h"
#include "HT_hashtable.h"
#include "SDL.h"

struct BDF_Font
{
    BDF_FormatVersion format_version;
    BDF_MetricSet metric_set;
};

static size_t line_len(const char *line, const char *lines_end)
{
    const char *c;

    SDL_assert(line != NULL);
    SDL_assert(lines_end != NULL);
    SDL_assert(line <= lines_end);

    for (
        c = line;
        c < lines_end && *c != '\0' && *c != '\n' && *c != '\r';
        c++
    );

    return c - line;
}

static const char *next_line(const char *line, const char *lines_end)
{
    const char *c;

    for (
        c = line;
        c < lines_end && *c != '\0' && (*c == '\n' || *c == '\r');
        c++
    );

    if (c == lines_end || *c == '\0') {
        return NULL;
    }
    else {
        return c;
    }
}

typedef enum FontParseType
{
	FONT_STARTFONT = 0,
	FONT_GLOBAL,
	FONT_PROPERTIES,
	FONT_NEXTCHAR,
	FONT_CHARDATA,
	FONT_BITMAP,
	FONT_ENDFONT
} FontParseType;

typedef struct FontParseState
{
    FontParseType parse_type;
} FontParseState;

static int parse_line(BDF_Font *font, FontParseState *state, const char *line, const char *line_end, size_t line_num)
{
    size_t end;
    Uint32 version_major, version_minor;
    const char *line_type;
    size_t line_type_len;
    const char *c;

    switch (state->parse_type) {
    case FONT_STARTFONT:
        if (line_num != 0) {
            return -1;
        }
        line_type = "STARTFONT";
        line_type_len = SDL_strlen(line_type);
        if (line_end - line >= line_type_len && SDL_memcmp(line, line_type, line_type_len) == 0) {
            for (
                c = line + line_type_len;
                c < line_end && (*c == ' ' || *c == '\t');
                c++
            );
            if (c == line_end || c == line + line_type_len) {
                return -1;
            }
            version_major = 0;
            for (; c < line_end && *c >= '0' && *c <= '9'; c++) {
                version_major *= 10;
                version_major += *c - '0';
            }
            if (c == line_end || *c != '.') {
                return -1;
            }
            /* TODO */
            return -1;
        }
        return 0;

    /* TODO: Remaining parse types. */

    default:
        return -1;
    }
}

BDF_Font *BDF_CreateFont(const char *str)
{
    SDL_assert(str != NULL);

    BDF_Font *font;
    FontParseState state = { 0 };
    const char *cur;
    const char *cur_end;
    size_t cur_num;
    const size_t str_size = SDL_strlen(str) + 1;
    const char *const str_end = str + str_size;

    font = SDL_malloc(sizeof(BDF_Font));
    if (!font) {
        return NULL;
    }

    for (
        cur = next_line(str, str_end), cur_num = 0;
        cur != NULL;
        cur = next_line(cur, str_end), cur_num++
    ) {
        size_t cur_len = line_len(cur, str_end);
        cur_end = cur + cur_len;
        if (parse_line(font, &state, cur, cur_end, cur_num) < 0) {
            /* TODO: Free stuff here in the font (glyphs, properties) before freeing the font struct. */
            SDL_free(font);
            return NULL;
        }
    }
    /* TODO */
    return NULL;
}

BDF_Font *BDF_LoadFont_RW(SDL_RWops *stream)
{
    SDL_assert(stream != NULL);

    /* TODO */
    return NULL;
}

int BDF_RenderFontText(SDL_Renderer *renderer, BDF_Font *font, const char *text, SDL_Color fg_color)
{
    SDL_assert(renderer != NULL);
    SDL_assert(font != NULL);
    SDL_assert(text != NULL);

    /* TODO */
    return -1;
}

const BDF_Property *BDF_GetFontProperty(BDF_Font *font, const char *name)
{
    SDL_assert(font != NULL);
    SDL_assert(name != NULL);

    /* TODO */
    return NULL;
}

BDF_FormatVersion BDF_GetFontFormatVersion(BDF_Font *font)
{
    SDL_assert(font != NULL);

    return font->format_version;
}

BDF_MetricSet BDF_GetFontMetricSet(BDF_Font *font)
{
    SDL_assert(font != NULL);

    return font->metric_set;
}

void BDF_DestroyFont(BDF_Font *font)
{
    SDL_assert(font != NULL);

    /* TODO */
    return;
}

/* vi: set ts=4 sw=4 expandtab: */
