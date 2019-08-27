#include <curves_map.h>
#include <assert.h>

void curves_map_create(curves_map_t *curves_map) {
	glGenBuffers(1, &curves_map->tile_buffer);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, curves_map->tile_buffer);
	glBufferStorage(GL_SHADER_STORAGE_BUFFER, CURVES_MAP_TILE_SIZE, NULL, GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT);

	curves_map->tiles_map = glMapBufferRange(GL_SHADER_STORAGE_BUFFER, 0, CURVES_MAP_TILE_SIZE, GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT);
	assert(curves_map->tiles_map);

	glGenBuffers(1, &curves_map->count_buffer);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, curves_map->count_buffer);
	glBufferStorage(GL_SHADER_STORAGE_BUFFER, CURVES_MAP_COUNT_SIZE, NULL, GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT);

	curves_map->counts_map = glMapBufferRange(GL_SHADER_STORAGE_BUFFER, 0, CURVES_MAP_COUNT_SIZE, GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT);
	assert(curves_map->counts_map);

	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
}
void curves_map_destroy(curves_map_t *curves_map) {
}

void curves_map_update(curves_map_t *curves_map, const visual_t *visual) {
	visual_entity_t *visual_entity;
	kh_foreach_value_ptr(&visual->map, visual_entity, {
		hmm_ivec2 mins_coord = map_coord(visual_entity->mins);
		hmm_ivec2 maxs_coord = map_coord(visual_entity->maxs);
		
		for (int x = mins_coord.X; x <= maxs_coord.X; ++x) {
			for (int y = mins_coord.Y; y <= maxs_coord.Y; ++y) {
				if (!map_coordinmap(HMM_IVec2(x, y))) {
					continue;
				}
				int count_index = lsm_index2d(x, y, MAP_SIZE, MAP_SIZE);
				int curve_index = curves_map->counts[count_index]++;
		
				int tile_index = lsm_index3d(x, y, curve_index, MAP_SIZE, MAP_SIZE, CURVES_MAP_TILE_CURVE_MAX);
				curves_map->tiles[tile_index].points[0].X = (visual_entity->position.X - visual_entity->radius);
				curves_map->tiles[tile_index].points[0].Y = (visual_entity->position.Y - visual_entity->radius);
				curves_map->tiles[tile_index].points[1].X = (visual_entity->position.X + visual_entity->radius);
				curves_map->tiles[tile_index].points[1].Y = (visual_entity->position.Y - visual_entity->radius);
				curves_map->tiles[tile_index].points[2].X = (visual_entity->position.X + visual_entity->radius);
				curves_map->tiles[tile_index].points[2].Y = (visual_entity->position.Y + visual_entity->radius);
				curves_map->tiles[tile_index].points[3].X = (visual_entity->position.X - visual_entity->radius);
				curves_map->tiles[tile_index].points[3].Y = (visual_entity->position.Y + visual_entity->radius);
			}
		}
	});

	for (int i = 0; i < CURVES_MAP_COUNT_COUNT; ++i) {
		curves_map->counts_map[i] = curves_map->counts[i];
	
		if (curves_map->counts[i]) {
			for (GLuint j = 0; j < curves_map->counts[i]; ++j) {
				int tile_index = lsm_index2d(i, j, CURVES_MAP_COUNT_COUNT, CURVES_MAP_TILE_CURVE_MAX);
			
				curves_map->tiles_map[tile_index] = curves_map->tiles[tile_index];
			}
		}
	}
}