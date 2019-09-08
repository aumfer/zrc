#ifndef _CONTROL_H_
#define _CONTROL_H_

#include <camera.hpp>
#include <ui.hpp>
#include <physics.hpp>
#include <map.hpp>
#include <flight.hpp>
#include <caster.hpp>

typedef struct control {
	const float ZOOM_DEFAULT = 512;

	float scroll_rate = 0.5f;
	float zoom_rate = 0.1f;

	glm::vec3 pick_start;
	glm::vec3 pick_end;
	id hover_entity;
	id select_entity;

	void update(camera &, const ui &, const physics &, const map &, flight &, caster &, float dt);
} control_t;

#endif