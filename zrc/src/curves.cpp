#include <curves.h>
#include <assert.h>
#include <lsm.h>

#define CURVES_INSTANCE_COUNT 16384
#define CURVES_INSTANCE_SIZE ((CURVES_INSTANCE_COUNT)*sizeof(curves_instance_t))

curves::curves() {
	assert(sizeof(curves_instance_t) % 32 == 0);

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

void curves::update(const visual &visual) {
	instance_count = 0;

	glBindBuffer(GL_SHADER_STORAGE_BUFFER, instance_buffer);
	curves_instance_t *instances = (curves_instance_t *)glMapBufferRange(GL_SHADER_STORAGE_BUFFER, 0, CURVES_INSTANCE_SIZE, GL_MAP_WRITE_BIT);
	assert(instances);

	visual.foreach([&](const visual_entity &visual_entity) {
		instances[instance_count].position[0] = (GLfloat)visual_entity.position.x;
		instances[instance_count].position[1] = (GLfloat)visual_entity.position.y;

		GLfloat size = (GLfloat)visual_entity.radius * 2;
		instances[instance_count].size[0] = size * 2;
		instances[instance_count].size[1] = size * 2;

		instances[instance_count].color[0] = visual_entity.color[0] / 255.0f;
		instances[instance_count].color[1] = visual_entity.color[1] / 255.0f;
		instances[instance_count].color[2] = visual_entity.color[2] / 255.0f;
		instances[instance_count].color[3] = visual_entity.color[3] / 255.0f;

		instances[instance_count].point0[0] = (GLfloat)(visual_entity.position.x + visual_entity.points[0].x*visual_entity.radius);
		instances[instance_count].point0[1] = (GLfloat)(visual_entity.position.y + visual_entity.points[0].y*visual_entity.radius);
		instances[instance_count].point1[0] = (GLfloat)(visual_entity.position.x + visual_entity.points[1].x*visual_entity.radius);
		instances[instance_count].point1[1] = (GLfloat)(visual_entity.position.y + visual_entity.points[1].y*visual_entity.radius);
		instances[instance_count].point2[0] = (GLfloat)(visual_entity.position.x + visual_entity.points[2].x*visual_entity.radius);
		instances[instance_count].point2[1] = (GLfloat)(visual_entity.position.y + visual_entity.points[2].y*visual_entity.radius);
		instances[instance_count].point3[0] = (GLfloat)(visual_entity.position.x + visual_entity.points[3].x*visual_entity.radius);
		instances[instance_count].point3[1] = (GLfloat)(visual_entity.position.y + visual_entity.points[3].y*visual_entity.radius);

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