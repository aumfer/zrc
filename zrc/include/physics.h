#ifndef _PHYSICS_H_
#define _PHYSICS_H_

#include <chipmunk/chipmunk_private.h>
#include <zsys.hpp>
#include <lsm.h>

#define PHYSICS_MAX_ENTITIES 16384

KHASH_INIT(physics_entity_contact_map, id_t, char, 0, id_hash_func, id_hash_equal)

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

	khash_t(physics_entity_contact_map) contact_map;
} physics_entity_t;

typedef struct physics : zsys<physics_entity, PHYSICS_MAX_ENTITIES> {
	cpSpace *space;
	cpCollisionHandler *collision_handler;

	physics();
	~physics();

	void add(const physics_entity &);
	void del(id);
	void update(float dt);
} physics_t;

#endif