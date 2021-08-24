#ifndef GL_KANJI_INCLUDE
#define GL_KANJI_INCLUDE

#include <vector>


namespace ist {

class glKanji
{
public:
	enum { SJIS, EUC, JIS };

	glKanji(int _code=SJIS);
	glKanji(const char *filename, int _code=SJIS);

	bool load(const char *filename);
	void print(int x, int y, const char *str);

	void setCode(int _code) { code = _code; }
	int getFontSize() { return font_size; }

protected:
	void init();
	const unsigned char* getChar(int index) { return &data[index*char_byte]; }

	int font_size;
	int char_byte;
	int max_index;
	int code;
	std::vector<unsigned char> data;
};

} // ist

#endif
