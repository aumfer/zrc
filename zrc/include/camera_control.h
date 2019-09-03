#ifndef _CAMERA_CONTROL_H_
#define _CAMERA_CONTROL_H_

#include <camera.h>
#include <ui.h>

typedef struct camera_control {
	const float ZOOM_DEFAULT = 512;

	float scroll_rate = 100;
	float zoom_rate = 100;

	void update(camera &, const ui &, float dt);
} camera_control_t;

#endif