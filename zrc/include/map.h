#ifndef _MAP_H_
#define _MAP_H_

#include <id.h>
#include <hash.h>
#include <lsm.h>

#define WORLD_SIZE (1024*16)
#define WORLD_HALF (WORLD_SIZE/2)
#define MAP_SCALE 16
#define MAP_SIZE (WORLD_SIZE/MAP_SCALE)
#define MAP_HALF (MAP_SIZE/2)

#define COORD_INVALID ((hmm_ivec2){.X=-1,.Y=-1})

typedef struct map_entity {
	hmm_ivec2 coord;
} map_entity_t;

KHASH_INIT(map_map, id_t, map_entity_t, 1, id_hash_func, id_hash_equal)

typedef struct map {
	khash_t(map_map) map;
} map_t;

void map_create(map_t *);
void map_destroy(map_t *);

void map_update(map_t *);

inline int map_pointinmap(hmm_vec2 v) {
	return v.X > -MAP_HALF && v.X < MAP_HALF
		&& v.Y > -MAP_HALF && v.Y < MAP_HALF;
}
inline hmm_vec2 map_point(hmm_ivec2 p) {
	return HMM_EqualsIVec2(p, COORD_INVALID) ? HMM_Vec2((float)p.X / MAP_SCALE - MAP_HALF, (float)p.Y / MAP_SCALE - MAP_HALF) : HMM_Vec2(NAN, NAN);
}
inline int map_coordinmap(hmm_ivec2 p) {
	return map_pointinmap(map_point(p));
}
inline hmm_ivec2 map_coord(hmm_vec2 v) {
	return map_pointinmap(v) ? (hmm_ivec2) { .X = (int)((v.X + MAP_HALF)*MAP_SCALE), .Y = (int)((v.Y + MAP_HALF)*MAP_SCALE) } : COORD_INVALID;
}

#endif