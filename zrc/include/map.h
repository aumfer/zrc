#ifndef _MAP_H_
#define _MAP_H_

#include <lsm.h>
#include <zsys.hpp>

#define WORLD_SIZE (1024*2)
#define WORLD_HALF (WORLD_SIZE/2)
#define MAP_SCALE 16
#define MAP_SIZE (WORLD_SIZE/MAP_SCALE)
#define MAP_HALF (MAP_SIZE/2)

#define COORD_INVALID glm::ivec2(-1, -1)
#define POINT_INVALID glm::vec2(NAN, NAN)

typedef struct map_entity {
	glm::ivec2 coord;
} map_entity_t;

KHASH_INIT(map_map, id_t, map_entity_t, 1, id_hash_func, id_hash_equal)

typedef struct map : zsys<map_entity> {
	map();
	~map();
	void update();
} map_t;

inline int map_pointinmap(glm::vec2 v) {
	return v.x > -MAP_HALF && v.x < MAP_HALF
		&& v.y > -MAP_HALF && v.y < MAP_HALF;
}
inline glm::vec2 map_point(glm::ivec2 p) {
	return p == COORD_INVALID ? glm::vec2((float)p.x / MAP_SCALE - MAP_HALF, (float)p.y / MAP_SCALE - MAP_HALF) : POINT_INVALID;
}
inline int map_coordinmap(glm::ivec2 p) {
	return map_pointinmap(map_point(p));
}
inline glm::ivec2 map_coord(glm::vec2 v) {
	glm::ivec2 p = glm::ivec2((int)((v.x + MAP_HALF)*MAP_SCALE), (int)((v.y + MAP_HALF)*MAP_SCALE));
	return map_pointinmap(v) ? p : COORD_INVALID;
}

#endif