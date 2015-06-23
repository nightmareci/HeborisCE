#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#include "include.h"
#include "opengl_utils.h"

#if		SDL_USE_OPENGL

static	float	s_fDrawRate = 1.0f;
extern	int		cpp_texpow2;
extern	int		cpp_texfilter;

/**********************************************************************/

void SDL_GL_Enter2DMode()
{
	SDL_Surface *screen = SDL_GetVideoSurface();

	/* Note, there may be other things you need to change,
	   depending on how you have your OpenGL state set up.
	*/
	glPushAttrib(GL_ENABLE_BIT);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	glEnable(GL_TEXTURE_2D);

	/* This allows alpha blending of 2D textures with the scene */
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glViewport(0, 0, screen->w, screen->h);

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();

	glOrtho(0.0, (GLdouble)screen->w, (GLdouble)screen->h, 0.0, 0.0, 1.0);

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);

	/* http://risky-safety.org/~zinnia/sdl/sourcetour/extra1/ */
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glColor4d(1.0f, 1.0f, 1.0f, 1.0f);
}

void SDL_GL_Leave2DMode()
{
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();

	glMatrixMode(GL_PROJECTION);
	glPopMatrix();

	glPopAttrib();
}

/* Quick utility function for texture creation */
static int power_of_two(int input)
{
	if ( cpp_texpow2 == 0 )
	{
		return input;
	}
	else
	{
		int value = 1;

		while ( value < input ) {
			value <<= 1;
		}
		return value;
	}
}

GLuint SDL_GL_LoadTexture(SDL_Surface *surface, GL_Texture *ptexture)
{
	GLuint texture;
	int w, h;
	SDL_Surface *image;
	SDL_Rect area;
	Uint32 saved_flags;
	Uint8  saved_alpha;

	memset(ptexture, 0, sizeof(*ptexture));

	/* Use the surface width and height expanded to powers of 2 */
	w = power_of_two(surface->w);
	h = power_of_two(surface->h);
	ptexture->texcoord[0] = 0.0f;			/* Min X */
	ptexture->texcoord[1] = 0.0f;			/* Min Y */
	ptexture->texcoord[2] = (GLfloat)surface->w / w;	/* Max X */
	ptexture->texcoord[3] = (GLfloat)surface->h / h;	/* Max Y */

	ptexture->real_width  = surface->w;
	ptexture->real_height = surface->h;
	ptexture->texture_width  = w;
	ptexture->texture_height = h;

	image = SDL_CreateRGBSurface(
			SDL_SWSURFACE,
			w, h,
			32,
#if SDL_BYTEORDER == SDL_LIL_ENDIAN /* OpenGL RGBA masks */
			0x000000FF,
			0x0000FF00,
			0x00FF0000,
			0xFF000000
#else
			0xFF000000,
			0x00FF0000,
			0x0000FF00,
			0x000000FF
#endif
		       );
	if ( image == NULL ) {
		return 0;
	}

	/* Save the alpha blending attributes */
	saved_flags = surface->flags&(SDL_SRCALPHA|SDL_RLEACCELOK);
	saved_alpha = surface->format->alpha;
	if ( (saved_flags & SDL_SRCALPHA) == SDL_SRCALPHA ) {
		SDL_SetAlpha(surface, 0, 0);
	}

	/* Copy the surface into the GL texture image */
	area.x = 0;
	area.y = 0;
	area.w = surface->w;
	area.h = surface->h;
	SDL_BlitSurface(surface, &area, image, &area);

	/* Restore the alpha blending attributes */
	if ( (saved_flags & SDL_SRCALPHA) == SDL_SRCALPHA ) {
		SDL_SetAlpha(surface, saved_flags, saved_alpha);
	}

	/* Create an OpenGL texture for the image */
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, cpp_texfilter != 0 ? GL_LINEAR : GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexImage2D(GL_TEXTURE_2D,
		     0,
		     GL_RGBA,
		     w, h,
		     0,
		     GL_RGBA,
		     GL_UNSIGNED_BYTE,
		     image->pixels);
	SDL_FreeSurface(image); /* No longer needed */

	ptexture->active = true;
	ptexture->texture = texture;

		return texture;
}

void SDL_GL_FreeTexture(GL_Texture *ptexture)
{
	if ( !ptexture->active ) { return; }

	glDeleteTextures( 1, &ptexture->texture );
	memset(ptexture, 0, sizeof(*ptexture));
}

void SDL_GL_PutTexture(GL_Texture *ptexture, int dx, int dy, int dw, int dh, int sx, int sy, int sw, int sh)
{
	if ( !ptexture->active ) { return; }

	int		x = (int)((float)dx * s_fDrawRate);
	int		y = (int)((float)dy * s_fDrawRate);
	int		w = (int)((float)(dw + dx) * s_fDrawRate) - x;
	int		h = (int)((float)(dh + dy) * s_fDrawRate) - y;

	float	texMinX = (float) sx       / (float)ptexture->texture_width;
	float	texMaxX = (float)(sx + sw) / (float)ptexture->texture_width;
	float	texMinY = (float) sy       / (float)ptexture->texture_height;
	float	texMaxY = (float)(sy + sh) / (float)ptexture->texture_height;

	/* Show the image on the screen */
	glBindTexture(GL_TEXTURE_2D, ptexture->texture);
	glBegin(GL_TRIANGLE_STRIP);
	glTexCoord2f(texMinX, texMinY); glVertex2i(x,   y  );
	glTexCoord2f(texMaxX, texMinY); glVertex2i(x+w, y  );
	glTexCoord2f(texMinX, texMaxY); glVertex2i(x,   y+h);
	glTexCoord2f(texMaxX, texMaxY); glVertex2i(x+w, y+h);
	glEnd();
}

void SDL_GL_SetDrawRate(float rate)
{
	s_fDrawRate = rate;
}

#endif
