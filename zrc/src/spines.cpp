#include <spines.hpp>
#include <map.h>
#include <stdlib.h>

spines::spines() {
	parsl_config parsl_config = {};
	parsl_config.thickness = 1;
	parsl_config.flags = PARSL_FLAG_ANNOTATIONS;
	parsl_config.u_mode = PAR_U_MODE_DISTANCE;
	parsl_config.streamlines_seed_spacing = MAP_SCALE;
	parsl_config.streamlines_seed_viewport.right = WORLD_SIZE;
	parsl_config.streamlines_seed_viewport.top = WORLD_SIZE;
	context = parsl_create_context(parsl_config);

	spine_list.vertices = (parsl_position *)calloc(SPINES_MAX_VERTICES, sizeof(parsl_position));
	spine_list.spine_lengths = (uint16_t *)calloc(SPINES_MAX_SPINES, sizeof(uint16_t));

	glGenBuffers(1, &positions_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, positions_buffer);
	glBufferStorage(GL_ARRAY_BUFFER, SPINES_POSITIONS_SIZE, NULL, GL_MAP_WRITE_BIT);

	glGenBuffers(1, &annotations_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, annotations_buffer);
	glBufferStorage(GL_ARRAY_BUFFER, SPINES_ANNOTATIONS_SIZE, NULL, GL_MAP_WRITE_BIT);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenBuffers(1, &index_buffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer);
	glBufferStorage(GL_ELEMENT_ARRAY_BUFFER, SPINES_INDEX_SIZE, NULL, GL_MAP_WRITE_BIT);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

spines::~spines() {

}

void spines::update(const visual &visual) {
	spine_list.num_spines = 0;
	spine_list.num_vertices = 0;
	visual.foreach([&](const visual_entity &visual_entity) {
		for (int i = 0; i < VISUAL_ENTITY_POINTS; ++i) {
			glm::vec4 p = visual_entity.transform * glm::vec4(visual_entity.points[i], 0, 1);
			spine_list.vertices[spine_list.num_vertices].x = p.x;
			spine_list.vertices[spine_list.num_vertices].y = p.y;
			++spine_list.num_vertices;
		}

		spine_list.spine_lengths[spine_list.num_spines] = VISUAL_ENTITY_POINTS;
		++spine_list.num_spines;
	});

	parsl_mesh *mesh = parsl_mesh_from_curves_cubic(context, spine_list);

	glBindBuffer(GL_ARRAY_BUFFER, positions_buffer);
	GLsizeiptr positions_size = mesh->num_vertices * sizeof(parsl_position);
	assert(positions_size < SPINES_POSITIONS_SIZE);
	parsl_position *positions = (parsl_position *)glMapBufferRange(GL_ARRAY_BUFFER, 0, positions_size, GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_RANGE_BIT);
	assert(positions);
	memcpy(positions, mesh->positions, positions_size);
	glUnmapBuffer(GL_ARRAY_BUFFER);

	glBindBuffer(GL_ARRAY_BUFFER, annotations_buffer);
	GLsizeiptr annotations_size = mesh->num_vertices * sizeof(parsl_annotation);
	assert(annotations_size < SPINES_ANNOTATIONS_SIZE);
	parsl_annotation *annotations = (parsl_annotation *)glMapBufferRange(GL_ARRAY_BUFFER, 0, annotations_size, GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_RANGE_BIT);
	assert(annotations);
	memcpy(annotations, mesh->annotations, annotations_size);
	glUnmapBuffer(GL_ARRAY_BUFFER);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer);
	GLsizeiptr indices_size = mesh->num_vertices * sizeof(uint32_t);
	assert(indices_size < SPINES_INDEX_SIZE);
	uint32_t *indices = (uint32_t *)glMapBufferRange(GL_ELEMENT_ARRAY_BUFFER, 0, indices_size, GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_RANGE_BIT);
	assert(indices);
	memcpy(indices, mesh->triangle_indices, indices_size);
	glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}