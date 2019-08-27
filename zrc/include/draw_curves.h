#ifndef _DRAW_CURVES_H_
#define _DRAW_CURVES_H_

#include <lsgl.h>
#include <curves.h>
#include <ui.h>
#include <map.h>
#include <camera.h>

typedef struct draw_curves {
	GLuint vertex_shader;
	GLuint fragment_shader;
	GLuint program;

	struct {
		GLint random_seed;
		GLint resolution;
		GLint projection;
	} uniforms;
} draw_curves_t;

void draw_curves_create(draw_curves_t *);
void draw_curves_destroy(draw_curves_t *);

void draw_curves_update(draw_curves_t *, curves_t *, const ui_t *, const camera_t *);

#endif