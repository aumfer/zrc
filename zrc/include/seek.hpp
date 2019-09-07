#pragma once

#include <locomotion.hpp>
#include <map.hpp>

typedef struct seek_entity {
	id id;
	glm::vec2 to;
} seek_entity_t;

typedef struct seek : zsys<seek_entity> {
	seek();
	~seek();
	void update(const physics &, locomotion &);
} seek_t;