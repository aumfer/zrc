#ifndef _PHYSICS_H_
#define _PHYSICS_H_

#include <chipmunk/chipmunk_private.h>
#include <zsys.hpp>
#include <lsm.hpp>

#define PHYSICS_MAX_ENTITIES 16384

typedef struct physics_entity_contact {
	id_t id;
} physics_entity_contact_t;

typedef struct physics_entity {
	id_t id;

	cpBodyType type;
	cpBitmask collide_flags;
	cpBitmask collide_mask;
	cpBitmask response_mask;

	float radius;
	float max_speed;
	float max_spin;

	glm::vec2 position;
	float angle;
	glm::vec2 velocity;
	float angular_velocity;

	glm::vec2 force;
	float torque;

	cpBody *body;
	cpCircleShape *circle;

	zsys<physics_entity_contact> contacts;
} physics_entity_t;

typedef struct physics : zsys<physics_entity, PHYSICS_MAX_ENTITIES> {
	cpSpace *space;
	cpCollisionHandler *collision_handler;

	physics();
	~physics();

	void add(const physics_entity &);
	void del(id);
	void update(float dt);

	physics_entity *query_ray(const glm::vec2 &start, const glm::vec2 &end, float radius) const;
} physics_t;

#endif