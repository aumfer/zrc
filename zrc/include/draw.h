#ifndef _DRAW_H_
#define _DRAW_H_

#include <fsq.h>
#include <ui.h>
#include <camera.h>
#include <curves_map.h>
#include <curves.h>

#define DRAW_INSTANCE_BUFFER_BINDING 0

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

void draw_update(draw_t *, const ui_t *, const camera_t *, const curves_map_t *, const curves_t *);

#endif