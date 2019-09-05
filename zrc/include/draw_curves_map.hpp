#ifndef _DRAW_CURVES_MAP_H_
#define _DRAW_CURVES_MAP_H_

#include <lsgl.h>
#include <curves.hpp>
#include <curves_map.hpp>

typedef struct draw_curves_map {
	GLuint vertex_shader;
	GLuint fragment_shader;
	GLuint program;

	struct {
		GLint random_seed;
		GLint resolution;
		GLint projection;
		GLint count_texture;
		GLint map_scale;
	} uniforms;

	draw_curves_map();
	~draw_curves_map();
	void update(curves_t &, const curves_map_t &);
} draw_curves_map_t;

#endif