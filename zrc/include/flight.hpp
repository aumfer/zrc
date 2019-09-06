#pragma once

#include <zsys.hpp>
#include <physics.hpp>

typedef struct flight_entity {
	id id;
	float max_thrust;
	float max_turn;
	float linear_damping;
	float angular_damping;

	glm::vec2 thrust;
	float turn;
} flight_entity_t;

typedef struct flight : zsys<flight_entity> {
	flight();
	~flight();

	void update(physics &);
} flight_t;