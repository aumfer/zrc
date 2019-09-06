#include <visual.hpp>
#include <assert.h>
#include <lsm.hpp>
#include <glm/gtc/matrix_transform.hpp>

visual::visual() {
}
visual::~visual() {
}

void visual::add(const visual_entity &create) {
	visual_entity visual_entity = create;
	for (int i = 0; i < VISUAL_ENTITY_POINTS; ++i) {
		visual_entity.points[i] = glm::circularRand(1.0f);
	}
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

		visual_entity.velocity = physics_entity->velocity + physics_entity->force * dt;
		visual_entity.angular_velocity = physics_entity->angular_velocity + physics_entity->torque * dt;

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), glm::vec3(position.x, position.y, 0));
		transform = glm::rotate(transform, angle, glm::vec3(0, 0, 1));
		glm::vec3 size = glm::vec3(physics_entity->radius * 2, physics_entity->radius * 2, 1);
		transform = glm::scale(transform, size);
		visual_entity.transform = transform;

		glm::vec2 mins = glm::vec2(position.x - radius, position.y - radius);
		glm::vec2 maxs = glm::vec2(position.x + radius, position.y + radius);
		visual_entity.mins = mins;
		visual_entity.maxs = maxs;
	});
}