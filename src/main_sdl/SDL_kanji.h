#pragma once

#include "include.h"

typedef enum { KANJI_SJIS, KANJI_EUC, KANJI_JIS } Kanji_CodingSystem;

typedef struct {
	int k_size;
	int a_size;
	Kanji_CodingSystem sys;
	/* メモリ効率わるいけど、まあ死にはしないわな。 */
	Uint32* moji[96*96+256];
} Kanji_Font;

Kanji_Font* Kanji_OpenFont(SDL_RWops *src, int size);

int Kanji_AddFont(Kanji_Font* font, SDL_RWops *src);

int Kanji_FontHeight(Kanji_Font* font);
/* text == 0 なら ASCII 一文字分の幅を返す */
int Kanji_FontWidth(Kanji_Font* font, const char* text);

void Kanji_SetCodingSystem(Kanji_Font* font, Kanji_CodingSystem sys);

void Kanji_PutText(Kanji_Font* font, int dx, int dy, float subx, float suby, void* dst, int dw, int dh, const char* txt, SDL_Color fg, void (* putPixel)(void* dst, int x, int y, float subx, float suby, SDL_Color color));
void Kanji_PutTextSurface(Kanji_Font* font, int dx, int dy, float subx, float suby, SDL_Surface* dst, const char* text, SDL_Color fg);
void Kanji_PutTextRenderer(Kanji_Font* font, int dx, int dy, float subx, float suby, SDL_Renderer* dst, const char* text, SDL_Color fg);

void Kanji_PutTextTate(Kanji_Font* font, int dx, int dy, float subx, float suby, void* dst, int dw, int dh, const char* txt, SDL_Color fg, void (* putPixel)(void* dst, int x, int y, float subx, float suby, SDL_Color color));
void Kanji_PutTextTateSurface(Kanji_Font* font, int dx, int dy, float subx, float suby, SDL_Surface* dst, const char* text, SDL_Color fg);
void Kanji_PutTextTateRenderer(Kanji_Font* font, int dx, int dy, float subx, float suby, SDL_Renderer* dst, const char* txt, SDL_Color fg);

SDL_Surface* Kanji_CreateSurface(Kanji_Font* font, const char* text, SDL_Color fg, int bpp);
SDL_Surface* Kanji_CreateSurfaceTate(Kanji_Font* font, const char* text, SDL_Color fg, int bpp);

void Kanji_CloseFont(Kanji_Font* font);