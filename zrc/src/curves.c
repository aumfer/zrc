#include <curves.h>
#include <assert.h>
#include <HandmadeMath.h>

#define CURVES_INSTANCE_COUNT 16384
#define CURVES_INSTANCE_SIZE ((CURVES_INSTANCE_COUNT)*sizeof(curves_instance_t))

void curves_create(curves_t *curves) {
	assert(sizeof(curves_instance_t) % 32 == 0);

	fsq_create(&curves->fsq);
	lsgl_object_label(GL_BUFFER, curves->fsq.index_buffer);
	lsgl_object_label(GL_BUFFER, curves->fsq.vertex_buffer);
	lsgl_object_label(GL_VERTEX_ARRAY, curves->fsq.vertex_array);

	glGenBuffers(1, &curves->instance_buffer);
	lsgl_object_label(GL_BUFFER, curves->instance_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, curves->instance_buffer);
	glBufferStorage(GL_ARRAY_BUFFER, CURVES_INSTANCE_SIZE, NULL, GL_MAP_WRITE_BIT);

	glBindVertexArray(curves->fsq.vertex_array);

	glVertexAttribPointer(CURVES_POSITION_ATTRIBUTE, 2, GL_FLOAT, GL_FALSE, sizeof(curves_instance_t), lsgl_offsetof(curves_instance_t, position));
	glEnableVertexAttribArray(CURVES_POSITION_ATTRIBUTE);
	glVertexAttribDivisor(CURVES_POSITION_ATTRIBUTE, 1);

	glVertexAttribPointer(CURVES_SIZE_ATTRIBUTE, 2, GL_FLOAT, GL_FALSE, sizeof(curves_instance_t), lsgl_offsetof(curves_instance_t, size));
	glEnableVertexAttribArray(CURVES_SIZE_ATTRIBUTE);
	glVertexAttribDivisor(CURVES_SIZE_ATTRIBUTE, 1);

	glVertexAttribPointer(CURVES_COLOR_ATTRIBUTE, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(curves_instance_t), lsgl_offsetof(curves_instance_t, color));
	glEnableVertexAttribArray(CURVES_COLOR_ATTRIBUTE);
	glVertexAttribDivisor(CURVES_COLOR_ATTRIBUTE, 1);

	glVertexAttribPointer(CURVES_POINT_0_ATTRIBUTE, 2, GL_FLOAT, GL_FALSE, sizeof(curves_instance_t), lsgl_offsetof(curves_instance_t, point0));
	glEnableVertexAttribArray(CURVES_POINT_0_ATTRIBUTE);
	glVertexAttribDivisor(CURVES_POINT_0_ATTRIBUTE, 1);

	glVertexAttribPointer(CURVES_POINT_1_ATTRIBUTE, 2, GL_FLOAT, GL_FALSE, sizeof(curves_instance_t), lsgl_offsetof(curves_instance_t, point1));
	glEnableVertexAttribArray(CURVES_POINT_1_ATTRIBUTE);
	glVertexAttribDivisor(CURVES_POINT_1_ATTRIBUTE, 1);

	glVertexAttribPointer(CURVES_POINT_2_ATTRIBUTE, 2, GL_FLOAT, GL_FALSE, sizeof(curves_instance_t), lsgl_offsetof(curves_instance_t, point2));
	glEnableVertexAttribArray(CURVES_POINT_2_ATTRIBUTE);
	glVertexAttribDivisor(CURVES_POINT_2_ATTRIBUTE, 1);

	glVertexAttribPointer(CURVES_POINT_3_ATTRIBUTE, 2, GL_FLOAT, GL_FALSE, sizeof(curves_instance_t), lsgl_offsetof(curves_instance_t, point3));
	glEnableVertexAttribArray(CURVES_POINT_3_ATTRIBUTE);
	glVertexAttribDivisor(CURVES_POINT_3_ATTRIBUTE, 1);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}
void curves_destroy(curves_t *curves) {

}

void curves_update(curves_t *curves, const visual_t *visual) {
	curves->instance_count = 0;

	glBindBuffer(GL_ARRAY_BUFFER, curves->instance_buffer);
	curves_instance_t *instances = glMapBufferRange(GL_ARRAY_BUFFER, 0, CURVES_INSTANCE_SIZE, GL_MAP_WRITE_BIT);
	assert(instances);

	visual_entity_t *visual_entity;
	kh_foreach_value_ptr(&visual->map, visual_entity, {
		instances[curves->instance_count].position[0] = (GLfloat)visual_entity->position.X;
		instances[curves->instance_count].position[1] = (GLfloat)visual_entity->position.Y;

		GLfloat size = (GLfloat)visual_entity->radius * 2;
		instances[curves->instance_count].size[0] = size;
		instances[curves->instance_count].size[1] = size;

		instances[curves->instance_count].color[0] = visual_entity->color[0];
		instances[curves->instance_count].color[1] = visual_entity->color[1];
		instances[curves->instance_count].color[2] = visual_entity->color[2];
		instances[curves->instance_count].color[3] = visual_entity->color[3];

		instances[curves->instance_count].point0[0] = (GLfloat)(visual_entity->position.X - visual_entity->radius);
		instances[curves->instance_count].point0[1] = (GLfloat)(visual_entity->position.Y - visual_entity->radius);
		instances[curves->instance_count].point1[0] = (GLfloat)(visual_entity->position.X + visual_entity->radius);
		instances[curves->instance_count].point1[1] = (GLfloat)(visual_entity->position.Y - visual_entity->radius);
		instances[curves->instance_count].point2[0] = (GLfloat)(visual_entity->position.X + visual_entity->radius);
		instances[curves->instance_count].point2[1] = (GLfloat)(visual_entity->position.Y + visual_entity->radius);
		instances[curves->instance_count].point3[0] = (GLfloat)(visual_entity->position.X - visual_entity->radius);
		instances[curves->instance_count].point3[1] = (GLfloat)(visual_entity->position.Y + visual_entity->radius);

		++curves->instance_count;
		assert(curves->instance_count < CURVES_INSTANCE_COUNT);
	});

	glUnmapBuffer(GL_ARRAY_BUFFER);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void curves_begin(curves_t *curves) {
	fsq_begin(&curves->fsq);
}
void curves_draw(curves_t *curves) {
	fsq_draw(&curves->fsq, curves->instance_count);
}
void curves_end(curves_t *curves) {
	fsq_end(&curves->fsq);
}