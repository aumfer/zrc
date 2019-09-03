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

fsq::fsq() {
	glGenBuffers(1, &index_buffer);
	lsgl_object_label(GL_BUFFER, index_buffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer);
	glBufferStorage(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, 0);

	glGenBuffers(1, &vertex_buffer);
	lsgl_object_label(GL_BUFFER, vertex_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
	glBufferStorage(GL_ARRAY_BUFFER, sizeof(vertices), vertices, 0);

	glGenVertexArrays(1, &vertex_array);
	lsgl_object_label(GL_VERTEX_ARRAY, vertex_array);
	glBindVertexArray(vertex_array);

	glVertexAttribPointer(FSQ_POSITION_ATTRIBUTE, 2, GL_BYTE, GL_TRUE, 0, 0);
	glEnableVertexAttribArray(FSQ_POSITION_ATTRIBUTE);

	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}
fsq::~fsq() {

}

void fsq::begin() const {
	glBindVertexArray(vertex_array);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer);
}
void fsq::draw(int instance_count) const {
	GLsizei index_count = lsgl_countof(indices);
	glDrawElementsInstanced(GL_TRIANGLES, index_count, GL_UNSIGNED_INT, NULL, instance_count);
}
void fsq::end() const {
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}