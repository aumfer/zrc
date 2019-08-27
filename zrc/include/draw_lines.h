#ifndef _DRAW_LINES_H_
#define _DRAW_LINES_H_

#include <lsgl.h>
#include <ui.h>
#include <lines.h>
#include <map.h>
#include <camera.h>

typedef struct draw_lines {
	GLuint vertex_shader;
	GLuint fragment_shader;
	GLuint program;

	struct {
		GLint random_seed;
		GLint resolution;
		GLint projection;
	} uniforms;
} draw_lines_t;

void draw_lines_create(draw_lines_t *);
void draw_lines_destroy(draw_lines_t *);

void draw_lines_update(draw_lines_t *, lines_t *, const ui_t *, const camera_t *);

#endif