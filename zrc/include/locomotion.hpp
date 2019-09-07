#pragma once

#include <zsys.hpp>
#include <lsm.hpp>
#include <functional>
#include <physics.hpp>
#include <flight.hpp>

typedef std::function<double(const glm::vec2 &, float)> locomotion_potential;

typedef struct locomotion_entity {
	id id;
	locomotion_potential behavior;
} locomotion_entity_t;

typedef struct locomotion : zsys<locomotion_entity> {
	locomotion();
	~locomotion();
	void update(const physics &, flight &, float dt);
} locomotion_t;