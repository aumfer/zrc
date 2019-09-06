#ifndef _CAMERA_H_
#define _CAMERA_H_

#include <lsm.hpp>
#include <ui.hpp>
#include <map.hpp>

typedef struct camera {
	glm::vec2 position = glm::vec2(WORLD_HALF, WORLD_HALF);
	float zoom = 512;

	glm::mat4 view;
	glm::mat4 projection;
	glm::mat4 view_projection;

	void update(const ui &);
} camera_t;


#endif