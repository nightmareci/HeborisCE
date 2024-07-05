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
#ifndef BDF_font_
#define BDF_font_

/**
 * This is a loader and rendering library for Adobe BDF fonts. This library
 * only supports rendering text in UTF-8 encoding, using Unicode BDF fonts.
 *
 * This library requires HT_hashtable.h and HT_hashtable.c to function.
 */

#include "SDL_render.h"

typedef enum BDF_FormatVersion
{
    BDF_FORMAT_VERSION_2_1,
    BDF_FORMAT_VERSION_2_2
} BDF_FormatVersion;

typedef enum BDF_MetricSet
{
    BDF_METRICSET_0,
    BDF_METRICSET_1,
    BDF_METRICSET_2,
    BDF_NUM_METRICSETS
} BDF_MetricSet;

typedef enum BDF_PropertyType
{
    BDF_PROPERTY_INTEGER,
    BDF_PROPERTY_STRING
} BDF_PropertyType;

typedef struct BDF_Property
{
    BDF_PropertyType type;
    union
    {
        Sint64 i;
        const char *str;
    };
} BDF_Property;

typedef struct BDF_Font BDF_Font;

/* Create a BDF font object from a NUL-terminated string containing the font text. */
BDF_Font *BDF_CreateFont(const char *str);

/* Create a BDF font object from data read from a stream. */
BDF_Font *BDF_LoadFont_RW(SDL_RWops *stream);

/* Render text using the BDF font. The text will not be wordwrapped. */
int BDF_RenderFontText(SDL_Renderer *renderer, BDF_Font *font, const char *text, SDL_Color fg_color);

/* Get a BDF font property. */
const BDF_Property *BDF_GetFontProperty(BDF_Font *font, const char *name);

/* Get the format version of a BDF font. */
BDF_FormatVersion BDF_GetFontFormatVersion(BDF_Font *font);

/* Get the metric set of a BDF font. */
BDF_MetricSet BDF_GetFontMetricSet(BDF_Font *font);

/* Destroy a BDF font. */
void BDF_DestroyFont(BDF_Font *font);

#endif /* BDF_font_ */

/* vi: set ts=4 sw=4 expandtab: */
