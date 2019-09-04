#ifndef _CURVES_H_
#define _CURVES_H_

#include <lsgl.h>
#include <fsq.h>
#include <visual.h>
#include <map.h>

typedef struct curves_instance {
	GLvec2 position;
	GLvec2 size;
	GLvec4 color;
	GLvec2 point0;
	GLvec2 point1;
	GLvec2 point2;
	GLvec2 point3;
} curves_instance_t;

#define CURVES_INSTANCE_BINDING 0

#define CURVES_MAX_SPLINES 16384
#define CURVES_MAX_VERTICES (CURVES_MAX_SPLINES*16)

typedef struct curves {
	curves();
	~curves();

	fsq_t fsq;

	GLuint instance_buffer;

	GLuint instance_count;

	void update(const visual &);

	void begin() const;
	void draw() const;
	void end() const;
} curves_t;

#endif