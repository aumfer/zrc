#pragma once

#include <par_streamlines.h>
#include <visual.h>

#define SPINES_MAX_SPINES 16384
#define SPINES_MAX_VERTICES (SPINES_MAX_SPINES*16)

#define SPINES_POSITIONS_COUNT SPINES_MAX_VERTICES
#define SPINES_POSITIONS_SIZE (SPINES_POSITIONS_COUNT*sizeof(parsl_position))

#define SPINES_ANNOTATIONS_COUNT SPINES_MAX_VERTICES
#define SPINES_ANNOTATIONS_SIZE (SPINES_ANNOTATIONS_COUNT*sizeof(parsl_annotation))

#define SPINES_INDEX_COUNT SPINES_MAX_VERTICES
#define SPINES_INDEX_SIZE (SPINES_INDEX_COUNT*sizeof(uint32_t))

typedef struct spines {
	parsl_context* context;
	parsl_spine_list spine_list;

	GLuint positions_buffer;
	GLuint annotations_buffer;
	GLuint index_buffer;
	GLuint vertex_array;

	spines();
	~spines();
	void update(const visual &);
} spines_t;