#include "SDL_kanji.h"

#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define BUF 256

static void InitFont(Kanji_Font* font) {
	int i;
	for (i = 0; i < 96*96 + 256; i++) {
		font->moji[i] = 0;
	}
}

Kanji_Font* Kanji_OpenFont(SDL_RWops* src, int size) {
	Kanji_Font* font;
	font = (Kanji_Font*)malloc(sizeof(Kanji_Font));

	font->k_size = size;
	font->a_size = size / 2;
	font->sys = KANJI_JIS;

	InitFont(font);
	if (0 == Kanji_AddFont(font, src)) {
		return font;
	}
	else {
		free(font);
		return 0;
	}
}

static char* sgets(char* buf, int count, char** src, char* end) {
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

static void ParseChar(Kanji_Font* font, int index, char** line, char *end, int shift) {
	/* 既にロードされている文字は読み込まない */
	if (font->moji[index] != 0) return;

	font->moji[index] = (Uint32*)malloc(sizeof(Uint32)*font->k_size);

	char buf[BUF];
	for (size_t y = 0; y < font->k_size; y++) {
		sgets(buf, BUF, line, end);
		font->moji[index][y] = (strtol(buf, 0, 16) >> shift);
	}
}

static int ParseFont(Kanji_Font* font, SDL_RWops* src) {
	char buf[BUF];

	Sint64 size = SDL_RWsize(src);
	char* srcdata = malloc(size);
	if (!srcdata) return 1;
	if (SDL_RWread(src, srcdata, size, 1) == 0) {
		free(srcdata);
		return 1;
	}

	char* end = srcdata + size;
	char *p;
	int index;
	int k_rshift, a_rshift;
	int s;

	for (s = 8; s < font->k_size; s += 8) {}
	k_rshift = s - font->k_size;
	for (s = 8; s < font->a_size; s += 8) {}
	a_rshift = s - font->a_size;

	char* line = srcdata;
	while (1) {
		if (sgets(buf, BUF, &line, end) == NULL) {
			break;
		}

		if (strstr(buf, "ENCODING") != NULL) {
			p = strchr(buf, ' ');
			index = strtol(p, 0, 10);

			while (strstr(buf, "BITMAP") == NULL) {
				sgets(buf, BUF, &line, end);
			}

			if (index > 255) {
				index = (((index & 0xff00) >> 8) - 0x20) * 96
					+ (index & 0xff) - 0x20 + 0xff;
				ParseChar(font, index, &line, end, k_rshift);
			}
			else {
				ParseChar(font, index, &line, end, a_rshift);
			}
		}
	}

	free(srcdata);

	return 0;
}

int Kanji_AddFont(Kanji_Font* font, SDL_RWops *src) {
	if (0 != ParseFont(font, src)) return -1;
	return 0;
}

int Kanji_FontHeight(Kanji_Font* font) {
	return font->k_size;
}

int Kanji_FontWidth(Kanji_Font* font, const char* text) {
	if (text == 0) return font->a_size;
	else return strlen(text) * font->a_size;
}

void Kanji_SetCodingSystem(Kanji_Font* font, Kanji_CodingSystem sys) {
	font->sys = sys;
}

static void euc2jis(unsigned char *c1, unsigned char *c2) {
	*c1 &= 0x7f;
	*c2 &= 0x7f;
}

static void sjis2jis(unsigned char *c1, unsigned char *c2) {
	if (*c2 < 0x9f) {
		if (*c1 < 0xa0) {
			*c1 -= 0x81;
			*c1 *= 2;
			*c1 += 0x21;
		}
		else {
			*c1 -= 0xe0;
			*c1 *= 2;
			*c1 += 0x5f;
		}
		if (*c2 > 0x7f) {
			--*c2;
		}
		*c2 -= 0x1f;
	}
	else {
		if (*c1 < 0xa0) {
			*c1 -= 0x81;
			*c1 *= 2;
			*c1 += 0x22;
		}
		else {
			*c1 -= 0xe0;
			*c1 *= 2;
			*c1 += 0x60;
		}
		*c2 -= 0x7e;
	}
}

static void ConvertCodingSystem(Kanji_Font* font, unsigned char *c1, unsigned char *c2) {
	if (font->sys == KANJI_SJIS) {
		sjis2jis(c1, c2);
	}
	else if (font->sys == KANJI_EUC) {
		euc2jis(c1, c2);
	}
}

static void Kanji_PutpixelSurface(SDL_Surface *s, int x, int y, SDL_Color color) {
	Uint8 *p,bpp;
	if(SDL_MUSTLOCK(s)){
		if(SDL_LockSurface(s)<0) return;
	}
	bpp=s->format->BytesPerPixel;
	p=(Uint8*)(s->pixels)+y*s->pitch+x*bpp;

	Uint32 mappedColor = SDL_MapRGB(s->format, color.r, color.g, color.b);
	switch(bpp){
	case 1:
		*((Uint8 *)p)=(Uint8)mappedColor;
		break;
	case 2:
		*((Uint16 *)p)=(Uint16)mappedColor;
		break;
	case 4:
		*((Uint32 *)p)=mappedColor;
		break;
	}

	if(SDL_MUSTLOCK(s)){
		SDL_UnlockSurface(s);
	}
}

static void Kanji_PutpixelRenderer(SDL_Renderer *s, int x, int y, SDL_Color pixel) {
	Uint8 r, g, b, a;
	SDL_GetRenderDrawColor(s, &r, &g, &b, &a);
	SDL_SetRenderDrawColor(s, pixel.r, pixel.g, pixel.b, pixel.a);
	SDL_RenderDrawPoint(s, x, y);
	SDL_SetRenderDrawColor(s, r, g, b, a);
}

void Kanji_PutText(Kanji_Font* font, int dx, int dy, void* dst, int dw, int dh, const char* txt, SDL_Color fg, void (* putPixel)(void* dst, int x, int y, SDL_Color color)) {
	int index;
	int x, y, cx = dx, cy = dy;
	unsigned char high, low;
	int minx, miny, maxx, maxy;
	int nowKanji = 0;
	const unsigned char* text = (const unsigned char*)txt;

	while (*text != 0) {
		if (*text == '\n') {
			text ++;
			cy += font->k_size;
			cx = dx;
			continue;
		}

		if (font->sys == KANJI_JIS && *text == 0x1b) {
			if (*(text+1) == 0x24 && *(text+2) == 0x42) {
				nowKanji = 1;
			}
			else if (*(text+1) == 0x28 && *(text+2) == 0x42) {
				nowKanji = 0;
			}
			text += 3;
			continue;
		}
		if (font->sys != KANJI_JIS) nowKanji = !isprint(*text);

		if (!nowKanji) {
			index = *text;
			text++;
			if (font->moji[index] == 0) {
				cx += font->a_size;
				continue;
			}

			minx = (cx >= 0) ? 0 : -cx;
			miny = (cy >= 0) ? 0 : -cy;
			maxx = (cx+font->a_size <= dw) ? font->a_size : dw-cx;
			maxy = (cy+font->k_size <= dh) ? font->k_size : dh-cy;

			for (y = miny; y < maxy; y++) {
				for (x = minx; x < maxx; x++) {
					if (font->moji[index][y] & (1 << (font->a_size-x-1))) {
						putPixel(dst, cx+x, cy+y, fg);
					}
				}
			}
			cx += font->a_size;
		}
		else {
			high = *text;
			low = *(text+1);
			ConvertCodingSystem(font, &high, &low);
			index = (high - 0x20) * 96 + low - 0x20 + 0xff;
			text += 2;
			if (font->moji[index] == 0) {
				cx += font->k_size;
				continue;
			}

			minx = (cx >= 0) ? 0 : -cx;
			miny = (cy >= 0) ? 0 : -cy;
			maxx = (cx+font->k_size <= dw) ? font->k_size : dw-cx;
			maxy = (cy+font->k_size <= dh) ? font->k_size : dh-cy;

			for (y = miny; y < maxy; y++) {
				for (x = minx; x < maxx; x++) {
					if (font->moji[index][y] & (1 << (font->k_size-x-1))) {
						putPixel(dst, cx+x, cy+y, fg);
					}
				}
			}
			cx += font->k_size;
		}
	}
}

void Kanji_PutTextSurface(Kanji_Font* font, int dx, int dy, SDL_Surface* dst, const char* txt, SDL_Color fg) {
	Kanji_PutText(font, dx, dy, dst, dst->w, dst->h, txt, fg, (void (*)(void*, int, int, SDL_Color))Kanji_PutpixelSurface);
}

void Kanji_PutTextRenderer(Kanji_Font* font, int dx, int dy, SDL_Renderer* dst, const char* txt, SDL_Color fg) {
	int dw, dh;
	SDL_RenderGetLogicalSize(dst, &dw, &dh);
	Kanji_PutText(font, dx, dy, dst, dw, dh, txt, fg, (void (*)(void*, int, int, SDL_Color))Kanji_PutpixelRenderer);
}

void Kanji_PutTextTate(Kanji_Font* font, int dx, int dy, void* dst, int dw, int dh, const char* txt, SDL_Color fg, void (* putPixel)(void* dst, int x, int y, SDL_Color color)) {
	int index;
	int x, y, cx = dx, cy = dy;
	unsigned char high, low;
	int minx, miny, maxx, maxy;
	int nowKanji = 0;
	const unsigned char* text = (const unsigned char*)txt;

	while (*text != 0) {
		if (font->sys == KANJI_JIS && *text == 0x1b) {
			if (*(text+1) == 0x24 && *(text+2) == 0x42) {
				nowKanji = 1;
			}
			else if (*(text+1) == 0x28 && *(text+2) == 0x42) {
				nowKanji = 0;
			}
			text += 3;
			continue;
		}
		if (font->sys != KANJI_JIS) nowKanji = !isprint(*text);

		/* ASCII は無視 */
		if (!nowKanji) {
			text++;
		}
		else {
			high = *text;
			low = *(text+1);
			ConvertCodingSystem(font, &high, &low);
			index = (high - 0x20) * 96 + low - 0x20 + 0xff;
			text += 2;
			if (font->moji[index] == 0) {
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
					if (font->moji[index][y] & (1 << (font->k_size-x-1))) {
						putPixel(dst, cx+x, cy+y, fg);
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
}

void Kanji_PutTextTateSurface(Kanji_Font* font, int dx, int dy, SDL_Surface* dst, const char* txt, SDL_Color fg) {
	Kanji_PutTextTate(font, dx, dy, dst, dst->w, dst->h, txt, fg, (void (*)(void*, int, int, SDL_Color))Kanji_PutpixelSurface);
}

void Kanji_PutTextTateRenderer(Kanji_Font* font, int dx, int dy, SDL_Renderer* dst, const char* txt, SDL_Color fg) {
	int dw, dh;
	SDL_RenderGetLogicalSize(dst, &dw, &dh);
	Kanji_PutTextTate(font, dx, dy, dst, dw, dh, txt, fg, (void (*)(void*, int, int, SDL_Color))Kanji_PutpixelRenderer);
}

SDL_Surface* Kanji_CreateSurface(Kanji_Font* font, const char* text, SDL_Color fg, int bpp) {
	SDL_Surface* textbuf;
	int len;
	Uint32 bgcol;

	if (text == NULL) return NULL;
	if (*text == 0 ) return NULL;
	len = strlen(text);

	textbuf = SDL_CreateRGBSurface(0, font->a_size*len, font->k_size, bpp, 0, 0, 0, 0);
	if (textbuf == NULL) {
		fprintf(stderr,"ERROR: at Kanji_RenderText\n");
		return NULL;
	}
	bgcol = SDL_MapRGB(textbuf->format, 255-fg.r, 255-fg.g, 255-fg.b);
	SDL_FillRect(textbuf, NULL, bgcol);
	SDL_SetColorKey(textbuf, SDL_TRUE, bgcol);

	Kanji_PutTextSurface(font, 0, 0, textbuf, text, fg);

	return textbuf;
}

SDL_Surface* Kanji_CreateSurfaceTate(Kanji_Font* font, const char* text, SDL_Color fg, int bpp) {
	SDL_Surface* textbuf;
	int len;
	Uint32 bgcol;

	if (text == NULL) return NULL;
	if (*text == 0 ) return NULL;
	len = strlen(text);

	textbuf = SDL_CreateRGBSurface(0, font->k_size, font->a_size*len,
							   bpp, 0, 0, 0, 0);
	if (textbuf == NULL) {
		fprintf(stderr,"ERROR: at Kanji_RenderText\n");
		return NULL;
	}

	bgcol = SDL_MapRGB(textbuf->format, 255-fg.r, 255-fg.g, 255-fg.b);
	SDL_FillRect(textbuf, NULL, bgcol);
	SDL_SetColorKey(textbuf, SDL_TRUE, bgcol);

	Kanji_PutTextTateSurface(font, 0, 0, textbuf, text, fg);

	return textbuf;
}

void Kanji_CloseFont(Kanji_Font* font) {
	int i;
	for (i = 0; i < 96*96 + 256; i++) {
		if (font->moji[i] != 0) {
			free(font->moji[i]);
		}
	}
	free(font);
}