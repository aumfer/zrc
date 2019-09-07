#include <map.hpp>

map::map() {

}
map::~map() {

}

map_entity &map::add(const map_entity &create) {
	map_entity& map_entity = zsys::add(create);
	map_entity.coord = COORD_INVALID;
	return map_entity;
}

void map::update(const physics &physics) {
	foreach([&](map_entity &map_entity) {
		physics.get(map_entity.id, [&](const physics_entity &physics_entity) {
			glm::ivec2 coord = map_coord(physics_entity.position);
			glm::ivec2 prev_coord = map_entity.coord;
			float prev_radius = map_entity.radius;

			if (coord != prev_coord) {
				if (map_coordinmap(prev_coord)) {
					tiles[prev_coord.x][prev_coord.y].entities.del(map_entity.id);
				}
				if (map_coordinmap(coord)) {
					tiles[coord.x][coord.y].entities.add(map_entity.id);
				}
				map_entity.coord = coord;
			}
		});
	});
}