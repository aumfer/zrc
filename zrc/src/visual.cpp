#include <visual.h>
#include <assert.h>
#include <lsm.h>
#include <glm/gtc/matrix_transform.hpp>

visual::visual() {
}
visual::~visual() {
}

void visual::add(const visual_entity &create) {
	visual_entity visual_entity = create;
	visual_entity.d0 = glm::normalize(glm::vec2(randf() - 0.5f, randf() - 0.5f));
	visual_entity.d1 = glm::normalize(glm::vec2(randf() - 0.5f, randf() - 0.5f));
	visual_entity.d2 = glm::normalize(glm::vec2(randf() - 0.5f, randf() - 0.5f));
	visual_entity.d3 = glm::normalize(glm::vec2(randf() - 0.5f, randf() - 0.5f));
	//visual_entity.d2 = -visual_entity.d0;
	//visual_entity.d3 = -visual_entity.d1;
	zsys::add(visual_entity);
}

void visual::update(physics &physics, float dt) {
	foreach([&](visual_entity_t &visual_entity) {
		physics_entity *physics_entity = physics.get(visual_entity.id);
		assert(physics_entity);

		float radius = physics_entity->radius;
		visual_entity.radius = radius;
		glm::vec2 position = physics_entity->position + physics_entity->velocity * dt;
		visual_entity.position = position;
		float angle = physics_entity->angle + physics_entity->angular_velocity * dt;
		visual_entity.angle = angle;
		cpVect front = cpvforangle(angle);
		visual_entity.front = glm::vec2((float)front.x, (float)front.y);

		glm::mat4 translate = glm::translate(glm::mat4(), glm::vec3(position.x, position.y, 0));
		glm::mat4 rotate = glm::rotate(glm::mat4(), angle, glm::vec3(0, 0, 1));
		glm::vec3 size = glm::vec3(physics_entity->radius * 2, physics_entity->radius * 2, 1);
		glm::mat4 scale = glm::scale(glm::mat4(), size);
		glm::mat4 transform = translate * rotate * scale;
		visual_entity.transform = transform;

		glm::vec2 mins = glm::vec2(position.x - radius, position.y - radius);
		glm::vec2 maxs = glm::vec2(position.x + radius, position.y + radius);
		visual_entity.mins = mins;
		visual_entity.maxs = maxs;
	});
}