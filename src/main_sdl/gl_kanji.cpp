#ifdef _WIN32
#	include <windows.h>
#endif

#include <GL/gl.h>
#include <GL/glu.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "gl_kanji.h"



class MatrixSaver
{
public:
	MatrixSaver(int matrix_type) {
		glGetFloatv(matrix_type, matrix);

		switch(matrix_type) {
		case GL_MODELVIEW_MATRIX:
			matrix_mode = GL_MODELVIEW;
			break;
		case GL_PROJECTION_MATRIX:
			matrix_mode = GL_PROJECTION;
			break;
		case GL_TEXTURE_MATRIX:
			matrix_mode = GL_TEXTURE;
			break;
		default:
			matrix_mode = GL_MODELVIEW;
			break;
		}
	}

	~MatrixSaver() {
		int current_matrix_mode;
		glGetIntegerv(GL_MATRIX_MODE, &current_matrix_mode);

		glMatrixMode(matrix_mode);
		glLoadMatrixf(matrix);
		glMatrixMode(current_matrix_mode);
	}

private:
	int matrix_mode;
	float matrix[16];
};


class ScreenMatrix {
public:
	ScreenMatrix() {
		int viewport[4];

		glGetIntegerv(GL_VIEWPORT, viewport);

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(0,viewport[2], viewport[3],0, -1.0, 50000.0);
		glGetFloatv(GL_PROJECTION_MATRIX, proj_matrix);

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
	}

	float proj_matrix[16];
};


static void sjis2jis(unsigned char &c1, unsigned char &c2)
{
	if (c1>=0xe0) { c1 = c1-0x40; }
	if (c2>=0x80) { c2 = c2-1; }
	if (c2>=0x9e) {
		c1 = (c1-0x70) * 2;
		c2 = c2-0x7d;
	} else {
		c1 = ((c1-0x70)*2)-1;
		c2 = c2-0x1f;
	}
}

static void euc2jis(unsigned char &c1, unsigned char &c2)
{
	c1-=0x80;
	c2-=0x80;
}



namespace ist {

glKanji::glKanji(int _code)
	: code(_code)
{
	init();
}

glKanji::glKanji(const char *filename, int _code)
	: code(_code)
{
	init();
	load(filename);
}

void glKanji::init()
{
	font_size = 0;
	char_byte = 0;
	max_index = 0;
}

void glKanji::print(int x, int y, const char *_str)
{
	int		lines = 1;
	if(_str==0 || data.empty()) return;

	const unsigned char *str = reinterpret_cast<const unsigned char*>(_str);

	MatrixSaver mmsaver(GL_MODELVIEW_MATRIX);	// 変換行列を退避
	MatrixSaver pmsaver(GL_PROJECTION_MATRIX);
	ScreenMatrix smatrix;		// 座標系をスクリーンにあわせる

	glRasterPos2i(x, y+font_size);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	while(*str!=0) {
		if(isprint(*str)) {	// ascii文字の場合
			if(*str < max_index)
				glBitmap(font_size, font_size, font_size/2, 0, font_size/2, 0, getChar(*str) );
			++str;
		}
		else if ( *str == '\n' ) {
			lines ++;
			glRasterPos2i(x, y+font_size * lines);
			glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
			++str;
		} else {	// 日本語文字の場合
			unsigned char c1 = str[0];
			unsigned char c2 = str[1];

			switch(code) {
				case SJIS: sjis2jis(c1,c2); break;
				case EUC:  euc2jis (c1,c2); break;
			}

			int index = (c1-0x20)*96 + c2-0x20 +0xff;
			if(index<max_index && index>=0)
				glBitmap(font_size, font_size, 0, 0, font_size, 0, getChar(index) );
			str+=2;
		}
	}
}

bool glKanji::load(const char *filename)
{
	if(filename==0)
		return false;

	data.clear();
	font_size = 0;
	char_byte = 0;

	FILE *in=fopen(filename, "rb");
	if(in==0)
		return false;

	fread(&font_size, sizeof(int), 1, in);

	int n = 0;
	while(n<font_size) { n+=8; }
	int char_bit = n*font_size;
	char_byte = char_bit/8;


	u_char length, temp;
	while(fread(&length, 1, 1, in)!=0) {
		if(length<0x80) {
			for(int i=0; i<length+1; ++i) {
				fread(&temp, 1, 1, in);
				data.push_back(temp);
			}
		}
		else {
			fread(&temp, 1, 1, in);

			for(int i=0x80; i<length+1; ++i)
				data.push_back(temp);
		}
	}
	fclose(in);

	max_index = data.size()/char_byte;
	return true;
}

} // ist

