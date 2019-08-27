#ifndef _DRAW_H_
#define _DRAW_H_

#include <fsq.h>
#include <ui.h>

typedef struct draw {
	fsq_t fsq;

	GLuint vertex_shader;
	GLuint fragment_shader;
	GLuint program;

	struct {
		GLint random_seed;
		GLint resolution;
	} uniforms;
} draw_t;

void draw_create(draw_t *);
void draw_destroy(draw_t *);

void draw_update(draw_t *, const ui_t *);

#endif