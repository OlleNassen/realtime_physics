#ifndef TEXT_HPP
#define TEXT_HPP
#include <string>
#include "buffer.hpp"
#include <map>
#include <glm/glm.hpp>
#include <ft2build.h>
#include FT_FREETYPE_H

struct character
{
    unsigned int texture_index;  // ID handle of the glyph texture
    long int advance;    // Offset to advance to next glyph
	glm::ivec2 size;       // Size of glyph
	glm::ivec2 bearing;    // Offset from baseline to left/top of glyph

};


class text
{
public:
	text();
	~text();
	void render_text(const std::string& text,
        float x, float y, float scale);

private:
	FT_Library library;
	unsigned int VAO, VBO;
	std::map<char, character> characters;
};

#endif
