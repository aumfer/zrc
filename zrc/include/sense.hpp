#pragma once

#include <zsys.hpp>
#include <zset.hpp>
#include <physics.hpp>

typedef struct sense_entity {
	id id;
	float radius;
	zset sensed;
} sense_entity_t;

typedef struct sense : zsys<sense_entity> {
	sense();
	~sense();
	void update(const physics &);
} sense_t;