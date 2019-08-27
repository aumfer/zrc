#ifndef _FONT_H_
#define _FONT_H_

#include <lsgl.h>
#include <ui.h>

typedef enum font_style {
	FONT_ARIAL_16,
	FONT_CONSOLAS_16
} font_style_t;

typedef struct texture_font texture_font_t;

typedef struct font {
	font_style_t style;
	texture_font_t *data;

	GLuint texture;
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
} font_t;

void font_create(font_t *, font_style_t);
void font_destroy(font_t *);

void font_begin(font_t *);
void font_print(font_t *, const char *, GLvec2 position, GLubvec4 color);
void font_draw(font_t *, const ui_t *ui);
void font_end(font_t *);

#endif