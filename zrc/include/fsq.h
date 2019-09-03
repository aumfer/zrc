#ifndef _FSQ_H_
#define _FSQ_H_

#include <lsgl.h>

#define FSQ_POSITION_ATTRIBUTE 0

typedef struct fsq {
	GLuint index_buffer;
	GLuint vertex_buffer;
	GLuint vertex_array;

	fsq();
	~fsq();

	void begin() const;
	void draw(int index_count) const;
	void end() const;
} fsq_t;

#endif