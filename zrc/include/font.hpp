#ifndef _FONT_H_
#define _FONT_H_

#include <lsgl.h>
#include <ui.hpp>
#include <lsm.hpp>

typedef enum font_style {
	FONT_ARIAL_16,
	FONT_CONSOLAS_16,
	FONT_COUNT
} font_style_t;

typedef struct texture_font texture_font_t;

typedef struct font {
	GLuint texture[FONT_COUNT];
	GLuint vertex_array;
	GLuint array_buffer, element_array_buffer;
	struct {
		GLint projection, font;
	} uniforms;
	struct {
		GLint color, texcoord, position;
	} attributes;
	GLuint program;
	struct {
		GLuint vertex, fragment;
	} shaders;
	GLuint glyph_count;

	font();
	~font();

	void begin();
	void print(font_style, const char *, const GLvec2 position, const GLubvec4 color);
	void draw(const ui &);
	void end();
} font_t;


#endif