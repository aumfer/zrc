#ifndef _MAP_H_
#define _MAP_H_

#include <lsm.hpp>
#include <zsys.hpp>
#include <zset.hpp>
#include <physics.hpp>

#define WORLD_SIZE (1024*8)
#define WORLD_HALF (WORLD_SIZE/2)
#define MAP_SCALE 16
#define MAP_SIZE (WORLD_SIZE/MAP_SCALE)
#define MAP_HALF (MAP_SIZE/2)

#define COORD_INVALID glm::ivec2(-1, -1)
#define POINT_INVALID glm::vec2(NAN, NAN)

typedef struct map_entity {
	id id;
	glm::ivec2 coord;
	float radius;
} map_entity_t;

typedef struct map_tile {
	zset entities;
} map_tile_t;

typedef struct map : zsys<map_entity> {
	map_tile tiles[MAP_SIZE][MAP_SIZE];

	map();
	~map();
	map_entity &add(const map_entity &);
	void update(const physics &);

	template<typename R>
	void query_point(glm::vec2 position, float radius, const R &receiver) const {
		glm::ivec2 mins = map_coord(position - radius);
		glm::ivec2 maxs = map_coord(position + radius);
		for (int x = mins.x; x <= maxs.x; ++x) {
			for (int y = mins.y; y <= maxs.y; ++y) {
				glm::ivec2 coord = glm::ivec2(x, y);
				if (!map_coordinmap(coord)) {
					continue;
				}

				tiles[coord.x][coord.y].entities.foreach(receiver);
			}
		}
	}
} map_t;

inline int map_pointinmap(glm::vec2 v) {
	return v.x > 0 && v.x < WORLD_SIZE
		&& v.y > 0 && v.y < WORLD_SIZE;
}
inline glm::vec2 map_point(glm::ivec2 p) {
	return p == COORD_INVALID ? glm::vec2((float)p.x * MAP_SCALE, (float)p.y * MAP_SCALE) : POINT_INVALID;
}
inline int map_coordinmap(glm::ivec2 p) {
	return p.x >= 0 && p.x < MAP_SIZE
		&& p.y >= 0 && p.y < MAP_SIZE;
}
inline glm::ivec2 map_coord(glm::vec2 v) {
	glm::ivec2 p = glm::ivec2((int)(v.x / MAP_SCALE), (int)(v.y / MAP_SCALE));
	return map_pointinmap(v) ? p : COORD_INVALID;
}

#endif