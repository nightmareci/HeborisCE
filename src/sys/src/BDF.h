#ifndef BDF_h_
#define BDF_h_

#include <SDL3/SDL.h>

struct BDF_Font;
typedef struct BDF_Font BDF_Font;

/**
 * Create a BDF font object, optionally loading characters from src. If src ==
 * NULL, an empty font object is created. Use BDF_AddFont to add font(s) to
 * an empty font object.
 */
BDF_Font* BDF_OpenFont(SDL_IOStream* src);

/**
 * Add characters in the font data src contains to the font object. Characters
 * already in the font object will not be replaced. The src font data's font
 * must be the same font size as the size of the font object, or can be any
 * size if the font object is currently empty.
 */
bool BDF_AddFont(BDF_Font* font, SDL_IOStream* src);

/**
 * Get the size of a font.
 */
int BDF_GetFontSize(BDF_Font* font);

/**
 * Get the width of a line of text in a font.
 */
int BDF_GetTextWidth(BDF_Font* font, const char* text);

/**
 * Get the dimensions of the rectangle containing all lines of the text in a
 * font.
 */
int BDF_GetTextDimensions(BDF_Font* font, const char *text, int* w, int* h);

typedef bool (*BDF_PutPixelCallback)(void* dst, int x, int y, float subx, float suby, SDL_Color color);
bool BDF_PutPixelSurface(SDL_Surface* s, int x, int y, float subx, float suby, SDL_Color color);
bool BDF_PutPixelRenderer(SDL_Renderer* s, int x, int y, float subx, float suby, SDL_Color pixel);

bool BDF_PutText(BDF_Font* font, int dx, int dy, float subx, float suby, void* dst, int dw, int dh, const char* txt, SDL_Color fg, BDF_PutPixelCallback put_pixel);
bool BDF_PutTextSurface(BDF_Font* font, int dx, int dy, float subx, float suby, SDL_Surface* dst, const char* text, SDL_Color fg);
bool BDF_PutTextRenderer(BDF_Font* font, int dx, int dy, float subx, float suby, SDL_Renderer* dst, const char* text, SDL_Color fg);

SDL_Surface* BDF_CreateTextSurface(BDF_Font* font, const char* text, SDL_Color fg, int depth);
SDL_Texture* BDF_CreateTextTexture(BDF_Font* font, SDL_Renderer* renderer, const char* text, SDL_Color fg, int depth, SDL_ScaleMode mode);

void BDF_CloseFont(BDF_Font* font);

#endif
