#ifndef _DRAW_CURVES_MAP_H_
#define _DRAW_CURVES_MAP_H_

#include <lsgl.h>
#include <curves.h>
#include <curves_map.h>

typedef struct draw_curves_map {
	GLuint vertex_shader;
	GLuint fragment_shader;
	GLuint program;

	struct {
		GLint random_seed;
		GLint resolution;
		GLint projection;
		GLint count_texture;
	} uniforms;
} draw_curves_map_t;

void draw_curves_map_create(draw_curves_map_t *);
void draw_curves_map_destroy(draw_curves_map_t *);

void draw_curves_map_update(draw_curves_map_t *, curves_t *, const curves_map_t *);

#endif