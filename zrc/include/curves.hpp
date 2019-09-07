#ifndef _CURVES_H_
#define _CURVES_H_

#include <lsgl.h>
#include <fsq.hpp>
#include <lsm.hpp>
#include <visual.hpp>
#include <map.hpp>
#include <control.hpp>

#define CURVES_POINTS VISUAL_ENTITY_POINTS

typedef struct curves_instance {
	glm::vec2 position;
	GLfloat radius;
	glm::u8vec4 color;
	glm::mat4 transform;
	glm::vec2 velocity;
	GLfloat angular_velocity;
	GLuint flags;
	glm::vec2 points[CURVES_POINTS];
} curves_instance_t;

#define CURVES_INSTANCE_BINDING 0

typedef struct curves {
	curves();
	~curves();

	fsq_t fsq;

	GLuint instance_buffer;

	GLuint instance_count;

	void update(const visual &, const control &);

	void begin() const;
	void draw() const;
	void end() const;
} curves_t;

#endif