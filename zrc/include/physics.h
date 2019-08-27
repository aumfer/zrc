#ifndef _PHYSICS_H_
#define _PHYSICS_H_

#include <chipmunk/chipmunk_private.h>
#include <id.h>
#include <hash.h>
#include <HandmadeMath.h>

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

	hmm_vec2 position;
	float angle;
	hmm_vec2 velocity;
	float angular_velocity;

	hmm_vec2 force;
	float torque;

	cpBody *body;
	cpCircleShape *circle;

	khash_t(physics_entity_contact_map) contact_map;
} physics_entity_t;

KHASH_INIT(physics_map, id_t, physics_entity_t, 1, id_hash_func, id_hash_equal)

typedef struct physics {
	khash_t(physics_map) map;
	cpSpace space;
	cpCollisionHandler *collision_handler;
} physics_t;

void physics_create(physics_t *);
void physics_destroy(physics_t *);

void physics_add(physics_t *, physics_entity_t *);
void physics_remove(physics_t *, id_t);
physics_entity_t *physics_get(physics_t *, id_t);

void physics_update(physics_t *, float dt);

#endif