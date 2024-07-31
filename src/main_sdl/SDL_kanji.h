#ifndef SDL_kanji_utf8_
#define SDL_kanji_utf8_

#include "SDL.h"

struct Kanji_Font;
typedef struct Kanji_Font Kanji_Font;

/**
 * Create a font object, optionally loading characters from src. If src ==
 * NULL, an empty font object is created. Use Kanji_AddFont to add font(s) to
 * an empty font object.
 */
Kanji_Font* Kanji_OpenFont(SDL_RWops* src);

/**
 * Add characters in the font data src contains to the font object. Characters
 * already in the font object will not be replaced. The src font data's font
 * must be the same font size as the size of the font object, or can be any
 * size if the font object is currently empty.
 */
int Kanji_AddFont(Kanji_Font* font, SDL_RWops* src);

/**
 * Get the size of a font.
 */
Uint32 Kanji_FontSize(Kanji_Font* font);

/**
 * Get the width of text in a font.
 */
Sint64 Kanji_TextWidth(Kanji_Font* font, const char* text);

typedef int (* Kanji_PutPixelCallback)(void* dst, int x, int y, float subx, float suby, SDL_Color color);
int Kanji_PutPixelSurface(SDL_Surface *s, int x, int y, float subx, float suby, SDL_Color color);
int Kanji_PutPixelRenderer(SDL_Renderer *s, int x, int y, float subx, float suby, SDL_Color pixel);

int Kanji_PutText(Kanji_Font* font, int dx, int dy, float subx, float suby, void* dst, int dw, int dh, const char* txt, SDL_Color fg, Kanji_PutPixelCallback put_pixel);
int Kanji_PutTextSurface(Kanji_Font* font, int dx, int dy, float subx, float suby, SDL_Surface* dst, const char* text, SDL_Color fg);
int Kanji_PutTextRenderer(Kanji_Font* font, int dx, int dy, float subx, float suby, SDL_Renderer* dst, const char* text, SDL_Color fg);

int Kanji_PutTextTate(Kanji_Font* font, int dx, int dy, float subx, float suby, void* dst, int dw, int dh, const char* txt, SDL_Color fg, Kanji_PutPixelCallback put_pixel);
int Kanji_PutTextTateSurface(Kanji_Font* font, int dx, int dy, float subx, float suby, SDL_Surface* dst, const char* text, SDL_Color fg);
int Kanji_PutTextTateRenderer(Kanji_Font* font, int dx, int dy, float subx, float suby, SDL_Renderer* dst, const char* txt, SDL_Color fg);

SDL_Surface* Kanji_CreateSurface(Kanji_Font* font, const char* text, SDL_Color fg, int bpp);
SDL_Surface* Kanji_CreateSurfaceTate(Kanji_Font* font, const char* text, SDL_Color fg, int bpp);

void Kanji_CloseFont(Kanji_Font* font);

#endif
