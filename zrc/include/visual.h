#ifndef _VISUAL_H_
#define _VISUAL_H_

#include <zsys.hpp>
#include <physics.h>
#include <lsm.h>
#include <color.h>

#define VISUAL_ENTITY_POINTS 8

typedef struct visual_entity {
	id_t id;
	glm::u8vec4 color;
	float radius;
	glm::vec2 position;
	float angle;
	glm::vec2 front;
	glm::mat4 transform;
	glm::vec2 mins;
	glm::vec2 maxs;

	glm::vec2 points[VISUAL_ENTITY_POINTS];
} visual_entity_t;

typedef struct visual : zsys<visual_entity> {
	visual();
	~visual();

	void add(const visual_entity &);

	void update(physics &, float dt);
} visual_t;


#endif