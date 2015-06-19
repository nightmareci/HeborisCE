#ifndef		__OPENGL_UTILS_H__
#define		__OPENGL_UTILS_H__

#include "include.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#include "SDL.h"
#include "SDL_opengl.h"

#if		SDL_USE_OPENGL

struct GL_Texture
{
	bool	active;
	GLuint	texture;
	int		real_width;
	int		real_height;
	int		texture_width;
	int		texture_height;
	GLfloat	texcoord[4];
};

void SDL_GL_Enter2DMode();
void SDL_GL_Leave2DMode();
GLuint SDL_GL_LoadTexture(SDL_Surface *surface, GL_Texture *ptexture);
void SDL_GL_FreeTexture(GL_Texture *ptexture);
void SDL_GL_PutTexture(GL_Texture *ptexture, int dx, int dy, int dw, int dh, int sx, int sy, int sw, int sh);
void SDL_GL_SetDrawRate(float rate);

#endif
#endif
