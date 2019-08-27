#ifndef _VISUAL_H_
#define _VISUAL_H_

#include <id.h>
#include <hash.h>
#include <physics.h>
#include <HandmadeMath.h>
#include <color.h>

typedef struct visual_entity {
	id_t id;
	GLubvec4 color;
	float radius;
	hmm_vec2 position;
	float angle;
	hmm_mat4 transform;
	hmm_vec2 mins;
	hmm_vec2 maxs;
} visual_entity_t;

KHASH_INIT(visual_map, id_t, visual_entity_t, 1, id_hash_func, id_hash_equal)

typedef struct visual {
	khash_t(visual_map) map;
} visual_t;

void visual_create(visual_t *);
void visual_destroy(visual_t *);

void visual_add(visual_t *, visual_entity_t *);
void visual_remove(visual_t *, id_t);

void visual_update(visual_t *, physics_t *, float dt);

#endif