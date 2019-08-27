#ifndef _CURVES_H_
#define _CURVES_H_

#include <lsgl.h>
#include <fsq.h>
#include <visual.h>

#define CURVES_POSITION_ATTRIBUTE 1
#define CURVES_SIZE_ATTRIBUTE 2
#define CURVES_COLOR_ATTRIBUTE 3
#define CURVES_POINT_0_ATTRIBUTE 4
#define CURVES_POINT_1_ATTRIBUTE 5
#define CURVES_POINT_2_ATTRIBUTE 6
#define CURVES_POINT_3_ATTRIBUTE 7

typedef struct curves {
	fsq_t fsq;

	GLuint instance_buffer;

	GLuint instance_count;
} curves_t;

void curves_create(curves_t *);
void curves_destroy(curves_t *);

void curves_update(curves_t *, const visual_t *);

void curves_begin(curves_t *);
void curves_draw(curves_t *);
void curves_end(curves_t *);

#endif