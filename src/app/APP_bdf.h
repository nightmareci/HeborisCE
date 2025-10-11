#ifndef APP_bdf_h_
#define APP_bdf_h_

#include <SDL3/SDL.h>

struct APP_BDFFont;
typedef struct APP_BDFFont APP_BDFFont;

/**
 * Create a BDF font object, optionally loading characters from src. If src ==
 * NULL, an empty font object is created. Use BDF_AddFont to add font(s) to
 * an empty font object.
 */
APP_BDFFont* APP_OpenBDFFont(SDL_IOStream* src);

/**
 * Add characters in the font data src contains to the font object. Characters
 * already in the font object will not be replaced. The src font data's font
 * must be the same font size as the size of the font object, or can be any
 * size if the font object is currently empty.
 */
bool APP_AddBDFFont(APP_BDFFont* font, SDL_IOStream* src);

/**
 * Get the size of a font.
 */
int APP_GetBDFFontSize(APP_BDFFont* font);

/**
 * Get the width of a line of text in a font.
 */
int APP_GetBDFTextWidth(APP_BDFFont* font, const char* text);

/**
 * Get the dimensions of the rectangle containing all lines of the text in a
 * font.
 */
int APP_GetBDFTextDimensions(APP_BDFFont* font, const char *text, int* w, int* h);

typedef bool (* APP_PutBDFPixelCallback)(void* dst, int x, int y, float subx, float suby, SDL_Color color);
bool APP_PutBDFPixelSurface(SDL_Surface *s, int x, int y, float subx, float suby, SDL_Color color);
bool APP_PutBDFPixelRenderer(SDL_Renderer *s, int x, int y, float subx, float suby, SDL_Color pixel);

bool APP_PutBDFText(APP_BDFFont* font, int dx, int dy, float subx, float suby, void* dst, int dw, int dh, const char* txt, SDL_Color fg, APP_PutBDFPixelCallback put_pixel);
bool APP_PutBDFTextSurface(APP_BDFFont* font, int dx, int dy, float subx, float suby, SDL_Surface* dst, const char* text, SDL_Color fg);
bool APP_PutBDFTextRenderer(APP_BDFFont* font, int dx, int dy, float subx, float suby, SDL_Renderer* dst, const char* text, SDL_Color fg);

SDL_Surface* APP_CreateBDFTextSurface(APP_BDFFont* font, const char* text, SDL_Color fg, int depth);
SDL_Texture* APP_CreateBDFTextTexture(APP_BDFFont* font, SDL_Renderer* renderer, const char* text, SDL_Color fg, int depth, SDL_ScaleMode mode);

void APP_CloseBDFFont(APP_BDFFont* font);

#endif
