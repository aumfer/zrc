#pragma once

#include <zsys.hpp>

typedef struct life_entity {
	id id;
	float health, strength, constitution;
	float max_health, max_strength, max_constitution;
	float mana, focus, willpower;
	float max_mana, max_focus, max_willpower;
	float rage, serenity, temper;
	float max_rage, max_serentiy, max_temper;
} life_entity_t;

typedef struct life : zsys<life_entity> {
	life();
	~life();
} life_t;
