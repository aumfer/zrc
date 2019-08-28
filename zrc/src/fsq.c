#include <fsq.h>

static GLbyte vertices[] = {
	SCHAR_MIN, SCHAR_MIN,
	SCHAR_MIN, SCHAR_MAX,
	SCHAR_MAX, SCHAR_MIN,
	SCHAR_MAX, SCHAR_MAX
};
static GLuint indices[] = {
	0, 1, 3,
	0, 3, 2
};

void fsq_create(fsq_t *fsq) {
	glGenBuffers(1, &fsq->index_buffer);
	lsgl_object_label(GL_BUFFER, fsq->index_buffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, fsq->index_buffer);
	glBufferStorage(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, 0);

	glGenBuffers(1, &fsq->vertex_buffer);
	lsgl_object_label(GL_BUFFER, fsq->vertex_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, fsq->vertex_buffer);
	glBufferStorage(GL_ARRAY_BUFFER, sizeof(vertices), vertices, 0);

	glGenVertexArrays(1, &fsq->vertex_array);
	lsgl_object_label(GL_VERTEX_ARRAY, fsq->vertex_array);
	glBindVertexArray(fsq->vertex_array);

	glVertexAttribPointer(FSQ_POSITION_ATTRIBUTE, 2, GL_BYTE, GL_TRUE, 0, 0);
	glEnableVertexAttribArray(FSQ_POSITION_ATTRIBUTE);

	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}
void fsq_destroy(fsq_t *fsq) {
}

void fsq_begin(fsq_t *fsq) {
	glBindVertexArray(fsq->vertex_array);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, fsq->index_buffer);
}
void fsq_draw(fsq_t *fsq, int instance_count) {
	GLsizei index_count = lsgl_countof(indices);
	glDrawElementsInstanced(GL_TRIANGLES, index_count, GL_UNSIGNED_INT, NULL, instance_count);
}
void fsq_end(fsq_t *fsq) {
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}