#ifndef _CONTROL_H_
#define _CONTROL_H_

#include <camera.hpp>
#include <ui.hpp>
#include <physics.hpp>
#include <flight.hpp>

typedef struct control {
	const float ZOOM_DEFAULT = 512;

	float scroll_rate = 100;
	float zoom_rate = 100;

	glm::vec3 pick_start;
	glm::vec3 pick_end;
	id hover_entity;
	id select_entity;

	void update(camera &, const ui &, const physics &, flight &, float dt);
} control_t;

#endif