#pragma once

#include <lsgl.h>
#include <spines.hpp>
#include <ui.hpp>
#include <camera.hpp>

typedef struct draw_spines {
	GLuint vertex_array;

	struct {
		GLint view_projection;
	} uniforms;
	struct {
		GLint position, annotations;
	} attributes;
	GLuint program;
	struct {
		GLuint vertex, fragment;
	} shaders;

	draw_spines();
	~draw_spines();
	void update(const spines &, const ui &, const camera &);
} draw_spines_t;