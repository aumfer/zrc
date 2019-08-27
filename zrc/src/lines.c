#include <lines.h>
#include <assert.h>

typedef struct lines_vertex {
	GLvec2 position;
	GLubvec4 color;
} lines_vertex_t;
typedef GLuint lines_index_t;

#define CURVES_VERTEX_COUNT (PHYSICS_MAX_ENTITIES*4)
#define CURVES_VERTEX_SIZE (CURVES_VERTEX_COUNT*sizeof(lines_vertex_t))

#define CURVES_INDEX_COUNT (PHYSICS_MAX_ENTITIES*8)
#define CURVES_INDEX_SIZE (CURVES_INDEX_COUNT*sizeof(lines_index_t))

void lines_create(lines_t *lines) {
	glGenBuffers(1, &lines->index_buffer);
	lsgl_object_label(GL_BUFFER, lines->index_buffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, lines->index_buffer);
	glBufferStorage(GL_ELEMENT_ARRAY_BUFFER, CURVES_INDEX_SIZE, NULL, GL_MAP_WRITE_BIT);

	glGenBuffers(1, &lines->vertex_buffer);
	lsgl_object_label(GL_BUFFER, lines->vertex_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, lines->vertex_buffer);
	glBufferStorage(GL_ARRAY_BUFFER, CURVES_VERTEX_SIZE, NULL, GL_MAP_WRITE_BIT);

	glGenVertexArrays(1, &lines->vertex_array);
	lsgl_object_label(GL_VERTEX_ARRAY, lines->vertex_array);
	glBindVertexArray(lines->vertex_array);

	glVertexAttribPointer(CURVES_ATTRIBUTE_POSITION, 2, GL_FLOAT, GL_FALSE, sizeof(lines_vertex_t), lsgl_offsetof(lines_vertex_t, position));
	glEnableVertexAttribArray(CURVES_ATTRIBUTE_POSITION);
	glVertexAttribPointer(CURVES_ATTRIBUTE_COLOR, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(lines_vertex_t), lsgl_offsetof(lines_vertex_t, color));
	glEnableVertexAttribArray(CURVES_ATTRIBUTE_COLOR);

	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}
void lines_destroy(lines_t *lines) {

}

void lines_update(lines_t *lines, const visual_t *visual) {
	lines->vertex_count = 0;
	lines->index_count = 0;

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, lines->index_buffer);
	lines_index_t *indices = glMapBufferRange(GL_ELEMENT_ARRAY_BUFFER, 0, CURVES_INDEX_SIZE, GL_MAP_WRITE_BIT);
	assert(indices);
	glBindBuffer(GL_ARRAY_BUFFER, lines->vertex_buffer);
	lines_vertex_t *vertices = glMapBufferRange(GL_ARRAY_BUFFER, 0, CURVES_VERTEX_SIZE, GL_MAP_WRITE_BIT);
	assert(vertices);

	visual_entity_t *visual_entity;
	kh_foreach_value_ptr(&visual->map, visual_entity, {
		vertices[lines->vertex_count + 0].position[0] = (GLfloat)(visual_entity->position.X - visual_entity->radius);
		vertices[lines->vertex_count + 0].position[1] = (GLfloat)(visual_entity->position.Y - visual_entity->radius);
		vertices[lines->vertex_count + 1].position[0] = (GLfloat)(visual_entity->position.X + visual_entity->radius);
		vertices[lines->vertex_count + 1].position[1] = (GLfloat)(visual_entity->position.Y - visual_entity->radius);
		vertices[lines->vertex_count + 2].position[0] = (GLfloat)(visual_entity->position.X + visual_entity->radius);
		vertices[lines->vertex_count + 2].position[1] = (GLfloat)(visual_entity->position.Y + visual_entity->radius);
		vertices[lines->vertex_count + 3].position[0] = (GLfloat)(visual_entity->position.X - visual_entity->radius);
		vertices[lines->vertex_count + 3].position[1] = (GLfloat)(visual_entity->position.Y + visual_entity->radius);

		indices[lines->index_count + 0] = lines->vertex_count + 0;
		indices[lines->index_count + 1] = lines->vertex_count + 1;
		indices[lines->index_count + 2] = lines->vertex_count + 1;
		indices[lines->index_count + 3] = lines->vertex_count + 2;
		indices[lines->index_count + 4] = lines->vertex_count + 2;
		indices[lines->index_count + 5] = lines->vertex_count + 3;
		indices[lines->index_count + 6] = lines->vertex_count + 3;
		indices[lines->index_count + 7] = lines->vertex_count + 0;

		lines->vertex_count += 4;
		lines->index_count += 8;
	});

	glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER);
	glUnmapBuffer(GL_ARRAY_BUFFER);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void lines_begin(lines_t *lines) {
	glBindVertexArray(lines->vertex_array);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, lines->index_buffer);
}
void lines_draw(lines_t *lines, int instance_count) {
	glDrawElementsInstanced(GL_LINES, lines->index_count, GL_UNSIGNED_INT, NULL, 1);
}
void lines_end(lines_t *lines) {
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}