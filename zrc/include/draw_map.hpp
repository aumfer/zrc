#ifndef _DRAW_H_
#define _DRAW_H_

#include <fsq.hpp>
#include <ui.hpp>
#include <camera.hpp>
#include <curves.hpp>

typedef struct draw_map {
	fsq_t fsq;

	GLuint vertex_shader;
	GLuint fragment_shader;
	GLuint program;

	struct {
		GLint random_seed;
		GLint resolution;
		GLint view_projection;
		GLint inv_view_projection;
		GLint map_scale;
	} uniforms;

	draw_map();
	~draw_map();

	void update(const ui &, const camera &, const curves &);
} draw_map_t;


#endif