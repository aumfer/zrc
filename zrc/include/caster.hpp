#pragma once

#include <zsys.hpp>
#include <ability.hpp>

typedef struct caster_ability {
	union {
		id id;
		name name;
	};
	float downtime;

	ability_target target;
	bool cast;
} caster_ability_t;

typedef struct caster_entity {
	id id;
	zsys<caster_ability> abilities;
} caster_entity_t;

typedef struct caster : zsys<caster_entity_t> {
	void update(const ability &, float dt);
} caster_t;