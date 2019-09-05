#ifndef _DRAW_CURVES_H_
#define _DRAW_CURVES_H_

#include <lsgl.h>
#include <curves.hpp>
#include <ui.hpp>
#include <map.hpp>
#include <camera.hpp>

typedef struct draw_curves {
	GLuint vertex_shader;
	GLuint fragment_shader;
	GLuint program;

	struct {
		GLint random_seed;
		GLint resolution;
		GLint projection;
	} uniforms;

	draw_curves();
	~draw_curves();

	void update(curves &, const ui&, const camera &);
} draw_curves_t;


#endif