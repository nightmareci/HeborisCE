#include "APP_bdf.h"
#include "APP_hashtable.h"
#include "APP_error.h"
#include <limits.h>

#define BDF_LINE_MAX 256

struct APP_BDFFont
{
	int size;
	APP_HashTable* glyphs;
};

static char* BDF_GetLine(char* buffer, int length, char** src, char* end)
{
	if (buffer != NULL && length > 0 && *src < end) {
		int i;
		for (i = 0; i < length - 1 && *src + i < end; i++) {
			if ((*src)[i] == '\n' || (*src)[i] == '\r') {
				char endc = (*src)[i];
				buffer[i] = endc;
				i++;
				if (i < length - 1 && *src + i < end) {
					if (((*src)[i] == '\n' || (*src)[i] == '\r') && (*src)[i] != endc) {
						buffer[i] = (*src)[i];
						i++;
					}
				}
				buffer[i] = '\0';
				*src += i;
				if (*src < end) {
					return buffer;
				}
				else {
					return NULL;
				}
			}
			else {
				buffer[i] = (*src)[i];
			}
		}
		buffer[i] = '\0';
		*src += i;
		return buffer;
	}

	return NULL;
}

typedef struct BDF_Glyph
{
	uint32_t encoding;
	int width;
	uint32_t* pixels;
} BDF_Glyph;

static const uint8_t* BDF_NextUTF8Encoding(const uint8_t* text, uint32_t* encoding)
{
	uint32_t tmpEncoding;
	int bytes;
	const uint8_t* c;

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

	tmpEncoding = *text & 0xFFu;
	if ((tmpEncoding & 0x80u) == 0x00u) {
		*encoding = tmpEncoding;
		return text + 1;
	}

	if ((tmpEncoding & 0xE0u) == 0xC0u) {
		if (text[1] == '\0') {
			return NULL;
		}
		tmpEncoding &= 0x1Fu;
		bytes = 2;
	}
	else if ((tmpEncoding & 0xF0u) == 0xE0u) {
		if (text[1] == '\0' || text[2] == '\0') {
			return NULL;
		}
		tmpEncoding &= 0xFu;
		bytes = 3;
	}
	else if ((tmpEncoding & 0xF1u) == 0xF0u) {
		if (text[1] == '\0' || text[2] == '\0' || text[3] == '\0') {
			return NULL;
		}
		tmpEncoding &= 0x7u;
		bytes = 4;
	}
	else {
		return NULL;
	}

	for (c = text + 1; c < text + bytes; c++) {
		const Uint8 byte = *c;
		if ((byte & 0xC0u) != 0x80u) {
			return NULL;
		}
		tmpEncoding = (tmpEncoding << 6) | (byte & 0x3Fu);
	}

	switch (bytes) {
	case 2:
		if (tmpEncoding <= 0x7Fu) {
			return NULL;
		}
		break;

	case 3:
		if (tmpEncoding <= 0x7FFu) {
			return NULL;
		}
		break;

	case 4:
		if (tmpEncoding <= 0xFFFFu) {
			return NULL;
		}
		break;

	default:
		return NULL;
	}

	if ((tmpEncoding >= 0xD800u && tmpEncoding <= 0xDFFFu) || tmpEncoding > 0x10FFFFu) {
		return NULL;
	}
	else {
		*encoding = tmpEncoding;
		return text + bytes;
	}
}

void BDF_DestroyGlyph(uint64_t key, void* value, void* userdata)
{
	(void)key;
	(void)userdata;
	SDL_free(value);
}

static bool BDF_ParseChar(APP_BDFFont* font, uint32_t encoding, int width, char** line, char* end, int rshift)
{
	// Do not load characters that have already been loaded
	if (APP_HashTableHasValue(font->glyphs, encoding)) {
		return true;
	}

	BDF_Glyph* glyph = SDL_malloc(sizeof(BDF_Glyph));
	if (!glyph) {
		return APP_SetError("Failed allocating glyph data to parse a BDF character");
	}
	glyph->width = width;

	glyph->pixels = SDL_malloc(sizeof(uint32_t) * font->size);
	if (!glyph->pixels) {
		APP_SetError("Failed to allocate memory for a character's pixels of a font.");
		SDL_free(glyph);
		return false;
	}

	char buffer[BDF_LINE_MAX];
	for (int y = 0; y < font->size; y++) {
		BDF_GetLine(buffer, sizeof(buffer), line, end);
		glyph->pixels[y] = (uint32_t)(SDL_strtol(buffer, NULL, 16) >> rshift);
	}

	if (APP_SetHashTableValue(font->glyphs, encoding, glyph, BDF_DestroyGlyph, NULL)) {
		return true;
	}
	else {
		SDL_free(glyph->pixels);
		SDL_free(glyph);
		return false;
	}
}

bool APP_AddBDFFont(APP_BDFFont* font, SDL_IOStream* src)
{
	char* srcData;
	char* end;
	char* line;
	char* p;
	long encoding;
	long fontSize;
	long width;
	int s;
	char buffer[BDF_LINE_MAX];
	int64_t srcSize;
	bool foundSize;
	bool foundEncoding;
	bool foundWidth;

	srcSize = SDL_GetIOSize(src);
	srcData = SDL_malloc(srcSize);
	if (!srcData) return false;
	if (SDL_ReadIO(src, srcData, srcSize) == 0) {
		APP_SetError("Failed to read the font data: %s", SDL_GetError());
		SDL_free(srcData);
		return false;
	}

	end = srcData + srcSize;
	line = srcData;

	foundSize = false;
	foundEncoding = false;
	foundWidth = false;
	while (BDF_GetLine(buffer, sizeof(buffer), &line, end) != NULL) {
		if (SDL_strstr(buffer, "SIZE") == buffer) {
			if (foundSize) {
				SDL_free(srcData);
				return APP_SetError("Erroneous extra SIZE line encountered.");
			}
			p = buffer;
			while (*p != ' ' && *p != '\t') p++;
			while (*p == ' ' || *p == '\t') p++;
			fontSize = SDL_strtol(p, NULL, 10);
			if (fontSize <= 0 || fontSize > INT_MAX) {
				SDL_free(srcData);
				return APP_SetError("Font size of %ld is invalid, must be between 0 and %d.", fontSize, INT_MAX);
			}
			if (font->size == 0) {
				font->size = (int)fontSize;
			}
			foundSize = true;
		}
		else if (SDL_strstr(buffer, "STARTCHAR") == buffer) {
			if (!foundSize) {
				SDL_free(srcData);
				return APP_SetError("SIZE line not found before a STARTCHAR line.");
			}
			foundEncoding = false;
			foundWidth = false;
		}
		else if (SDL_strstr(buffer, "ENCODING") == buffer) {
			if (!foundSize) {
				SDL_free(srcData);
				return APP_SetError("SIZE line not found before an ENCODING line.");
			}
			p = buffer;
			while (*p != ' ' && *p != '\t') p++;
			while (*p == ' ' || *p == '\t') p++;
			encoding = SDL_strtol(p, NULL, 10);
			if (encoding < 0) {
				SDL_free(srcData);
				return APP_SetError("Character encoding of %ld is invalid, must be nonnegative.", encoding);
			}
			foundEncoding = true;
		}
		else if (SDL_strstr(buffer, "DWIDTH") == buffer) {
			if (!foundSize) {
				SDL_free(srcData);
				return APP_SetError("SIZE line not found before a DWIDTH line.");
			}
			p = buffer;
			while (*p != ' ' && *p != '\t') p++;
			while (*p == ' ' || *p == '\t') p++;
			width = SDL_strtol(p, NULL, 10);
			if (width <= 0 || width > INT_MAX) {
				SDL_free(srcData);
				return APP_SetError("Character width of %ld is invalid, must be between 0 and %d.", width, INT_MAX);
			}
			foundWidth = true;
		}
		else if (SDL_strstr(buffer, "BITMAP") == buffer) {
			int rshift;
			if (!foundSize || !foundEncoding || !foundWidth) {
				SDL_free(srcData);
				return APP_SetError("BITMAP line encountered before all of SIZE, ENCODING, and DWIDTH lines have been found.");
			}
			for (s = 8; s < width; s += 8) {}
			rshift = s - (int)width;
			if (!BDF_ParseChar(font, (Uint32)encoding, (int)width, &line, end, rshift)) {
				SDL_free(srcData);
				return APP_SetError("Failed to parse a font character: %s", SDL_GetError());
			}
		}
	}

	SDL_free(srcData);
	return true;
}

APP_BDFFont* APP_OpenBDFFont(SDL_IOStream* src)
{
	APP_BDFFont* font;

	font = SDL_malloc(sizeof(APP_BDFFont));
	if (!font) {
		APP_SetError("Failed to allocate memory for the font object.");
		return NULL;
	}

	font->size = 0;

	font->glyphs = APP_CreateHashTable(0x100);
	if (!font->glyphs) {
		SDL_free(font);
		return NULL;
	}

	if (!src) {
		return font;
	}
	else if (!APP_AddBDFFont(font, src)) {
		SDL_free(font);
		return NULL;
	}
	else {
		return font;
	}
}

int APP_GetBDFFontSize(APP_BDFFont* font)
{
	return font->size;
}

int APP_GetBDFTextWidth(APP_BDFFont* font, const char* text)
{
	const uint8_t* p;
	uint32_t encoding;
	int w;

	if (font == NULL) {
		return APP_SetError("Parameter \"font\" must not be NULL.");
	}
	else if (text == NULL) {
		return APP_SetError("Parameter \"text\" must not be NULL.");
	}

	w = 0;
	for (p = BDF_NextUTF8Encoding((const Uint8*)text, &encoding); p != NULL && encoding != '\0' && encoding != '\n' && encoding != '\r'; p = BDF_NextUTF8Encoding(p, &encoding)) {
		BDF_Glyph* glyph;

		if (!APP_GetHashTableValue(font->glyphs, encoding, (void**)&glyph)) {
			return APP_SetError("The text contains a character not in the font, so width cannot be calculated.");
		}
		if (w > INT_MAX - glyph->width) {
			return APP_SetError("The calculated width of the text overflowed (maximum possible width is %d).", INT_MAX);
		}

		w += glyph->width;
	}

	if (p != NULL) {
		return w;
	}
	else {
		return APP_SetError("Invalid data found in the text. The text must be valid UTF-8 data.");
	}
}

int APP_GetBDFTextDimensions(APP_BDFFont* font, const char *text, int* w, int* h)
{
	const uint8_t* p;
	uint32_t encoding;

	int line_w = 0;
	int temp_w = 0;
	int temp_h = font->size;
	for (p = BDF_NextUTF8Encoding((const Uint8*)text, &encoding); p != NULL && encoding != '\0'; p = BDF_NextUTF8Encoding(p, &encoding)) {
		// Handle any newline format
		if (encoding == '\n' || encoding == '\r') {
			// If we see '\r' and '\n' adjacent, we should consider those two bytes as meaning one newline.
			if (encoding == '\r' && *p == '\n') {
				p++;
			}
			// Only one byte for newline formats of only one '\r' or '\n', so don't increment.

			if (temp_h > INT_MAX - font->size) {
				return APP_SetError("The calculated height of the text overflowed (maximum possible height is %d).", INT_MAX);
			}
			temp_h += font->size;
			if (line_w > temp_w) {
				temp_w = line_w;
			}
			line_w = 0;
			continue;
		}

		BDF_Glyph* glyph;

		if (!APP_GetHashTableValue(font->glyphs, encoding, (void**)&glyph)) {
			return APP_SetError("The text contains a character not in the font, so dimensions cannot be calculated.");
		}
		if (temp_w > INT_MAX - glyph->width) {
			return APP_SetError("The calculated width of the text overflowed (maximum possible width is %d).", INT_MAX);
		}

		line_w += glyph->width;
	}
	if (line_w > temp_w) {
		temp_w = line_w;
	}

	*w = temp_w;
	*h = temp_h;
	return 0;
}

bool APP_PutBDFPixelSurface(SDL_Surface *s, int x, int y, float subx, float suby, SDL_Color color)
{
	(void)subx;
	(void)suby;
	return SDL_WriteSurfacePixel(s, x, y, color.r, color.g, color.b, color.a);
}

bool APP_PutBDFPixelRenderer(SDL_Renderer *s, int x, int y, float subx, float suby, SDL_Color pixel)
{
	uint8_t r, g, b, a;

	return
		SDL_GetRenderDrawColor(s, &r, &g, &b, &a) &&
		SDL_SetRenderDrawColor(s, pixel.r, pixel.g, pixel.b, pixel.a) &&
		SDL_RenderPoint(s, subx + x, suby + y) &&
		SDL_SetRenderDrawColor(s, r, g, b, a);
}

bool APP_PutBDFText(APP_BDFFont* font, int dx, int dy, float subx, float suby, void* dst, int dw, int dh, const char* txt, SDL_Color fg, APP_PutBDFPixelCallback put_pixel)
{
	int cx, cy;
	const uint8_t* p;
	uint32_t encoding;

	cx = dx;
	cy = dy;
	for (p = BDF_NextUTF8Encoding((const uint8_t*)txt, &encoding); p != NULL && encoding != '\0'; p = BDF_NextUTF8Encoding(p, &encoding)) {
		const BDF_Glyph* glyph;
		int x, y;
		int minx, miny, maxx, maxy;

		// Handle any newline format
		if (encoding == '\n' || encoding == '\r') {
			// If we see '\r' and '\n' adjacent, we should consider those two bytes as meaning one newline.
			if (encoding == '\r' && *p == '\n') {
				p++;
			}
			// Only one byte for newline formats of only one '\r' or '\n', so don't increment.

			cy += font->size;
			cx = dx;
			continue;
		}

		if (!APP_GetHashTableValue(font->glyphs, encoding, (void**)&glyph)) {
			return APP_SetError("The text contains a character not in the font (U+%04" SDL_PRIX32 "), so it cannot be drawn.", encoding);
		}

		minx = (cx >= 0) ? 0 : -cx;
		miny = (cy >= 0) ? 0 : -cy;
		maxx = (cx + glyph->width <= dw) ? glyph->width : dw - cx;
		maxy = (cy + font->size <= dh) ? font->size : dh - cy;

		for (y = miny; y < maxy; y++) {
			for (x = minx; x < maxx; x++) {
				if (glyph->pixels[y] & (1 << (glyph->width - x - 1))) {
					if (!put_pixel(dst, cx + x, cy + y, subx, suby, fg)) {
						return false;
					}
				}
			}
		}
		cx += glyph->width;
	}

	return true;
}

bool APP_PutBDFTextSurface(APP_BDFFont* font, int dx, int dy, float subx, float suby, SDL_Surface* dst, const char* txt, SDL_Color fg)
{
	return APP_PutBDFText(font, dx, dy, subx, suby, dst, dst->w, dst->h, txt, fg, (APP_PutBDFPixelCallback)APP_PutBDFPixelSurface);
}

bool APP_PutBDFTextRenderer(APP_BDFFont* font, int dx, int dy, float subx, float suby, SDL_Renderer* dst, const char* txt, SDL_Color fg)
{
	int dw, dh;
	SDL_RendererLogicalPresentation mode;
	SDL_GetRenderLogicalPresentation(dst, &dw, &dh, &mode);
	return APP_PutBDFText(font, dx, dy, subx, suby, dst, dw, dh, txt, fg, (APP_PutBDFPixelCallback)APP_PutBDFPixelRenderer);
}

SDL_Surface* APP_CreateBDFTextSurface(APP_BDFFont* font, const char* text, SDL_Color fg, int depth)
{
	int w, h;
	if (APP_GetBDFTextDimensions(font, text, &w, &h) < 0) {
		return NULL;
	}

	uint32_t format;
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
		APP_SetError("Invalid depth of %d, only 16 or 32 are supported.", depth);
		return NULL;
	}

	SDL_Surface* surface = SDL_CreateSurface(w, h, format);
	if (!surface) {
		return NULL;
	}

	if (!SDL_ClearSurface(surface, 0.0f, 0.0f, 0.0f, SDL_ALPHA_TRANSPARENT_FLOAT)) {
		SDL_DestroySurface(surface);
		return NULL;
	}

	if (!APP_PutBDFTextSurface(font, 0, 0, 0.0f, 0.0f, surface, text, fg)) {
		SDL_DestroySurface(surface);
		return NULL;
	}

	return surface;
}

SDL_Texture* APP_CreateBDFTextTexture(APP_BDFFont* font, SDL_Renderer* renderer, const char* text, SDL_Color fg, int depth, SDL_ScaleMode mode)
{
	SDL_Surface* surface = APP_CreateBDFTextSurface(font, text, fg, depth);
	if (!surface) {
		return NULL;
	}

	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_DestroySurface(surface);
	if (!texture) {
		return NULL;
	}
	if (!SDL_SetTextureScaleMode(texture, mode)) {
		SDL_DestroyTexture(texture);
		return NULL;
	}

	return texture;
}

void APP_CloseBDFFont(APP_BDFFont* font)
{
	SDL_assert(font != NULL);

	APP_DestroyHashTable(font->glyphs);
	SDL_free(font);
}
