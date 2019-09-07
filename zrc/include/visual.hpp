#ifndef _VISUAL_H_
#define _VISUAL_H_

#include <zsys.hpp>
#include <physics.hpp>
#include <lsm.hpp>
#include <color.h>

#define VISUAL_ENTITY_POINTS 8

#define VISUAL_FLAGS_NONE 0
#define VISUAL_FLAGS_SELECTED 1

typedef struct visual_entity {
	id_t id;
	glm::u8vec4 color;
	float radius;
	glm::vec2 position;
	float angle;
	glm::vec2 front;
	glm::vec2 velocity;
	float angular_velocity;
	glm::mat4 transform;
	glm::vec2 mins;
	glm::vec2 maxs;

	glm::vec2 points[VISUAL_ENTITY_POINTS];
	unsigned flags;
} visual_entity_t;

typedef struct visual : zsys<visual_entity> {
	visual();
	~visual();

	void add(const visual_entity &);

	void update(physics &, float dt);
} visual_t;


#endif