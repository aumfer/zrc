#ifndef _DRAW_H_
#define _DRAW_H_

#include <fsq.h>
#include <ui.h>
#include <camera.h>
#include <curves_map.h>
#include <curves.h>

typedef struct draw_map {
	fsq_t fsq;

	GLuint vertex_shader;
	GLuint fragment_shader;
	GLuint program;

	struct {
		GLint random_seed;
		GLint resolution;
		GLint projection;
		GLint map_scale;
	} uniforms;

	draw_map();
	~draw_map();

	void update(const ui &, const camera &, const curves_map&, const curves &);
} draw_map_t;


#endif