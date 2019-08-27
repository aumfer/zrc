#ifndef _DRAW_H_
#define _DRAW_H_

#include <fsq.h>
#include <ui.h>
#include <camera.h>
#include <lines_map.h>

typedef struct draw {
	fsq_t fsq;

	GLuint vertex_shader;
	GLuint fragment_shader;
	GLuint program;

	struct {
		GLint random_seed;
		GLint resolution;
		GLint projection;
	} uniforms;
} draw_t;

void draw_create(draw_t *);
void draw_destroy(draw_t *);

void draw_update(draw_t *, const ui_t *, const camera_t *, const lines_map_t *);

#endif