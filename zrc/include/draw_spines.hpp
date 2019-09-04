#pragma once

#include <lsgl.h>
#include <spines.hpp>
#include <ui.h>
#include <camera.h>

typedef struct draw_spines {
	GLuint vertex_array;

	struct {
		GLint projection;
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