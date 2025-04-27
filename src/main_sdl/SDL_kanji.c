#include "SDL_kanji.h"
#include "hashmap.h"
#include <limits.h>

#define LINE_SIZE 256

struct Kanji_Font {
	int size;
	struct hashmap* mojis;
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
	Uint32 encoding;
	int width;
	Uint32* pixels;
} Moji;

static uint64_t Moji_Hash(const void *item, uint64_t seed0, uint64_t seed1) {
	const Moji* const moji = item;
	return hashmap_sip(&moji->encoding, sizeof(moji->encoding), seed0, seed1);
}

static int Moji_Compare(const void* a, const void* b, void* udata) {
	const Moji* const mojiA = a;
	const Moji* const mojiB = b;
	if (mojiA->encoding == mojiB->encoding) {
		return 0;
	}
	else if (mojiA->encoding > mojiB->encoding) {
		return 1;
	}
	else {
		return -1;
	}
}

static void Moji_Free(void *item) {
	Moji* const moji = item;
	SDL_free(moji->pixels);
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

static int Parse_Char(Kanji_Font* font, Uint32 encoding, int width, char** line, char* end, int rshift) {
	Uint32* key;
	Moji moji;
	char buf[LINE_SIZE];
	Uint32 y;

	/* 既にロードされている文字は読み込まない */
	/* Do not load characters that have already been loaded */
	moji.encoding = encoding;
	if (hashmap_get(font->mojis, &moji)) {
		return 0;
	}

	moji.width = width;

	moji.pixels = SDL_malloc(sizeof(Uint32) * font->size);
	if (!moji.pixels) {
		SDL_SetError("Failed to allocate memory for a character's pixels of a font.");
		return -1;
	}

	for (y = 0; y < font->size; y++) {
		Line_Get(buf, sizeof(buf), line, end);
		moji.pixels[y] = (SDL_strtol(buf, NULL, 16) >> rshift);
	}

	if (!hashmap_set(font->mojis, &moji)) {
		return 0;
	}
	else {
		SDL_SetError("Failed to insert a character into a font's hashmap.");
		SDL_free(moji.pixels);
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
				SDL_free(src_data);
				return SDL_SetError("Erroneous extra SIZE line encountered.");
			}
			p = buf;
			while (*p != ' ' && *p != '\t') p++;
			while (*p == ' ' || *p == '\t') p++;
			font_size = SDL_strtol(p, NULL, 10);
			if (font_size <= 0 || font_size > INT_MAX) {
				SDL_free(src_data);
				return SDL_SetError("Font size of %ld is invalid, must be between 0 and %d.", font_size, INT_MAX);
			}
			if (font->size == 0) {
				font->size = (int)font_size;
			}
			found_size = SDL_TRUE;
		}
		else if (SDL_strstr(buf, "STARTCHAR") == buf) {
			if (!found_size) {
				SDL_free(src_data);
				return SDL_SetError("SIZE line not found before a STARTCHAR line.");
			}
			found_encoding = SDL_FALSE;
			found_width = SDL_FALSE;
			found_bitmap = SDL_FALSE;
		}
		else if (SDL_strstr(buf, "ENCODING") == buf) {
			if (!found_size) {
				SDL_free(src_data);
				return SDL_SetError("SIZE line not found before an ENCODING line.");
			}
			p = buf;
			while (*p != ' ' && *p != '\t') p++;
			while (*p == ' ' || *p == '\t') p++;
			encoding = SDL_strtol(p, NULL, 10);
			if (encoding < 0) {
				SDL_free(src_data);
				return SDL_SetError("Character encoding of %ld is invalid, must be nonnegative.", encoding);
			}
			found_encoding = SDL_TRUE;
		}
		else if (SDL_strstr(buf, "DWIDTH") == buf) {
			if (!found_size) {
				SDL_free(src_data);
				return SDL_SetError("SIZE line not found before a DWIDTH line.");
			}
			p = buf;
			while (*p != ' ' && *p != '\t') p++;
			while (*p == ' ' || *p == '\t') p++;
			width = SDL_strtol(p, NULL, 10);
			if (width <= 0 || width > INT_MAX) {
				SDL_free(src_data);
				return SDL_SetError("Character width of %ld is invalid, must be between 0 and %d.", width, INT_MAX);
			}
			found_width = SDL_TRUE;
		}
		else if (SDL_strstr(buf, "BITMAP") == buf) {
			int rshift;
			if (!found_size || !found_encoding || !found_width) {
				SDL_free(src_data);
				return SDL_SetError("BITMAP line encountered before all of SIZE, ENCODING, and DWIDTH lines have been found.");
			}
			for (s = 8; s < width; s += 8) {}
			rshift = s - width;
			if (Parse_Char(font, (Uint32)encoding, width, &line, end, rshift) < 0) {
				SDL_free(src_data);
				return SDL_SetError("Failed to parse a font character: %s", SDL_GetError());
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

	font->mojis = hashmap_new(sizeof(Moji), 16, 0, 0, Moji_Hash, Moji_Compare, Moji_Free, NULL);
	if (!font->mojis) {
		SDL_free(font);
		SDL_SetError("Failed to create the hashmap for the font object: %s", SDL_GetError());
		return NULL;
	}

	if (src == NULL) {
		return font;
	}
	else if (Kanji_AddFont(font, src) < 0) {
		SDL_free(font);
		SDL_SetError("Failed to parse the font: %s", SDL_GetError());
		return NULL;
	}
	else {
		return font;
	}
}

int Kanji_FontSize(Kanji_Font* font) {
	return font->size;
}

int Kanji_TextWidth(Kanji_Font* font, const char* text) {
	const Uint8* p;
	Uint32 encoding;
	int w;

	if (font == NULL) {
		return SDL_SetError("Parameter \"font\" must not be NULL.");
	}
	else if (text == NULL) {
		return SDL_SetError("Parameter \"text\" must not be NULL.");
	}

	w = 0;
	for (p = UTF8_Next((const Uint8*)text, &encoding); p != NULL && encoding != '\0' && encoding != '\n' && encoding != '\r'; p = UTF8_Next(p, &encoding)) {
		Moji getMoji;
		const Moji* gotMoji;

		getMoji.encoding = encoding;

		if (!(gotMoji = hashmap_get(font->mojis, &getMoji))) {
			return SDL_SetError("The text contains a character not in the font, so width cannot be calculated.");
		}

		if (w > INT_MAX - gotMoji->width) {
			return SDL_SetError("The calculated width of the text overflowed (maximum possible width is %d).", INT_MAX);
		}

		w += gotMoji->width;
	}

	if (p != NULL) {
		return w;
	}
	else {
		return SDL_SetError("Invalid data found in the text. The text must be valid UTF-8 data.");
	}
}

int Kanji_TextDimensions(Kanji_Font* font, const char *text, int* w, int* h) {
	const Uint8* p;
	Uint32 encoding;

	int line_w = 0;
	int temp_w = 0;
	int temp_h = font->size;
	for (p = UTF8_Next((const Uint8*)text, &encoding); p != NULL && encoding != '\0'; p = UTF8_Next(p, &encoding)) {
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

			if (temp_h > INT_MAX - font->size) {
				return SDL_SetError("The calculated height of the text overflowed (maximum possible height is %d).", INT_MAX);
			}
			temp_h += font->size;
			if (line_w > temp_w) {
				temp_w = line_w;
			}
			line_w = 0;
			continue;
		}

		Moji getMoji;
		const Moji* gotMoji;

		getMoji.encoding = encoding;
		if (!(gotMoji = hashmap_get(font->mojis, &getMoji))) {
			return SDL_SetError("The text contains a character not in the font, so dimensions cannot be calculated.");
		}

		if (temp_w > INT_MAX - gotMoji->width) {
			return SDL_SetError("The calculated width of the text overflowed (maximum possible width is %d).", INT_MAX);
		}

		line_w += gotMoji->width;
	}
	if (line_w > temp_w) {
		temp_w = line_w;
	}

	*w = temp_w;
	*h = temp_h;
	return 0;
}

int Kanji_PutPixelSurface(SDL_Surface *s, int x, int y, float subx, float suby, SDL_Color color) {
	Uint8* p, bpp;
	Uint32 mapped_color;

	if (SDL_MUSTLOCK(s)) {
		if (SDL_LockSurface(s) < 0) {
			return SDL_SetError("Failed to lock surface for putting a pixel.");
		}
	}

	bpp = s->format->BytesPerPixel;
	if (bpp != 1 && bpp != 2 && bpp != 4) {
		if(SDL_MUSTLOCK(s)){
			SDL_UnlockSurface(s);
		}
		return SDL_SetError("bpp of surface is not supported (must be 1, 2, or 4).");
	}
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
	}

	if(SDL_MUSTLOCK(s)){
		SDL_UnlockSurface(s);
	}
	return 0;
}

int Kanji_PutPixelRenderer(SDL_Renderer *s, int x, int y, float subx, float suby, SDL_Color pixel) {
	Uint8 r, g, b, a;

	if (SDL_GetRenderDrawColor(s, &r, &g, &b, &a) < 0) return -1;
	if (SDL_SetRenderDrawColor(s, pixel.r, pixel.g, pixel.b, pixel.a) < 0) return -1;
	if (SDL_RenderDrawPointF(s, subx + x, suby + y) < 0) return -1;
	if (SDL_SetRenderDrawColor(s, r, g, b, a) < 0) return -1;
	return 0;
}

int Kanji_PutText(Kanji_Font* font, int dx, int dy, float subx, float suby, void* dst, int dw, int dh, const char* txt, SDL_Color fg, Kanji_PutPixelCallback put_pixel) {
	int cx, cy;
	const Uint8* p;
	Uint32 encoding;

	cx = dx;
	cy = dy;
	for (p = UTF8_Next((const Uint8*)txt, &encoding); p != NULL && encoding != '\0'; p = UTF8_Next(p, &encoding)) {
		Moji getMoji;
		const Moji* gotMoji;
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

		getMoji.encoding = encoding;
		if (!(gotMoji = hashmap_get(font->mojis, &getMoji))) {
			return SDL_SetError("The text contains a character not in the font (U+%04" SDL_PRIX32 "), so it cannot be drawn.", encoding);
		}

		minx = (cx >= 0) ? 0 : -cx;
		miny = (cy >= 0) ? 0 : -cy;
		maxx = (cx + gotMoji->width <= dw) ? gotMoji->width : dw - cx;
		maxy = (cy + font->size <= dh) ? font->size : dh - cy;

		for (y = miny; y < maxy; y++) {
			for (x = minx; x < maxx; x++) {
				if (gotMoji->pixels[y] & (1 << (gotMoji->width - x - 1))) {
					if (put_pixel(dst, cx + x, cy + y, subx, suby, fg) < 0) {
						return -1;
					}
				}
			}
		}
		cx += gotMoji->width;
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

SDL_Surface* Kanji_CreateSurface(Kanji_Font* font, const char* text, SDL_Color fg, int depth) {
	int w, h;
	if (Kanji_TextDimensions(font, text, &w, &h) < 0) {
		return NULL;
	}

	Uint32 format;
	// TODO: Consider supporting additional formats. Indexed should be
	// fine, since the surface only has to support full-transparent and a
	// single color.
	switch (depth) {
	case 16:
		format = SDL_PIXELFORMAT_RGBA5551;
		break;

	case 32:
		format = SDL_PIXELFORMAT_RGBA8888;
		break;

	default:
		SDL_SetError("Invalid depth of %d, only 16 or 32 are supported.", depth);
		return NULL;
	}

	SDL_Surface* surface = SDL_CreateRGBSurfaceWithFormat(0, w, h, depth, format);
	if (!surface) {
		return NULL;
	}

	const Uint32 alpha = SDL_MapRGBA(surface->format, 0x00, 0x00, 0x00, SDL_ALPHA_TRANSPARENT);
	if (SDL_FillRect(surface, NULL, alpha) < 0) {
		SDL_FreeSurface(surface);
		return NULL;
	}

	if (Kanji_PutTextSurface(font, 0, 0, 0.0f, 0.0f, surface, text, fg) < 0) {
		SDL_FreeSurface(surface);
		return NULL;
	}

	return surface;
}

SDL_Texture* Kanji_CreateTexture(Kanji_Font* font, SDL_Renderer* renderer, const char* text, SDL_Color fg, int depth) {
	SDL_Surface* surface = Kanji_CreateSurface(font, text, fg, depth);
	if (!surface) {
		return NULL;
	}

	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_FreeSurface(surface);
	if (!texture) {
		return NULL;
	}

	return texture;
}

void Kanji_CloseFont(Kanji_Font* font) {
	SDL_assert(font != NULL);

	hashmap_free(font->mojis);
	SDL_free(font);
}
