#include "SDL_kanji.h"
#include "HT_hashtable.h"

#define LINE_SIZE 256

struct Kanji_Font {
	Uint32 size;
	HT_HashTable* mojis;
};

static char* Line_Get(char* buf, int count, char** src, char* end) {
	if (buf != NULL && count > 0 && *src < end) {
		int i;
		for (i = 0; i < count - 1 && *src + i < end; i++) {
			if ((*src)[i] == '\n' || (*src)[i] == '\r') {
				char endc = (*src)[i];
				buf[i] = endc;
				i++;
				if (i < count - 1 && *src + i < end) {
					if (((*src)[i] == '\n' || (*src)[i] == '\r') && (*src)[i] != endc) {
						buf[i] = (*src)[i];
						i++;
					}
				}
				buf[i] = '\0';
				*src += i;
				if (*src < end) {
					return buf;
				}
				else {
					return NULL;
				}
			}
			else {
				buf[i] = (*src)[i];
			}
		}
		buf[i] = '\0';
		*src += i;
		return buf;
	}

	return NULL;
}

typedef struct Moji {
	unsigned int width;
	Uint32* pixels;
} Moji;

static Uint32 Moji_Hash(const void* key, void* data) {
	return *(Uint32*)key;
}

static SDL_bool Moji_KeyMatch(const void* a, const void* b, void* data) {
	return *(Uint32*)a == *(Uint32*)b;
}

static void Moji_Nuke(const void* key, const void* value, void* data) {
	Moji* moji = (Moji*)value;

	SDL_free((Uint32*)key);
	SDL_free(moji->pixels);
	SDL_free(moji);
}

static const Uint8* UTF8_Next(const Uint8* text, Uint32* encoding) {
	Uint32 res;
	int bytes;
	const Uint8* c;

	if (text == NULL) {
		return NULL;
	}
	if (encoding == NULL) {
		return NULL;
	}

	if (*text == '\0') {
		*encoding = 0;
		return text;
	}

	res = *text & 0xFFu;
	if ((res & 0x80u) == 0x00u) {
		*encoding = res;
		return text + 1;
	}

	if ((res & 0xE0u) == 0xC0u) {
		if (text[1] == '\0') {
			return NULL;
		}
		res &= 0x1Fu;
		bytes = 2;
	}
	else if ((res & 0xF0u) == 0xE0u) {
		if (text[1] == '\0' || text[2] == '\0') {
			return NULL;
		}
		res &= 0xFu;
		bytes = 3;
	}
	else if ((res & 0xF1u) == 0xF0u) {
		if (text[1] == '\0' || text[2] == '\0' || text[3] == '\0') {
			return NULL;
		}
		res &= 0x7u;
		bytes = 4;
	}

	for (c = text + 1; c < text + bytes; c++) {
		const Uint8 byte = *c;
		if ((byte & 0xC0u) != 0x80u) {
			return NULL;
		}
		res = (res << 6) | (byte & 0x3Fu);
	}

	switch (bytes) {
	case 2:
		if (res <= 0x7Fu) {
			return NULL;
		}
		break;

	case 3:
		if (res <= 0x7FFu) {
			return NULL;
		}
		break;

	case 4:
		if (res <= 0xFFFFu) {
			return NULL;
		}
		break;

	default:
		return NULL;
	}

	if ((res >= 0xD800u && res <= 0xDFFFu) || res > 0x10FFFFu) {
		return NULL;
	}
	else {
		*encoding = res;
		return text + bytes;
	}
}

static int Parse_Char(Kanji_Font* font, Uint32 encoding, Uint32 width, char** line, char* end, int rshift) {
	Uint32* key;
	Moji* moji;
	char buf[LINE_SIZE];
	Uint32 y;

	/* 既にロードされている文字は読み込まない */
	/* Do not load characters that have already been loaded */
	if (HT_FindInHashTable(font->mojis, &encoding, (const void**)&moji)) {
		return 0;
	}

	key = SDL_malloc(sizeof(Uint32));
	if (!key) {
		SDL_SetError("Failed to allocate memory for the hashtable key of a font.");
		return -1;
	}

	moji = SDL_malloc(sizeof(Moji));
	if (!moji) {
		SDL_SetError("Failed to allocate memory for a character of a font.");
		SDL_free(key);
		return -1;
	}

	moji->width = width;

	moji->pixels = SDL_malloc(sizeof(Uint32) * font->size);
	if (!moji->pixels) {
		SDL_SetError("Failed to allocate memory for a character's pixels of a font.");
		SDL_free(moji);
		SDL_free(key);
		return -1;
	}

	*key = encoding;

	for (y = 0; y < font->size; y++) {
		Line_Get(buf, sizeof(buf), line, end);
		moji->pixels[y] = (SDL_strtol(buf, 0, 16) >> rshift);
	}

	if (HT_InsertIntoHashTable(font->mojis, key, moji)) {
		return 0;
	}
	else {
		SDL_SetError("Failed to insert a character into a font's hashtable.");
		SDL_free(moji->pixels);
		SDL_free(moji);
		SDL_free(key);
		return -1;
	}
}

int Kanji_AddFont(Kanji_Font* font, SDL_RWops* src) {
	char* src_data;
	char* end;
	char* line;
	char *p;
	char sc;
	long encoding;
	long font_size;
	long width;
	int s;
	char buf[LINE_SIZE];
	Sint64 src_size;
	SDL_bool found_size;
	SDL_bool found_encoding;
	SDL_bool found_width;
	SDL_bool found_bitmap;

	src_size = SDL_RWsize(src);
	src_data = SDL_malloc(src_size);
	if (!src_data) return 1;
	if (SDL_RWread(src, src_data, src_size, 1) == 0) {
		SDL_SetError("Failed to read the font data: %s", SDL_GetError());
		SDL_free(src_data);
		return -1;
	}

	end = src_data + src_size;
	line = src_data;

	found_size = SDL_FALSE;
	found_encoding = SDL_FALSE;
	found_width = SDL_FALSE;
	found_bitmap = SDL_FALSE;
	while (Line_Get(buf, sizeof(buf), &line, end) != NULL) {
		if (SDL_strstr(buf, "SIZE") == buf) {
			if (found_size) {
				SDL_SetError("Erroneous extra SIZE line encountered.");
				SDL_free(src_data);
				return -1;
			}
			p = SDL_strchr(buf, ' ');
			if (!p) {
				SDL_SetError("Invalid format of a SIZE line.");
				SDL_free(src_data);
				return -1;
			}
			font_size = SDL_strtol(p, 0, 10);
			if (font_size <= 0 || font_size > SDL_MAX_UINT32) {
				SDL_SetError("Font size of %ld is invalid, must be nonzero and less than SDL_MAX_UINT32.", font_size);
				SDL_free(src_data);
				return -1;
			}
			if (font->size == 0) {
				font->size = (Uint32)font_size;
			}
			found_size = SDL_TRUE;
		}
		else if (SDL_strstr(buf, "STARTCHAR") == buf) {
			if (!found_size) {
				SDL_SetError("SIZE line not found before a STARTCHAR line.");
				SDL_free(src_data);
				return -1;
			}
			found_encoding = SDL_FALSE;
			found_width = SDL_FALSE;
			found_bitmap = SDL_FALSE;
		}
		else if (SDL_strstr(buf, "ENCODING") == buf) {
			if (!found_size) {
				SDL_SetError("SIZE line not found before an ENCODING line.");
				SDL_free(src_data);
				return -1;
			}
			p = SDL_strchr(buf, ' ');
			if (!p) {
				SDL_SetError("Invalid format of an ENCODING line.");
			}
			encoding = SDL_strtol(p, 0, 10);
			if (encoding < 0 || encoding > SDL_MAX_UINT32) {
				SDL_free(src_data);
				return -1;
			}
			found_encoding = SDL_TRUE;
		}
		else if (SDL_strstr(buf, "DWIDTH") == buf) {
			if (!found_size) {
				SDL_SetError("SIZE line not found before a DWIDTH line.");
				SDL_free(src_data);
				return -1;
			}
			p = SDL_strchr(buf, ' ');
			if (!p) {
				SDL_SetError("Invalid format of a DWIDTH line.");
				SDL_free(src_data);
				return -1;
			}
			width = SDL_strtol(p, 0, 10);
			if (width <= 0 || width > SDL_MAX_UINT32) {
				SDL_free(src_data);
				return -1;
			}
			found_width = SDL_TRUE;
		}
		else if (SDL_strstr(buf, "BITMAP") == buf) {
			int rshift;
			if (!found_size || !found_encoding || !found_width) {
				SDL_SetError("BITMAP line encountered before all of SIZE, ENCODING, and DWIDTH lines have been found.");
				SDL_free(src_data);
				return -1;
			}
			for (s = 8; s < width; s += 8) {}
			rshift = s - width;
			if (Parse_Char(font, (Uint32)encoding, (Uint32)width, &line, end, rshift) < 0) {
				SDL_SetError("Failed to parse a font character: %s", SDL_GetError());
				SDL_free(src_data);
				return -1;
			}
		}
	}

	SDL_free(src_data);
	return 0;
}

Kanji_Font* Kanji_OpenFont(SDL_RWops* src) {
	Kanji_Font* font;

	font = SDL_malloc(sizeof(Kanji_Font));
	if (!font) {
		SDL_SetError("Failed to allocate memory for the font object.");
		return NULL;
	}

	font->size = 0;

	font->mojis= HT_CreateHashTable(NULL, 16, Moji_Hash, Moji_KeyMatch, Moji_Nuke, SDL_TRUE);
	if (!font->mojis) {
		SDL_SetError("Failed to create the hashtable for the font object: %s", SDL_GetError());
		SDL_free(font);
		return NULL;
	}

	if (src == NULL) {
		return font;
	}
	else if (Kanji_AddFont(font, src) < 0) {
		SDL_SetError("Failed to parse the font: %s", SDL_GetError());
		SDL_free(font);
		return NULL;
	}
	else {
		return font;
	}
}

Uint32 Kanji_FontSize(Kanji_Font* font) {
	return font->size;
}

Sint64 Kanji_TextWidth(Kanji_Font* font, const char* text) {
	const Uint8* p;
	Uint32 encoding;
	Sint64 width;

	if (font == NULL) {
		SDL_SetError("Parameter \"font\" must not be NULL.");
		return -1;
	}
	else if (text == NULL) {
		SDL_SetError("Parameter \"text\" must not be NULL.");
		return -1;
	}

	width = 0;
	for (p = UTF8_Next((const Uint8*)text, &encoding); p != NULL && encoding != 0x0u; p = UTF8_Next(p, &encoding)) {
		Moji* moji;

		if (!HT_FindInHashTable(font->mojis, &encoding, (const void**)&moji)) {
			SDL_SetError("The text contains a character not in the font, so width cannot be calculated.");
			return -1;
		}

		if (width > SDL_MAX_SINT64 - moji->width) {
			SDL_SetError("The calculated width of the text overflowed (maximum possible width is %" SDL_PRIs64 ").", SDL_MAX_SINT64);
			return -1;
		}

		width += moji->width;
	}

	if (p != NULL) {
		return width;
	}
	else {
		SDL_SetError("Invalid data found in the text. The text must be valid UTF-8 data.");
		return -1;
	}
}

int Kanji_PutPixelSurface(SDL_Surface *s, int x, int y, float subx, float suby, SDL_Color color) {
	Uint8* p, bpp;
	Uint32 mapped_color;
	int status = 0;

	if (SDL_MUSTLOCK(s)) {
		if (SDL_LockSurface(s) < 0) {
			return -1;
		}
	}

	bpp = s->format->BytesPerPixel;
	p = (Uint8*)(s->pixels) + y * s->pitch + x * bpp;
	mapped_color = SDL_MapRGB(s->format, color.r, color.g, color.b);

	switch(bpp){
	case 1:
		*((Uint8*)p) = (Uint8)mapped_color;
		break;
	case 2:
		*((Uint16*)p) = (Uint16)mapped_color;
		break;
	case 4:
		*((Uint32*)p) = mapped_color;
		break;

	default:
		status = -1;
		break;
	}

	if(SDL_MUSTLOCK(s)){
		SDL_UnlockSurface(s);
	}
	return status;
}

int Kanji_PutPixelRenderer(SDL_Renderer *s, int x, int y, float subx, float suby, SDL_Color pixel) {
	Uint8 r, g, b, a;

	/* TODO: Handle errors. */
	SDL_GetRenderDrawColor(s, &r, &g, &b, &a);
	SDL_SetRenderDrawColor(s, pixel.r, pixel.g, pixel.b, pixel.a);
	SDL_RenderDrawPointF(s, subx + x, suby + y);
	SDL_SetRenderDrawColor(s, r, g, b, a);
	return 0;
}

int Kanji_PutText(Kanji_Font* font, int dx, int dy, float subx, float suby, void* dst, int dw, int dh, const char* txt, SDL_Color fg, Kanji_PutPixelCallback put_pixel) {
	int cx, cy;
	const Uint8* p;
	Uint32 encoding;

	cx = dx;
	cy = dy;
	for (p = UTF8_Next((const Uint8*)txt, &encoding); p != NULL && encoding != '\0'; p = UTF8_Next(p, &encoding)) {
		Moji* moji;
		int x, y;
		int minx, miny, maxx, maxy;

		/* Handle any newline format */
		if (encoding == '\n' || encoding == '\r') {
            /* If we see '\r' and '\n' adjacent, we should consider those two bytes as meaning one newline. */
			if (encoding == '\r' && *p == '\n') {
				p++;
			}
			else if (encoding == '\n' && *p == '\r') {
				p++;
			}
			/* Only one byte for newline formats of only one '\r' or '\n', so don't increment. */

			cy += font->size;
			cx = dx;
			continue;
		}

		if (!HT_FindInHashTable(font->mojis, &encoding, (const void**)&moji)) {
			SDL_SetError("The text contains a character not in the font (U+%04" SDL_PRIX32 "), so it cannot be drawn.", encoding);
			return -1;
		}

		minx = (cx >= 0) ? 0 : -cx;
		miny = (cy >= 0) ? 0 : -cy;
		maxx = (cx + moji->width <= dw) ? moji->width : dw - cx;
		maxy = (cy + font->size <= dh) ? font->size : dh - cy;

		for (y = miny; y < maxy; y++) {
			for (x = minx; x < maxx; x++) {
				if (moji->pixels[y] & (1 << (moji->width - x - 1))) {
					if (put_pixel(dst, cx + x, cy + y, subx, suby, fg) < 0) {
						return -1;
					}
				}
			}
		}
		cx += moji->width;
	}

	return 0;
}

int Kanji_PutTextSurface(Kanji_Font* font, int dx, int dy, float subx, float suby, SDL_Surface* dst, const char* txt, SDL_Color fg) {
	return Kanji_PutText(font, dx, dy, subx, suby, dst, dst->w, dst->h, txt, fg, (Kanji_PutPixelCallback)Kanji_PutPixelSurface);
}

int Kanji_PutTextRenderer(Kanji_Font* font, int dx, int dy, float subx, float suby, SDL_Renderer* dst, const char* txt, SDL_Color fg) {
	int dw, dh;
	SDL_RenderGetLogicalSize(dst, &dw, &dh);
	return Kanji_PutText(font, dx, dy, subx, suby, dst, dw, dh, txt, fg, (Kanji_PutPixelCallback)Kanji_PutPixelRenderer);
}

#if 0
int Kanji_PutTextTate(Kanji_Font* font, int dx, int dy, float subx, float suby, void* dst, int dw, int dh, const char* txt, SDL_Color fg, int (* put_pixel)(void* dst, int x, int y, float subx, float suby, SDL_Color color)) {
	int encoding;
	int x, y, cx = dx, cy = dy;
	unsigned char high, low;
	int minx, miny, maxx, maxy;
	int now_kanji = 0;
	const unsigned char* text = (const unsigned char*)txt;

	while (*text != 0) {
		if (font->sys == KANJI_JIS && *text == 0x1b) {
			if (*(text+1) == 0x24 && *(text+2) == 0x42) {
				now_kanji = 1;
			}
			else if (*(text+1) == 0x28 && *(text+2) == 0x42) {
				now_kanji = 0;
			}
			text += 3;
			continue;
		}
		if (font->sys != KANJI_JIS) now_kanji = !isprint(*text);

		/* ASCII は無視 */
		if (!now_kanji) {
			text++;
		}
		else {
			high = *text;
			low = *(text+1);
			ConvertCodingSystem(font, &high, &low);
			encoding = (high - 0x20) * 96 + low - 0x20 + 0xff;
			text += 2;
			if (font->moji[encoding] == 0) {
				cy += font->k_size;
				continue;
			}

			if (high == 0x21 && (low >= 0x22 || low <= 0x25)) {
				cx += font->k_size * 0.6;
				cy -= font->k_size * 0.6;
			}

			minx = (cx >= 0) ? 0 : -cx;
			miny = (cy >= 0) ? 0 : -cy;
			maxx = (cx+font->k_size <= dw) ? font->k_size : dw-cx;
			maxy = (cy+font->k_size <= dh) ? font->k_size : dh-cy;

			for (y = miny; y < maxy; y++) {
				for (x = minx; x < maxx; x++) {
					if (font->moji[encoding][y] & (1 << (font->k_size-x-1))) {
						if (put_pixel(dst, cx + x, cy + y, subx, suby, fg) < 0) {
							return -1;
						}
					}
				}
			}

			if (high == 0x21 && (low >= 0x22 || low <= 0x25)) {
				cx -= font->k_size * 0.6;
				cy += font->k_size * 1.6;
			}
			else {
				cy += font->k_size;
			}
		}
	}
	return 0;
}

int Kanji_PutTextTateSurface(Kanji_Font* font, int dx, int dy, float subx, float suby, SDL_Surface* dst, const char* txt, SDL_Color fg) {
	return Kanji_PutTextTate(font, dx, dy, subx, suby, dst, dst->w, dst->h, txt, fg, (int (*)(void*, int, int, float, float, SDL_Color))Kanji_PutPixelSurface);
}

int Kanji_PutTextTateRenderer(Kanji_Font* font, int dx, int dy, float subx, float suby, SDL_Renderer* dst, const char* txt, SDL_Color fg) {
	int dw, dh;
	SDL_RenderGetLogicalSize(dst, &dw, &dh);
	return Kanji_PutTextTate(font, dx, dy, subx, suby, dst, dw, dh, txt, fg, (int (*)(void*, int, int, float, float, SDL_Color))Kanji_PutPixelRenderer);
}

SDL_Surface* Kanji_CreateSurface(Kanji_Font* font, const char* text, SDL_Color fg, int bpp) {
	SDL_Surface* textbuf;
	int len;
	Uint32 bgcol;

	if (font == NULL) return NULL;
	if (text == NULL) return NULL;
	if (*text == 0 ) return NULL;
	len = SDL_strlen(text);

	textbuf = SDL_CreateRGBSurface(0, font->a_size*len, font->k_size, bpp, 0, 0, 0, 0);
	if (textbuf == NULL) {
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,  "ERROR: at SDL_kanji.c(%d)\n", __LINE__);
		return NULL;
	}
	bgcol = SDL_MapRGB(textbuf->format, 255-fg.r, 255-fg.g, 255-fg.b);
	if (SDL_FillRect(textbuf, NULL, bgcol) < 0) {
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,  "ERROR: at SDL_kanji.c(%d)\n", __LINE__);
		SDL_FreeSurface(textbuf);
		return NULL;
	}
	if (SDL_SetColorKey(textbuf, SDL_TRUE, bgcol) < 0) {
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,  "ERROR: at SDL_kanji.c(%d)\n", __LINE__);
		SDL_FreeSurface(textbuf);
		return NULL;
	}

	Kanji_PutTextSurface(font, 0, 0, 0.0f, 0.0f, textbuf, text, fg);

	return textbuf;
}

SDL_Surface* Kanji_CreateSurfaceTate(Kanji_Font* font, const char* text, SDL_Color fg, int bpp) {
	SDL_Surface* textbuf;
	size_t len;
	Uint32 bgcol;

	if (font == NULL) return NULL;
	if (text == NULL) return NULL;
	if (*text == 0 ) return NULL;
	len = SDL_strlen(text);

	textbuf = SDL_CreateRGBSurface(0, font->k_size, font->a_size*len, bpp, 0, 0, 0, 0);
	if (textbuf == NULL) {
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "ERROR: at SDL_kanji.c(%d)\n", __LINE__);
		return NULL;
	}

	bgcol = SDL_MapRGB(textbuf->format, 255-fg.r, 255-fg.g, 255-fg.b);
	if (SDL_FillRect(textbuf, NULL, bgcol) < 0) {
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "ERROR: at SDL_kanji.c(%d)\n", __LINE__);
		SDL_FreeSurface(textbuf);
		return NULL;
	}
	if (SDL_SetColorKey(textbuf, SDL_TRUE, bgcol) < 0) {
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "ERROR: at SDL_kanji.c(%d)\n", __LINE__);
		SDL_FreeSurface(textbuf);
		return NULL;
	}

	Kanji_PutTextTateSurface(font, 0, 0, 0.0f, 0.0f, textbuf, text, fg);

	return textbuf;
}

#endif

void Kanji_CloseFont(Kanji_Font* font) {
	SDL_assert(font != NULL);

	HT_DestroyHashTable(font->mojis);
	SDL_free(font);
}
