#ifndef _DRAW_LINES_MAP_H_
#define _DRAW_LINES_MAP_H_

#include <lsgl.h>
#include <fsq.h>
#include <lines.h>
#include <lines_map.h>

typedef struct draw_lines_map {
	GLuint vertex_shader;
	GLuint fragment_shader;
	GLuint program;

	struct {
		GLint random_seed;
		GLint resolution;
		GLint projection;
		GLint count_texture;
	} uniforms;
} draw_lines_map_t;

void draw_lines_map_create(draw_lines_map_t *);
void draw_lines_map_destroy(draw_lines_map_t *);

void draw_lines_map_update(draw_lines_map_t *, lines_t *, const lines_map_t *);

#endif