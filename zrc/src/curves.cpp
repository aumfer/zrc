#include <curves.hpp>
#include <assert.h>
#include <lsm.hpp>
#include <stdio.h>

#define CURVES_INSTANCE_COUNT 16384
#define CURVES_INSTANCE_SIZE ((CURVES_INSTANCE_COUNT)*sizeof(curves_instance_t))

//static_assert(sizeof(curves_instance_t) % 32 == 0, "sizeof curves_instance must be multiple of 32");
static_assert(sizeof(curves_instance_t) % 16 == 0, "sizeof curves_instance must be multiple of 16");

curves::curves() {
	printf("sizeof(curves_instance_t) %dx%zd=%zd\n", CURVES_INSTANCE_COUNT, sizeof(curves_instance_t), CURVES_INSTANCE_SIZE);
	lsgl_object_label(GL_BUFFER, fsq.index_buffer);
	lsgl_object_label(GL_BUFFER, fsq.vertex_buffer);
	lsgl_object_label(GL_VERTEX_ARRAY, fsq.vertex_array);

	glGenBuffers(1, &instance_buffer);
	lsgl_object_label(GL_BUFFER, instance_buffer);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, instance_buffer);
	glBufferStorage(GL_SHADER_STORAGE_BUFFER, CURVES_INSTANCE_SIZE, NULL, GL_MAP_WRITE_BIT);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, CURVES_INSTANCE_BINDING, instance_buffer);
}
curves::~curves() {

}

void curves::update(const visual &visual, const control &control) {
	instance_count = 0;

	glBindBuffer(GL_SHADER_STORAGE_BUFFER, instance_buffer);
	curves_instance_t *instances = (curves_instance_t *)glMapBufferRange(GL_SHADER_STORAGE_BUFFER, 0, CURVES_INSTANCE_SIZE, GL_MAP_WRITE_BIT);
	assert(instances);

	visual.foreach([&](const visual_entity &visual_entity) {
		instances[instance_count].position = visual_entity.position;
		instances[instance_count].radius = visual_entity.radius;
		instances[instance_count].color = visual_entity.color;
		instances[instance_count].transform = visual_entity.transform;
		instances[instance_count].velocity = visual_entity.velocity;
		instances[instance_count].angular_velocity = visual_entity.angular_velocity;

		for (int i = 0; i < CURVES_POINTS; ++i) {
			glm::vec4 p = visual_entity.transform * glm::vec4(visual_entity.points[i], 0, 1);
			instances[instance_count].points[i] = p;
		}

		GLuint flags = CURVES_FLAGS_NONE;
		if (visual_entity.id == control.select_entity) {
			flags |= CURVES_FLAGS_SELECTED;
		}
		instances[instance_count].flags = flags;

		++instance_count;
		assert(instance_count < CURVES_INSTANCE_COUNT);
	});

	glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
}

void curves::begin() const {
	fsq.begin();
}
void curves::draw() const {
	fsq.draw(instance_count);
}
void curves::end() const {
	fsq.end();
}