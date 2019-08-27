#ifndef _LINES_MAP_H_
#define _LINES_MAP_H_

#include <lsgl.h>
#include <lines.h>
#include <map.h>

#define LINES_MAP_COUNT 8

#define COUNT_IMAGE_BINDING 0

typedef struct lines_map {
	GLuint count_texture;
	GLuint tile_texture;
	GLuint framebuffer;
} lines_map_t;

void lines_map_create(lines_map_t *);
void lines_map_destroy(lines_map_t *);

void lines_map_update(lines_map_t *, lines_t *);

#endif