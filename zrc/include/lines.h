#ifndef _LINES_H_
#define _LINES_H_

#include <lsgl.h>
#include <visual.h>

#define CURVES_ATTRIBUTE_POSITION 0
#define CURVES_ATTRIBUTE_COLOR 1

typedef struct lines {
	GLuint index_buffer;
	GLuint vertex_buffer;
	GLuint vertex_array;

	GLuint vertex_count;
	GLuint index_count;
} lines_t;

void lines_create(lines_t *);
void lines_destroy(lines_t *);

void lines_update(lines_t *, const visual_t *);

void lines_begin(lines_t *);
void lines_draw(lines_t *, int instance_count);
void lines_end(lines_t *);

#endif