#ifndef _CAMERA_H_
#define _CAMERA_H_

#include <HandmadeMath.h>
#include <ui.h>
#include <map.h>

typedef struct camera {
	hmm_vec2 position;
	float zoom;

	hmm_mat4 view;
	hmm_mat4 projection;
} camera_t;

void camera_create(camera_t *);
void camera_destroy(camera_t *);

void camera_update(camera_t *, const ui_t *);

#endif