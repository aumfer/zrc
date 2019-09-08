#pragma once

#include <zsys.hpp>
#include <lsm.hpp>
#include <functional>

typedef enum ability_target_flags {
	ABILITY_TARGET_NONE		= 0,
	ABILITY_TARGET_UNIT		= 1,
	ABILITY_TARGET_POINT	= 2,
	ABILITY_TARGET_FRIEND	= 4 | ABILITY_TARGET_UNIT,
	ABILITY_TARGET_ENEMY	= 8 | ABILITY_TARGET_UNIT
} ability_target_flags_t;

typedef union ability_target {
	id unit;
	glm::vec2 point;
} ability_target_t;

typedef std::function<void(id caster, const ability_target &, float)> ability_cast;

typedef struct ability_entity {
	union {
		id id;
		name name;
	};
	ability_target_flags target_flags;
	ability_cast cast;
	float range;
	float cooldown;
	float mana;
	float rage;
} ability_entity_t;

typedef struct ability : zsys<ability_entity> {
	ability();
	~ability();
} ability_t;
