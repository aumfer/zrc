#include <draw_spines.hpp>

static GLchar vertex_src[] = GLSL_BEGIN
#include <shaders/util.glsl>
GLSL(
uniform mat4 projection;

layout(location = 0) in vec2 position;
layout(location = 1) in vec4 annotations;

out vec2 v_position;
out vec4 v_annotations;

void main() {
	v_position = position;
	v_annotations = annotations;

	vec4 p = vec4(position, 0, 1);
	gl_Position = projection * p;
});

static GLchar fragment_src[] = GLSL_BEGIN
#include <shaders/util.glsl>
#include <shaders/sdf.glsl>
GLSL(

in vec2 v_position;
in vec4 v_annotations;

out vec4 fragColor;

void main() {
	vec3 color = vec3(1, 0, 0);
	float d = v_annotations.y;
	d *= 10;
	//color /= (d*d);
	fragColor = vec4(color, 1);
});

draw_spines::draw_spines() {
	shaders.vertex = glCreateShader(GL_VERTEX_SHADER);
	lsgl_compileshader(shaders.vertex, vertex_src, lsgl_countof(vertex_src));

	shaders.fragment = glCreateShader(GL_FRAGMENT_SHADER);
	lsgl_compileshader(shaders.fragment, fragment_src, lsgl_countof(fragment_src));

	program = glCreateProgram();
	lsgl_object_label(GL_PROGRAM, program);
	glAttachShader(program, shaders.vertex);
	glAttachShader(program, shaders.fragment);
	lsgl_linkprogram(program);

	glUseProgram(program);
	uniforms.projection = glGetUniformLocation(program, "projection");
	attributes.position = glGetAttribLocation(program, "position");
	attributes.annotations = glGetAttribLocation(program, "annotations");
	glUseProgram(0);

	glGenVertexArrays(1, &vertex_array);
}
draw_spines::~draw_spines() {

}

void draw_spines::update(const spines &spines, const ui &ui, const camera &camera) {
	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE, GL_ONE);

	glUseProgram(program);

	glBindVertexArray(vertex_array);

	glBindBuffer(GL_ARRAY_BUFFER, spines.positions_buffer);
	glVertexAttribPointer(attributes.position, 2, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(attributes.position);

	glBindBuffer(GL_ARRAY_BUFFER, spines.annotations_buffer);
	glVertexAttribPointer(attributes.annotations, 4, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(attributes.annotations);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	//glm::mat4 projection = glm::ortho(0.0f, (float)ui.framebuffer_size.x, 0.0f, (float)ui.framebuffer_size.y);
	glm::mat4 projection = camera.projection;
	glViewport(0, 0, ui.framebuffer_size.x, ui.framebuffer_size.y);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, spines.index_buffer);
	glUniformMatrix4fv(uniforms.projection, 1, GL_FALSE, glm::value_ptr(projection));
	glDrawElements(GL_TRIANGLES, spines.spine_list.num_vertices, GL_UNSIGNED_INT, NULL);

	glUseProgram(0);

	glDisable(GL_BLEND);
}