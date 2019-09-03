#ifndef _CURVES_MAP_H_
#define _CURVES_MAP_H_

#include <lsgl.h>
#include <visual.h>
#include <map.h>

#define CURVES_MAP_COUNT 8

#define CURVES_MAP_COUNT_IMAGE_BINDING 1

typedef struct curves_map {
	GLuint count_texture;
	GLuint tile_texture;
	GLuint framebuffer;

	curves_map();
	~curves_map();

	void update(const visual &);
} curves_map_t;

#endif