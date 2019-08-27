#ifndef _MAP_H_
#define _MAP_H_

#include <id.h>
#include <hash.h>

#define WORLD_SIZE (1024*16)
#define WORLD_HALF (WORLD_SIZE/2)
#define MAP_SIZE (WORLD_SIZE/16)
#define MAP_HALF (MAP_SIZE/2)

typedef struct coord {
	int x, y;
} coord_t;

typedef struct map_entity {
	coord_t coord;
} map_entity_t;

KHASH_INIT(map_map, id_t, map_entity_t, 1, id_hash_func, id_hash_equal)

typedef struct map {
	khash_t(map_map) map;
} map_t;

void map_create(map_t *);
void map_destroy(map_t *);

void map_update(map_t *);

#endif