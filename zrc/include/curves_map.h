#ifndef _CURVES_MAP_H_
#define _CURVES_MAP_H_

#include <lsgl.h>
#include <visual.h>
#include <map.h>

#define CURVES_MAP_TILE_CURVE_MAX 8

#define CURVES_MAP_TILE_COUNT (MAP_SIZE*MAP_SIZE*CURVES_MAP_TILE_CURVE_MAX)
#define CURVES_MAP_TILE_SIZE ((CURVES_MAP_TILE_COUNT)*sizeof(curves_map_tile_t))

#define CURVES_MAP_COUNT_COUNT (MAP_SIZE*MAP_SIZE)
#define CURVES_MAP_COUNT_SIZE ((CURVES_MAP_COUNT_COUNT)*sizeof(GLuint))

typedef struct curves_map_tile {
	hmm_vec2 points[4];
} curves_map_tile_t;

typedef struct curves_map {
	GLuint tile_buffer;
	GLuint count_buffer;

	curves_map_tile_t tiles[CURVES_MAP_TILE_COUNT];
	GLuint counts[CURVES_MAP_COUNT_COUNT];

	curves_map_tile_t *tiles_map;
	GLuint *counts_map;
} curves_map_t;

void curves_map_create(curves_map_t *);
void curves_map_destroy(curves_map_t *);

void curves_map_update(curves_map_t *, const visual_t *);

#endif