#ifndef _FSQ_H_
#define _FSQ_H_

#include <lsgl.h>

#define FSQ_POSITION_ATTRIBUTE 0

typedef struct fsq {
	GLuint index_buffer;
	GLuint vertex_buffer;
	GLuint vertex_array;
} fsq_t;

void fsq_create(fsq_t *);
void fsq_destroy(fsq_t *);

void fsq_begin(fsq_t *);
void fsq_draw(fsq_t *, int instance_count);
void fsq_end(fsq_t *);

#endif